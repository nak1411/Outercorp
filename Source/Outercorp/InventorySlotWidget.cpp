// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySlotWidget.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "InventoryListRowWidget.h"
#include "TooltipWidget.h"
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
TMap<UInventoryComponent *, TSet<int32>> UInventorySlotWidget::InventorySelections;
TMap<UInventoryComponent *, TArray<TWeakObjectPtr<UInventorySlotWidget>>> UInventorySlotWidget::SlotWidgetRegistry;

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Ensure hover state is reset on construction
	bIsHovered = false;

	// Register this slot widget with the registry
	if (InventoryComponent)
	{
		TArray<TWeakObjectPtr<UInventorySlotWidget>> &Slots = SlotWidgetRegistry.FindOrAdd(InventoryComponent);
		Slots.AddUnique(this);
	}

	// Bind button click
	if (SlotButton)
	{
		SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotClicked);
	}

	// Ensure SelectionBorder fills entire slot and is hit-test invisible
	if (SelectionBorder)
	{
		if (UCanvasPanelSlot *CanvasSlot = Cast<UCanvasPanelSlot>(SelectionBorder->Slot))
		{
			CanvasSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
			CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
			CanvasSlot->SetZOrder(100); // Ensure it renders on top
		}
		SelectionBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// Make sure the border has no content (it's just a frame)
		SelectionBorder->ClearChildren();

		// Set initial color to transparent
		SelectionBorder->SetBrushColor(FLinearColor::Transparent);
	}

	// Initialize appearance
	UpdateAppearance();
}

void UInventorySlotWidget::NativeDestruct()
{
	// Clear hover state when widget is destroyed
	bIsHovered = false;

	// Hide and destroy tooltip if active
	HideTooltip();

	// Unregister from the registry
	if (InventoryComponent)
	{
		TArray<TWeakObjectPtr<UInventorySlotWidget>> *Slots = SlotWidgetRegistry.Find(InventoryComponent);
		if (Slots)
		{
			Slots->Remove(this);
		}
	}

	Super::NativeDestruct();
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{

	// Handle right-click for context menu - call Blueprint event
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		// IMPORTANT: Close any existing context menu FIRST
		CloseCurrentContextMenu();

		// Get mouse position in viewport pixel coordinates
		APlayerController *PC = GetOwningPlayer();
		if (PC)
		{
			float MouseX, MouseY;
			PC->GetMousePosition(MouseX, MouseY);
			FVector2D MousePosition(MouseX, MouseY);

			// Check if slot is empty and call appropriate event
			if (IsSlotEmpty())
			{
				OnEmptySlotRightClicked(MousePosition);
			}
			else
			{
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

FReply UInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	// If left mouse button released and we didn't start a drag
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && !bDragStarted)
	{
		// Check if slot has an item
		if (CurrentItem.IsValid())
		{
			// Check if Ctrl is held
			if (InMouseEvent.IsControlDown())
			{
				// Ctrl+Click: Toggle this item's selection
				ToggleSelection();
			}
			else
			{
				// Normal click: Clear all selections and select only this item
				if (!InventoryComponent)
				{
					return FReply::Handled();
				}

				// Update this slot's selection state immediately for instant feedback
				TSet<int32> &SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);
				SelectionSet.Empty();
				SelectionSet.Add(SlotIndex);

				// Update THIS slot's visual immediately
				bIsSelected = true;
				UpdateAppearance();

				// Then broadcast to other slots (this might take a frame)
				BroadcastSelectionChanged();
			}

			return FReply::Handled();
		}
		else
		{
			// Clicked on empty slot - clear all selections
			if (InventoryComponent)
			{
				TSet<int32> &SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);
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

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent, UDragDropOperation *&OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// Mark that we started dragging (so mouse up doesn't trigger selection)
	bDragStarted = true;

	if (!CurrentItem.IsValid() || !InventoryComponent)
	{
		return;
	}

	// Clear all selections when starting a drag operation (both grid and list)
	TSet<int32> &SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);
	if (SelectionSet.Num() > 0)
	{
		SelectionSet.Empty();
		BroadcastSelectionChanged();
	}

	// Also clear list view selections to keep both views in sync
	UInventoryListRowWidget::ClearAllRowSelections();

	// Create drag-drop operation
	UInventoryDragDropOperation *DragDropOp = NewObject<UInventoryDragDropOperation>();
	DragDropOp->SourceSlotIndex = SlotIndex;
	DragDropOp->DraggedItem = CurrentItem;
	DragDropOp->InventoryComponent = InventoryComponent;

	// Check if shift is held for split operation
	DragDropOp->bIsSplitOperation = InMouseEvent.IsShiftDown() && CurrentItem.Quantity > 1;

	// Create visual widget for dragging
	UInventorySlotWidget *DragVisual = CreateWidget<UInventorySlotWidget>(GetOwningPlayer(), GetClass());
	if (DragVisual)
	{
		DragVisual->SetItem(CurrentItem);
		DragDropOp->DefaultDragVisual = DragVisual;
		DragDropOp->DraggedVisual = DragVisual;
		DragDropOp->Pivot = EDragPivot::CenterCenter;
		DragDropOp->Offset = FVector2D(0, 0);
	}

	OutOperation = DragDropOp;
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UInventoryDragDropOperation *DragDropOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (!DragDropOp || !InventoryComponent)
	{
		// Reset drag flag even if drop fails
		bDragStarted = false;
		return false;
	}

	// Clear the drag hover color
	if (SelectionBorder)
	{
		// Reset to transparent - Blueprint will handle proper hover/selection color via OnHoverStateChanged
		SelectionBorder->SetBrushColor(FLinearColor::Transparent);
	}

	// Trigger hover state update to restore proper SelectionBorder appearance
	OnHoverStateChanged(bIsHovered);

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
			TSet<int32> &SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);
			if (SelectionSet.Contains(DragDropOp->SourceSlotIndex))
			{
				SelectionSet.Remove(DragDropOp->SourceSlotIndex);
				BroadcastSelectionChanged();
			}
		}
		bDragStarted = false;
		return true;
	}

	// When moving items, update selection to follow the item BEFORE moving
	// This ensures bIsSelected is correct when UpdateAppearance is called during MoveItem
	TSet<int32> &SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);
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
	}

	// Handle normal move/swap
	InventoryComponent->MoveItem(DragDropOp->SourceSlotIndex, SlotIndex);

	bDragStarted = false;
	return true;
}

void UInventorySlotWidget::NativeOnDragEnter(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	// Highlight slot when dragging over
	if (SelectionHighlight)
	{
		SelectionHighlight->SetColorAndOpacity(DragHoverColor);
		SelectionHighlight->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (SelectionBorder)
	{
		SelectionBorder->SetBrushColor(DragHoverColor);
		SelectionBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UInventorySlotWidget::NativeOnDragLeave(const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	// Clear the drag hover and restore normal state
	UpdateAppearance(); // This will restore selection state or hide the highlight

	// Trigger hover state update
	OnHoverStateChanged(bIsHovered);

	// Reset drag flag when drag leaves
	bDragStarted = false;
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	// Only update if state actually changed
	if (!bIsHovered)
	{
		bIsHovered = true;

		// Update appearance to apply hover tint to item icon
		UpdateAppearance();

		// Notify Blueprint of hover state change
		OnHoverStateChanged(true);

		// Show tooltip if enabled and slot has an item
		if (bEnableTooltip && CurrentItem.IsValid())
		{
			ShowTooltip();
		}
	}
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent &InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	// Only update if state actually changed
	if (bIsHovered)
	{
		bIsHovered = false;

		// Update appearance to remove hover tint from item icon
		UpdateAppearance();

		// Notify Blueprint of hover state change
		OnHoverStateChanged(false);

		// Hide tooltip when mouse leaves (unless it's persistent)
		if (bEnableTooltip && ActiveTooltip && !TooltipConfig.bPersistent)
		{
			HideTooltip();
		}
	}
}

void UInventorySlotWidget::SetItem(const FInventoryItem &Item)
{
	CurrentItem = Item;
	UpdateAppearance();
}

void UInventorySlotWidget::OnSlotClicked()
{
	// This is called by the button, but we handle selection in NativeOnMouseButtonDown instead
}

void UInventorySlotWidget::HandleContextMenuAction_Implementation(FName ActionID)
{
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
	else if (ActionID == "StackAllEmpty" || ActionID == "StackAllItems")
	{
		HandleStackAllItems();
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
	if (!InventoryComponent)
	{
		return;
	}

	// Get all selected slots
	TArray<int32> SelectedSlots = GetSelectedSlots();

	// If there are selected items, split all of them
	if (SelectedSlots.Num() > 0)
	{
		// Begin batch update to reduce UI refreshes
		InventoryComponent->BeginBatchUpdate();

		for (int32 SelectedSlot : SelectedSlots)
		{
			// Get fresh item data for this slot
			FInventoryItem Item = InventoryComponent->GetItemAtSlot(SelectedSlot);

			// Skip if item is invalid or quantity is 1 or less
			if (!Item.IsValid() || Item.Quantity <= 1)
			{
				continue;
			}

			// Find first empty slot
			int32 EmptySlot = InventoryComponent->FindEmptySlot();
			if (EmptySlot == -1)
			{
				// No more empty slots - stop splitting
				break;
			}

			// Split stack in half
			int32 SplitAmount = Item.Quantity / 2;
			InventoryComponent->SplitStack(SelectedSlot, EmptySlot, SplitAmount);
		}

		// End batch update - this will trigger UI refresh automatically
		InventoryComponent->EndBatchUpdate();
	}
	else
	{
		// No selection - split just the current item (right-clicked item)
		if (!CurrentItem.IsValid())
		{
			return;
		}

		if (CurrentItem.Quantity <= 1)
		{
			return;
		}

		// Find first empty slot
		int32 EmptySlot = InventoryComponent->FindEmptySlot();
		if (EmptySlot == -1)
		{
			return;
		}

		// Split stack in half
		int32 SplitAmount = CurrentItem.Quantity / 2;
		InventoryComponent->SplitStack(SlotIndex, EmptySlot, SplitAmount);
	}

	// No need to reflow/compress - the split operation already places the item in a valid slot
	// The inventory update broadcast will handle UI refresh automatically
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
	UInventoryComponent *CachedInventoryComponent = InventoryComponent;

	// CRITICAL: Defer the actual item removal until the next frame
	// This prevents the inventory update broadcast from destroying the slot widget
	// while the context menu button is still processing the click event
	if (UWorld *World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick([CachedInventoryComponent, CachedSlotIndex, CachedQuantity, ItemName]()
													 {
			if (IsValid(CachedInventoryComponent))
			{
				// Now it's safe to remove the item and trigger the inventory update
				CachedInventoryComponent->RemoveItemAtSlot(CachedSlotIndex, CachedQuantity);
			} });
	}
}

void UInventorySlotWidget::HandleShowInfo()
{
	if (!CurrentItem.IsValid())
	{
		return;
	}

	// Call Blueprint event (for modular window creation)
	ShowItemInfoUI(CurrentItem);

	// If ItemInfoWidgetClass is set, also auto-create and show the widget
	if (ItemInfoWidgetClass)
	{
		// Close existing window if open
		if (ActiveItemInfoWindow)
		{
			ActiveItemInfoWindow->RemoveFromParent();
			ActiveItemInfoWindow = nullptr;
		}

		// Create and show the item info window
		ActiveItemInfoWindow = CreateWidget<UUserWidget>(GetOwningPlayer(), ItemInfoWidgetClass);
		if (ActiveItemInfoWindow)
		{
			ActiveItemInfoWindow->AddToViewport(999); // High Z-order so it's on top
		}
	}
}

void UInventorySlotWidget::HandleStackAll()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Get the current item fresh from the inventory component
	FInventoryItem ItemToStack = InventoryComponent->GetItemAtSlot(SlotIndex);

	// If slot is empty, stack ALL items in the inventory (same as Stack All Items)
	if (!ItemToStack.IsValid())
	{
		HandleStackAllItems();
		return;
	}

	// Get the item ID we want to stack
	FName TargetItemID = ItemToStack.ItemData->ItemID;

	// Get all items and find matching items
	TArray<FInventoryItem> AllItems = InventoryComponent->GetAllItems();
	TArray<int32> MatchingSlots;

	// Find all slots containing the same item type
	for (int32 i = 0; i < AllItems.Num(); i++)
	{
		if (AllItems[i].IsValid() &&
			AllItems[i].ItemData &&
			AllItems[i].ItemData->ItemID == TargetItemID &&
			AllItems[i].ItemData->MaxStackSize > 1)
		{
			MatchingSlots.Add(i);
		}
	}

	if (MatchingSlots.Num() <= 1)
	{
		return;
	}

	// Sort slots so we process them in order
	MatchingSlots.Sort();

	// Begin batch update to reduce UI refreshes
	InventoryComponent->BeginBatchUpdate();

	// Merge all stacks together
	// Process from the end backwards to avoid index issues when items are removed
	for (int32 i = MatchingSlots.Num() - 1; i > 0; i--)
	{
		int32 SourceSlot = MatchingSlots[i];

		// Try to find a target slot that has space (search from beginning)
		for (int32 j = 0; j < i; j++)
		{
			int32 TargetSlot = MatchingSlots[j];

			// Re-fetch items each time to get current state
			FInventoryItem SourceItem = InventoryComponent->GetItemAtSlot(SourceSlot);
			FInventoryItem TargetItem = InventoryComponent->GetItemAtSlot(TargetSlot);

			// Skip if source is already empty (already merged)
			if (!SourceItem.IsValid())
			{
				break;
			}

			// Check if target has space
			if (TargetItem.IsValid() && TargetItem.Quantity < TargetItem.ItemData->MaxStackSize)
			{
				InventoryComponent->MergeStacks(SourceSlot, TargetSlot);

				// Check if source is now empty, if so we're done with this source
				SourceItem = InventoryComponent->GetItemAtSlot(SourceSlot);
				if (!SourceItem.IsValid())
				{
					break;
				}
				// If source still has items, continue trying other targets
			}
		}
	}

	// End batch update - this will trigger a single UI refresh
	InventoryComponent->EndBatchUpdate();

	// Clear all selections after stacking since items have moved and many slots are now empty
	ClearAllSelections();

	// Also clear list view selections to keep both views in sync
	UInventoryListRowWidget::ClearAllRowSelections();
}

void UInventorySlotWidget::HandleStackAllItems()
{
	if (!InventoryComponent)
	{
		return;
	}

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

	// Begin batch update to reduce UI refreshes
	InventoryComponent->BeginBatchUpdate();

	// For each group, try to stack them together
	for (const auto &Group : ItemGroups)
	{
		const TArray<int32> &SlotIndices = Group.Value;
		if (SlotIndices.Num() > 1)
		{
			// Try to merge all items in this group into the first slot
			for (int32 i = 1; i < SlotIndices.Num(); i++)
			{
				InventoryComponent->MergeStacks(SlotIndices[i], SlotIndices[0]);
			}
		}
	}

	// End batch update - this will trigger a single UI refresh
	InventoryComponent->EndBatchUpdate();

	// Clear all selections after stacking since items have moved and many slots are now empty
	ClearAllSelections();

	// Also clear list view selections to keep both views in sync
	UInventoryListRowWidget::ClearAllRowSelections();
}

void UInventorySlotWidget::HandleSelectAll()
{
	if (!InventoryComponent)
	{
		return;
	}

	TArray<FInventoryItem> AllItems = InventoryComponent->GetAllItems();
	TSet<int32> &SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);

	// Clear and rebuild selection set with all occupied slots
	SelectionSet.Empty();

	for (int32 i = 0; i < AllItems.Num(); i++)
	{
		// Re-fetch the current item to ensure it's still valid after stacking
		FInventoryItem ItemAtSlot = InventoryComponent->GetItemAtSlot(i);
		if (ItemAtSlot.IsValid())
		{
			SelectionSet.Add(i);
		}
	}

	// Broadcast to all slots in the inventory
	BroadcastSelectionChanged();
}

void UInventorySlotWidget::HandleInvertSelection()
{
	if (!InventoryComponent)
	{
		return;
	}

	TArray<FInventoryItem> AllItems = InventoryComponent->GetAllItems();
	TSet<int32> &SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);

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
			}
		}
	}

	// Update the shared selection state
	SelectionSet = NewSelection;

	// Broadcast to all slots in the inventory
	BroadcastSelectionChanged();
}

void UInventorySlotWidget::UpdateAppearance()
{
	// Control hover overlay visibility and color
	if (HoverOverlay)
	{
		// Only show hover overlay if:
		// 1. Mouse is hovering
		// 2. Slot is not selected (selection takes priority)
		// 3. Either the slot has an item, OR bShowHoverOnEmptySlots is true
		bool bShouldShowHover = bIsHovered && !bIsSelected && (CurrentItem.IsValid() || bShowHoverOnEmptySlots);

		if (bShouldShowHover)
		{
			// Show hover overlay with specified color
			HoverOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			HoverOverlay->SetBrushColor(HoverOverlayColor);
		}
		else
		{
			// Hide hover overlay
			HoverOverlay->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (CurrentItem.IsValid() && CurrentItem.ItemData)
	{
		// Set item icon
		if (ItemIcon)
		{
			if (CurrentItem.ItemData->ItemIcon.IsNull())
			{
				ItemIcon->SetBrushFromTexture(nullptr);
				ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				// Load icon synchronously (already loaded in most cases due to caching)
				UTexture2D *IconTexture = CurrentItem.ItemData->ItemIcon.LoadSynchronous();
				if (IconTexture)
				{
					ItemIcon->SetBrushFromTexture(IconTexture);
					ItemIcon->SetOpacity(ItemIconOpacity);
					ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
		}

		// Set quantity text
		if (QuantityText)
		{
			if (CurrentItem.Quantity > 0)
			{
				QuantityText->SetText(FText::AsNumber(CurrentItem.Quantity));
				QuantityText->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				QuantityText->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		// Set item name text
		if (ItemText)
		{
			FText NewText = CurrentItem.ItemData->ItemName;
			FLinearColor NewColor = CurrentItem.ItemData->GetRarityColor();

			// Check if text or visibility needs updating
			bool bTextChanged = !CachedItemName.EqualTo(NewText);
			bool bIsHidden = ItemText->GetVisibility() != ESlateVisibility::Visible;

			// Update text if it changed OR if the widget is currently hidden (first time showing)
			if (bTextChanged || bIsHidden)
			{
				// Text changed or first time showing - update both color and text
				ItemText->SetColorAndOpacity(NewColor);
				ItemText->SetText(NewText);
				ItemText->SetVisibility(ESlateVisibility::Visible);
				CachedItemName = NewText;
			}
			else
			{
				// Text is the same and visible, just update color if needed
				ItemText->SetColorAndOpacity(NewColor);
			}
		}

		// Set quantity border visibility
		if (QuantityBorder)
		{
			if (CurrentItem.Quantity > 0)
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
		// Empty slot - hide icon completely and clear cached text
		if (ItemIcon)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}

		if (QuantityText)
		{
			QuantityText->SetVisibility(ESlateVisibility::Hidden);
		}

		if (ItemText)
		{
			ItemText->SetVisibility(ESlateVisibility::Hidden);
			CachedItemName = FText::GetEmpty(); // Clear cache for empty slots
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

	// Update selection highlight - prefer SelectionHighlight image if available
	if (SelectionHighlight)
	{
		if (bIsSelected)
		{
			SelectionHighlight->SetColorAndOpacity(SelectionBorderColor);
			SelectionHighlight->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SelectionHighlight->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (SelectionBorder)
	{
		// Fallback to SelectionBorder if SelectionHighlight doesn't exist
		if (bIsSelected)
		{
			SelectionBorder->SetBrushColor(SelectionBorderColor);
			SelectionBorder->SetContentColorAndOpacity(SelectionBorderColor);
			SelectionBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SelectionBorder->SetBrushColor(FLinearColor::Transparent);
			SelectionBorder->SetContentColorAndOpacity(FLinearColor(1, 1, 1, 1));
		}
	}

	// Also fire Blueprint events for additional customization
	// OnSelectionChanged and OnHoverStateChanged can be used in Blueprint for extra effects
}

void UInventorySlotWidget::CloseCurrentContextMenu()
{
	if (CurrentOpenContextMenu.IsValid())
	{
		UUserWidget *MenuToClose = CurrentOpenContextMenu.Get();

		// Clear the reference FIRST before closing
		// This prevents IsValid checks in Blueprint from finding stale references
		CurrentOpenContextMenu = nullptr;

		// Then close the menu
		if (UContextMenuWidget *ContextMenu = Cast<UContextMenuWidget>(MenuToClose))
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

void UInventorySlotWidget::SetCurrentContextMenu(UUserWidget *ContextMenu)
{
	if (ContextMenu)
	{
		CurrentOpenContextMenu = ContextMenu;
	}
}

UUserWidget *UInventorySlotWidget::GetCurrentContextMenu()
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
	TSet<int32> &SelectionSet = InventorySelections.FindOrAdd(InventoryComponent);

	if (bSelected)
	{
		SelectionSet.Add(SlotIndex);
	}
	else
	{
		SelectionSet.Remove(SlotIndex);
	}

	// Update THIS slot's visual immediately for instant feedback
	bIsSelected = bSelected;
	UpdateAppearance();

	// Then broadcast to all slots in the inventory
	BroadcastSelectionChanged();
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

	const TSet<int32> *SelectionSet = InventorySelections.Find(InventoryComponent);
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
}

void UInventorySlotWidget::ClearAllSelectionsForInventory(UInventoryComponent *InInventoryComponent)
{
	if (!InInventoryComponent)
	{
		return;
	}

	// Clear the selection set for this inventory
	InventorySelections.Remove(InInventoryComponent);

	// Find and broadcast to all slots in the inventory
	TArray<TWeakObjectPtr<UInventorySlotWidget>> *AllSlots = SlotWidgetRegistry.Find(InInventoryComponent);
	if (AllSlots)
	{
		for (int32 i = AllSlots->Num() - 1; i >= 0; i--)
		{
			if (!(*AllSlots)[i].IsValid())
			{
				AllSlots->RemoveAt(i);
				continue;
			}

			UInventorySlotWidget *SlotWidget = (*AllSlots)[i].Get();
			if (SlotWidget)
			{
				SlotWidget->bIsSelected = false;
				SlotWidget->UpdateAppearance();
			}
		}
	}
}

bool UInventorySlotWidget::HasAnySelection() const
{
	if (!InventoryComponent)
	{
		return false;
	}

	const TSet<int32> *SelectionSet = InventorySelections.Find(InventoryComponent);
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
	TArray<TWeakObjectPtr<UInventorySlotWidget>> *AllSlots = SlotWidgetRegistry.Find(InventoryComponent);
	if (AllSlots)
	{
		const TSet<int32> *SelectionSet = InventorySelections.Find(InventoryComponent);

		// Clean up invalid weak pointers and notify all valid slots
		for (int32 i = AllSlots->Num() - 1; i >= 0; i--)
		{
			if (!(*AllSlots)[i].IsValid())
			{
				// Remove invalid weak pointer
				AllSlots->RemoveAt(i);
				continue;
			}

			UInventorySlotWidget *SlotWidget = (*AllSlots)[i].Get();
			if (SlotWidget)
			{
				// Update selection state for ALL slots (including the one that initiated the change)
				bool bWasSelected = SlotWidget->bIsSelected;
				SlotWidget->bIsSelected = SelectionSet && SelectionSet->Contains(SlotWidget->SlotIndex);

				// Only update appearance if the state actually changed (optimization)
				if (bWasSelected != SlotWidget->bIsSelected)
				{
					SlotWidget->UpdateAppearance();
				}

				// Fire the Blueprint event on each slot
				SlotWidget->OnSelectionChanged(SelectedSlots);
			}
		}
	}
}

void UInventorySlotWidget::ShowTooltip()
{
	// Don't show tooltip if disabled, no item, or no tooltip class
	if (!bEnableTooltip || !CurrentItem.IsValid() || !TooltipClass)
	{
		return;
	}

	// Create tooltip if it doesn't exist
	if (!ActiveTooltip)
	{
		ActiveTooltip = CreateWidget<UTooltipWidget>(GetOwningPlayer(), TooltipClass);
		if (ActiveTooltip)
		{
			// Set item data BEFORE adding to viewport
			ActiveTooltip->SetItem(CurrentItem);

			// Set display configuration BEFORE adding to viewport
			ActiveTooltip->SetDisplayConfig(TooltipConfig);

			// Set source widget for snap positioning
			ActiveTooltip->SetSourceWidget(this);

			// Add to viewport - this creates the canvas slot
			ActiveTooltip->AddToViewport(TooltipConfig.ZOrder);

			// Defer Show() to next tick so the slot is available
			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().SetTimerForNextTick([this]()
				{
					if (ActiveTooltip)
					{
						ActiveTooltip->Show();
					}
				});
			}
		}
	}
	else if (ActiveTooltip)
	{
		// Tooltip already exists, just show it
		ActiveTooltip->Show();
	}
}

void UInventorySlotWidget::HideTooltip()
{
	if (ActiveTooltip)
	{
		ActiveTooltip->Hide();
		ActiveTooltip->RemoveFromParent();
		ActiveTooltip = nullptr;
	}
}
