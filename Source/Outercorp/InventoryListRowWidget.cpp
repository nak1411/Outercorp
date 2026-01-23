// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryListRowWidget.h"
#include "InventoryColumnSettings.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "InventoryItemData.h"
#include "InventorySlotWidget.h"
#include "QuantityInputDialog.h"
#include "OutercorpCharacter.h"
#include "ContextMenuWidget.h"
#include "Engine/Texture2D.h"
#include "Input/Reply.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Initialize static members
TSet<UInventoryListRowWidget *> UInventoryListRowWidget::SelectedRows;
TWeakObjectPtr<UUserWidget> UInventoryListRowWidget::CurrentContextMenu;
TWeakObjectPtr<UInventoryListRowWidget> UInventoryListRowWidget::LastClickedRow;

void UInventoryListRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RowButton)
	{
		RowButton->OnClicked.AddDynamic(this, &UInventoryListRowWidget::OnRowButtonClicked);
	}

	// Configure text blocks to clip overflow and prevent overlapping
	if (ItemNameText)
	{
		ItemNameText->SetAutoWrapText(false);
	}

	if (QuantityText)
	{
		QuantityText->SetAutoWrapText(false);
	}

	if (WeightText)
	{
		WeightText->SetAutoWrapText(false);
	}

	if (ValueText)
	{
		ValueText->SetAutoWrapText(false);
	}

	if (RarityText)
	{
		RarityText->SetAutoWrapText(false);
	}

	if (CategoryText)
	{
		CategoryText->SetAutoWrapText(false);
	}

	// Update column widths if we have settings
	UpdateColumnWidths();
}

void UInventoryListRowWidget::NativeDestruct()
{
	// Hide and destroy tooltip if active
	HideTooltip();

	Super::NativeDestruct();
}

void UInventoryListRowWidget::NativeOnListItemObjectSet(UObject *ListItemObject)
{
	ItemData = Cast<UInventoryListItemData>(ListItemObject);

	if (!ItemData)
	{
		return;
	}

	// Set row index from slot index for alternating colors
	RowIndex = ItemData->SlotIndex;

	RefreshDisplay();

	// Update background color based on alternating rows setting
	UpdateSelectionVisual();
}

void UInventoryListRowWidget::NativeOnItemSelectionChanged(bool bIsItemSelected)
{
	bIsSelected = bIsItemSelected;

	// Update selection visual
	UpdateSelectionVisual();
}

void UInventoryListRowWidget::RefreshDisplay()
{
	if (!ItemData || !ItemData->Item.IsValid())
	{
		// Empty row - clear all fields
		if (ItemNameText)
			ItemNameText->SetText(FText::FromString(TEXT("-")));
		if (QuantityText)
			QuantityText->SetText(FText::FromString(TEXT("-")));
		if (WeightText)
			WeightText->SetText(FText::FromString(TEXT("-")));
		if (ValueText)
			ValueText->SetText(FText::FromString(TEXT("-")));
		if (RarityText)
			RarityText->SetText(FText::FromString(TEXT("-")));
		if (CategoryText)
			CategoryText->SetText(FText::FromString(TEXT("-")));
		if (ItemIcon)
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	const FInventoryItem &Item = ItemData->Item;
	UInventoryItemData *ItemDataAsset = Item.ItemData;

	if (!ItemDataAsset)
	{
		return;
	}

	// Set item name
	if (ItemNameText)
	{
		ItemNameText->SetText(ItemDataAsset->ItemName);
		ItemNameText->SetColorAndOpacity(GetRarityColor(ItemDataAsset->Rarity));

		// Ensure text doesn't overflow - clip to bounds
		ItemNameText->SetAutoWrapText(false);
	}

	// Set quantity
	if (QuantityText)
	{
		if (Item.Quantity > 1)
		{
			QuantityText->SetText(FText::AsNumber(Item.Quantity));
		}
		else
		{
			QuantityText->SetText(FText::FromString(TEXT("1")));
		}
	}

	// Set weight (total weight for stack)
	if (WeightText)
	{
		float TotalWeight = Item.GetTotalWeight();
		WeightText->SetText(FText::AsNumber(FMath::RoundToFloat(TotalWeight * 100.0f) / 100.0f));
	}

	// Set value (total value for stack)
	if (ValueText)
	{
		int32 TotalValue = Item.GetTotalValue();
		ValueText->SetText(FText::AsNumber(TotalValue));
	}

	// Set rarity
	if (RarityText)
	{
		RarityText->SetText(GetRarityText(ItemDataAsset->Rarity));
		RarityText->SetColorAndOpacity(GetRarityColor(ItemDataAsset->Rarity));
	}

	// Set category
	if (CategoryText)
	{
		CategoryText->SetText(GetCategoryText(ItemDataAsset->Category));
	}

	// Set icon
	if (ItemIcon)
	{
		if (!ItemDataAsset->ItemIcon.IsNull())
		{
			if (UTexture2D *IconTexture = ItemDataAsset->ItemIcon.LoadSynchronous())
			{
				ItemIcon->SetBrushFromTexture(IconTexture);
				ItemIcon->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

const FInventoryItem &UInventoryListRowWidget::GetItem() const
{
	static FInventoryItem EmptyItem;
	return ItemData ? ItemData->Item : EmptyItem;
}

void UInventoryListRowWidget::SetRowIndex(int32 Index)
{
	RowIndex = Index;

	// Update visual in case alternating colors are enabled
	UpdateSelectionVisual();
}

FReply UInventoryListRowWidget::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	// Handle right-click for context menu (both empty and occupied rows)
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// Get mouse position in viewport pixel coordinates
		APlayerController *PC = GetOwningPlayer();
		if (PC)
		{
			float MouseX, MouseY;
			PC->GetMousePosition(MouseX, MouseY);
			FVector2D MousePosition(MouseX, MouseY);

			// Check if row is empty or has an item
			if (!ItemData || !ItemData->Item.IsValid())
			{
				// Empty row - call empty row context menu event
				OnEmptyRowRightClicked(MousePosition);
			}
			else
			{
				// Occupied row - select this row if not already selected, then show context menu
				if (!bIsSelected)
				{
					SelectRow(false);
				}

				OnRightClicked(MousePosition);
			}
		}

		return FReply::Handled();
	}

	// Handle left-click
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// Only start drag if row has an item
		if (ItemData && ItemData->Item.IsValid())
		{
			// Reset drag flag
			bDragStarted = false;

			// Start drag detection
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
		}
	}

	return FReply::Unhandled();
}

FReply UInventoryListRowWidget::NativeOnMouseButtonUp(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	// If left mouse button released and we didn't start a drag
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && !bDragStarted)
	{
		// Only handle if row has an item
		if (ItemData && ItemData->Item.IsValid())
		{
			bool bShiftDown = InMouseEvent.IsShiftDown();
			bool bCtrlDown = InMouseEvent.IsControlDown();

			if (bShiftDown)
			{
				// Shift-click: Range selection from last clicked to this row
				SelectRange();
			}
			else if (bCtrlDown)
			{
				// Ctrl-click: Toggle this row while keeping other selections
				ToggleSelection(true);
			}
			else
			{
				// Normal click: Select only this row (clear others)
				ToggleSelection(false);
			}

			// Update last clicked row for future range selections
			LastClickedRow = this;

			return FReply::Handled();
		}
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UInventoryListRowWidget::OnRowButtonClicked()
{
	// Selection is now handled by NativeOnMouseButtonDown
	// Keep this for backward compatibility with button clicks
	bool bAddToSelection = false; // Buttons don't have modifier key info
	ToggleSelection(bAddToSelection);
}

void UInventoryListRowWidget::SelectRow(bool bAddToSelection)
{
	if (!ItemData || !ItemData->Item.IsValid())
	{
		return;
	}

	// If not adding to selection, clear all other selections first
	if (!bAddToSelection)
	{
		ClearAllRowSelections();
	}

	// Select this row
	bIsSelected = true;
	SelectedRows.Add(this);

	// Update visual
	UpdateSelectionVisual();

	// Update last clicked row
	LastClickedRow = this;
}

void UInventoryListRowWidget::DeselectRow()
{
	if (!bIsSelected)
	{
		return;
	}

	bIsSelected = false;
	SelectedRows.Remove(this);

	// Update visual
	UpdateSelectionVisual();
}

void UInventoryListRowWidget::ToggleSelection(bool bAddToSelection)
{
	if (bIsSelected)
	{
		// If already selected and adding to selection (Ctrl), deselect this row
		if (bAddToSelection)
		{
			DeselectRow();
		}
		else
		{
			// Normal click on already selected row - clear all other selections
			// This handles the case where multiple items are selected and you click one of them
			if (SelectedRows.Num() > 1)
			{
				ClearAllRowSelections();
				SelectRow(false);
			}
			// If only this row is selected, clicking it again does nothing (keeps it selected)
		}
	}
	else
	{
		// Not selected - select it
		SelectRow(bAddToSelection);
	}
}

void UInventoryListRowWidget::ClearAllRowSelections()
{
	// Create a copy of the set to iterate over, since we'll be modifying the original
	TSet<UInventoryListRowWidget *> RowsToDeselect = SelectedRows;

	for (UInventoryListRowWidget *Row : RowsToDeselect)
	{
		if (Row && Row->IsValidLowLevel())
		{
			Row->DeselectRow();
		}
	}

	// Clear the set just in case
	SelectedRows.Empty();
}

void UInventoryListRowWidget::UpdateSelectionVisual()
{
	if (!BackgroundBorder)
	{
		return;
	}

	FLinearColor BackgroundColor;

	if (bIsSelected)
	{
		// Show selection color when selected (priority: selection state)
		BackgroundColor = SelectedColor;
	}
	else if (bIsHovered)
	{
		// Show hover color when mouse is over the row (priority: hover state)
		// Blend with base color for subtle effect
		FLinearColor BaseColor;
		if (bUseAlternatingRowColors)
		{
			BaseColor = (RowIndex % 2 == 0) ? EvenRowColor : OddRowColor;
		}
		else
		{
			BaseColor = UnselectedColor;
		}

		// Blend hover color with base color
		BackgroundColor = BaseColor + HoverColor;
	}
	else if (bUseAlternatingRowColors)
	{
		// Use alternating row colors when not selected or hovered
		BackgroundColor = (RowIndex % 2 == 0) ? EvenRowColor : OddRowColor;
	}
	else
	{
		// Use standard unselected color
		BackgroundColor = UnselectedColor;
	}

	BackgroundBorder->SetBrushColor(BackgroundColor);
}

void UInventoryListRowWidget::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bIsHovered = true;

	// Update visual to show hover effect
	UpdateSelectionVisual();

	// Show tooltip if enabled and row has an item
	if (bEnableTooltip && ItemData && ItemData->Item.IsValid())
	{
		ShowTooltip();
	}
}

void UInventoryListRowWidget::NativeOnMouseLeave(const FPointerEvent &InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsHovered = false;

	// Update visual to remove hover effect
	UpdateSelectionVisual();

	// Hide tooltip when mouse leaves (unless it's persistent)
	if (bEnableTooltip && ActiveTooltip && !TooltipConfig.bPersistent)
	{
		HideTooltip();
	}
}

void UInventoryListRowWidget::NativeOnDragDetected(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent, UDragDropOperation *&OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// Mark that we started dragging (so mouse up doesn't trigger selection)
	bDragStarted = true;

	if (!ItemData || !ItemData->Item.IsValid() || !ItemData->InventoryComponent)
	{
		return;
	}

	// Clear all selections when starting a drag operation (both list and grid)
	if (SelectedRows.Num() > 0)
	{
		ClearAllRowSelections();
	}

	// Also clear grid view selections to keep both views in sync
	UInventorySlotWidget::ClearAllSelectionsForInventory(ItemData->InventoryComponent);

	// Create drag-drop operation
	UInventoryDragDropOperation *DragDropOp = NewObject<UInventoryDragDropOperation>();
	DragDropOp->SourceSlotIndex = ItemData->SlotIndex;
	DragDropOp->DraggedItem = ItemData->Item;
	DragDropOp->InventoryComponent = ItemData->InventoryComponent;

	// Check if shift is held for split operation
	bool bShiftHeld = InMouseEvent.IsShiftDown();
	bool bCanSplit = ItemData->Item.Quantity > 1;
	DragDropOp->bIsSplitOperation = bShiftHeld && bCanSplit;

	// Create visual widget for dragging
	// Use the DragVisualClass if set in Blueprint, otherwise try to find the BP widget
	TSubclassOf<UInventorySlotWidget> WidgetClass = DragVisualClass;

	if (WidgetClass)
	{
		UInventorySlotWidget *DragVisual = CreateWidget<UInventorySlotWidget>(GetOwningPlayer(), WidgetClass);
		if (DragVisual)
		{
			DragVisual->SetInventoryComponent(ItemData->InventoryComponent);
			DragVisual->SetSlotIndex(ItemData->SlotIndex);
			DragVisual->SetItem(ItemData->Item);
			DragDropOp->DefaultDragVisual = DragVisual;
			DragDropOp->DraggedVisual = DragVisual;
			DragDropOp->Pivot = EDragPivot::CenterCenter;
			DragDropOp->Offset = FVector2D(0, 0);
		}
	}

	OutOperation = DragDropOp;
}

bool UInventoryListRowWidget::NativeOnDrop(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UInventoryDragDropOperation *DragDropOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (!DragDropOp || !ItemData || !ItemData->InventoryComponent)
	{
		// Reset drag flag even if drop fails
		bDragStarted = false;
		UpdateDragVisual(false);
		return false;
	}

	// Reset background color
	UpdateDragVisual(false);

	// Don't drop on same slot
	if (DragDropOp->SourceSlotIndex == ItemData->SlotIndex)
	{
		bDragStarted = false;
		return false;
	}

	// Mark that the item was dropped on a valid target
	DragDropOp->bWasDroppedOnValidTarget = true;

	// Handle split operation
	if (DragDropOp->bIsSplitOperation)
	{
		int32 SplitAmount = DragDropOp->DraggedItem.Quantity / 2;
		if (SplitAmount > 0)
		{
			// Find first empty slot for the split (SplitStack requires target to be empty)
			int32 EmptySlot = ItemData->InventoryComponent->FindEmptySlot();
			if (EmptySlot == -1)
			{
				bDragStarted = false;
				return false;
			}

			ItemData->InventoryComponent->SplitStack(DragDropOp->SourceSlotIndex, EmptySlot, SplitAmount);
		}
		bDragStarted = false;
		return true;
	}

	// Get the dragged item and target item
	FInventoryItem DraggedItem = DragDropOp->DraggedItem;
	FInventoryItem TargetItem = ItemData->Item;

	// Check if items are the same type (merge) or different (swap)
	if (TargetItem.IsValid() &&
		DraggedItem.ItemData &&
		TargetItem.ItemData &&
		DraggedItem.ItemData->ItemID == TargetItem.ItemData->ItemID)
	{
		// Same item type - merge stacks
		ItemData->InventoryComponent->MergeStacks(DragDropOp->SourceSlotIndex, ItemData->SlotIndex);
	}
	else
	{
		// Different items or target is empty - swap
		ItemData->InventoryComponent->MoveItem(DragDropOp->SourceSlotIndex, ItemData->SlotIndex);
	}

	bDragStarted = false;
	return true;
}

void UInventoryListRowWidget::NativeOnDragEnter(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	// Show visual feedback that this row can accept the drop
	UpdateDragVisual(true);
}

void UInventoryListRowWidget::NativeOnDragLeave(const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	// Remove visual feedback
	UpdateDragVisual(false);

	// Reset drag flag when drag leaves
	bDragStarted = false;
}

void UInventoryListRowWidget::UpdateDragVisual(bool bIsDragTarget)
{
	if (!BackgroundBorder)
	{
		return;
	}

	if (bIsDragTarget)
	{
		// Show the drag drop highlight color when being dragged over
		BackgroundBorder->SetBrushColor(DragDropHighlightColor);
	}
	else
	{
		// Clear the drag visual - restore normal visual state
		UpdateSelectionVisual();
	}
}

FLinearColor UInventoryListRowWidget::GetRarityColor(EItemRarity Rarity) const
{
	switch (Rarity)
	{
	case EItemRarity::Common:
		return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f); // Gray
	case EItemRarity::Uncommon:
		return FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green
	case EItemRarity::Rare:
		return FLinearColor(0.0f, 0.5f, 1.0f, 1.0f); // Blue
	case EItemRarity::Epic:
		return FLinearColor(0.7f, 0.0f, 1.0f, 1.0f); // Purple
	case EItemRarity::Legendary:
		return FLinearColor(1.0f, 0.5f, 0.0f, 1.0f); // Orange
	default:
		return FLinearColor::White;
	}
}

FText UInventoryListRowWidget::GetRarityText(EItemRarity Rarity) const
{
	switch (Rarity)
	{
	case EItemRarity::Common:
		return FText::FromString(TEXT("Common"));
	case EItemRarity::Uncommon:
		return FText::FromString(TEXT("Uncommon"));
	case EItemRarity::Rare:
		return FText::FromString(TEXT("Rare"));
	case EItemRarity::Epic:
		return FText::FromString(TEXT("Epic"));
	case EItemRarity::Legendary:
		return FText::FromString(TEXT("Legendary"));
	default:
		return FText::FromString(TEXT("Unknown"));
	}
}

FText UInventoryListRowWidget::GetCategoryText(EItemCategory Category) const
{
	switch (Category)
	{
	case EItemCategory::Weapon:
		return FText::FromString(TEXT("Weapon"));
	case EItemCategory::Armor:
		return FText::FromString(TEXT("Armor"));
	case EItemCategory::Consumable:
		return FText::FromString(TEXT("Consumable"));
	case EItemCategory::Resource:
		return FText::FromString(TEXT("Resource"));
	case EItemCategory::Module:
		return FText::FromString(TEXT("Module"));
	case EItemCategory::Ammunition:
		return FText::FromString(TEXT("Ammunition"));
	case EItemCategory::Misc:
		return FText::FromString(TEXT("Miscellaneous"));
	default:
		return FText::FromString(TEXT("Unknown"));
	}
}

void UInventoryListRowWidget::SetColumnSettings(UInventoryColumnSettings *InColumnSettings)
{
	ColumnSettings = InColumnSettings;

	if (ColumnSettings)
	{
		// Bind to column width changes
		ColumnSettings->OnColumnWidthsChanged.AddDynamic(this, &UInventoryListRowWidget::OnColumnWidthsChanged);

		// Initial update
		UpdateColumnWidths();
	}
}

void UInventoryListRowWidget::UpdateColumnWidths()
{
	if (!ColumnSettings)
	{
		return;
	}

	// Update SizeBox widths for each column to match header
	if (IconColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Icon"));
		IconColumnBox->SetWidthOverride(Width);
		IconColumnBox->SetMinDesiredWidth(Width);
		IconColumnBox->SetMaxDesiredWidth(Width);
	}

	if (NameColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Name"));
		NameColumnBox->SetWidthOverride(Width);
		NameColumnBox->SetMinDesiredWidth(Width);
		NameColumnBox->SetMaxDesiredWidth(Width);
	}

	if (QuantityColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Quantity"));
		QuantityColumnBox->SetWidthOverride(Width);
		QuantityColumnBox->SetMinDesiredWidth(Width);
		QuantityColumnBox->SetMaxDesiredWidth(Width);
	}

	if (WeightColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Weight"));
		WeightColumnBox->SetWidthOverride(Width);
		WeightColumnBox->SetMinDesiredWidth(Width);
		WeightColumnBox->SetMaxDesiredWidth(Width);
	}

	if (ValueColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Value"));
		ValueColumnBox->SetWidthOverride(Width);
		ValueColumnBox->SetMinDesiredWidth(Width);
		ValueColumnBox->SetMaxDesiredWidth(Width);
	}

	if (RarityColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Rarity"));
		RarityColumnBox->SetWidthOverride(Width);
		RarityColumnBox->SetMinDesiredWidth(Width);
		RarityColumnBox->SetMaxDesiredWidth(Width);
	}

	if (CategoryColumnBox)
	{
		float Width = ColumnSettings->GetColumnWidth(TEXT("Category"));
		CategoryColumnBox->SetWidthOverride(Width);
		CategoryColumnBox->SetMinDesiredWidth(Width);
		CategoryColumnBox->SetMaxDesiredWidth(Width);
	}
}

void UInventoryListRowWidget::OnColumnWidthsChanged()
{
	UpdateColumnWidths();
}

void UInventoryListRowWidget::HandleContextMenuAction_Implementation(FName ActionID)
{
	UE_LOG(LogTemp, Log, TEXT("ListRowWidget::HandleContextMenuAction called with ActionID: %s"), *ActionID.ToString());

	if (ActionID == "Split")
	{
		HandleSplitItem();
	}
	else if (ActionID == "Destroy")
	{
		HandleDestroyItem();
	}
	else if (ActionID == "Drop")
	{
		HandleDropItem();
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
	else if (ActionID == "Equip")
	{
		HandleEquipItem();
	}
}

void UInventoryListRowWidget::HandleSplitItem()
{
	if (!ItemData || !ItemData->Item.IsValid() || !ItemData->InventoryComponent)
	{
		return;
	}

	// Can't split if quantity is 1 or less
	if (ItemData->Item.Quantity <= 1)
	{
		return;
	}

	// Show quantity dialog to ask how many items to split
	if (QuantityDialogClass)
	{
		ActiveQuantityDialog = CreateWidget<UQuantityInputDialog>(GetOwningPlayer(), QuantityDialogClass);
		if (ActiveQuantityDialog)
		{
			// Max split amount is Quantity - 1 (can't split all items)
			int32 MaxSplitAmount = ItemData->Item.Quantity - 1;

			FText Title = FText::FromString(TEXT("Split Items"));
			FText Description = FText::Format(
				FText::FromString(TEXT("How many {0} do you want to split off?")),
				ItemData->Item.ItemData->ItemName);
			FText ConfirmText = FText::FromString(TEXT("Split"));

			ActiveQuantityDialog->SetupDialog(MaxSplitAmount, Title, Description, ConfirmText);

			// Bind delegate
			TWeakObjectPtr<UInventoryListRowWidget> WeakThis = this;
			ActiveQuantityDialog->OnQuantityConfirmedDelegate.BindLambda([WeakThis](int32 Quantity)
			{
				if (WeakThis.IsValid())
				{
					WeakThis->HandleSplitItemConfirmed(Quantity);
				}
			});

			ActiveQuantityDialog->AddToViewport(1000);
		}
	}
}

void UInventoryListRowWidget::HandleSplitItemConfirmed(int32 Quantity)
{
	if (!ItemData || !ItemData->InventoryComponent)
	{
		return;
	}

	// Find first empty slot
	int32 EmptySlot = ItemData->InventoryComponent->FindEmptySlot();
	if (EmptySlot == -1)
	{
		return;
	}

	// Split stack with the specified quantity
	ItemData->InventoryComponent->SplitStack(ItemData->SlotIndex, EmptySlot, Quantity);
}

void UInventoryListRowWidget::HandleDestroyItem()
{
	if (!ItemData || !ItemData->Item.IsValid() || !ItemData->InventoryComponent)
	{
		return;
	}

	// If only one item, destroy immediately
	if (ItemData->Item.Quantity <= 1)
	{
		HandleDestroyItemConfirmed(1);
		return;
	}

	// Show quantity dialog to ask how many items to destroy
	if (QuantityDialogClass)
	{
		ActiveQuantityDialog = CreateWidget<UQuantityInputDialog>(GetOwningPlayer(), QuantityDialogClass);
		if (ActiveQuantityDialog)
		{
			FText Title = FText::FromString(TEXT("Destroy Items"));
			FText Description = FText::Format(
				FText::FromString(TEXT("How many {0} do you want to destroy?")),
				ItemData->Item.ItemData->ItemName);
			FText ConfirmText = FText::FromString(TEXT("Destroy"));

			ActiveQuantityDialog->SetupDialog(ItemData->Item.Quantity, Title, Description, ConfirmText);

			// Bind delegate
			TWeakObjectPtr<UInventoryListRowWidget> WeakThis = this;
			ActiveQuantityDialog->OnQuantityConfirmedDelegate.BindLambda([WeakThis](int32 Quantity)
			{
				if (WeakThis.IsValid())
				{
					WeakThis->HandleDestroyItemConfirmed(Quantity);
				}
			});

			ActiveQuantityDialog->AddToViewport(1000);
		}
	}
}

void UInventoryListRowWidget::HandleDestroyItemConfirmed(int32 Quantity)
{
	if (!ItemData || !ItemData->InventoryComponent)
	{
		return;
	}

	// Cache the values we need before any widgets are destroyed
	int32 CachedSlotIndex = ItemData->SlotIndex;
	UInventoryComponent *CachedInventoryComponent = ItemData->InventoryComponent;

	// Defer the actual item removal until the next frame
	if (UWorld *World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick([CachedInventoryComponent, CachedSlotIndex, Quantity]()
		{
			if (IsValid(CachedInventoryComponent))
			{
				// Use DestroyItem instead of RemoveItemAtSlot to trigger notification
				CachedInventoryComponent->DestroyItem(CachedSlotIndex, Quantity);
			}
		});
	}
}

void UInventoryListRowWidget::HandleDropItem()
{
	if (!ItemData || !ItemData->Item.IsValid() || !ItemData->InventoryComponent)
	{
		return;
	}

	// If only one item, drop immediately
	if (ItemData->Item.Quantity <= 1)
	{
		HandleDropItemConfirmed(1);
		return;
	}

	// Show quantity dialog to ask how many items to drop
	if (QuantityDialogClass)
	{
		ActiveQuantityDialog = CreateWidget<UQuantityInputDialog>(GetOwningPlayer(), QuantityDialogClass);
		if (ActiveQuantityDialog)
		{
			FText Title = FText::FromString(TEXT("Drop Items"));
			FText Description = FText::Format(
				FText::FromString(TEXT("How many {0} do you want to drop?")),
				ItemData->Item.ItemData->ItemName);
			FText ConfirmText = FText::FromString(TEXT("Drop"));

			ActiveQuantityDialog->SetupDialog(ItemData->Item.Quantity, Title, Description, ConfirmText);

			// Bind delegate
			TWeakObjectPtr<UInventoryListRowWidget> WeakThis = this;
			ActiveQuantityDialog->OnQuantityConfirmedDelegate.BindLambda([WeakThis](int32 Quantity)
			{
				if (WeakThis.IsValid())
				{
					WeakThis->HandleDropItemConfirmed(Quantity);
				}
			});

			ActiveQuantityDialog->AddToViewport(1000);
		}
	}
}

void UInventoryListRowWidget::HandleDropItemConfirmed(int32 Quantity)
{
	if (!ItemData || !ItemData->InventoryComponent)
	{
		return;
	}

	// Cache the values we need
	int32 CachedSlotIndex = ItemData->SlotIndex;
	UInventoryComponent *CachedInventoryComponent = ItemData->InventoryComponent;

	// Defer the drop operation until the next frame
	if (UWorld *World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick([CachedInventoryComponent, CachedSlotIndex, Quantity]()
		{
			if (IsValid(CachedInventoryComponent))
			{
				// Drop the item in front of the owner (uses default drop distance)
				bool bSuccess = CachedInventoryComponent->DropItemInFront(CachedSlotIndex, Quantity);

				if (!bSuccess)
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to drop item from slot %d"), CachedSlotIndex);
				}
			}
		});
	}
}

void UInventoryListRowWidget::HandleShowInfo()
{
	if (!ItemData || !ItemData->Item.IsValid())
	{
		return;
	}

	// Call Blueprint event to show UI
	ShowItemInfoUI(ItemData->Item);
}

void UInventoryListRowWidget::HandleStackAll()
{
	if (!ItemData || !ItemData->InventoryComponent)
	{
		return;
	}

	// Get the current item fresh from the inventory component
	FInventoryItem ItemToStack = ItemData->InventoryComponent->GetItemAtSlot(ItemData->SlotIndex);

	// If slot is empty, stack ALL items in the inventory (same as Stack All Empty)
	if (!ItemToStack.IsValid())
	{
		HandleStackAllEmpty();
		return;
	}

	// Get the item ID we want to stack
	FName TargetItemID = ItemToStack.ItemData->ItemID;

	// Get all items and find matching items
	TArray<FInventoryItem> AllItems = ItemData->InventoryComponent->GetAllItems();
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
			FInventoryItem SourceItem = ItemData->InventoryComponent->GetItemAtSlot(SourceSlot);
			FInventoryItem TargetItem = ItemData->InventoryComponent->GetItemAtSlot(TargetSlot);

			// Skip if source is already empty (already merged)
			if (!SourceItem.IsValid())
			{
				break;
			}

			// Check if target has space
			if (TargetItem.IsValid() && TargetItem.Quantity < TargetItem.ItemData->MaxStackSize)
			{
				ItemData->InventoryComponent->MergeStacks(SourceSlot, TargetSlot);

				// Check if source is now empty, if so we're done with this source
				SourceItem = ItemData->InventoryComponent->GetItemAtSlot(SourceSlot);
				if (!SourceItem.IsValid())
				{
					break;
				}
				// If source still has items, continue trying other targets
			}
		}
	}
}

void UInventoryListRowWidget::HandleStackAllEmpty()
{
	if (!ItemData || !ItemData->InventoryComponent)
	{
		return;
	}

	// Get all items
	TArray<FInventoryItem> AllItems = ItemData->InventoryComponent->GetAllItems();

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
	for (const auto &Group : ItemGroups)
	{
		const TArray<int32> &SlotIndices = Group.Value;
		if (SlotIndices.Num() > 1)
		{
			// Try to merge all items in this group into the first slot
			for (int32 i = 1; i < SlotIndices.Num(); i++)
			{
				ItemData->InventoryComponent->MergeStacks(SlotIndices[i], SlotIndices[0]);
			}
		}
	}
}

void UInventoryListRowWidget::HandleSelectAll()
{
	// Select all rows
	for (UInventoryListRowWidget *Row : SelectedRows)
	{
		if (Row && Row->IsValidLowLevel())
		{
			Row->SelectRow(true);
		}
	}

	// Also need to select all rows that aren't in the set yet
	// This would require access to all row widgets, which we don't have here
}

void UInventoryListRowWidget::HandleInvertSelection()
{
	// Invert selection of all rows
	// This would require access to all row widgets, which we don't have here
}

void UInventoryListRowWidget::HandleEquipItem()
{
	UE_LOG(LogTemp, Log, TEXT("ListRowWidget::HandleEquipItem called"));

	if (!ItemData || !ItemData->Item.IsValid() || !ItemData->Item.ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ListRowWidget::HandleEquipItem: ItemData is invalid"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ListRowWidget::HandleEquipItem: Item = %s, bIsEquippable = %s"),
		*ItemData->Item.ItemData->ItemName.ToString(),
		ItemData->Item.ItemData->bIsEquippable ? TEXT("true") : TEXT("false"));

	// Check if the item is equippable
	if (!ItemData->Item.ItemData->bIsEquippable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ListRowWidget::HandleEquipItem: Item is not equippable"));
		return;
	}

	// Get the owning player character
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ListRowWidget::HandleEquipItem: No PlayerController"));
		return;
	}

	AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(PC->GetPawn());
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("ListRowWidget::HandleEquipItem: Failed to cast to OutercorpCharacter"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ListRowWidget::HandleEquipItem: Calling EquipToolFromInventory"));
	// Equip the item
	Character->EquipToolFromInventory(ItemData->Item);
}

void UInventoryListRowWidget::SelectRange()
{
	// Get the last clicked row
	UInventoryListRowWidget *StartRow = LastClickedRow.Get();
	if (!StartRow || !StartRow->IsValidLowLevel())
	{
		// No previous selection, just select this row
		SelectRow(false);
		return;
	}

	// Get all rows in the list
	TArray<UInventoryListRowWidget *> AllRows = GetAllRowsInList();
	if (AllRows.Num() == 0)
	{
		// Fallback to simple selection
		SelectRow(false);
		return;
	}

	// Find indices of start and end rows in the list
	int32 StartIndex = -1;
	int32 EndIndex = -1;

	for (int32 i = 0; i < AllRows.Num(); ++i)
	{
		if (AllRows[i] == StartRow)
		{
			StartIndex = i;
		}
		if (AllRows[i] == this)
		{
			EndIndex = i;
		}
	}

	// Validate indices
	if (StartIndex == -1 || EndIndex == -1)
	{
		// Could not find rows in list, fallback to simple selection
		SelectRow(false);
		return;
	}

	// Ensure StartIndex is less than EndIndex
	if (StartIndex > EndIndex)
	{
		Swap(StartIndex, EndIndex);
	}

	// Clear all selections first
	ClearAllRowSelections();

	// Select all rows in range
	for (int32 i = StartIndex; i <= EndIndex; ++i)
	{
		if (AllRows.IsValidIndex(i) && AllRows[i] && AllRows[i]->IsValidLowLevel())
		{
			UInventoryListRowWidget *Row = AllRows[i];
			if (Row->ItemData && Row->ItemData->Item.IsValid())
			{
				Row->bIsSelected = true;
				SelectedRows.Add(Row);
				Row->UpdateSelectionVisual();
			}
		}
	}
}

TArray<UInventoryListRowWidget *> UInventoryListRowWidget::GetAllRowsInList() const
{
	TArray<UInventoryListRowWidget *> AllRows;

	// Try to get the parent VerticalBox or ListView container
	UWidget *Parent = GetParent();
	while (Parent)
	{
		// Check if parent is a VerticalBox (for table view)
		if (UVerticalBox *VerticalBox = Cast<UVerticalBox>(Parent))
		{
			// Get all children of the vertical box
			TArray<UWidget *> Children = VerticalBox->GetAllChildren();
			for (UWidget *Child : Children)
			{
				if (UInventoryListRowWidget *RowWidget = Cast<UInventoryListRowWidget>(Child))
				{
					AllRows.Add(RowWidget);
				}
			}
			break;
		}

		// Move up the widget hierarchy
		Parent = Parent->GetParent();
	}

	// Sort by slot index to ensure correct order
	AllRows.Sort([](const UInventoryListRowWidget &A, const UInventoryListRowWidget &B) -> bool
				 {
		if (!A.ItemData || !B.ItemData)
		{
			return false;
		}
		return A.ItemData->SlotIndex < B.ItemData->SlotIndex; });

	return AllRows;
}

void UInventoryListRowWidget::CloseCurrentContextMenu()
{
	if (CurrentContextMenu.IsValid())
	{
		CurrentContextMenu->RemoveFromParent();
		CurrentContextMenu.Reset();
	}
}

void UInventoryListRowWidget::SetCurrentContextMenu(UUserWidget *ContextMenu)
{
	// Close any existing menu first
	CloseCurrentContextMenu();

	if (ContextMenu)
	{
		CurrentContextMenu = ContextMenu;
	}
}

void UInventoryListRowWidget::SetCurrentContextMenuWithItemData(UUserWidget *ContextMenu)
{
	// Close any existing menu first
	CloseCurrentContextMenu();

	if (!ContextMenu)
	{
		return;
	}

	// Set as the current context menu
	CurrentContextMenu = ContextMenu;

	// If it's a ContextMenuWidget, set the equippable flag
	if (UContextMenuWidget *ContextMenuWidget = Cast<UContextMenuWidget>(ContextMenu))
	{
		ContextMenuWidget->bIsItemEquippable = IsCurrentItemEquippable();
		ContextMenuWidget->OwningSlot = this;
	}
}

bool UInventoryListRowWidget::IsCurrentItemEquippable() const
{
	return ItemData && ItemData->Item.IsValid() && ItemData->Item.ItemData && ItemData->Item.ItemData->bIsEquippable;
}

UUserWidget *UInventoryListRowWidget::GetCurrentContextMenu()
{
	return CurrentContextMenu.Get();
}

void UInventoryListRowWidget::ShowTooltip()
{
	// Don't show tooltip if disabled, no item, or no tooltip class
	if (!bEnableTooltip || !ItemData || !ItemData->Item.IsValid() || !TooltipClass)
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
			ActiveTooltip->SetItem(ItemData->Item);

			// Set display configuration BEFORE adding to viewport
			ActiveTooltip->SetDisplayConfig(TooltipConfig);

			// Set source widget for snap positioning
			ActiveTooltip->SetSourceWidget(this);

			// Add to viewport - this creates the canvas slot
			ActiveTooltip->AddToViewport(TooltipConfig.ZOrder);

			// Defer Show() to next tick so the slot is available
			if (UWorld *World = GetWorld())
			{
				World->GetTimerManager().SetTimerForNextTick([this]()
															 {
					if (ActiveTooltip)
					{
						ActiveTooltip->Show();
					} });
			}
		}
	}
	else if (ActiveTooltip)
	{
		// Tooltip already exists, just show it
		ActiveTooltip->Show();
	}
}

void UInventoryListRowWidget::HideTooltip()
{
	if (ActiveTooltip)
	{
		ActiveTooltip->Hide();
		ActiveTooltip->RemoveFromParent();
		ActiveTooltip = nullptr;
	}
}
