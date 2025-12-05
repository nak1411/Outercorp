// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize inventory array
	Items.SetNum(MaxSlots);
}

bool UInventoryComponent::AddItem(UInventoryItemData* ItemData, int32 Quantity, int32& OutSlotIndex)
{
	if (!ItemData || Quantity <= 0)
	{
		OutSlotIndex = -1;
		return false;
	}

	if (!CanAddItem(ItemData, Quantity))
	{
		OutSlotIndex = -1;
		return false;
	}

	int32 RemainingQuantity = Quantity;

	// Try to stack with existing items first
	if (ItemData->MaxStackSize > 1)
	{
		if (TryStackItem(ItemData, RemainingQuantity, OutSlotIndex))
		{
			if (RemainingQuantity <= 0)
			{
				return true;
			}
		}
	}

	// Add to empty slots
	while (RemainingQuantity > 0)
	{
		int32 EmptySlot = FindEmptySlot();
		if (EmptySlot == -1)
		{
			OutSlotIndex = -1;
			return false;
		}

		int32 QuantityToAdd = FMath::Min(RemainingQuantity, ItemData->MaxStackSize);

		FInventoryItem NewItem(ItemData, QuantityToAdd);
		Items[EmptySlot] = NewItem;

		OutSlotIndex = EmptySlot;
		RemainingQuantity -= QuantityToAdd;

		OnInventoryUpdated.Broadcast(EmptySlot, NewItem);
	}

	return true;
}

bool UInventoryComponent::RemoveItemAtSlot(int32 SlotIndex, int32 Quantity)
{
	if (!Items.IsValidIndex(SlotIndex) || !Items[SlotIndex].IsValid())
	{
		return false;
	}

	if (Quantity <= 0 || Quantity > Items[SlotIndex].Quantity)
	{
		return false;
	}

	Items[SlotIndex].Quantity -= Quantity;

	if (Items[SlotIndex].Quantity <= 0)
	{
		Items[SlotIndex] = FInventoryItem();
	}

	OnInventoryUpdated.Broadcast(SlotIndex, Items[SlotIndex]);
	return true;
}

bool UInventoryComponent::RemoveItemByInstanceID(FGuid InstanceID, int32 Quantity)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].IsValid() && Items[i].InstanceID == InstanceID)
		{
			return RemoveItemAtSlot(i, Quantity);
		}
	}
	return false;
}

bool UInventoryComponent::MoveItem(int32 FromSlot, int32 ToSlot, int32 Quantity)
{
	if (!Items.IsValidIndex(FromSlot) || !Items.IsValidIndex(ToSlot))
	{
		return false;
	}

	if (!Items[FromSlot].IsValid())
	{
		return false;
	}

	// Determine quantity to move
	int32 QuantityToMove = (Quantity <= 0) ? Items[FromSlot].Quantity : FMath::Min(Quantity, Items[FromSlot].Quantity);

	// If target slot is empty, just move the item
	if (!Items[ToSlot].IsValid())
	{
		if (QuantityToMove == Items[FromSlot].Quantity)
		{
			// Move entire stack
			Items[ToSlot] = Items[FromSlot];
			Items[FromSlot] = FInventoryItem();
		}
		else
		{
			// Split stack
			return SplitStack(FromSlot, ToSlot, QuantityToMove);
		}

		OnInventoryUpdated.Broadcast(FromSlot, Items[FromSlot]);
		OnInventoryUpdated.Broadcast(ToSlot, Items[ToSlot]);
		return true;
	}

	// If both slots have items, try to merge or swap
	if (CanStack(Items[FromSlot], Items[ToSlot]))
	{
		return MergeStacks(FromSlot, ToSlot);
	}
	else
	{
		// Swap items
		FInventoryItem Temp = Items[FromSlot];
		Items[FromSlot] = Items[ToSlot];
		Items[ToSlot] = Temp;

		OnInventoryUpdated.Broadcast(FromSlot, Items[FromSlot]);
		OnInventoryUpdated.Broadcast(ToSlot, Items[ToSlot]);
		return true;
	}
}

bool UInventoryComponent::SplitStack(int32 SourceSlot, int32 TargetSlot, int32 Quantity)
{
	if (!Items.IsValidIndex(SourceSlot) || !Items.IsValidIndex(TargetSlot))
	{
		return false;
	}

	if (!Items[SourceSlot].IsValid() || Items[TargetSlot].IsValid())
	{
		return false;
	}

	if (Quantity <= 0 || Quantity >= Items[SourceSlot].Quantity)
	{
		return false;
	}

	// Create new stack
	FInventoryItem NewStack(Items[SourceSlot].ItemData, Quantity);
	NewStack.InstanceMetadata = Items[SourceSlot].InstanceMetadata;

	Items[TargetSlot] = NewStack;
	Items[SourceSlot].Quantity -= Quantity;

	OnInventoryUpdated.Broadcast(SourceSlot, Items[SourceSlot]);
	OnInventoryUpdated.Broadcast(TargetSlot, Items[TargetSlot]);

	return true;
}

bool UInventoryComponent::MergeStacks(int32 SourceSlot, int32 TargetSlot)
{
	if (!Items.IsValidIndex(SourceSlot) || !Items.IsValidIndex(TargetSlot))
	{
		return false;
	}

	if (!CanStack(Items[SourceSlot], Items[TargetSlot]))
	{
		return false;
	}

	int32 SpaceAvailable = Items[TargetSlot].ItemData->MaxStackSize - Items[TargetSlot].Quantity;
	int32 QuantityToMove = FMath::Min(SpaceAvailable, Items[SourceSlot].Quantity);

	Items[TargetSlot].Quantity += QuantityToMove;
	Items[SourceSlot].Quantity -= QuantityToMove;

	if (Items[SourceSlot].Quantity <= 0)
	{
		Items[SourceSlot] = FInventoryItem();
	}

	OnInventoryUpdated.Broadcast(SourceSlot, Items[SourceSlot]);
	OnInventoryUpdated.Broadcast(TargetSlot, Items[TargetSlot]);

	return true;
}

FInventoryItem UInventoryComponent::GetItemAtSlot(int32 SlotIndex) const
{
	if (Items.IsValidIndex(SlotIndex))
	{
		return Items[SlotIndex];
	}
	return FInventoryItem();
}

bool UInventoryComponent::IsSlotEmpty(int32 SlotIndex) const
{
	if (Items.IsValidIndex(SlotIndex))
	{
		return !Items[SlotIndex].IsValid();
	}
	return true;
}

int32 UInventoryComponent::GetOccupiedSlots() const
{
	int32 Count = 0;
	for (const FInventoryItem& Item : Items)
	{
		if (Item.IsValid())
		{
			Count++;
		}
	}
	return Count;
}

float UInventoryComponent::GetCurrentWeight() const
{
	float TotalWeight = 0.0f;
	for (const FInventoryItem& Item : Items)
	{
		if (Item.IsValid())
		{
			TotalWeight += Item.GetTotalWeight();
		}
	}
	return TotalWeight;
}

float UInventoryComponent::GetCurrentVolume() const
{
	// For now, volume is not implemented
	return 0.0f;
}

bool UInventoryComponent::CanAddItem(UInventoryItemData* ItemData, int32 Quantity) const
{
	if (!ItemData || Quantity <= 0)
	{
		return false;
	}

	// Check weight limit
	if (MaxWeight > 0.0f)
	{
		float TotalWeight = GetCurrentWeight() + (ItemData->Weight * Quantity);
		if (TotalWeight > MaxWeight)
		{
			return false;
		}
	}

	// Check if we have space
	int32 RemainingQuantity = Quantity;

	// Check existing stacks
	if (ItemData->MaxStackSize > 1)
	{
		for (const FInventoryItem& Item : Items)
		{
			if (Item.IsValid() && Item.ItemData == ItemData)
			{
				int32 SpaceInStack = ItemData->MaxStackSize - Item.Quantity;
				RemainingQuantity -= SpaceInStack;
				if (RemainingQuantity <= 0)
				{
					return true;
				}
			}
		}
	}

	// Check empty slots
	int32 RequiredSlots = FMath::CeilToInt(static_cast<float>(RemainingQuantity) / ItemData->MaxStackSize);
	int32 EmptySlots = MaxSlots - GetOccupiedSlots();

	return EmptySlots >= RequiredSlots;
}

int32 UInventoryComponent::FindEmptySlot() const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (!Items[i].IsValid())
		{
			return i;
		}
	}
	return -1;
}

int32 UInventoryComponent::FindItemByID(FName ItemID) const
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].IsValid() && Items[i].ItemData->ItemID == ItemID)
		{
			return i;
		}
	}
	return -1;
}

void UInventoryComponent::SetMaxSlots(int32 NewMaxSlots)
{
	if (NewMaxSlots < MaxSlots)
	{
		// Shrinking inventory - check if items would be lost
		bool bHasItemsBeyondNewSize = false;
		for (int32 i = NewMaxSlots; i < Items.Num(); ++i)
		{
			if (Items[i].IsValid())
			{
				bHasItemsBeyondNewSize = true;
				break;
			}
		}

		if (bHasItemsBeyondNewSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot shrink inventory - items would be lost"));
			return;
		}

		Items.SetNum(NewMaxSlots);
	}
	else
	{
		Items.SetNum(NewMaxSlots);
	}

	MaxSlots = NewMaxSlots;
	OnInventoryCapacityChanged.Broadcast(MaxSlots);
}

void UInventoryComponent::ClearInventory()
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].IsValid())
		{
			Items[i] = FInventoryItem();
			OnInventoryUpdated.Broadcast(i, Items[i]);
		}
	}
}

void UInventoryComponent::SortInventory(bool bByName)
{
	// Extract valid items
	TArray<FInventoryItem> ValidItems;
	for (const FInventoryItem& Item : Items)
	{
		if (Item.IsValid())
		{
			ValidItems.Add(Item);
		}
	}

	// Sort items
	if (bByName)
	{
		ValidItems.Sort([](const FInventoryItem& A, const FInventoryItem& B)
		{
			return A.ItemData->ItemName.ToString() < B.ItemData->ItemName.ToString();
		});
	}
	else
	{
		// Sort by rarity, then by name
		ValidItems.Sort([](const FInventoryItem& A, const FInventoryItem& B)
		{
			if (A.ItemData->Rarity != B.ItemData->Rarity)
			{
				return A.ItemData->Rarity > B.ItemData->Rarity;
			}
			return A.ItemData->ItemName.ToString() < B.ItemData->ItemName.ToString();
		});
	}

	// Clear inventory
	Items.Init(FInventoryItem(), Items.Num());

	// Place sorted items back
	for (int32 i = 0; i < ValidItems.Num(); ++i)
	{
		Items[i] = ValidItems[i];
		OnInventoryUpdated.Broadcast(i, Items[i]);
	}
}

bool UInventoryComponent::TryStackItem(UInventoryItemData* ItemData, int32& Quantity, int32& OutSlotIndex)
{
	bool bStackedAny = false;

	for (int32 i = 0; i < Items.Num() && Quantity > 0; ++i)
	{
		if (Items[i].IsValid() && Items[i].ItemData == ItemData)
		{
			int32 SpaceInStack = ItemData->MaxStackSize - Items[i].Quantity;
			if (SpaceInStack > 0)
			{
				int32 QuantityToAdd = FMath::Min(SpaceInStack, Quantity);
				Items[i].Quantity += QuantityToAdd;
				Quantity -= QuantityToAdd;
				OutSlotIndex = i;
				bStackedAny = true;

				OnInventoryUpdated.Broadcast(i, Items[i]);
			}
		}
	}

	return bStackedAny;
}

bool UInventoryComponent::CanStack(const FInventoryItem& ItemA, const FInventoryItem& ItemB) const
{
	if (!ItemA.IsValid() || !ItemB.IsValid())
	{
		return false;
	}

	if (ItemA.ItemData != ItemB.ItemData)
	{
		return false;
	}

	if (ItemA.ItemData->MaxStackSize <= 1)
	{
		return false;
	}

	return true;
}
