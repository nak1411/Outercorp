// HarvestableResourceActor.cpp

#include "HarvestableResourceActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "InteractableComponent.h"
#include "OutercorpCharacter.h"
#include "InventoryComponent.h"
#include "PickupableItem.h"
#include "EquippableTool.h"
#include "NotificationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/BodySetup.h"

// Console variables for debug visualization
static TAutoConsoleVariable<int32> CVarHarvestableDebugCollision(
	TEXT("Harvesting.Debug.Collision"),
	0,
	TEXT("Show collision debug visualization for harvestable resources.\n")
	TEXT("0: Disabled (default)\n")
	TEXT("1: Enabled"),
	ECVF_Default);

static TAutoConsoleVariable<int32> CVarHarvestableShowHealth(
	TEXT("Harvesting.Debug.ShowHealth"),
	0,
	TEXT("Show health bars above harvestable resources.\n")
	TEXT("0: Disabled (default)\n")
	TEXT("1: Enabled"),
	ECVF_Default);

AHarvestableResourceActor::AHarvestableResourceActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	// Create mesh component - collision completely disabled
	// The ISM handles world collision, we just need this for visuals
	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
	ResourceMesh->SetupAttachment(RootSceneComponent);
	ResourceMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ResourceMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ResourceMesh->SetGenerateOverlapEvents(false);

	// Create interaction volume - query only for visibility traces, no physics
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootSceneComponent);
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	InteractionVolume->SetGenerateOverlapEvents(false);
	InteractionVolume->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	// Create interactable component
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));

	// Initialize state
	ResourceData = nullptr;
	CurrentHealth = 100.0f;
	bIsDepleted = false;
	RespawnTimeRemaining = 0.0f;
	PCGInstanceIndex = INDEX_NONE;
	CurrentStageIndex = INDEX_NONE;
	OriginalScale = FVector(1.0f);
	MeshMaxHealth = 100.0f;
	bIsDirectPickup = false;
	DirectPickupItem = nullptr;
	DirectPickupQuantity = 1;
}

void AHarvestableResourceActor::BeginPlay()
{
	Super::BeginPlay();

	// Store original transform for respawning (if not already set by PCG initialization)
	if (PCGInstanceIndex == INDEX_NONE)
	{
		OriginalTransform = GetActorTransform();
	}

	// Initialize from data first - this will set up collision properly
	if (ResourceData)
	{
		InitializeFromData(ResourceData);
	}

	// NOTE: Collision setup is handled in InitializeFromData
	// We use the mesh's native collision instead of InteractionVolume
}

void AHarvestableResourceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDepleted)
	{
		HandleRespawnTimer(DeltaTime);
	}

	// Debug collision visualization (toggleable via console OR data asset)
	bool bShowCollisionDebug = CVarHarvestableDebugCollision.GetValueOnGameThread() || (ResourceData && ResourceData->bDebugShowCollision);
	if (bShowCollisionDebug)
	{
		if (InteractionVolume)
		{
			FVector BoxCenter = InteractionVolume->GetComponentLocation();
			FVector BoxExtent = InteractionVolume->GetScaledBoxExtent();
			FQuat BoxRotation = InteractionVolume->GetComponentQuat();

			// Green = QueryOnly, Red = physics enabled, Blue = NoCollision
			FColor BoxColor = FColor::Blue;
			if (InteractionVolume->GetCollisionEnabled() == ECollisionEnabled::QueryOnly)
			{
				BoxColor = FColor::Green;
			}
			else if (InteractionVolume->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
			{
				BoxColor = FColor::Red;
			}

			DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, BoxRotation, BoxColor, false, -1.0f, 0, 2.0f);

			// Draw collision info text
			FString CollisionInfo = FString::Printf(TEXT("InteractionVolume: %d\nMesh Collision: %d"),
				static_cast<int32>(InteractionVolume->GetCollisionEnabled()),
				ResourceMesh ? static_cast<int32>(ResourceMesh->GetCollisionEnabled()) : -1);
			DrawDebugString(GetWorld(), BoxCenter + FVector(0, 0, BoxExtent.Z + 50.0f), CollisionInfo, nullptr, FColor::White, 0.0f);
		}

		// Draw debug sphere for ResourceMesh bounds
		if (ResourceMesh && ResourceMesh->GetStaticMesh())
		{
			FBoxSphereBounds Bounds = ResourceMesh->Bounds;
			// Yellow sphere for mesh bounds
			DrawDebugSphere(GetWorld(), Bounds.Origin, Bounds.SphereRadius * 0.1f, 8, FColor::Yellow, false, -1.0f, 0, 1.0f);
		}
	}

	// Health display (toggleable via console OR data asset)
	bool bShowHealthDebug = CVarHarvestableShowHealth.GetValueOnGameThread() || (ResourceData && ResourceData->bDebugShowHealth);
	if (bShowHealthDebug && !bIsDepleted)
	{
		FVector ActorLocation = GetActorLocation();
		FVector HealthBarLocation = ActorLocation + FVector(0, 0, 200.0f); // 200 units above actor

		// Calculate health percentage
		float HealthPercent = MeshMaxHealth > 0.0f ? (CurrentHealth / MeshMaxHealth) : 1.0f;

		// Color gradient: Green -> Yellow -> Red
		FColor HealthColor;
		if (HealthPercent > 0.5f)
		{
			// Green to Yellow (100% -> 50%)
			float t = (1.0f - HealthPercent) * 2.0f; // 0 to 1
			HealthColor = FColor(FMath::RoundToInt(255 * t), 255, 0);
		}
		else
		{
			// Yellow to Red (50% -> 0%)
			float t = HealthPercent * 2.0f; // 0 to 1
			HealthColor = FColor(255, FMath::RoundToInt(255 * t), 0);
		}

		// Draw health bar background (black)
		float BarWidth = 100.0f;
		float BarHeight = 8.0f;
		FVector BarStart = HealthBarLocation - FVector(BarWidth / 2.0f, 0, 0);
		FVector BarEnd = HealthBarLocation + FVector(BarWidth / 2.0f, 0, 0);
		DrawDebugLine(GetWorld(), BarStart, BarEnd, FColor::Black, false, -1.0f, 0, BarHeight + 2.0f);

		// Draw health bar foreground (colored based on health)
		FVector HealthBarEnd = BarStart + FVector(BarWidth * HealthPercent, 0, 0);
		DrawDebugLine(GetWorld(), BarStart, HealthBarEnd, HealthColor, false, -1.0f, 0, BarHeight);

		// Draw health text
		FString HealthText = FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MeshMaxHealth);
		DrawDebugString(GetWorld(), HealthBarLocation + FVector(0, 0, 15.0f), HealthText, nullptr, FColor::White, 0.0f);
	}
}

// ============================================
// INTERACTABLE INTERFACE
// ============================================

void AHarvestableResourceActor::OnLookAt_Implementation(AActor* LookingActor)
{
	if (InteractableComponent && ResourceData && ResourceData->bHighlightOnLookAt)
	{
		InteractableComponent->SetHighlighted(true);
	}
}

void AHarvestableResourceActor::OnLookAway_Implementation(AActor* LookingActor)
{
	if (InteractableComponent)
	{
		InteractableComponent->SetHighlighted(false);
	}
}

void AHarvestableResourceActor::OnInteract_Implementation(AActor* InteractingActor)
{
	AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(InteractingActor);
	if (!Character || !ResourceData || bIsDepleted)
	{
		return;
	}

	// Handle direct pickup mode
	if (bIsDirectPickup && DirectPickupItem)
	{
		UInventoryComponent* Inventory = Character->GetInventoryComponent();
		if (Inventory)
		{
			int32 OutSlotIndex;
			if (Inventory->AddItem(DirectPickupItem, DirectPickupQuantity, OutSlotIndex))
			{
				// InventoryComponent::AddItem already shows pickup notification
				Destroy();
			}
		}
		return;
	}

	// Get equipped tool info
	EHarvestToolType ToolType = EHarvestToolType::None;
	int32 ToolTier = 0;
	float BaseDamage = ResourceData->HandHarvestDamage;

	AEquippableTool* EquippedTool = Character->GetEquippedTool();
	if (EquippedTool)
	{
		// Check if tool has harvest properties via metadata
		if (EquippedTool->ItemData)
		{
			const FString* ToolTypeStr = EquippedTool->ItemData->Metadata.Find(TEXT("HarvestToolType"));
			if (ToolTypeStr)
			{
				if (*ToolTypeStr == TEXT("Axe")) ToolType = EHarvestToolType::Axe;
				else if (*ToolTypeStr == TEXT("Pickaxe")) ToolType = EHarvestToolType::Pickaxe;
				else if (*ToolTypeStr == TEXT("Shovel")) ToolType = EHarvestToolType::Shovel;
				else if (*ToolTypeStr == TEXT("Sickle")) ToolType = EHarvestToolType::Sickle;
				else if (*ToolTypeStr == TEXT("Knife")) ToolType = EHarvestToolType::Knife;
			}

			const FString* TierStr = EquippedTool->ItemData->Metadata.Find(TEXT("ToolTier"));
			if (TierStr)
			{
				ToolTier = FCString::Atoi(**TierStr);
			}

			const FString* DamageStr = EquippedTool->ItemData->Metadata.Find(TEXT("HarvestDamage"));
			if (DamageStr)
			{
				BaseDamage = FCString::Atof(**DamageStr);
			}
		}
	}

	// Apply harvest damage
	ApplyHarvestDamage(Character, ToolType, ToolTier, BaseDamage);
}

FText AHarvestableResourceActor::GetInteractionName_Implementation() const
{
	// Use per-mesh display name if set
	if (!MeshDisplayName.IsEmpty())
	{
		return MeshDisplayName;
	}
	return FText::FromString(TEXT("Harvestable"));
}

FText AHarvestableResourceActor::GetInteractionPrompt_Implementation() const
{
	if (bIsDepleted)
	{
		return FText::FromString(TEXT("Depleted"));
	}

	if (bIsDirectPickup)
	{
		return FText::FromString(TEXT("Pick Up"));
	}

	if (ResourceData && !ResourceData->InteractionPrompt.IsEmpty())
	{
		return ResourceData->InteractionPrompt;
	}

	return FText::FromString(TEXT("Harvest"));
}

bool AHarvestableResourceActor::CanInteract_Implementation(AActor* InteractingActor) const
{
	return !bIsDepleted && ResourceData != nullptr;
}

float AHarvestableResourceActor::GetInteractionRange_Implementation() const
{
	if (ResourceData)
	{
		return ResourceData->InteractionRange;
	}
	return 300.0f;
}

bool AHarvestableResourceActor::ShouldHighlight_Implementation() const
{
	return !bIsDepleted && ResourceData && ResourceData->bHighlightOnLookAt;
}

// ============================================
// HARVESTING
// ============================================

bool AHarvestableResourceActor::ApplyHarvestDamage(AOutercorpCharacter* Harvester, EHarvestToolType ToolType, int32 ToolTier, float BaseDamage)
{
	if (!ResourceData || bIsDepleted)
	{
		return false;
	}

	// Get tool effectiveness
	float DamageMultiplier = 0.0f;
	float YieldMultiplier = 0.0f;
	bool bCanHarvest = ResourceData->GetToolEffectiveness(ToolType, ToolTier, DamageMultiplier, YieldMultiplier);

	if (!bCanHarvest)
	{
		// Incompatible tool - show feedback to player
		if (Harvester && Harvester->GetNotificationComponent())
		{
			Harvester->GetNotificationComponent()->ShowSimpleNotification(
				FText::FromString(TEXT("Wrong tool! Cannot harvest this resource.")),
				ENotificationType::Error,
				2.0f
			);
		}
		return false;
	}

	// Apply tool effectiveness to damage
	float ModifiedDamage = BaseDamage * DamageMultiplier;

	// Calculate actual damage after armor
	float ActualDamage = ResourceData->CalculateDamage(ModifiedDamage);

	if (ActualDamage <= 0.0f)
	{
		return false;
	}

	// Apply damage
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Max(0.0f, CurrentHealth - ActualDamage);

	// Update material strength parameter based on damage (trunk is material slot 2)
	if (ResourceMesh)
	{
		UMaterialInstanceDynamic* MatInstance = Cast<UMaterialInstanceDynamic>(ResourceMesh->GetMaterial(2));

		if (!MatInstance)
		{
			MatInstance = ResourceMesh->CreateAndSetMaterialInstanceDynamic(2);
		}

		if (MatInstance)
		{
			float DamageRatio = 1.0f - GetHealthPercent();
			MatInstance->SetScalarParameterValue(FName(TEXT("Strength")), DamageRatio);
			UE_LOG(LogTemp, Warning, TEXT("Updated Strength to %.2f on %s"), DamageRatio, *ResourceMesh->GetName());
		}
	}

	// Check if depleted
	bool bWasDepleted = CurrentHealth <= 0.0f;

	// Only spawn yields when fully depleted
	TArray<FHarvestYield> Yields;
	if (bWasDepleted)
	{
		// Use tool effectiveness yield multiplier
		float FinalYieldMultiplier = YieldMultiplier;

		// Roll for main yields from per-mesh data
		for (const FHarvestYield& Yield : MeshHarvestYields)
		{
			if (ToolTier < Yield.RequiredToolTier)
			{
				continue;
			}
			if (FMath::FRand() <= Yield.DropChance)
			{
				FHarvestYield RolledYield = Yield;
				int32 BaseQuantity = FMath::RandRange(Yield.MinQuantity, Yield.MaxQuantity);
				RolledYield.MinQuantity = FMath::RoundToInt(static_cast<float>(BaseQuantity) * FinalYieldMultiplier);
				RolledYield.MaxQuantity = RolledYield.MinQuantity;
				if (RolledYield.MinQuantity > 0)
				{
					Yields.Add(RolledYield);
				}
			}
		}

		// Add depletion bonus yields
		for (const FHarvestYield& Yield : MeshDepletionBonusYields)
		{
			if (ToolTier < Yield.RequiredToolTier)
			{
				continue;
			}
			if (FMath::FRand() <= Yield.DropChance)
			{
				FHarvestYield RolledYield = Yield;
				int32 BaseQuantity = FMath::RandRange(Yield.MinQuantity, Yield.MaxQuantity);
				RolledYield.MinQuantity = FMath::RoundToInt(static_cast<float>(BaseQuantity) * FinalYieldMultiplier);
				RolledYield.MaxQuantity = RolledYield.MinQuantity;
				if (RolledYield.MinQuantity > 0)
				{
					Yields.Add(RolledYield);
				}
			}
		}
	}

	// Spawn yield items and show notifications
	if (Yields.Num() > 0)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50.0f);
		SpawnYieldItems(Yields, SpawnLocation);

		// Show notification for each yield type
		if (UNotificationComponent* NotificationComp = Harvester->GetNotificationComponent())
		{
			for (const FHarvestYield& Yield : Yields)
			{
				if (Yield.ItemData && Yield.MinQuantity > 0)
				{
					UTexture2D* ItemIcon = Yield.ItemData->ItemIcon.LoadSynchronous();
					NotificationComp->ShowItemHarvestedNotification(Yield.ItemData->ItemName, Yield.MinQuantity, ItemIcon);
				}
			}
		}
	}

	// Play feedback
	PlayHarvestFeedback(GetActorLocation());

	// Update visual state
	UpdateVisualState();

	// Broadcast harvested event
	OnResourceHarvested.Broadcast(this, ActualDamage);

	// Handle depletion
	if (bWasDepleted)
	{
		Deplete();
	}

	return true;
}

void AHarvestableResourceActor::SpawnYieldItems(const TArray<FHarvestYield>& Yields, const FVector& SpawnLocation)
{
	TSubclassOf<APickupableItem> PickupClass = GetPickupItemClass();
	if (!PickupClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (const FHarvestYield& Yield : Yields)
	{
		if (!Yield.ItemData || Yield.MinQuantity <= 0)
		{
			continue;
		}

		// Calculate base spawn location with yield-specific offset
		FVector YieldBaseLocation = SpawnLocation + Yield.SpawnOffset;

		// Spawn individual items for each unit of quantity
		for (int32 i = 0; i < Yield.MinQuantity; ++i)
		{
			// Calculate offset based on index to spread items in a spiral pattern
			float Angle = i * 137.5f * (PI / 180.0f); // Golden angle for even distribution
			// Only expand the spiral if SpawnRandomRadius > 0, otherwise keep items close together
			float SpiralExpansion = (Yield.SpawnRandomRadius > 0.0f) ? (i * 40.0f) : (i * 5.0f);
			float Radius = Yield.SpawnRandomRadius + SpiralExpansion;
			FVector RandomOffset = FVector(
				FMath::Cos(Angle) * Radius,
				FMath::Sin(Angle) * Radius,
				Yield.SpawnHeightOffset + (i * 20.0f) // Use yield's height offset
			);

			FVector FinalLocation = YieldBaseLocation + RandomOffset;
			FRotator SpawnRotation = FRotator(0, FMath::RandRange(0.0f, 360.0f), 0);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			APickupableItem* SpawnedItem = World->SpawnActor<APickupableItem>(PickupClass, FinalLocation, SpawnRotation, SpawnParams);
			if (SpawnedItem)
			{
				// Initialize the item properly (this sets up mesh, interaction, etc.)
				SpawnedItem->InitializeItem(Yield.ItemData, 1);

				// Configure physics based on yield settings
				if (UStaticMeshComponent* MeshComp = SpawnedItem->ItemMesh)
				{
					if (Yield.bEnablePhysics)
					{
						MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
						MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
						MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
						MeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
						MeshComp->SetSimulatePhysics(true);

						// Radial impulse outward from center + upward
						FVector OutwardDir = (FinalLocation - YieldBaseLocation).GetSafeNormal2D();
						FVector Impulse = OutwardDir * FMath::RandRange(150.0f, 300.0f) + FVector(0, 0, FMath::RandRange(300.0f, 500.0f));
						MeshComp->AddImpulse(Impulse);
					}
					else
					{
						// Static spawn - no physics, just query collision for pickup
						MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
						MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
						MeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
						MeshComp->SetSimulatePhysics(false);
					}
				}
			}
		}
	}
}

float AHarvestableResourceActor::GetHealthPercent() const
{
	if (MeshMaxHealth <= 0.0f)
	{
		return 0.0f;
	}
	return CurrentHealth / MeshMaxHealth;
}

// ============================================
// INITIALIZATION
// ============================================

void AHarvestableResourceActor::InitializeFromData(UHarvestableResourceData* Data)
{
	ResourceData = Data;
	if (!ResourceData)
	{
		return;
	}

	// Set health from per-mesh value
	CurrentHealth = MeshMaxHealth;
	bIsDepleted = false;

	// Store original scale
	OriginalScale = GetActorScale3D();

	// Look up direct pickup settings from source mesh if not already set
	if (ResourceMesh && ResourceMesh->GetStaticMesh())
	{
		const FHarvestableSourceMesh* MeshEntry = ResourceData->GetSourceMeshEntry(ResourceMesh->GetStaticMesh());
		if (MeshEntry)
		{
			bIsDirectPickup = MeshEntry->bDirectPickup;
			DirectPickupItem = MeshEntry->DirectPickupItem;
			DirectPickupQuantity = MeshEntry->DirectPickupQuantity;
		}
	}

	// Use the mesh's actual collision for interaction traces instead of the box volume
	// Enable query-only collision on the ResourceMesh using its native collision shape
	if (ResourceMesh && ResourceMesh->GetStaticMesh())
	{
		// Use the mesh's built-in collision for visibility traces
		ResourceMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ResourceMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		ResourceMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

		// Disable the box InteractionVolume since we're using mesh collision
		InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Store original mesh
	if (ResourceMesh)
	{
		OriginalMesh = ResourceMesh->GetStaticMesh();
	}

	UpdateVisualState();
}

void AHarvestableResourceActor::InitializeFromPCGInstance(UHarvestableResourceData* Data, UInstancedStaticMeshComponent* ISMComponent, int32 InstanceIndex, const FTransform& InstanceTransform)
{
	// Store PCG reference info
	SourceISMComponent = ISMComponent;
	PCGInstanceIndex = InstanceIndex;
	OriginalTransform = InstanceTransform;

	// Get per-mesh settings from data asset
	MeshDisplayName = FText::GetEmpty();
	MeshMaxHealth = 100.0f;
	MeshHarvestYields.Empty();
	MeshDepletionBonusYields.Empty();
	bIsDirectPickup = false;
	DirectPickupItem = nullptr;
	DirectPickupQuantity = 1;
	if (Data && ISMComponent)
	{
		UStaticMesh* SourceMesh = ISMComponent->GetStaticMesh();
		const FHarvestableSourceMesh* MeshEntry = Data->GetSourceMeshEntry(SourceMesh);
		if (MeshEntry)
		{
			MeshDisplayName = MeshEntry->DisplayName;
			MeshMaxHealth = MeshEntry->MaxHealth;
			MeshHarvestYields = MeshEntry->HarvestYields;
			MeshDepletionBonusYields = MeshEntry->DepletionBonusYields;
			bIsDirectPickup = MeshEntry->bDirectPickup;
			DirectPickupItem = MeshEntry->DirectPickupItem;
			DirectPickupQuantity = MeshEntry->DirectPickupQuantity;
		}
	}

	// Set transform to match instance
	SetActorTransform(InstanceTransform);

	// Copy mesh from ISM
	if (ISMComponent && ResourceMesh)
	{
		ResourceMesh->SetStaticMesh(ISMComponent->GetStaticMesh());

		// Copy materials from ISM
		for (int32 i = 0; i < ISMComponent->GetNumMaterials(); i++)
		{
			ResourceMesh->SetMaterial(i, ISMComponent->GetMaterial(i));
		}

		// For the trunk material (slot 2), create a dynamic instance from the source material
		// This ensures we can update the Strength parameter for the WPO effect
		if (ISMComponent->GetNumMaterials() > 2)
		{
			UMaterialInterface* TrunkMaterial = ISMComponent->GetMaterial(2);
			if (TrunkMaterial)
			{
				// Create dynamic instance from the trunk material itself
				UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(TrunkMaterial, ResourceMesh);
				ResourceMesh->SetMaterial(2, DynMat);
				DynMat->SetScalarParameterValue(FName(TEXT("Strength")), 0.0f);
			}
		}
	}

	// Initialize from data (will apply health multiplier)
	InitializeFromData(Data);
}

// ============================================
// STATE MANAGEMENT
// ============================================

void AHarvestableResourceActor::UpdateVisualState()
{
	if (!ResourceData || !ResourceMesh)
	{
		return;
	}

	float HealthPercent = GetHealthPercent();

	// Find appropriate destruction stage
	const FHarvestStage* Stage = ResourceData->GetStageForHealth(HealthPercent);
	if (Stage)
	{
		// Apply stage mesh if specified
		if (Stage->StageMesh)
		{
			ResourceMesh->SetStaticMesh(Stage->StageMesh);
		}

		// Apply stage material if specified
		if (Stage->StageMaterial)
		{
			ResourceMesh->SetMaterial(0, Stage->StageMaterial);
		}

		// Apply scale modifier
		SetActorScale3D(OriginalScale * Stage->ScaleModifier);
	}
}

void AHarvestableResourceActor::Deplete()
{
	bIsDepleted = true;
	CurrentHealth = 0.0f;

	// Play depletion feedback
	PlayDepletionFeedback();

	// Start respawn timer if applicable
	if (ResourceData && ResourceData->bCanRespawn)
	{
		RespawnTimeRemaining = ResourceData->RespawnTime;
	}

	// Hide mesh
	if (ResourceMesh)
	{
		ResourceMesh->SetVisibility(false);
		// Disable collision on mesh so traces can hit spawned items below
		ResourceMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Disable interaction
	if (InteractionVolume)
	{
		InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Broadcast event
	OnResourceDepleted.Broadcast(this);
}

void AHarvestableResourceActor::Respawn()
{
	if (!ResourceData)
	{
		return;
	}

	bIsDepleted = false;
	CurrentHealth = MeshMaxHealth;
	RespawnTimeRemaining = 0.0f;

	// Reset mesh and physics
	if (ResourceMesh)
	{
		if (OriginalMesh)
		{
			ResourceMesh->SetStaticMesh(OriginalMesh);
		}
		ResourceMesh->SetVisibility(true);

		// Reset physics
		ResourceMesh->SetSimulatePhysics(false);
		ResourceMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ResourceMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		ResourceMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	// Reset scale and rotation
	SetActorScale3D(OriginalScale);
	SetActorTransform(OriginalTransform);

	// Re-enable interaction
	if (InteractionVolume)
	{
		InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	UpdateVisualState();

	// Broadcast event
	OnResourceRespawned.Broadcast(this);
}

void AHarvestableResourceActor::RestorePCGInstance()
{
	// If this was spawned from PCG, restore the original instance
	if (SourceISMComponent.IsValid() && PCGInstanceIndex != INDEX_NONE)
	{
		// Note: Adding instances back to ISM at runtime is complex
		// For now, we just respawn in place
		// A more sophisticated system would track removed instances and restore them
		Respawn();
	}
}

void AHarvestableResourceActor::HandleRespawnTimer(float DeltaTime)
{
	if (!bIsDepleted || !ResourceData || !ResourceData->bCanRespawn)
	{
		return;
	}

	RespawnTimeRemaining -= DeltaTime;
	if (RespawnTimeRemaining <= 0.0f)
	{
		Respawn();
	}
}

void AHarvestableResourceActor::PlayHarvestFeedback(const FVector& HitLocation)
{
	if (!ResourceData)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Play sound
	if (ResourceData->HarvestSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ResourceData->HarvestSound, HitLocation);
	}

	// Play particle effect
	if (ResourceData->HarvestNiagaraEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World,
			ResourceData->HarvestNiagaraEffect,
			HitLocation,
			GetActorRotation()
		);
	}
	else if (ResourceData->HarvestParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			World,
			ResourceData->HarvestParticle,
			HitLocation,
			GetActorRotation()
		);
	}

	// Play camera shake
	if (ResourceData->HarvestCameraShake)
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC)
		{
			PC->ClientStartCameraShake(ResourceData->HarvestCameraShake, ResourceData->CameraShakeIntensity);
		}
	}
}

void AHarvestableResourceActor::PlayDepletionFeedback()
{
	if (!ResourceData)
	{
		return;
	}

	// Play depletion sound
	if (ResourceData->DepletionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ResourceData->DepletionSound, GetActorLocation());
	}
}

TSubclassOf<APickupableItem> AHarvestableResourceActor::GetPickupItemClass() const
{
	// Try to get from game mode or use default
	// For now, return the default pickupable item class
	// This could be configured per-project

	static TSubclassOf<APickupableItem> CachedClass;
	if (!CachedClass)
	{
		CachedClass = APickupableItem::StaticClass();
	}
	return CachedClass;
}
