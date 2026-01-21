// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PickupableItem.h"
#include "EquippableTool.generated.h"

class USkeletalMeshComponent;
class AOutercorpCharacter;
class UAnimMontage;

/**
 * Tool usage mode enum
 */
UENUM(BlueprintType)
enum class EToolUsageMode : uint8
{
	None UMETA(DisplayName = "None"),
	Primary UMETA(DisplayName = "Primary Action"),
	Secondary UMETA(DisplayName = "Secondary Action"),
	Toggle UMETA(DisplayName = "Toggle State")
};

/**
 * Base class for equippable tools that can be picked up and used by the character
 * Extends PickupableItem to inherit inventory and interaction functionality
 */
UCLASS()
class OUTERCORP_API AEquippableTool : public APickupableItem
{
	GENERATED_BODY()

public:
	AEquippableTool();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Item data asset this tool was created from */
	UPROPERTY(BlueprintReadOnly, Category = "Tool|Data")
	UInventoryItemData* SourceItemData;

	/** First-person mesh for when tool is equipped (seen by player) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* FirstPersonMesh;

	/** Third-person mesh override (optional, uses ItemMesh if not set) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Mesh")
	USkeletalMesh* ThirdPersonSkeletalMesh;

	/** First-person skeletal mesh for equipped tool */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Mesh")
	USkeletalMesh* FirstPersonSkeletalMesh;

	/** Socket name to attach to on character's first-person mesh (e.g., "GripPoint", "hand_r") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Attachment")
	FName AttachSocketName;

	/** Animation state to set on character when this tool is equipped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Animation")
	EEquippableState ToolAnimationState;

	/** Whether this tool requires continuous hold for primary action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Usage")
	bool bRequiresContinuousHold;

	/** Cooldown time between tool uses (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Usage")
	float UsageCooldown;

	/** Current tool usage mode */
	UPROPERTY(BlueprintReadOnly, Category = "Tool|State")
	EToolUsageMode CurrentUsageMode;

	/** Whether tool is currently equipped */
	UPROPERTY(BlueprintReadOnly, Category = "Tool|State")
	bool bIsEquipped;

	/** Whether tool is currently in use */
	UPROPERTY(BlueprintReadOnly, Category = "Tool|State")
	bool bIsInUse;

	/** Character that has this tool equipped */
	UPROPERTY(BlueprintReadOnly, Category = "Tool|State")
	AOutercorpCharacter* OwningCharacter;

	/** Animation to play on character when using tool (primary action) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Animation")
	UAnimMontage* PrimaryUseAnimation;

	/** Animation to play on character when using tool (secondary action) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Animation")
	UAnimMontage* SecondaryUseAnimation;

	/** Maximum durability of the tool (0 = infinite durability) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Durability")
	float MaxDurability;

	/** Current durability */
	UPROPERTY(BlueprintReadOnly, Category = "Tool|Durability")
	float CurrentDurability;

	/** Durability cost per use */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool|Durability")
	float DurabilityCostPerUse;

	// === Tool Lifecycle Functions ===

	/** Called when tool is equipped by a character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tool")
	void OnEquipped(AOutercorpCharacter* Character);

	/** Called when tool is unequipped */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tool")
	void OnUnequipped();

	/** Equip this tool to a character */
	UFUNCTION(BlueprintCallable, Category = "Tool")
	void EquipTool(AOutercorpCharacter* Character);

	/** Unequip this tool from character */
	UFUNCTION(BlueprintCallable, Category = "Tool")
	void UnequipTool();

	// === Tool Usage Functions ===

	/** Start using the tool (primary action) - called when use button is pressed */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tool")
	void StartPrimaryUse();

	/** Stop using the tool (primary action) - called when use button is released */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tool")
	void StopPrimaryUse();

	/** Start secondary use (alternative action) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tool")
	void StartSecondaryUse();

	/** Stop secondary use */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tool")
	void StopSecondaryUse();

	/** Perform the primary tool action (override in subclasses) */
	UFUNCTION(BlueprintNativeEvent, Category = "Tool")
	void PerformPrimaryAction();

	/** Perform the secondary tool action (override in subclasses) */
	UFUNCTION(BlueprintNativeEvent, Category = "Tool")
	void PerformSecondaryAction();

	/** Check if tool can be used right now */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tool")
	bool CanUseTool() const;

	/** Check if tool has durability remaining */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tool")
	bool HasDurability() const;

	/** Reduce tool durability */
	UFUNCTION(BlueprintCallable, Category = "Tool")
	void ConsumeDurability(float Amount);

	/** Get durability percentage (0-1) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tool")
	float GetDurabilityPercentage() const;

	/** Initialize tool properties from an inventory item data asset */
	UFUNCTION(BlueprintCallable, Category = "Tool")
	void InitializeFromItemData(UInventoryItemData* InItemData);

	/** Get the source item data asset */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tool")
	UInventoryItemData* GetSourceItemData() const { return SourceItemData; }

protected:
	/** Timer handle for cooldown */
	FTimerHandle CooldownTimerHandle;

	/** Whether tool is currently on cooldown */
	bool bIsOnCooldown;

	/** Reset cooldown */
	void ResetCooldown();

	/** Play tool use animation on character */
	void PlayUseAnimation(UAnimMontage* Animation);
};
