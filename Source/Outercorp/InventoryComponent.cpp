// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryComponent.h"
#include "PickupableItem.h"
#include "NotificationComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "OutercorpCharacter.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DropImpulseStrength = 500.0f;
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
		// Show notification that there's not enough room
		if (AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(GetOwner()))
		{
			if (UNotificationComponent* NotificationComp = Character->GetNotificationComponent())
			{
				NotificationComp->ShowNotEnoughRoomWarning(ItemData->ItemName);
			}
		}
		OutSlotIndex = -1;
		return false;
	}

	int32 RemainingQuantity = Quantity;
	int32 OriginalQuantity = Quantity;

	// Try to stack with existing items first
	if (ItemData->MaxStackSize > 1)
	{
		UE_LOG(LogTemp, Log, TEXT("Item '%s' is stackable (MaxStackSize: %d), attempting to stack %d items"),
			*ItemData->ItemName.ToString(), ItemData->MaxStackSize, Quantity);

		if (TryStackItem(ItemData, RemainingQuantity, OutSlotIndex))
		{
			if (RemainingQuantity <= 0)
			{
				UE_LOG(LogTemp, Log, TEXT("Successfully stacked all items into existing stacks"));

				// Show notification for successful add (unless suppressed or canvas not ready)
				if (!bSuppressNotifications)
				{
					if (AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(GetOwner()))
					{
						if (UNotificationComponent* NotificationComp = Character->GetNotificationComponent())
						{
							// Only show if canvas is set up (prevents notifications during initial load)
							if (NotificationComp->NotificationCanvas)
							{
								UTexture2D* ItemIcon = ItemData->ItemIcon.LoadSynchronous();
								NotificationComp->ShowItemPickupNotification(ItemData->ItemName, OriginalQuantity, ItemIcon);
							}
						}
					}
				}

				return true;
			}
			UE_LOG(LogTemp, Log, TEXT("Partially stacked, %d items remaining to add to new slots"), RemainingQuantity);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item '%s' is NOT stackable (MaxStackSize: %d) - will create separate slots"),
			*ItemData->ItemName.ToString(), ItemData->MaxStackSize);
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

	// Show notification for successful add (unless suppressed or canvas not ready)
	if (!bSuppressNotifications)
	{
		if (AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(GetOwner()))
		{
			if (UNotificationComponent* NotificationComp = Character->GetNotificationComponent())
			{
				// Only show if canvas is set up (prevents notifications during initial load)
				if (NotificationComp->NotificationCanvas)
				{
					UTexture2D* ItemIcon = ItemData->ItemIcon.LoadSynchronous();
					NotificationComp->ShowItemPickupNotification(ItemData->ItemName, OriginalQuantity, ItemIcon);
				}
			}
		}
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

	// Store item info before removing for notification
	FInventoryItem RemovedItem = Items[SlotIndex];
	int32 RemovedQuantity = Quantity;

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

		// If in batch mode, queue updates; otherwise broadcast immediately
		if (bIsBatchUpdating)
		{
			PendingUpdateSlots.Add(FromSlot);
			PendingUpdateSlots.Add(ToSlot);
		}
		else
		{
			OnInventoryUpdated.Broadcast(FromSlot, Items[FromSlot]);
			OnInventoryUpdated.Broadcast(ToSlot, Items[ToSlot]);
		}
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

		// If in batch mode, queue updates; otherwise broadcast immediately
		if (bIsBatchUpdating)
		{
			PendingUpdateSlots.Add(FromSlot);
			PendingUpdateSlots.Add(ToSlot);
		}
		else
		{
			OnInventoryUpdated.Broadcast(FromSlot, Items[FromSlot]);
			OnInventoryUpdated.Broadcast(ToSlot, Items[ToSlot]);
		}
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

	// If in batch mode, queue updates; otherwise broadcast immediately
	if (bIsBatchUpdating)
	{
		PendingUpdateSlots.Add(SourceSlot);
		PendingUpdateSlots.Add(TargetSlot);
	}
	else
	{
		OnInventoryUpdated.Broadcast(SourceSlot, Items[SourceSlot]);
		OnInventoryUpdated.Broadcast(TargetSlot, Items[TargetSlot]);
	}

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

int32 UInventoryComponent::FindEmptySlotWithinColumns(int32 MaxColumns) const
{
	// This function is kept for compatibility but now just calls FindEmptySlot()
	// The compress/reflow system handles moving items into proper grid positions
	return FindEmptySlot();
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

void UInventoryComponent::SortInventoryCustom(TFunction<bool(const FInventoryItem&, const FInventoryItem&)> Predicate)
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

	// Sort items using custom predicate
	ValidItems.Sort(Predicate);

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
	int32 MatchingStacksFound = 0;

	for (int32 i = 0; i < Items.Num() && Quantity > 0; ++i)
	{
		if (Items[i].IsValid() && Items[i].ItemData == ItemData)
		{
			MatchingStacksFound++;
			int32 SpaceInStack = ItemData->MaxStackSize - Items[i].Quantity;
			if (SpaceInStack > 0)
			{
				int32 QuantityToAdd = FMath::Min(SpaceInStack, Quantity);
				Items[i].Quantity += QuantityToAdd;
				Quantity -= QuantityToAdd;
				OutSlotIndex = i;
				bStackedAny = true;

				UE_LOG(LogTemp, Log, TEXT("Stacked %d items into slot %d (now has %d/%d)"),
					QuantityToAdd, i, Items[i].Quantity, ItemData->MaxStackSize);

				OnInventoryUpdated.Broadcast(i, Items[i]);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Found matching stack at slot %d but it's full (%d/%d)"),
					i, Items[i].Quantity, ItemData->MaxStackSize);
			}
		}
	}

	if (MatchingStacksFound == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("No existing stacks found for item '%s'"), *ItemData->ItemName.ToString());
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

void UInventoryComponent::BeginBatchUpdate()
{
	bIsBatchUpdating = true;
	PendingUpdateSlots.Empty();
}

void UInventoryComponent::EndBatchUpdate()
{
	if (!bIsBatchUpdating)
	{
		return;
	}

	bIsBatchUpdating = false;

	// Broadcast all pending updates
	for (int32 SlotIndex : PendingUpdateSlots)
	{
		if (Items.IsValidIndex(SlotIndex))
		{
			OnInventoryUpdated.Broadcast(SlotIndex, Items[SlotIndex]);
		}
	}

	PendingUpdateSlots.Empty();
}

bool UInventoryComponent::DropItem(int32 SlotIndex, int32 Quantity, FVector DropLocation, FRotator DropRotation)
{
	// Validate inputs
	if (!Items.IsValidIndex(SlotIndex) || !Items[SlotIndex].IsValid())
	{
		return false;
	}

	if (Quantity <= 0 || Quantity > Items[SlotIndex].Quantity)
	{
		return false;
	}

	if (!PickupableItemClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickupableItemClass not set on InventoryComponent!"));
		return false;
	}

	// Get item data
	FInventoryItem ItemToDrop = Items[SlotIndex];

	// Spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn individual items for each quantity
	int32 SuccessfulDrops = 0;
	for (int32 i = 0; i < Quantity; i++)
	{
		// Calculate spawn location with slight offset for each item
		FVector CurrentDropLocation = DropLocation;
		if (i > 0)
		{
			// Add small random offset to spread items out slightly
			// Keep them close together (max ~20 units from center)
			float RandomAngle = FMath::RandRange(0.0f, 360.0f);
			float RandomDistance = FMath::RandRange(5.0f, 20.0f);
			FVector Offset = FVector(
				FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomDistance,
				FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomDistance,
				FMath::RandRange(-5.0f, 5.0f)
			);
			CurrentDropLocation += Offset;
		}

		// Spawn the pickup actor
		APickupableItem* DroppedItem = GetWorld()->SpawnActor<APickupableItem>(
			PickupableItemClass,
			CurrentDropLocation,
			DropRotation,
			SpawnParams
		);

		if (!DroppedItem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn dropped item %d!"), i);
			continue;
		}

		// Initialize the dropped item with quantity 1
		DroppedItem->InitializeItem(ItemToDrop.ItemData, 1);

		// Start drop cooldown to prevent immediate re-pickup
		DroppedItem->StartDropCooldown();

		// Enable physics simulation on the dropped item
		if (DroppedItem->ItemMesh)
		{
			// Enable physics simulation and gravity
			DroppedItem->ItemMesh->SetSimulatePhysics(true);
			DroppedItem->ItemMesh->SetEnableGravity(true);

			// Set collision to block all dynamic objects
			DroppedItem->ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			DroppedItem->ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
			DroppedItem->ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

			// Apply impulse to throw the item forward with slight variation
			FVector ImpulseDirection = DropRotation.Vector();
			// Add slight randomness to impulse for spreading
			FVector RandomSpread = FVector(
				FMath::RandRange(-0.2f, 0.2f),
				FMath::RandRange(-0.2f, 0.2f),
				FMath::RandRange(0.0f, 0.3f)
			);
			FVector FinalImpulse = (ImpulseDirection + RandomSpread) * DropImpulseStrength;
			DroppedItem->ItemMesh->AddImpulse(FinalImpulse, NAME_None, true);
		}

		SuccessfulDrops++;
	}

	if (SuccessfulDrops > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Dropped %d x '%s' as individual items"), SuccessfulDrops, *ItemToDrop.ItemData->ItemName.ToString());

		// Remove items from inventory
		RemoveItemAtSlot(SlotIndex, SuccessfulDrops);

		// Show notification for dropped items
		if (AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(GetOwner()))
		{
			if (UNotificationComponent* NotificationComp = Character->GetNotificationComponent())
			{
				FText DropMessage;
				if (SuccessfulDrops > 1)
				{
					DropMessage = FText::Format(FText::FromString(TEXT("Dropped {0} x{1}")),
						ItemToDrop.ItemData->ItemName,
						FText::AsNumber(SuccessfulDrops));
				}
				else
				{
					DropMessage = FText::Format(FText::FromString(TEXT("Dropped {0}")),
						ItemToDrop.ItemData->ItemName);
				}

				// Show notification with icon
				UTexture2D* ItemIcon = ItemToDrop.ItemData->ItemIcon.LoadSynchronous();
				FNotificationData NotifData(DropMessage, ENotificationType::Info, 2.0f, ItemIcon);
				NotificationComp->ShowNotification(NotifData);
			}
		}

		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to drop any items!"));
		return false;
	}
}

bool UInventoryComponent::DropItemInFront(int32 SlotIndex, int32 Quantity, float DropDistance)
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}

	// Try to get the camera for more accurate drop position
	UCameraComponent* Camera = Owner->FindComponentByClass<UCameraComponent>();

	FVector DropLocation;
	FRotator DropRotation;

	if (Camera)
	{
		// Drop in front of camera
		DropLocation = Camera->GetComponentLocation() + (Camera->GetForwardVector() * DropDistance);
		DropRotation = Camera->GetComponentRotation();
	}
	else
	{
		// Fall back to actor's forward vector
		DropLocation = Owner->GetActorLocation() + (Owner->GetActorForwardVector() * DropDistance);
		DropRotation = Owner->GetActorRotation();
	}

	// Adjust drop location slightly downward so item doesn't spawn in player's face
	DropLocation.Z -= 50.0f;

	return DropItem(SlotIndex, Quantity, DropLocation, DropRotation);
}

bool UInventoryComponent::DestroyItem(int32 SlotIndex, int32 Quantity)
{
	if (!Items.IsValidIndex(SlotIndex) || !Items[SlotIndex].IsValid())
	{
		return false;
	}

	if (Quantity <= 0 || Quantity > Items[SlotIndex].Quantity)
	{
		return false;
	}

	// Store item info for notification before removing
	FInventoryItem DestroyedItem = Items[SlotIndex];
	int32 DestroyedQuantity = Quantity;

	// Remove the item
	bool bSuccess = RemoveItemAtSlot(SlotIndex, Quantity);

	// Show notification if successful
	if (bSuccess)
	{
		if (AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(GetOwner()))
		{
			if (UNotificationComponent* NotificationComp = Character->GetNotificationComponent())
			{
				FText DestroyMessage;
				if (DestroyedQuantity > 1)
				{
					DestroyMessage = FText::Format(FText::FromString(TEXT("Destroyed {0} x{1}")),
						DestroyedItem.ItemData->ItemName,
						FText::AsNumber(DestroyedQuantity));
				}
				else
				{
					DestroyMessage = FText::Format(FText::FromString(TEXT("Destroyed {0}")),
						DestroyedItem.ItemData->ItemName);
				}

				// Show notification with icon
				UTexture2D* ItemIcon = DestroyedItem.ItemData->ItemIcon.LoadSynchronous();
				FNotificationData NotifData(DestroyMessage, ENotificationType::Info, 2.0f, ItemIcon);
				NotificationComp->ShowNotification(NotifData);
			}
		}
	}

	return bSuccess;
}
