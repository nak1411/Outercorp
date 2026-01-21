// Copyright Epic Games, Inc. All Rights Reserved.

#include "EquippableTool.h"
#include "InventoryItemData.h"
#include "OutercorpCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"

AEquippableTool::AEquippableTool()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create first-person mesh component
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(Root);
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonMesh->SetCastShadow(false);
	FirstPersonMesh->SetVisibility(false); // Hidden until equipped

	// Ensure mesh updates in sync with parent skeletal mesh to avoid lag
	FirstPersonMesh->bUseAttachParentBound = true;
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	// Default values
	AttachSocketName = FName("GripPoint");
	ToolAnimationState = EEquippableState::OneHandedTool;
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
	SourceItemData = nullptr;
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

	// Set character's animation state
	Character->SetEquippableState(ToolAnimationState);

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

		// Attach the MESH COMPONENT directly to the character socket (not the actor)
		FAttachmentTransformRules AttachRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			true  // Weld simulated bodies
		);

		MeshToAttach->AttachToComponent(
			CharacterFPMesh,
			AttachRules,
			SocketToUse
		);

		// Set owner for visibility
		SetOwner(Character);

		// Configure mesh visibility for first-person view
		if (USkeletalMeshComponent* SkelMesh = Cast<USkeletalMeshComponent>(MeshToAttach))
		{
			SkelMesh->SetOnlyOwnerSee(false);
			SkelMesh->SetOwnerNoSee(false);
			SkelMesh->SetCastShadow(true);
			SkelMesh->SetVisibility(true, true);
			SkelMesh->SetHiddenInGame(false);
			UE_LOG(LogTemp, Log, TEXT("Skeletal mesh attached and configured - Asset: %s, Visible: %s, HiddenInGame: %s"),
				SkelMesh->GetSkeletalMeshAsset() ? *SkelMesh->GetSkeletalMeshAsset()->GetName() : TEXT("NULL"),
				SkelMesh->IsVisible() ? TEXT("true") : TEXT("false"),
				SkelMesh->bHiddenInGame ? TEXT("true") : TEXT("false"));
		}
		else if (UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(MeshToAttach))
		{
			StaticMesh->SetOnlyOwnerSee(false);
			StaticMesh->SetOwnerNoSee(false);
			StaticMesh->SetCastShadow(true);
			StaticMesh->SetVisibility(true, true);
			StaticMesh->SetHiddenInGame(false);
			UE_LOG(LogTemp, Log, TEXT("Static mesh attached and configured - Mesh: %s, Visible: %s"),
				StaticMesh->GetStaticMesh() ? *StaticMesh->GetStaticMesh()->GetName() : TEXT("NULL"),
				StaticMesh->IsVisible() ? TEXT("true") : TEXT("false"));
		}

		UE_LOG(LogTemp, Log, TEXT("Tool mesh attached to socket: %s, Attach Parent: %s"),
			*SocketToUse.ToString(),
			MeshToAttach->GetAttachParent() ? *MeshToAttach->GetAttachParent()->GetName() : TEXT("NULL"));
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

	// Reset character's animation state to unarmed
	if (OwningCharacter)
	{
		OwningCharacter->SetEquippableState(EEquippableState::Unarmed);
	}

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

void AEquippableTool::InitializeFromItemData(UInventoryItemData* InItemData)
{
	if (!InItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEquippableTool::InitializeFromItemData - ItemData is null"));
		return;
	}

	if (!InItemData->bIsEquippable)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEquippableTool::InitializeFromItemData - Item '%s' is not equippable"), *InItemData->ItemName.ToString());
		return;
	}

	SourceItemData = InItemData;

	// Set mesh references
	FirstPersonSkeletalMesh = InItemData->ToolFirstPersonMesh;
	ThirdPersonSkeletalMesh = InItemData->ToolThirdPersonMesh;

	// Set attachment
	AttachSocketName = InItemData->ToolAttachSocketName;

	// Set animation state
	ToolAnimationState = InItemData->ToolAnimationState;

	// Set animations
	PrimaryUseAnimation = InItemData->ToolPrimaryUseAnimation;
	SecondaryUseAnimation = InItemData->ToolSecondaryUseAnimation;

	// Set usage properties
	bRequiresContinuousHold = InItemData->bToolRequiresContinuousHold;
	UsageCooldown = InItemData->ToolUsageCooldown;

	// Set durability
	MaxDurability = InItemData->ToolMaxDurability;
	DurabilityCostPerUse = InItemData->ToolDurabilityCostPerUse;
	if (MaxDurability > 0.0f)
	{
		CurrentDurability = MaxDurability;
	}

	// Apply first-person mesh if set
	if (FirstPersonSkeletalMesh && FirstPersonMesh)
	{
		FirstPersonMesh->SetSkeletalMesh(FirstPersonSkeletalMesh);
		UE_LOG(LogTemp, Log, TEXT("Set FirstPersonMesh skeletal mesh to: %s"), *FirstPersonSkeletalMesh->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No FirstPersonSkeletalMesh to set (FPMesh: %s, SkeletalMesh: %s)"),
			FirstPersonMesh ? TEXT("Valid") : TEXT("NULL"),
			FirstPersonSkeletalMesh ? *FirstPersonSkeletalMesh->GetName() : TEXT("NULL"));
	}

	UE_LOG(LogTemp, Log, TEXT("Tool initialized from item: %s (ToolFirstPersonMesh in data: %s)"),
		*InItemData->ItemName.ToString(),
		InItemData->ToolFirstPersonMesh ? *InItemData->ToolFirstPersonMesh->GetName() : TEXT("NULL"));
}
