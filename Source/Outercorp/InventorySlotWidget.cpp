// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySlotWidget.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Engine/Texture2D.h"
#include "Input/Reply.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "ContextMenuWidget.h"

// Initialize static members
TWeakObjectPtr<UUserWidget> UInventorySlotWidget::CurrentOpenContextMenu = nullptr;
TMap<UInventoryComponent*, TSet<int32>> UInventorySlotWidget::InventorySelections;
TMap<UInventoryComponent*, TArray<TWeakObjectPtr<UInventorySlotWidget>>> UInventorySlotWidget::SlotWidgetRegistry;

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Register this slot widget with the registry
	if (InventoryComponent)
	{
		TArray<TWeakObjectPtr<UInventorySlotWidget>>& Slots = SlotWidgetRegistry.FindOrAdd(InventoryComponent);
		Slots.AddUnique(this);
	}

	// Bind button click
	if (SlotButton)
	{
		SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotClicked);
	}

	// Initialize appearance
	UpdateAppearance();
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	// Handle right-click for context menu - call Blueprint event
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Log, TEXT("Right-click detected on slot %d, IsSlotEmpty=%d, ItemData=%s, Quantity=%d"),
			SlotIndex, IsSlotEmpty(),
			CurrentItem.ItemData ? *CurrentItem.ItemData->ItemName.ToString() : TEXT("null"),
			CurrentItem.Quantity);

		// IMPORTANT: Close any existing context menu FIRST
		CloseCurrentContextMenu();

		// Get mouse position in viewport pixel coordinates
		APlayerController* PC = GetOwningPlayer();
		if (PC)
		{
			float MouseX, MouseY;
			PC->GetMousePosition(MouseX, MouseY);
			FVector2D MousePosition(MouseX, MouseY);

			// Check if slot is empty and call appropriate event
			if (IsSlotEmpty())
			{
				UE_LOG(LogTemp, Log, TEXT("Calling OnEmptySlotRightClicked for slot %d at position (%f, %f)"), SlotIndex, MousePosition.X, MousePosition.Y);
				OnEmptySlotRightClicked(MousePosition);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Calling OnRightClicked for slot %d at position (%f, %f)"), SlotIndex, MousePosition.X, MousePosition.Y);
				OnRightClicked(MousePosition);
			}
		}

		return FReply::Handled();
	}

	// Handle left-click - only if slot has an item
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (CurrentItem.IsValid())
		{
			// Reset drag flag
			bDragStarted = false;

			// Start drag detection
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
		}
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Log, TEXT("NativeOnMouseButtonUp: Slot %d, Button=%s, bDragStarted=%d"),
		SlotIndex,
		*InMouseEvent.GetEffectingButton().ToString(),
		bDragStarted);

	// If left mouse button released and we didn't start a drag
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && !bDragStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("  Left mouse up without drag on slot %d"), SlotIndex);

		// Check if slot has an item
		if (CurrentItem.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("  Slot %d has valid item"), SlotIndex);

			// Check if Ctrl is held
			if (InMouseEvent.IsControlDown())
			{
				// Ctrl+Click: Toggle this item's selection
				UE_LOG(LogTemp, Log, TEXT("  Ctrl+Click - toggling selection for slot %d"), SlotIndex);
				ToggleSelection();
			}
			else
			{
				// Normal click: Clear all selections and select only this item
				if (!InventoryComponent)
				{
					UE_LOG(LogTemp, Warning, TEXT("  No InventoryComponent!"));
					return FReply::Handled();
				}

				UE_LOG(LogTemp, Log, TEXT("  Normal click - selecting only slot %d"), SlotIndex);

				// Clear and set new selection in one operation to avoid double broadcast
				TSet<int32>& SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);
				SelectionSet.Empty();
				SelectionSet.Add(SlotIndex);

				// Broadcast once
				BroadcastSelectionChanged();
			}

			return FReply::Handled();
		}
		else
		{
			// Clicked on empty slot - clear all selections
			if (InventoryComponent)
			{
				TSet<int32>& SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);
				if (SelectionSet.Num() > 0)
				{
					SelectionSet.Empty();
					BroadcastSelectionChanged();
				}
			}

			return FReply::Handled();
		}
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// Mark that we started dragging (so mouse up doesn't trigger selection)
	bDragStarted = true;

	if (!CurrentItem.IsValid() || !InventoryComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Drag detected on slot %d"), SlotIndex);

	// Create drag-drop operation
	UInventoryDragDropOperation* DragDropOp = NewObject<UInventoryDragDropOperation>();
	DragDropOp->SourceSlotIndex = SlotIndex;
	DragDropOp->DraggedItem = CurrentItem;
	DragDropOp->InventoryComponent = InventoryComponent;

	// Check if shift is held for split operation
	DragDropOp->bIsSplitOperation = InMouseEvent.IsShiftDown() && CurrentItem.Quantity > 1;

	// Create visual widget for dragging
	UInventorySlotWidget* DragVisual = CreateWidget<UInventorySlotWidget>(GetOwningPlayer(), GetClass());
	if (DragVisual)
	{
		DragVisual->SetItem(CurrentItem);
		DragDropOp->DefaultDragVisual = DragVisual;
		DragDropOp->DraggedVisual = DragVisual;
		DragDropOp->Pivot = EDragPivot::MouseDown;
	}

	OutOperation = DragDropOp;
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UInventoryDragDropOperation* DragDropOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (!DragDropOp || !InventoryComponent)
	{
		// Reset drag flag even if drop fails
		bDragStarted = false;
		return false;
	}

	// Reset background color - but preserve selection if selected
	if (BackgroundBorder && !bIsSelected)
	{
		BackgroundBorder->SetBrushColor(NormalColor);
	}

	// Don't drop on same slot
	if (DragDropOp->SourceSlotIndex == SlotIndex)
	{
		bDragStarted = false;
		return false;
	}

	// Handle split operation
	if (DragDropOp->bIsSplitOperation)
	{
		int32 SplitAmount = DragDropOp->DraggedItem.Quantity / 2;
		if (SplitAmount > 0)
		{
			InventoryComponent->SplitStack(DragDropOp->SourceSlotIndex, SlotIndex, SplitAmount);

			// When splitting, deselect the source (since it's now two different items)
			TSet<int32>& SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);
			if (SelectionSet.Contains(DragDropOp->SourceSlotIndex))
			{
				SelectionSet.Remove(DragDropOp->SourceSlotIndex);
				BroadcastSelectionChanged();
				UE_LOG(LogTemp, Log, TEXT("Deselected slot %d after split"), DragDropOp->SourceSlotIndex);
			}
		}
		bDragStarted = false;
		return true;
	}

	// When moving items, update selection to follow the item BEFORE moving
	// This ensures bIsSelected is correct when UpdateAppearance is called during MoveItem
	TSet<int32>& SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);
	bool bSourceWasSelected = SelectionSet.Contains(DragDropOp->SourceSlotIndex);

	if (bSourceWasSelected)
	{
		// Remove source slot from selection
		SelectionSet.Remove(DragDropOp->SourceSlotIndex);

		// Add destination slot to selection (item moved here)
		SelectionSet.Add(SlotIndex);

		// Broadcast the change BEFORE moving the item
		// This ensures all slots have correct bIsSelected when UpdateAppearance is called
		BroadcastSelectionChanged();

		UE_LOG(LogTemp, Log, TEXT("Moved selection from slot %d to slot %d"), DragDropOp->SourceSlotIndex, SlotIndex);
	}

	// Handle normal move/swap
	InventoryComponent->MoveItem(DragDropOp->SourceSlotIndex, SlotIndex);

	bDragStarted = false;
	return true;
}

void UInventorySlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	// Highlight slot when dragging over
	if (BackgroundBorder)
	{
		BackgroundBorder->SetBrushColor(HoverColor);
	}
}

void UInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	// Reset background color - but preserve selection color if selected
	if (BackgroundBorder && !bIsSelected)
	{
		BackgroundBorder->SetBrushColor(NormalColor);
	}

	// Reset drag flag when drag leaves
	bDragStarted = false;
}

void UInventorySlotWidget::SetItem(const FInventoryItem& Item)
{
	CurrentItem = Item;
	UpdateAppearance();
}

void UInventorySlotWidget::OnSlotClicked()
{
	// This is called by the button, but we handle selection in NativeOnMouseButtonDown instead
	// Keep this for potential future use or debugging
	if (CurrentItem.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Slot %d button clicked: %s"), SlotIndex, *CurrentItem.ItemData->ItemName.ToString());
	}
}

void UInventorySlotWidget::HandleContextMenuAction(FName ActionID)
{
	UE_LOG(LogTemp, Log, TEXT("Context menu action: %s"), *ActionID.ToString());

	if (ActionID == "Split")
	{
		HandleSplitItem();
	}
	else if (ActionID == "Destroy")
	{
		HandleDestroyItem();
	}
	else if (ActionID == "ShowInfo")
	{
		HandleShowInfo();
	}
	else if (ActionID == "StackAll")
	{
		HandleStackAll();
	}
	else if (ActionID == "StackAllEmpty")
	{
		HandleStackAllEmpty();
	}
	else if (ActionID == "SelectAll")
	{
		HandleSelectAll();
	}
	else if (ActionID == "InvertSelection")
	{
		HandleInvertSelection();
	}
}

void UInventorySlotWidget::HandleSplitItem()
{
	if (!CurrentItem.IsValid() || !InventoryComponent)
	{
		return;
	}

	if (CurrentItem.Quantity <= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot split item with quantity 1 or less"));
		return;
	}

	// Find first empty slot
	int32 EmptySlot = InventoryComponent->FindEmptySlot();
	if (EmptySlot == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No empty slot available for split"));
		return;
	}

	// Split stack in half
	int32 SplitAmount = CurrentItem.Quantity / 2;
	InventoryComponent->SplitStack(SlotIndex, EmptySlot, SplitAmount);

	UE_LOG(LogTemp, Log, TEXT("Split %d items from slot %d to slot %d"), SplitAmount, SlotIndex, EmptySlot);
}

void UInventorySlotWidget::HandleDestroyItem()
{
	if (!CurrentItem.IsValid() || !InventoryComponent)
	{
		return;
	}

	// Cache the values we need before any widgets are destroyed
	int32 CachedSlotIndex = SlotIndex;
	int32 CachedQuantity = CurrentItem.Quantity;
	FString ItemName = CurrentItem.ItemData->ItemName.ToString();
	UInventoryComponent* CachedInventoryComponent = InventoryComponent;

	// CRITICAL: Defer the actual item removal until the next frame
	// This prevents the inventory update broadcast from destroying the slot widget
	// while the context menu button is still processing the click event
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick([CachedInventoryComponent, CachedSlotIndex, CachedQuantity, ItemName]()
		{
			if (IsValid(CachedInventoryComponent))
			{
				// Now it's safe to remove the item and trigger the inventory update
				CachedInventoryComponent->RemoveItemAtSlot(CachedSlotIndex, CachedQuantity);

				UE_LOG(LogTemp, Log, TEXT("Destroyed %d x %s from slot %d"),
					CachedQuantity,
					*ItemName,
					CachedSlotIndex);
			}
		});
	}
}

void UInventorySlotWidget::HandleShowInfo()
{
	if (!CurrentItem.IsValid())
	{
		return;
	}

	// Call Blueprint event to show UI
	ShowItemInfoUI(CurrentItem);
}

void UInventorySlotWidget::HandleStackAll()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Placeholder: Stack all similar items into this slot
	// This is a simplified implementation
	UE_LOG(LogTemp, Log, TEXT("Stack All triggered for slot %d"), SlotIndex);

	// In a real implementation, you would:
	// 1. Find all items of the same type in the inventory
	// 2. Merge them into existing stacks, prioritizing filling up partial stacks
	// 3. Move consolidated stacks to fill gaps

	// For now, just log that it was called
	UE_LOG(LogTemp, Warning, TEXT("Stack All functionality - to be implemented"));
}

void UInventorySlotWidget::HandleStackAllEmpty()
{
	if (!InventoryComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Stack All (Empty) triggered - consolidating all stackable items"));

	// Get all items
	TArray<FInventoryItem> AllItems = InventoryComponent->GetAllItems();

	// Group items by ItemID to find stackable groups
	TMap<FName, TArray<int32>> ItemGroups;
	for (int32 i = 0; i < AllItems.Num(); i++)
	{
		if (AllItems[i].IsValid() && AllItems[i].ItemData && AllItems[i].ItemData->MaxStackSize > 1)
		{
			FName ItemID = AllItems[i].ItemData->ItemID;
			ItemGroups.FindOrAdd(ItemID).Add(i);
		}
	}

	// For each group, try to stack them together
	for (const auto& Group : ItemGroups)
	{
		const TArray<int32>& SlotIndices = Group.Value;
		if (SlotIndices.Num() > 1)
		{
			// Try to merge all items in this group into the first slot
			for (int32 i = 1; i < SlotIndices.Num(); i++)
			{
				InventoryComponent->MergeStacks(SlotIndices[i], SlotIndices[0]);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Stack All completed"));
}

void UInventorySlotWidget::HandleSelectAll()
{
	if (!InventoryComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Select All triggered"));

	TArray<FInventoryItem> AllItems = InventoryComponent->GetAllItems();
	TSet<int32>& SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);

	// Clear and rebuild selection set with all occupied slots
	SelectionSet.Empty();

	for (int32 i = 0; i < AllItems.Num(); i++)
	{
		if (AllItems[i].IsValid())
		{
			SelectionSet.Add(i);
		}
	}

	// Broadcast to all slots in the inventory
	BroadcastSelectionChanged();

	UE_LOG(LogTemp, Log, TEXT("Selected %d slots"), SelectionSet.Num());
}

void UInventorySlotWidget::HandleInvertSelection()
{
	if (!InventoryComponent)
	{
		return;
	}

	TArray<FInventoryItem> AllItems = InventoryComponent->GetAllItems();
	TSet<int32>& SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);

	UE_LOG(LogTemp, Log, TEXT("Invert Selection - Before: %d items selected"), SelectionSet.Num());

	// Build a new selection set with inverted state
	TSet<int32> NewSelection;

	for (int32 i = 0; i < AllItems.Num(); i++)
	{
		// Only process slots that have items
		if (AllItems[i].IsValid())
		{
			bool bWasSelected = SelectionSet.Contains(i);
			// Invert: if currently selected, deselect; if not selected, select
			if (!bWasSelected)
			{
				NewSelection.Add(i);
				UE_LOG(LogTemp, Log, TEXT("  Slot %d: was NOT selected, now selecting"), i);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("  Slot %d: was selected, now deselecting"), i);
			}
		}
	}

	// Update the shared selection state
	SelectionSet = NewSelection;

	UE_LOG(LogTemp, Log, TEXT("Invert Selection - After: %d items selected"), SelectionSet.Num());

	// Broadcast to all slots in the inventory
	BroadcastSelectionChanged();
}

void UInventorySlotWidget::UpdateAppearance()
{
	if (CurrentItem.IsValid() && CurrentItem.ItemData)
	{
		// Set item icon
		if (ItemIcon)
		{
			ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (CurrentItem.ItemData->ItemIcon.IsNull())
			{
				ItemIcon->SetOpacity(ItemNoIconOpacity);
				ItemIcon->SetBrushFromTexture(nullptr);
			}
			else
			{
				ItemIcon->SetOpacity(ItemIconOpacity);
				// Load icon asynchronously
				UTexture2D* IconTexture = CurrentItem.ItemData->ItemIcon.LoadSynchronous();
				ItemIcon->SetBrushFromTexture(IconTexture);
			}
		}

		// Set quantity text
		if (QuantityText)
		{
			if (CurrentItem.Quantity > 1)
			{
				QuantityText->SetText(FText::AsNumber(CurrentItem.Quantity));
				QuantityText->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				QuantityText->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		// Set quantity border visibility
		if (QuantityBorder)
		{
			if (CurrentItem.Quantity > 1)
			{
				QuantityBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				QuantityBorder->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		// Set rarity border color
		if (RarityBorder)
		{
			RarityBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			FLinearColor RarityColor = CurrentItem.ItemData->GetRarityColor();
			RarityBorder->SetBrushColor(RarityColor);
		}
	}
	else
	{
		// Empty slot - hide icon completely
		if (ItemIcon)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}

		if (QuantityText)
		{
			QuantityText->SetVisibility(ESlateVisibility::Hidden);
		}

		if (QuantityBorder)
		{
			QuantityBorder->SetVisibility(ESlateVisibility::Hidden);
		}

		if (RarityBorder)
		{
			RarityBorder->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// Set background color - but preserve selection color if selected
	if (BackgroundBorder && !bIsSelected)
	{
		BackgroundBorder->SetBrushColor(NormalColor);
	}
	// Note: If bIsSelected is true, don't override the selection color set by Blueprint
}

void UInventorySlotWidget::CloseCurrentContextMenu()
{
	if (CurrentOpenContextMenu.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Closing previous context menu"));

		UUserWidget* MenuToClose = CurrentOpenContextMenu.Get();

		// Clear the reference FIRST before closing
		// This prevents IsValid checks in Blueprint from finding stale references
		CurrentOpenContextMenu = nullptr;

		// Then close the menu
		if (UContextMenuWidget* ContextMenu = Cast<UContextMenuWidget>(MenuToClose))
		{
			ContextMenu->CloseMenu();
		}
		else
		{
			// Fallback for generic widgets
			MenuToClose->RemoveFromParent();
		}
	}
}

void UInventorySlotWidget::SetCurrentContextMenu(UUserWidget* ContextMenu)
{
	if (ContextMenu)
	{
		UE_LOG(LogTemp, Log, TEXT("Setting new current context menu"));
		CurrentOpenContextMenu = ContextMenu;
	}
}

UUserWidget* UInventorySlotWidget::GetCurrentContextMenu()
{
	return CurrentOpenContextMenu.IsValid() ? CurrentOpenContextMenu.Get() : nullptr;
}

void UInventorySlotWidget::SetSelected(bool bSelected)
{
	if (!InventoryComponent)
	{
		return;
	}

	// Update shared selection state
	TSet<int32>& SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);

	if (bSelected)
	{
		SelectionSet.Add(SlotIndex);
	}
	else
	{
		SelectionSet.Remove(SlotIndex);
	}

	// Broadcast to all slots in the inventory
	BroadcastSelectionChanged();

	UE_LOG(LogTemp, Log, TEXT("Slot %d selection state: %s"), SlotIndex, bSelected ? TEXT("Selected") : TEXT("Deselected"));
}

void UInventorySlotWidget::ToggleSelection()
{
	SetSelected(!bIsSelected);
}

TArray<int32> UInventorySlotWidget::GetSelectedSlots() const
{
	if (!InventoryComponent)
	{
		return TArray<int32>();
	}

	const TSet<int32>* SelectionSet = InventorySelections.Find(InventoryComponent);
	if (SelectionSet)
	{
		return SelectionSet->Array();
	}

	return TArray<int32>();
}

void UInventorySlotWidget::ClearAllSelections()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Clear the selection set for this inventory
	InventorySelections.Remove(InventoryComponent);

	// Broadcast to all slots in the inventory
	BroadcastSelectionChanged();

	UE_LOG(LogTemp, Log, TEXT("Cleared all selections for inventory"));
}

bool UInventorySlotWidget::HasAnySelection() const
{
	if (!InventoryComponent)
	{
		return false;
	}

	const TSet<int32>* SelectionSet = InventorySelections.Find(InventoryComponent);
	return SelectionSet && SelectionSet->Num() > 0;
}

void UInventorySlotWidget::BroadcastSelectionChanged()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Get the current selection array
	TArray<int32> SelectedSlots = GetSelectedSlots();

	// Use the registry to find all slot widgets for this inventory component
	TArray<TWeakObjectPtr<UInventorySlotWidget>>* AllSlots = SlotWidgetRegistry.Find(InventoryComponent);
	if (AllSlots)
	{
		// Clean up invalid weak pointers and notify all valid slots
		for (int32 i = AllSlots->Num() - 1; i >= 0; i--)
		{
			if (!(*AllSlots)[i].IsValid())
			{
				// Remove invalid weak pointer
				AllSlots->RemoveAt(i);
				continue;
			}

			UInventorySlotWidget* SlotWidget = (*AllSlots)[i].Get();
			if (SlotWidget)
			{
				// Update each slot's selection state
				const TSet<int32>* SelectionSet = InventorySelections.Find(InventoryComponent);
				SlotWidget->bIsSelected = SelectionSet && SelectionSet->Contains(SlotWidget->SlotIndex);

				// Fire the Blueprint event on each slot
				SlotWidget->OnSelectionChanged(SelectedSlots);
			}
		}
	}
}
