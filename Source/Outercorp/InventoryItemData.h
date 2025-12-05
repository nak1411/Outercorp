// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemData.generated.h"

/**
 * Item rarity/quality enum
 */
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common		UMETA(DisplayName = "Common"),
	Uncommon	UMETA(DisplayName = "Uncommon"),
	Rare		UMETA(DisplayName = "Rare"),
	Epic		UMETA(DisplayName = "Epic"),
	Legendary	UMETA(DisplayName = "Legendary")
};

/**
 * Item category enum
 */
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Weapon		UMETA(DisplayName = "Weapon"),
	Armor		UMETA(DisplayName = "Armor"),
	Consumable	UMETA(DisplayName = "Consumable"),
	Resource	UMETA(DisplayName = "Resource"),
	Module		UMETA(DisplayName = "Module"),
	Ammunition	UMETA(DisplayName = "Ammunition"),
	Misc		UMETA(DisplayName = "Miscellaneous")
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

	/** Item metadata (for custom properties) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TMap<FName, FString> Metadata;

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
		: ItemData(nullptr)
		, Quantity(1)
		, InstanceID(FGuid::NewGuid())
	{
	}

	FInventoryItem(UInventoryItemData* InItemData, int32 InQuantity = 1)
		: ItemData(InItemData)
		, Quantity(InQuantity)
		, InstanceID(FGuid::NewGuid())
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
	bool operator==(const FInventoryItem& Other) const
	{
		return InstanceID == Other.InstanceID;
	}
};
