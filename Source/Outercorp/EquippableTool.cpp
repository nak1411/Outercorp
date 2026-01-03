// Copyright Epic Games, Inc. All Rights Reserved.

#include "EquippableTool.h"
#include "OutercorpCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

AEquippableTool::AEquippableTool()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create first-person mesh component
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(Root);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonMesh->SetCastShadow(false);
	FirstPersonMesh->SetVisibility(false); // Hidden until equipped

	// Ensure mesh updates in sync with parent skeletal mesh to avoid lag
	FirstPersonMesh->bUseAttachParentBound = true;
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	// Default values
	AttachSocketName = FName("GripPoint");
	RelativeTransform = FTransform::Identity;
	bRequiresContinuousHold = false;
	UsageCooldown = 0.5f;
	CurrentUsageMode = EToolUsageMode::None;
	bIsEquipped = false;
	bIsInUse = false;
	bIsOnCooldown = false;
	OwningCharacter = nullptr;
	MaxDurability = 0.0f; // Infinite by default
	CurrentDurability = 0.0f;
	DurabilityCostPerUse = 1.0f;
	ThirdPersonSkeletalMesh = nullptr;
	FirstPersonSkeletalMesh = nullptr;
	PrimaryUseAnimation = nullptr;
	SecondaryUseAnimation = nullptr;
	bShowDebugVisualization = false;
}

void AEquippableTool::BeginPlay()
{
	Super::BeginPlay();

	// Initialize durability
	if (MaxDurability > 0.0f)
	{
		CurrentDurability = MaxDurability;
	}

	// Set up meshes if provided
	if (FirstPersonSkeletalMesh && FirstPersonMesh)
	{
		FirstPersonMesh->SetSkeletalMesh(FirstPersonSkeletalMesh);
	}

	if (ThirdPersonSkeletalMesh && ItemMesh)
	{
		// Note: ItemMesh is a StaticMeshComponent, so we'd need to handle this differently
		// For now, leave static mesh as is for world representation
	}
}

void AEquippableTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Draw debug visualization if enabled
	if (bShowDebugVisualization && bIsEquipped)
	{
		DrawDebugVisualization();
	}

	// Tool-specific tick logic can be added in subclasses
}

void AEquippableTool::EquipTool(AOutercorpCharacter* Character)
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquippableTool::EquipTool - Character is null"));
		return;
	}

	OwningCharacter = Character;
	OnEquipped(Character);
}

void AEquippableTool::OnEquipped_Implementation(AOutercorpCharacter* Character)
{
	if (!Character)
	{
		return;
	}

	bIsEquipped = true;
	OwningCharacter = Character;

	// Get character's first-person mesh
	USkeletalMeshComponent* CharacterFPMesh = Character->GetFirstPersonMesh();
	if (!CharacterFPMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Character first-person mesh is null!"));
		return;
	}

	// Determine which mesh to use (skeletal first-person mesh or static item mesh as fallback)
	USceneComponent* MeshToAttach = nullptr;
	bool bUseSkeletalMesh = FirstPersonMesh && FirstPersonMesh->GetSkeletalMeshAsset();

	if (bUseSkeletalMesh)
	{
		MeshToAttach = FirstPersonMesh;
		UE_LOG(LogTemp, Log, TEXT("Using skeletal FirstPersonMesh for equipped tool"));
	}
	else if (ItemMesh && ItemMesh->GetStaticMesh())
	{
		MeshToAttach = ItemMesh;
		UE_LOG(LogTemp, Log, TEXT("Using static ItemMesh for equipped tool (no skeletal mesh assigned)"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Tool has no mesh assigned! Assign a mesh in the Blueprint."));
		return;
	}

	// Attach the mesh to character's first-person mesh
	if (CharacterFPMesh && MeshToAttach)
	{
		// Log available sockets for debugging
		TArray<FName> SocketNames = CharacterFPMesh->GetAllSocketNames();
		UE_LOG(LogTemp, Log, TEXT("Available sockets on character mesh:"));
		for (const FName& SocketName : SocketNames)
		{
			UE_LOG(LogTemp, Log, TEXT("  - %s"), *SocketName.ToString());
		}

		// Try common socket names if AttachSocketName doesn't exist
		FName SocketToUse = AttachSocketName;
		if (!CharacterFPMesh->DoesSocketExist(AttachSocketName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Socket '%s' not found on character mesh. Trying common alternatives..."),
				*AttachSocketName.ToString());

			// Try common socket names
			TArray<FName> CommonSockets = {
				FName("hand_r"),
				FName("hand_rSocket"),
				FName("GripPoint"),
				FName("WeaponSocket"),
				FName("RightHandSocket")
			};

			bool bFoundSocket = false;
			for (const FName& CommonSocket : CommonSockets)
			{
				if (CharacterFPMesh->DoesSocketExist(CommonSocket))
				{
					SocketToUse = CommonSocket;
					bFoundSocket = true;
					UE_LOG(LogTemp, Log, TEXT("Found alternative socket: %s"), *CommonSocket.ToString());
					break;
				}
			}

			if (!bFoundSocket)
			{
				UE_LOG(LogTemp, Warning, TEXT("No suitable socket found. Attaching to root bone."));
				SocketToUse = NAME_None; // Attach to root
			}
		}

		// Attach the actor's root component to the character socket
		// Using SnapToTargetIncludingScale ensures proper transform inheritance from animated bone
		FAttachmentTransformRules AttachRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false  // Don't weld simulated bodies
		);

		AttachToComponent(
			CharacterFPMesh,
			AttachRules,
			SocketToUse
		);

		// Apply the custom relative transform offset
		SetActorRelativeTransform(RelativeTransform);

		// Make sure the mesh is visible
		MeshToAttach->SetVisibility(true);

		// Set owner-see-only if it's a skeletal mesh component
		if (USkeletalMeshComponent* SkelMesh = Cast<USkeletalMeshComponent>(MeshToAttach))
		{
			SkelMesh->SetOnlyOwnerSee(true);
		}
		else if (UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(MeshToAttach))
		{
			StaticMesh->SetOnlyOwnerSee(true);
		}

		UE_LOG(LogTemp, Log, TEXT("Tool mesh attached to socket: %s"), *SocketToUse.ToString());
	}

	// Disable collision when equipped
	SetActorEnableCollision(false);

	UE_LOG(LogTemp, Log, TEXT("Tool '%s' equipped by '%s'"),
		*GetName(), *Character->GetName());
}

void AEquippableTool::UnequipTool()
{
	OnUnequipped();
}

void AEquippableTool::OnUnequipped_Implementation()
{
	bIsEquipped = false;

	// Stop any ongoing use
	if (bIsInUse)
	{
		StopPrimaryUse();
	}

	// Detach and hide first-person mesh (skeletal)
	if (FirstPersonMesh)
	{
		FirstPersonMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		FirstPersonMesh->SetVisibility(false);
	}

	// Also detach and hide item mesh (static) if it was used
	if (ItemMesh && ItemMesh->GetAttachParent())
	{
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ItemMesh->SetVisibility(false);
	}

	// Re-enable collision
	SetActorEnableCollision(true);

	UE_LOG(LogTemp, Log, TEXT("Tool '%s' unequipped"), *GetName());

	OwningCharacter = nullptr;
}

void AEquippableTool::StartPrimaryUse_Implementation()
{
	if (!CanUseTool())
	{
		return;
	}

	bIsInUse = true;
	CurrentUsageMode = EToolUsageMode::Primary;

	// Play animation
	PlayUseAnimation(PrimaryUseAnimation);

	// Perform the action
	PerformPrimaryAction();

	// If not continuous hold, stop immediately
	if (!bRequiresContinuousHold)
	{
		StopPrimaryUse();
	}
}

void AEquippableTool::StopPrimaryUse_Implementation()
{
	bIsInUse = false;
	CurrentUsageMode = EToolUsageMode::None;

	// Start cooldown if applicable
	if (UsageCooldown > 0.0f && GetWorld())
	{
		bIsOnCooldown = true;
		GetWorld()->GetTimerManager().SetTimer(
			CooldownTimerHandle,
			this,
			&AEquippableTool::ResetCooldown,
			UsageCooldown,
			false
		);
	}
}

void AEquippableTool::StartSecondaryUse_Implementation()
{
	if (!CanUseTool())
	{
		return;
	}

	bIsInUse = true;
	CurrentUsageMode = EToolUsageMode::Secondary;

	// Play animation
	PlayUseAnimation(SecondaryUseAnimation);

	// Perform the action
	PerformSecondaryAction();

	// If not continuous hold, stop immediately
	if (!bRequiresContinuousHold)
	{
		StopSecondaryUse();
	}
}

void AEquippableTool::StopSecondaryUse_Implementation()
{
	bIsInUse = false;
	CurrentUsageMode = EToolUsageMode::None;

	// Start cooldown if applicable
	if (UsageCooldown > 0.0f && GetWorld())
	{
		bIsOnCooldown = true;
		GetWorld()->GetTimerManager().SetTimer(
			CooldownTimerHandle,
			this,
			&AEquippableTool::ResetCooldown,
			UsageCooldown,
			false
		);
	}
}

void AEquippableTool::PerformPrimaryAction_Implementation()
{
	// Base implementation - override in subclasses
	UE_LOG(LogTemp, Log, TEXT("Tool '%s' performing primary action"), *GetName());

	// Consume durability
	if (MaxDurability > 0.0f)
	{
		ConsumeDurability(DurabilityCostPerUse);
	}
}

void AEquippableTool::PerformSecondaryAction_Implementation()
{
	// Base implementation - override in subclasses
	UE_LOG(LogTemp, Log, TEXT("Tool '%s' performing secondary action"), *GetName());
}

bool AEquippableTool::CanUseTool() const
{
	// Must be equipped
	if (!bIsEquipped)
	{
		return false;
	}

	// Must not be on cooldown
	if (bIsOnCooldown)
	{
		return false;
	}

	// Must have durability (if applicable)
	if (!HasDurability())
	{
		return false;
	}

	return true;
}

bool AEquippableTool::HasDurability() const
{
	// If durability system is not used (MaxDurability == 0), always return true
	if (MaxDurability <= 0.0f)
	{
		return true;
	}

	return CurrentDurability > 0.0f;
}

void AEquippableTool::ConsumeDurability(float Amount)
{
	if (MaxDurability <= 0.0f)
	{
		return; // Infinite durability
	}

	CurrentDurability = FMath::Max(0.0f, CurrentDurability - Amount);

	if (CurrentDurability <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tool '%s' has broken (no durability remaining)"), *GetName());
		// Could trigger a "tool broken" event here
	}
}

float AEquippableTool::GetDurabilityPercentage() const
{
	if (MaxDurability <= 0.0f)
	{
		return 1.0f; // Infinite durability = 100%
	}

	return CurrentDurability / MaxDurability;
}

void AEquippableTool::ResetCooldown()
{
	bIsOnCooldown = false;
}

void AEquippableTool::PlayUseAnimation(UAnimMontage* Animation)
{
	if (!Animation || !OwningCharacter)
	{
		return;
	}

	USkeletalMeshComponent* CharacterMesh = OwningCharacter->GetFirstPersonMesh();
	if (CharacterMesh && CharacterMesh->GetAnimInstance())
	{
		CharacterMesh->GetAnimInstance()->Montage_Play(Animation);
	}
}

// ============================================================================
// Transform Adjustment Helpers
// ============================================================================

void AEquippableTool::SetToolRelativeTransform(FVector Location, FRotator Rotation, FVector Scale)
{
	RelativeTransform.SetLocation(Location);
	RelativeTransform.SetRotation(Rotation.Quaternion());
	RelativeTransform.SetScale3D(Scale);

	// Apply immediately if equipped
	if (bIsEquipped && GetAttachParentActor())
	{
		// Set the actor's relative transform since we attach the whole actor now
		SetActorRelativeTransform(RelativeTransform);
	}
}

void AEquippableTool::PrintCurrentTransform()
{
	FVector Loc = RelativeTransform.GetLocation();
	FRotator Rot = RelativeTransform.GetRotation().Rotator();
	FVector Scale = RelativeTransform.GetScale3D();

	UE_LOG(LogTemp, Warning, TEXT("=== Current Tool Transform ==="));
	UE_LOG(LogTemp, Warning, TEXT("Location: X=%.2f Y=%.2f Z=%.2f"), Loc.X, Loc.Y, Loc.Z);
	UE_LOG(LogTemp, Warning, TEXT("Rotation: Pitch=%.2f Yaw=%.2f Roll=%.2f"), Rot.Pitch, Rot.Yaw, Rot.Roll);
	UE_LOG(LogTemp, Warning, TEXT("Scale: X=%.2f Y=%.2f Z=%.2f"), Scale.X, Scale.Y, Scale.Z);
	UE_LOG(LogTemp, Warning, TEXT("============================="));

	// Also log in easy-to-copy format
	UE_LOG(LogTemp, Warning, TEXT("Copy-paste values:"));
	UE_LOG(LogTemp, Warning, TEXT("Location: (%f, %f, %f)"), Loc.X, Loc.Y, Loc.Z);
	UE_LOG(LogTemp, Warning, TEXT("Rotation: (%f, %f, %f)"), Rot.Pitch, Rot.Yaw, Rot.Roll);
	UE_LOG(LogTemp, Warning, TEXT("Scale: (%f, %f, %f)"), Scale.X, Scale.Y, Scale.Z);
}

void AEquippableTool::ToggleDebugVisualization()
{
	bShowDebugVisualization = !bShowDebugVisualization;
	UE_LOG(LogTemp, Log, TEXT("Tool debug visualization: %s"), bShowDebugVisualization ? TEXT("ON") : TEXT("OFF"));
}

void AEquippableTool::DrawDebugVisualization()
{
	if (!OwningCharacter)
	{
		return;
	}

	USkeletalMeshComponent* CharacterFPMesh = OwningCharacter->GetFirstPersonMesh();
	if (!CharacterFPMesh)
	{
		return;
	}

	// Find which mesh is attached
	USceneComponent* AttachedMesh = nullptr;
	if (FirstPersonMesh && FirstPersonMesh->GetAttachParent())
	{
		AttachedMesh = FirstPersonMesh;
	}
	else if (ItemMesh && ItemMesh->GetAttachParent())
	{
		AttachedMesh = ItemMesh;
	}

	if (!AttachedMesh)
	{
		return;
	}

	// Get world transform of the attached mesh
	FTransform MeshWorldTransform = AttachedMesh->GetComponentTransform();
	FVector MeshLocation = MeshWorldTransform.GetLocation();
	FRotator MeshRotation = MeshWorldTransform.GetRotation().Rotator();

	// Draw coordinate axes at the mesh location
	float AxisLength = 20.0f;
	FVector Forward = MeshRotation.RotateVector(FVector::ForwardVector) * AxisLength;
	FVector Right = MeshRotation.RotateVector(FVector::RightVector) * AxisLength;
	FVector Up = MeshRotation.RotateVector(FVector::UpVector) * AxisLength;

	DrawDebugLine(GetWorld(), MeshLocation, MeshLocation + Forward, FColor::Red, false, -1.0f, 0, 2.0f);
	DrawDebugLine(GetWorld(), MeshLocation, MeshLocation + Right, FColor::Green, false, -1.0f, 0, 2.0f);
	DrawDebugLine(GetWorld(), MeshLocation, MeshLocation + Up, FColor::Blue, false, -1.0f, 0, 2.0f);

	// Draw a sphere at the origin
	DrawDebugSphere(GetWorld(), MeshLocation, 5.0f, 8, FColor::Yellow, false, -1.0f, 0, 1.0f);

	// Draw text with current transform values
	FVector Loc = RelativeTransform.GetLocation();
	FRotator Rot = RelativeTransform.GetRotation().Rotator();
	FString TransformText = FString::Printf(TEXT("Loc: (%.1f, %.1f, %.1f)\nRot: (%.1f, %.1f, %.1f)"),
		Loc.X, Loc.Y, Loc.Z, Rot.Pitch, Rot.Yaw, Rot.Roll);

	DrawDebugString(GetWorld(), MeshLocation + FVector(0, 0, 30), TransformText, nullptr, FColor::White, 0.0f, true);
}
