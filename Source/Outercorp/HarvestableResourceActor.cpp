// HarvestableResourceActor.cpp

#include "HarvestableResourceActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "InteractableComponent.h"
#include "OutercorpCharacter.h"
#include "PickupableItem.h"
#include "EquippableTool.h"
#include "NotificationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AHarvestableResourceActor::AHarvestableResourceActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	// Create mesh component - collision disabled since ISM already has collision
	// and we want to avoid double collision when converting instances
	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
	ResourceMesh->SetupAttachment(RootSceneComponent);
	ResourceMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create interaction volume
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootSceneComponent);
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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
}

void AHarvestableResourceActor::BeginPlay()
{
	Super::BeginPlay();

	if (ResourceData)
	{
		InitializeFromData(ResourceData);
	}
}

void AHarvestableResourceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDepleted)
	{
		HandleRespawnTimer(DeltaTime);
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

	// Check if tool is valid for this resource
	bool bToolValid = ResourceData->IsToolTypeValid(ToolType, ToolTier);
	if (!bToolValid)
	{
		// Wrong tool - could show feedback to player here
		return false;
	}

	// Calculate actual damage after armor
	float ActualDamage = ResourceData->CalculateDamage(BaseDamage);

	if (ActualDamage <= 0.0f)
	{
		return false;
	}

	// Apply damage
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Max(0.0f, CurrentHealth - ActualDamage);

	// Check if depleted
	bool bWasDepleted = CurrentHealth <= 0.0f;

	// Roll for yields from per-mesh data
	TArray<FHarvestYield> Yields;
	float YieldMultiplier = FMath::Pow(ResourceData->ToolTierYieldMultiplier, static_cast<float>(FMath::Max(0, ToolTier - ResourceData->MinimumToolTier)));

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
			RolledYield.MinQuantity = FMath::RoundToInt(static_cast<float>(BaseQuantity) * YieldMultiplier);
			RolledYield.MaxQuantity = RolledYield.MinQuantity;
			if (RolledYield.MinQuantity > 0)
			{
				Yields.Add(RolledYield);
			}
		}
	}

	// Add depletion bonus yields if depleted
	if (bWasDepleted)
	{
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
				RolledYield.MinQuantity = FMath::RoundToInt(static_cast<float>(BaseQuantity) * YieldMultiplier);
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

	// Setup interaction volume based on mesh bounds
	if (ResourceMesh && ResourceMesh->GetStaticMesh())
	{
		FBox MeshBounds = ResourceMesh->GetStaticMesh()->GetBoundingBox();
		FVector BoundsExtent = MeshBounds.GetExtent() * GetActorScale3D();
		InteractionVolume->SetBoxExtent(BoundsExtent * 1.2f); // Slightly larger than mesh
		InteractionVolume->SetRelativeLocation(MeshBounds.GetCenter());
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
		}
	}

	// Set transform to match instance
	SetActorTransform(InstanceTransform);

	// Copy mesh from ISM
	if (ISMComponent && ResourceMesh)
	{
		ResourceMesh->SetStaticMesh(ISMComponent->GetStaticMesh());

		// Copy materials
		for (int32 i = 0; i < ISMComponent->GetNumMaterials(); i++)
		{
			ResourceMesh->SetMaterial(i, ISMComponent->GetMaterial(i));
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

	// Hide mesh or switch to depleted visual
	if (ResourceMesh)
	{
		ResourceMesh->SetVisibility(false);
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

	// Reset mesh
	if (ResourceMesh)
	{
		if (OriginalMesh)
		{
			ResourceMesh->SetStaticMesh(OriginalMesh);
		}
		ResourceMesh->SetVisibility(true);
	}

	// Reset scale
	SetActorScale3D(OriginalScale);

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
