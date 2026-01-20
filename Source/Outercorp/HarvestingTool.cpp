// HarvestingTool.cpp

#include "HarvestingTool.h"
#include "HarvestableResourceActor.h"
#include "OutercorpCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"

AHarvestingTool::AHarvestingTool()
{
	// Set default values
	HarvestToolType = EHarvestToolType::Axe;
	ToolTier = 1;
	HarvestDamage = 20.0f;
	HarvestRange = 250.0f;
	SwingTime = 0.5f;
	HitCameraShakeIntensity = 0.3f;

	bIsSwinging = false;
	TargetedResource = nullptr;

	// Override base tool properties
	UsageCooldown = SwingTime;
	bRequiresContinuousHold = true;
}

void AHarvestingTool::BeginPlay()
{
	Super::BeginPlay();

	// Sync cooldown with swing time
	UsageCooldown = SwingTime;
}

void AHarvestingTool::StartPrimaryUse_Implementation()
{
	Super::StartPrimaryUse_Implementation();

	// Start continuous harvesting
	if (CanUseTool() && !bIsSwinging)
	{
		PerformHarvestSwing();
	}
}

void AHarvestingTool::StopPrimaryUse_Implementation()
{
	Super::StopPrimaryUse_Implementation();

	// Clear swing timer if stopping
	GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);
	bIsSwinging = false;
}

void AHarvestingTool::PerformPrimaryAction_Implementation()
{
	// Single swing action
	if (!bIsSwinging)
	{
		PerformHarvestSwing();
	}
}

void AHarvestingTool::PerformHarvestSwing()
{
	if (bIsSwinging || !OwningCharacter)
	{
		return;
	}

	bIsSwinging = true;

	// Play swing feedback
	PlaySwingFeedback();

	// Play animation
	if (PrimaryUseAnimation)
	{
		PlayUseAnimation(PrimaryUseAnimation);
	}

	// Find target resource
	TargetedResource = FindTargetResource();

	if (TargetedResource)
	{
		// Check if we can harvest this resource
		if (CanHarvestResource(TargetedResource))
		{
			// Apply damage
			bool bDamageApplied = TargetedResource->ApplyHarvestDamage(
				OwningCharacter,
				HarvestToolType,
				ToolTier,
				HarvestDamage
			);

			if (bDamageApplied)
			{
				// Play hit feedback
				PlayHitFeedback(TargetedResource->GetActorLocation());

				// Consume durability
				ConsumeDurability(DurabilityCostPerUse);
			}
		}
		else
		{
			// Wrong tool type - play miss sound
			PlayMissFeedback();
		}
	}
	else
	{
		// No target - play miss sound
		PlayMissFeedback();
	}

	// Set timer for swing completion
	GetWorld()->GetTimerManager().SetTimer(
		SwingTimerHandle,
		this,
		&AHarvestingTool::OnSwingComplete,
		SwingTime,
		false
	);
}

AHarvestableResourceActor* AHarvestingTool::FindTargetResource()
{
	if (!OwningCharacter)
	{
		return nullptr;
	}

	UCameraComponent* Camera = OwningCharacter->GetFirstPersonCameraComponent();
	if (!Camera)
	{
		return nullptr;
	}

	FVector StartLocation = Camera->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * HarvestRange);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwningCharacter);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	// Perform line trace
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams
	);

	if (bHit && HitResult.GetActor())
	{
		// Check if we hit a harvestable resource
		AHarvestableResourceActor* Resource = Cast<AHarvestableResourceActor>(HitResult.GetActor());
		if (Resource)
		{
			return Resource;
		}
	}

	return nullptr;
}

bool AHarvestingTool::CanHarvestResource(AHarvestableResourceActor* Resource) const
{
	if (!Resource || !Resource->ResourceData)
	{
		return false;
	}

	return Resource->ResourceData->IsToolTypeValid(HarvestToolType, ToolTier);
}

void AHarvestingTool::OnSwingComplete()
{
	bIsSwinging = false;

	// If still holding primary use, swing again
	if (CurrentUsageMode == EToolUsageMode::Primary && bIsInUse)
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

void AHarvestingTool::PlayHitFeedback(const FVector& HitLocation)
{
	UWorld* World = GetWorld();
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
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World,
			HitEffect,
			HitLocation,
			FRotator::ZeroRotator
		);
	}

	// Play camera shake
	if (HitCameraShake)
	{
		APlayerController* PC = World->GetFirstPlayerController();
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
