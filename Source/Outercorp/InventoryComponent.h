// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryUpdated, int32, SlotIndex, const FInventoryItem&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryCapacityChanged, int32, NewCapacity);

/**
 * Component that manages an inventory system
 * Inspired by Eve Online's container system
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OUTERCORP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

	/** Array of inventory slots */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryItem> Items;

public:
	/** Maximum number of item slots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxSlots = 30;

	/** Maximum weight capacity (0 = unlimited) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float MaxWeight = 0.0f;

	/** Maximum volume capacity (0 = unlimited) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float MaxVolume = 0.0f;
	/** Called when inventory is updated */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	/** Called when inventory capacity changes */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryCapacityChanged OnInventoryCapacityChanged;

	/** Add an item to the inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UInventoryItemData* ItemData, int32 Quantity, int32& OutSlotIndex);

	/** Remove item from specific slot */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemAtSlot(int32 SlotIndex, int32 Quantity = 1);

	/** Remove item by instance ID */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemByInstanceID(FGuid InstanceID, int32 Quantity = 1);

	/** Move item from one slot to another */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool MoveItem(int32 FromSlot, int32 ToSlot, int32 Quantity = -1);

	/** Split stack into new slot */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SplitStack(int32 SourceSlot, int32 TargetSlot, int32 Quantity);

	/** Merge two stacks */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool MergeStacks(int32 SourceSlot, int32 TargetSlot);

	/** Get item at specific slot */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FInventoryItem GetItemAtSlot(int32 SlotIndex) const;

	/** Check if slot is empty */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool IsSlotEmpty(int32 SlotIndex) const;

	/** Get all items */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<FInventoryItem> GetAllItems() const { return Items; }

	/** Get current number of occupied slots */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetOccupiedSlots() const;

	/** Get current total weight */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	float GetCurrentWeight() const;

	/** Get current total volume */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	float GetCurrentVolume() const;

	/** Check if can add item */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool CanAddItem(UInventoryItemData* ItemData, int32 Quantity = 1) const;

	/** Find first empty slot */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 FindEmptySlot() const;

	/** Find first empty slot that fits within column bounds (for wrapping) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 FindEmptySlotWithinColumns(int32 MaxColumns) const;

	/** Find item by ID */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 FindItemByID(FName ItemID) const;

	/** Set max slots */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetMaxSlots(int32 NewMaxSlots);

	/** Clear all items */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();

	/** Sort inventory by criteria */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortInventory(bool bByName = true);

	/** Sort inventory by custom predicate */
	void SortInventoryCustom(TFunction<bool(const FInventoryItem&, const FInventoryItem&)> Predicate);

	/** Begin batch update - suppresses broadcasts until EndBatchUpdate */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BeginBatchUpdate();

	/** End batch update - broadcasts all pending updates */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EndBatchUpdate();

	/** Drop item from slot into the world */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropItem(int32 SlotIndex, int32 Quantity, FVector DropLocation, FRotator DropRotation);

	/** Drop item with automatic positioning in front of owner */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropItemInFront(int32 SlotIndex, int32 Quantity, float DropDistance = 200.0f);

	/** Destroy item at slot and show notification */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DestroyItem(int32 SlotIndex, int32 Quantity);

	/** Class to spawn when dropping items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Drop")
	TSubclassOf<class APickupableItem> PickupableItemClass;

	/** How much force to apply when dropping items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Drop")
	float DropImpulseStrength;

	/** Suppress notifications (useful during initial setup) */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool bSuppressNotifications = false;

protected:
	/** Try to stack item with existing items */
	bool TryStackItem(UInventoryItemData* ItemData, int32& Quantity, int32& OutSlotIndex);

	/** Check if two items can stack */
	bool CanStack(const FInventoryItem& ItemA, const FInventoryItem& ItemB) const;

	/** Whether we're currently in batch update mode */
	bool bIsBatchUpdating = false;

	/** Slots that were updated during batch mode */
	TSet<int32> PendingUpdateSlots;
};
