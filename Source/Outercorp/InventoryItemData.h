// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemData.generated.h"

class USkeletalMesh;
class UAnimMontage;
class USoundBase;
class UNiagaraSystem;

/**
 * Tool type for categorization and harvesting checks
 */
UENUM(BlueprintType)
enum class EToolType : uint8
{
	None		UMETA(DisplayName = "None"),
	Axe			UMETA(DisplayName = "Axe"),
	Pickaxe		UMETA(DisplayName = "Pickaxe"),
	Shovel		UMETA(DisplayName = "Shovel"),
	Sickle		UMETA(DisplayName = "Sickle"),
	Knife		UMETA(DisplayName = "Knife"),
	Hammer		UMETA(DisplayName = "Hammer"),
	Wrench		UMETA(DisplayName = "Wrench"),
	Generic		UMETA(DisplayName = "Generic Tool")
};

/**
 * Equippable state for animation blueprint blend poses
 */
UENUM(BlueprintType)
enum class EEquippableState : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	OneHandedTool UMETA(DisplayName = "One-Handed Tool"),
	TwoHandedTool UMETA(DisplayName = "Two-Handed Tool"),
	Axe UMETA(DisplayName = "Axe"),
	Pickaxe UMETA(DisplayName = "Pickaxe"),
	Shovel UMETA(DisplayName = "Shovel")
};

/**
 * Item rarity/quality enum
 */
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

/**
 * Item category enum
 */
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Consumable UMETA(DisplayName = "Consumable"),
	Resource UMETA(DisplayName = "Resource"),
	Blueprint UMETA(DisplayName = "Blueprint"),
	Module UMETA(DisplayName = "Module"),
	Ammunition UMETA(DisplayName = "Ammunition"),
	Tool UMETA(DisplayName = "Tool"),
	Misc UMETA(DisplayName = "Miscellaneous")
};

/**
 * Data asset defining an item type
 */
UCLASS(BlueprintType)
class OUTERCORP_API UInventoryItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Unique identifier for this item type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;

	/** Display name shown in UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	/** Detailed description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
	FText Description;

	/** Icon for UI display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	/** 3D mesh for world representation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	/** Item category */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemCategory Category;

	/** Item rarity/quality */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemRarity Rarity;

	/** Maximum stack size (1 for non-stackable items) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize = 1;

	/** Base value/price */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 BaseValue = 0;

	/** Weight in kilograms */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	float Weight = 1.0f;

	/** Can this item be sold */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bIsSellable = true;

	/** Can this item be traded */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bIsTradeable = true;

	/** Can this item be dropped */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bIsDroppable = true;

	/** Can this item be placed freely anywhere (thrown) or must it be placed on surfaces */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Placement")
	bool bCanFreePlacement = false;

	/** Should this item have physics enabled when placed (only applies to free placement items) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Placement", meta = (EditCondition = "bCanFreePlacement"))
	bool bEnablePhysicsOnPlacement = true;

	/** Interaction range for picking up this item in the world (in centimeters) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Interaction")
	float InteractionRange = 500.0f;

	/** Is this item equippable as a tool */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment")
	bool bIsEquippable = false;

	// ============================================
	// TOOL PROPERTIES (only used if bIsEquippable)
	// ============================================

	/** Tool type for categorization and harvesting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Tool", meta = (EditCondition = "bIsEquippable"))
	EToolType ToolType = EToolType::None;

	/** Tool tier/quality level (affects harvesting efficiency) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Tool", meta = (EditCondition = "bIsEquippable", ClampMin = "1"))
	int32 ToolTier = 1;

	/** First-person skeletal mesh (seen by player when equipped) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Mesh", meta = (EditCondition = "bIsEquippable"))
	USkeletalMesh* ToolFirstPersonMesh = nullptr;

	/** Third-person skeletal mesh (seen by others) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Mesh", meta = (EditCondition = "bIsEquippable"))
	USkeletalMesh* ToolThirdPersonMesh = nullptr;

	/** Socket name to attach to on character's first-person mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Attachment", meta = (EditCondition = "bIsEquippable"))
	FName ToolAttachSocketName = FName("GripPoint");

	/** Animation state to set on character when this tool is equipped */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Animation", meta = (EditCondition = "bIsEquippable"))
	EEquippableState ToolAnimationState = EEquippableState::OneHandedTool;

	/** Animation to play on character when using tool (primary action) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Animation", meta = (EditCondition = "bIsEquippable"))
	UAnimMontage* ToolPrimaryUseAnimation = nullptr;

	/** Animation to play on character when using tool (secondary action) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Animation", meta = (EditCondition = "bIsEquippable"))
	UAnimMontage* ToolSecondaryUseAnimation = nullptr;

	/** Whether this tool requires continuous hold for primary action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Usage", meta = (EditCondition = "bIsEquippable"))
	bool bToolRequiresContinuousHold = false;

	/** Cooldown time between tool uses (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Usage", meta = (EditCondition = "bIsEquippable", ClampMin = "0.0"))
	float ToolUsageCooldown = 0.5f;

	/** Base damage dealt by this tool (for harvesting) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Usage", meta = (EditCondition = "bIsEquippable", ClampMin = "0.0"))
	float ToolBaseDamage = 10.0f;

	/** Maximum durability of the tool (0 = infinite durability) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Durability", meta = (EditCondition = "bIsEquippable", ClampMin = "0.0"))
	float ToolMaxDurability = 0.0f;

	/** Durability cost per use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Durability", meta = (EditCondition = "bIsEquippable", ClampMin = "0.0"))
	float ToolDurabilityCostPerUse = 1.0f;

	/** Sound played when using the tool */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Feedback", meta = (EditCondition = "bIsEquippable"))
	USoundBase* ToolUseSound = nullptr;

	/** Sound played when equipping the tool */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Feedback", meta = (EditCondition = "bIsEquippable"))
	USoundBase* ToolEquipSound = nullptr;

	/** Niagara effect when using the tool */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment|Feedback", meta = (EditCondition = "bIsEquippable"))
	UNiagaraSystem* ToolUseEffect = nullptr;

	/** Item metadata (for custom properties) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TMap<FName, FString> Metadata;

	/** Default camera distance for 3D preview (optional override) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Preview")
	float PreviewCameraDistance = 150.0f;

	/** Default rotation for 3D preview (optional override) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Preview")
	FRotator DefaultPreviewRotation = FRotator(0.0f, 45.0f, 0.0f);

	/** Get color based on rarity */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FLinearColor GetRarityColor() const;
};

/**
 * Runtime instance of an inventory item
 */
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	/** Reference to item data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UInventoryItemData> ItemData;

	/** Current stack size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Quantity = 1;

	/** Unique instance ID */
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGuid InstanceID;

	/** Custom instance data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TMap<FName, FString> InstanceMetadata;

	FInventoryItem()
		: ItemData(nullptr), Quantity(1), InstanceID(FGuid::NewGuid())
	{
	}

	FInventoryItem(UInventoryItemData *InItemData, int32 InQuantity = 1)
		: ItemData(InItemData), Quantity(InQuantity), InstanceID(FGuid::NewGuid())
	{
	}

	/** Check if this is a valid item */
	bool IsValid() const
	{
		return ItemData != nullptr && Quantity > 0;
	}

	/** Get total weight of this stack */
	float GetTotalWeight() const
	{
		return ItemData ? ItemData->Weight * Quantity : 0.0f;
	}

	/** Get total value of this stack */
	int32 GetTotalValue() const
	{
		return ItemData ? ItemData->BaseValue * Quantity : 0;
	}

	/** Equality operator based on instance ID */
	bool operator==(const FInventoryItem &Other) const
	{
		return InstanceID == Other.InstanceID;
	}
};
