// HarvestingTool.cpp

#include "HarvestingTool.h"
#include "HarvestableResourceActor.h"
#include "HarvestableResourceData.h"
#include "InventoryItemData.h"
#include "OutercorpCharacter.h"
#include "InteractionManagerComponent.h"
#include "PCGHarvestableManager.h"
#include "Camera/CameraComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"

// Console variable for debug visualization
static TAutoConsoleVariable<int32> CVarHarvestingToolDebugRaycast(
	TEXT("Harvesting.Debug.ToolRaycast"),
	0,
	TEXT("Show raycast debug visualization for harvesting tools.\n")
		TEXT("0: Disabled (default)\n")
			TEXT("1: Enabled"),
	ECVF_Default);

AHarvestingTool::AHarvestingTool()
{
	// Set default values
	HarvestToolType = EHarvestToolType::Axe;
	ToolTier = 1;
	HarvestDamage = 20.0f;
	HarvestRange = 250.0f;
	SwingTime = 0.5f;
	HitTime = 0.2f;
	HitCameraShakeIntensity = 0.3f;

	bIsSwinging = false;
	bHitDelivered = false;
	TargetedResource = nullptr;

	// Override base tool properties
	UsageCooldown = SwingTime;
	bRequiresContinuousHold = false;
}

void AHarvestingTool::BeginPlay()
{
	Super::BeginPlay();

	// Sync cooldown with swing time
	UsageCooldown = SwingTime;
}

void AHarvestingTool::InitializeFromItemData(UInventoryItemData *InItemData)
{
	// Call base class first
	Super::InitializeFromItemData(InItemData);

	if (!InItemData)
	{
		return;
	}

	// Map EToolType to EHarvestToolType
	switch (InItemData->ToolType)
	{
	case EToolType::Axe:
		HarvestToolType = EHarvestToolType::Axe;
		break;
	case EToolType::Pickaxe:
		HarvestToolType = EHarvestToolType::Pickaxe;
		break;
	case EToolType::Shovel:
		HarvestToolType = EHarvestToolType::Shovel;
		break;
	case EToolType::Sickle:
		HarvestToolType = EHarvestToolType::Sickle;
		break;
	case EToolType::Knife:
		HarvestToolType = EHarvestToolType::Knife;
		break;
	default:
		HarvestToolType = EHarvestToolType::Any;
		break;
	}

	// Copy harvesting properties
	ToolTier = InItemData->ToolTier;
	HarvestDamage = InItemData->ToolBaseDamage;
	SwingTime = InItemData->ToolUsageCooldown;

	// Validate SwingTime - prevent 0.0s cooldown
	if (SwingTime <= 0.05f)
	{
		SwingTime = 0.5f; // Default to 0.5s if invalid
	}

	HitTime = InItemData->ToolHitTime;

	// Note: We no longer force a default HitTime.
	// Users should set ToolHitTime > 0 in DataAsset for a timer-based hit,
	// OR ensure the animation has a valid AnimNotify_ToolHit for frame-perfect hits.

	// Copy feedback sounds/effects
	SwingSound = InItemData->ToolUseSound;
	HitSound = InItemData->ToolHitSound;
	HitEffect = InItemData->ToolUseEffect;

	/*
	UE_LOG(LogTemp, Log, TEXT("HarvestingTool initialized: Type=%d, Tier=%d, Damage=%.1f"),
		   static_cast<int32>(HarvestToolType), ToolTier, HarvestDamage);
	*/
}

void AHarvestingTool::StartPrimaryUse_Implementation()
{
	// Don't call Super - we handle everything ourselves for harvesting tools
	if (!CanUseTool())
	{
		return;
	}

	bIsInUse = true;
	CurrentUsageMode = EToolUsageMode::Primary;

	// Start the first swing
	PerformHarvestSwing();
}

void AHarvestingTool::StopPrimaryUse_Implementation()
{
	bIsInUse = false;
	CurrentUsageMode = EToolUsageMode::None;

	// Do NOT clear timers here.
	// We want the current swing to finish (Trigger hit, then OnSwingComplete to reset bIsSwinging).
	// If we clear SwingTimerHandle, OnSwingComplete never runs, and bIsSwinging stays true forever.

	// OnSwingComplete handles the check for bIsInUse to decide whether to loop or stop.
}

void AHarvestingTool::PerformPrimaryAction_Implementation()
{
	// This is called by base class continuous loop - we don't use it
	// Harvesting is driven by PerformHarvestSwing and OnSwingComplete
}

void AHarvestingTool::PerformHarvestSwing()
{
	if (bIsSwinging || !OwningCharacter)
	{
		return;
	}

	// Play animation - the actual harvest happens when the ToolHit notify fires
	bool bAnimationStarted = false;
	if (PrimaryUseAnimation)
	{
		bAnimationStarted = PlayUseAnimation(PrimaryUseAnimation);
	}

	// Only proceed if animation actually started (not queued)
	if (!bAnimationStarted)
	{
		return;
	}

	bIsSwinging = true;
	bHitDelivered = false;

	/*
	UE_LOG(LogTemp, Log, TEXT("HarvestingTool: PerformHarvestSwing - Starting swing (SwingTime: %.2f, HitTime: %.2f)"),
		   SwingTime, HitTime);
	*/

	// Play swing feedback (whoosh sound) at start of swing
	PlaySwingFeedback();

	// Set timer for swing completion (allows next swing if still holding)
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			SwingTimerHandle,
			this,
			&AHarvestingTool::OnSwingComplete,
			SwingTime,
			false);

		// Schedule hit event if HitTime is set (fallback for missing AnimNotify)
		if (HitTime > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(
				HitTimerHandle,
				this,
				&AHarvestingTool::OnToolHitNotify_Implementation,
				HitTime,
				false);
		}
	}
}

void AHarvestingTool::OnToolHitNotify_Implementation()
{
	/*
	UE_LOG(LogTemp, Log, TEXT("HarvestingTool: OnToolHitNotify_Implementation called!"));
	*/

	// Prevent double hits (from both Timer and AnimNotify)
	if (bHitDelivered)
	{
		// UE_LOG(LogTemp, Verbose, TEXT("HarvestingTool: Hit already delivered for this swing"));
		return;
	}
	bHitDelivered = true;

	// This is called from the ToolHit anim notify at the moment of impact
	if (!OwningCharacter)
	{
		// UE_LOG(LogTemp, Warning, TEXT("HarvestingTool: OnToolHitNotify - no OwningCharacter!"));
		return;
	}

	// Find target resource at the moment of impact (need this first to check debug settings)
	TargetedResource = FindTargetResource();

	// Debug: Draw player position before hit (toggleable via console OR target's data asset)
	bool bShowRaycastDebug = CVarHarvestingToolDebugRaycast.GetValueOnGameThread() ||
							 (TargetedResource && TargetedResource->ResourceData && TargetedResource->ResourceData->bDebugShowToolRaycast);
	if (bShowRaycastDebug)
	{
		FVector PlayerPos = OwningCharacter->GetActorLocation();
		DrawDebugSphere(GetWorld(), PlayerPos, 30.0f, 8, FColor::Cyan, false, 3.0f, 0, 2.0f);
		DrawDebugString(GetWorld(), PlayerPos + FVector(0, 0, 100.0f),
						FString::Printf(TEXT("Player Z: %.1f"), PlayerPos.Z), nullptr, FColor::Cyan, 3.0f);
	}
	/*
	UE_LOG(LogTemp, Log, TEXT("HarvestingTool: OnToolHitNotify - TargetedResource: %s"),
		   TargetedResource ? *TargetedResource->GetName() : TEXT("None"));
	*/

	if (TargetedResource)
	{
		// Skip direct pickup items - they can only be picked up with E key
		if (TargetedResource->bIsDirectPickup)
		{
			return;
		}

		// Apply damage - the resource actor will check tool effectiveness
		bool bDamageApplied = TargetedResource->ApplyHarvestDamage(
			OwningCharacter,
			HarvestToolType,
			ToolTier,
			HarvestDamage);

		if (bDamageApplied)
		{
			// Get actual impact point by tracing to the resource
			FVector HitLocation = GetImpactPoint(TargetedResource);

			// Play hit feedback (sound, effects, camera shake)
			PlayHitFeedback(HitLocation);

			// Consume durability
			ConsumeDurability(DurabilityCostPerUse);

			// Debug: Draw player position after hit
			if (bShowRaycastDebug)
			{
				FVector PlayerPos = OwningCharacter->GetActorLocation();
				FVector PlayerPosAfter = OwningCharacter->GetActorLocation();
				DrawDebugSphere(GetWorld(), PlayerPosAfter, 30.0f, 8, FColor::Magenta, false, 3.0f, 0, 2.0f);
				DrawDebugString(GetWorld(), PlayerPosAfter + FVector(0, 0, 150.0f),
								FString::Printf(TEXT("After Hit Z: %.1f (Delta: %.1f)"), PlayerPosAfter.Z, PlayerPosAfter.Z - PlayerPos.Z),
								nullptr, FColor::Magenta, 3.0f);
			}
		}
		else
		{
			// Incompatible tool or no damage dealt - play miss sound
			PlayMissFeedback();
		}
	}
	else
	{
		// No target - play miss sound
		PlayMissFeedback();
	}
}

AHarvestableResourceActor *AHarvestingTool::FindTargetResource()
{
	if (!OwningCharacter)
	{
		return nullptr;
	}

	// Use the InteractionManagerComponent - it already tracks what player is looking at
	UInteractionManagerComponent *InteractionManager = OwningCharacter->GetInteractionManagerComponent();
	if (!InteractionManager)
	{
		return nullptr;
	}

	// Check if looking at an already-converted HarvestableResourceActor
	// Use more robust detection for finding resources - simple sphere check in frobt of player
	FVector Start = OwningCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector Forward = OwningCharacter->GetFirstPersonCameraComponent()->GetForwardVector();
	FVector End = Start + (Forward * HarvestRange);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwningCharacter);
	QueryParams.AddIgnoredActor(this);

	// First try line trace for precision
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
	{
		AActor *HitActor = HitResult.GetActor();
		if (HitActor)
		{
			// Check for HarvestableResourceActor
			if (AHarvestableResourceActor *Resource = Cast<AHarvestableResourceActor>(HitActor))
			{
				return Resource;
			}

			// Check for attached components (some resources might be composed of components)
			if (AHarvestableResourceActor *Resource = Cast<AHarvestableResourceActor>(HitActor->GetOwner()))
			{
				return Resource;
			}
		}
	}

	// Fallback to Interaction Manager logic if direct trace fails
	// Use the InteractionManagerComponent - it already tracks what player is looking at
	AActor *CurrentInteractable = InteractionManager->GetCurrentInteractableActor();
	if (CurrentInteractable)
	{
		AHarvestableResourceActor *Resource = Cast<AHarvestableResourceActor>(CurrentInteractable);
		if (Resource)
		{
			return Resource;
		}
	}

	// Check if looking at a harvestable ISM that needs conversion
	if (InteractionManager->IsLookingAtHarvestableISM())
	{
		// Get the PCGHarvestableManager to convert the instance
		APCGHarvestableManager *Manager = InteractionManager->GetHarvestableManager();
		TWeakObjectPtr<UInstancedStaticMeshComponent> PendingISM = InteractionManager->GetPendingISMComponent();
		int32 PendingIndex = InteractionManager->GetPendingInstanceIndex();

		if (Manager && PendingISM.IsValid() && PendingIndex != INDEX_NONE)
		{
			// Convert the ISM instance to a HarvestableResourceActor
			AHarvestableResourceActor *SpawnedActor = Manager->ConvertInstance(PendingISM.Get(), PendingIndex);

			if (SpawnedActor)
			{
				// Clear the pending state since we converted it
				InteractionManager->ClearPendingISMState();
				return SpawnedActor;
			}
		}
	}

	return nullptr;
}

void AHarvestingTool::OnSwingComplete()
{
	bIsSwinging = false;

	// Only auto-swing again if continuous hold is enabled AND still holding
	if (bRequiresContinuousHold && CurrentUsageMode == EToolUsageMode::Primary && bIsInUse)
	{
		PerformHarvestSwing();
	}
}

void AHarvestingTool::PlaySwingFeedback()
{
	if (SwingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SwingSound, GetActorLocation());
	}
}

void AHarvestingTool::PlayHitFeedback(const FVector &HitLocation)
{
	UWorld *World = GetWorld();
	if (!World)
	{
		return;
	}

	// Play hit sound
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, HitLocation);
	}

	// Play hit effect
	if (HitEffect)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawning HitEffect '%s' at location: %s"),
			   *HitEffect->GetName(), *HitLocation.ToString());

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World,
			HitEffect,
			HitLocation,
			FRotator::ZeroRotator,
			FVector(1.0f), // Scale
			true,		   // bAutoDestroy
			true		   // bAutoActivate
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayHitFeedback: HitEffect is NULL!"));
	}

	// Play camera shake
	if (HitCameraShake)
	{
		APlayerController *PC = World->GetFirstPlayerController();
		if (PC)
		{
			PC->ClientStartCameraShake(HitCameraShake, HitCameraShakeIntensity);
		}
	}
}

void AHarvestingTool::PlayMissFeedback()
{
	if (MissSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, MissSound, GetActorLocation());
	}
}

FVector AHarvestingTool::GetImpactPoint(AHarvestableResourceActor *Resource) const
{
	if (!Resource || !OwningCharacter)
	{
		return Resource ? Resource->GetActorLocation() : FVector::ZeroVector;
	}

	// Trace from camera to the resource to get actual impact point
	UCameraComponent *Camera = OwningCharacter->GetFirstPersonCameraComponent();
	if (!Camera)
	{
		return Resource->GetActorLocation();
	}

	FVector StartLocation = Camera->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * HarvestRange * 2.0f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwningCharacter);
	QueryParams.AddIgnoredActor(this);

	UWorld *World = GetWorld();
	bool bHit = World && World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	// Debug raycast visualization (toggleable via console OR target's data asset)
	bool bShowRaycastDebug = CVarHarvestingToolDebugRaycast.GetValueOnGameThread() ||
							 (Resource && Resource->ResourceData && Resource->ResourceData->bDebugShowToolRaycast);
	if (bShowRaycastDebug && World)
	{
		// Draw the raycast line
		FColor LineColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(World, StartLocation, EndLocation, LineColor, false, 3.0f, 0, 2.0f);

		// Draw start point (camera)
		DrawDebugSphere(World, StartLocation, 10.0f, 8, FColor::Blue, false, 3.0f, 0, 1.0f);

		if (bHit)
		{
			// Draw impact point
			DrawDebugSphere(World, HitResult.ImpactPoint, 15.0f, 12, FColor::Green, false, 3.0f, 0, 2.0f);

			// Draw what we hit
			FString HitInfo = FString::Printf(TEXT("Hit: %s\nComponent: %s\nChannel Block: Visibility"),
											  HitResult.GetActor() ? *HitResult.GetActor()->GetName() : TEXT("None"),
											  HitResult.GetComponent() ? *HitResult.GetComponent()->GetName() : TEXT("None"));
			DrawDebugString(World, HitResult.ImpactPoint + FVector(0, 0, 30.0f), HitInfo, nullptr, FColor::Green, 3.0f);
		}
		else
		{
			// Draw end point (no hit)
			DrawDebugSphere(World, EndLocation, 10.0f, 8, FColor::Red, false, 3.0f, 0, 1.0f);
			DrawDebugString(World, EndLocation, TEXT("No Hit"), nullptr, FColor::Red, 3.0f);
		}
	}

	if (bHit)
	{
		return HitResult.ImpactPoint;
	}

	// Fallback to actor location
	return Resource->GetActorLocation();
}
