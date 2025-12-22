// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryListRowWidget.h"
#include "InventoryColumnSettings.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "InventoryItemData.h"
#include "InventorySlotWidget.h"
#include "Engine/Texture2D.h"
#include "Input/Reply.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Initialize static members
TSet<UInventoryListRowWidget*> UInventoryListRowWidget::SelectedRows;
TWeakObjectPtr<UUserWidget> UInventoryListRowWidget::CurrentContextMenu;

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

void UInventoryListRowWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryListRowWidget: NativeOnListItemObjectSet called"));

	ItemData = Cast<UInventoryListItemData>(ListItemObject);

	if (!ItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryListRowWidget: Failed to cast ListItemObject to UInventoryListItemData!"));
		return;
	}

	// Set row index from slot index for alternating colors
	RowIndex = ItemData->SlotIndex;

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: ItemData set successfully, calling RefreshDisplay"));
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
	UE_LOG(LogTemp, Warning, TEXT("InventoryListRowWidget: RefreshDisplay called"));

	// Check all widgets
	UE_LOG(LogTemp, Log, TEXT("  ItemNameText: %s"), ItemNameText ? TEXT("OK") : TEXT("NULL"));
	UE_LOG(LogTemp, Log, TEXT("  QuantityText: %s"), QuantityText ? TEXT("OK") : TEXT("NULL"));
	UE_LOG(LogTemp, Log, TEXT("  WeightText: %s"), WeightText ? TEXT("OK") : TEXT("NULL"));
	UE_LOG(LogTemp, Log, TEXT("  ValueText: %s"), ValueText ? TEXT("OK") : TEXT("NULL"));
	UE_LOG(LogTemp, Log, TEXT("  RarityText: %s"), RarityText ? TEXT("OK") : TEXT("NULL"));
	UE_LOG(LogTemp, Log, TEXT("  CategoryText: %s"), CategoryText ? TEXT("OK") : TEXT("NULL"));
	UE_LOG(LogTemp, Log, TEXT("  ItemIcon: %s"), ItemIcon ? TEXT("OK") : TEXT("NULL"));

	if (!ItemData || !ItemData->Item.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryListRowWidget: No valid item data, clearing fields"));
		// Empty row - clear all fields
		if (ItemNameText) ItemNameText->SetText(FText::FromString(TEXT("-")));
		if (QuantityText) QuantityText->SetText(FText::FromString(TEXT("-")));
		if (WeightText) WeightText->SetText(FText::FromString(TEXT("-")));
		if (ValueText) ValueText->SetText(FText::FromString(TEXT("-")));
		if (RarityText) RarityText->SetText(FText::FromString(TEXT("-")));
		if (CategoryText) CategoryText->SetText(FText::FromString(TEXT("-")));
		if (ItemIcon) ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: Item data valid, updating display"));

	const FInventoryItem& Item = ItemData->Item;
	UInventoryItemData* ItemDataAsset = Item.ItemData;

	if (!ItemDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryListRowWidget: ItemDataAsset is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: Setting item name to '%s'"), *ItemDataAsset->ItemName.ToString());

	// Check this widget's size
	FVector2D WidgetSize = GetCachedGeometry().GetLocalSize();
	UE_LOG(LogTemp, Warning, TEXT("InventoryListRowWidget: Row widget size: %.2f x %.2f"), WidgetSize.X, WidgetSize.Y);
	UE_LOG(LogTemp, Warning, TEXT("InventoryListRowWidget: Row widget visibility: %d"), (int32)GetVisibility());

	// Set item name
	if (ItemNameText)
	{
		ItemNameText->SetText(ItemDataAsset->ItemName);
		ItemNameText->SetColorAndOpacity(GetRarityColor(ItemDataAsset->Rarity));

		// Ensure text doesn't overflow - clip to bounds
		ItemNameText->SetAutoWrapText(false);

		UE_LOG(LogTemp, Log, TEXT("  ItemNameText size: %.2f x %.2f, visibility: %d"),
			ItemNameText->GetCachedGeometry().GetLocalSize().X,
			ItemNameText->GetCachedGeometry().GetLocalSize().Y,
			(int32)ItemNameText->GetVisibility());
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
			if (UTexture2D* IconTexture = ItemDataAsset->ItemIcon.LoadSynchronous())
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

const FInventoryItem& UInventoryListRowWidget::GetItem() const
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

FReply UInventoryListRowWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Handle right-click for context menu (both empty and occupied rows)
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// Get mouse position in viewport pixel coordinates
		APlayerController* PC = GetOwningPlayer();
		if (PC)
		{
			float MouseX, MouseY;
			PC->GetMousePosition(MouseX, MouseY);
			FVector2D MousePosition(MouseX, MouseY);

			// Check if row is empty or has an item
			if (!ItemData || !ItemData->Item.IsValid())
			{
				// Empty row - call empty row context menu event
				UE_LOG(LogTemp, Log, TEXT("Calling OnEmptyRowRightClicked at position (%f, %f)"), MousePosition.X, MousePosition.Y);
				OnEmptyRowRightClicked(MousePosition);
			}
			else
			{
				// Occupied row - select this row if not already selected, then show context menu
				if (!bIsSelected)
				{
					SelectRow(false);
				}

				UE_LOG(LogTemp, Log, TEXT("Calling OnRightClicked at position (%f, %f)"), MousePosition.X, MousePosition.Y);
				OnRightClicked(MousePosition);
			}
		}

		return FReply::Handled();
	}

	// Handle left-click
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Log, TEXT("Left mouse down on list row - ItemData=%s, Item.IsValid=%s"),
			ItemData ? TEXT("YES") : TEXT("NO"),
			(ItemData && ItemData->Item.IsValid()) ? TEXT("YES") : TEXT("NO"));

		// Only start drag if row has an item
		if (ItemData && ItemData->Item.IsValid())
		{
			// Reset drag flag
			bDragStarted = false;

			UE_LOG(LogTemp, Log, TEXT("Starting drag detection for row slot %d"), ItemData->SlotIndex);

			// Start drag detection
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
		}
	}

	return FReply::Unhandled();
}

FReply UInventoryListRowWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// If left mouse button released and we didn't start a drag
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && !bDragStarted)
	{
		// Only handle if row has an item
		if (ItemData && ItemData->Item.IsValid())
		{
			// Check if Shift or Ctrl is held for multi-selection
			bool bAddToSelection = InMouseEvent.IsControlDown() || InMouseEvent.IsShiftDown();

			ToggleSelection(bAddToSelection);

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

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: Row selected - Slot %d (%d total selected)"),
		ItemData->SlotIndex, SelectedRows.Num());
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

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: Row deselected - Slot %d (%d total selected)"),
		ItemData ? ItemData->SlotIndex : -1, SelectedRows.Num());
}

void UInventoryListRowWidget::ToggleSelection(bool bAddToSelection)
{
	if (bIsSelected)
	{
		// If already selected and adding to selection (Ctrl/Shift), deselect this row
		if (bAddToSelection)
		{
			DeselectRow();
		}
		// If not adding to selection, clicking selected row does nothing (keeps it selected)
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
	TSet<UInventoryListRowWidget*> RowsToDeselect = SelectedRows;

	for (UInventoryListRowWidget* Row : RowsToDeselect)
	{
		if (Row && Row->IsValidLowLevel())
		{
			Row->DeselectRow();
		}
	}

	// Clear the set just in case
	SelectedRows.Empty();

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: All row selections cleared"));
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

void UInventoryListRowWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bIsHovered = true;

	// Update visual to show hover effect
	UpdateSelectionVisual();
}

void UInventoryListRowWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsHovered = false;

	// Update visual to remove hover effect
	UpdateSelectionVisual();
}

void UInventoryListRowWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// Mark that we started dragging (so mouse up doesn't trigger selection)
	bDragStarted = true;

	if (!ItemData || !ItemData->Item.IsValid() || !ItemData->InventoryComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: Drag detected on row with slot %d"), ItemData->SlotIndex);

	// Create drag-drop operation
	UInventoryDragDropOperation* DragDropOp = NewObject<UInventoryDragDropOperation>();
	DragDropOp->SourceSlotIndex = ItemData->SlotIndex;
	DragDropOp->DraggedItem = ItemData->Item;
	DragDropOp->InventoryComponent = ItemData->InventoryComponent;

	// Check if shift is held for split operation
	DragDropOp->bIsSplitOperation = InMouseEvent.IsShiftDown() && ItemData->Item.Quantity > 1;

	// Create visual widget for dragging
	// Use the DragVisualClass if set in Blueprint, otherwise try to find the BP widget
	TSubclassOf<UInventorySlotWidget> WidgetClass = DragVisualClass;

	if (!WidgetClass)
	{
		// Try to load the default WBP_InventorySlot class
		UE_LOG(LogTemp, Warning, TEXT("DragVisualClass not set - please set it in WBP_InventoryListRow to WBP_InventorySlot"));
		// We could try to load it here, but it's better to have it set in Blueprint
	}

	if (WidgetClass)
	{
		UInventorySlotWidget* DragVisual = CreateWidget<UInventorySlotWidget>(GetOwningPlayer(), WidgetClass);
		if (DragVisual)
		{
			DragVisual->SetInventoryComponent(ItemData->InventoryComponent);
			DragVisual->SetSlotIndex(ItemData->SlotIndex);
			DragVisual->SetItem(ItemData->Item);
			DragDropOp->DefaultDragVisual = DragVisual;
			DragDropOp->DraggedVisual = DragVisual;
			DragDropOp->Pivot = EDragPivot::CenterCenter;
			DragDropOp->Offset = FVector2D(0, 0); // Snap instantly to cursor, no lerping

			UE_LOG(LogTemp, Log, TEXT("Created drag visual for item: %s"),
				ItemData->Item.ItemData ? *ItemData->Item.ItemData->ItemName.ToString() : TEXT("NULL"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create drag visual widget from class!"));
		}
	}

	OutOperation = DragDropOp;
}

bool UInventoryListRowWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UInventoryDragDropOperation* DragDropOp = Cast<UInventoryDragDropOperation>(InOperation);
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

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: Drop from slot %d to slot %d"),
		DragDropOp->SourceSlotIndex, ItemData->SlotIndex);

	// Handle split operation
	if (DragDropOp->bIsSplitOperation)
	{
		int32 SplitAmount = DragDropOp->DraggedItem.Quantity / 2;
		if (SplitAmount > 0)
		{
			ItemData->InventoryComponent->SplitStack(DragDropOp->SourceSlotIndex, ItemData->SlotIndex, SplitAmount);
			UE_LOG(LogTemp, Log, TEXT("Split %d items from slot %d to slot %d"),
				SplitAmount, DragDropOp->SourceSlotIndex, ItemData->SlotIndex);
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
		UE_LOG(LogTemp, Log, TEXT("Merging stacks: slot %d to slot %d"),
			DragDropOp->SourceSlotIndex, ItemData->SlotIndex);
		ItemData->InventoryComponent->MergeStacks(DragDropOp->SourceSlotIndex, ItemData->SlotIndex);
	}
	else
	{
		// Different items or target is empty - swap
		UE_LOG(LogTemp, Log, TEXT("Swapping items: slot %d with slot %d"),
			DragDropOp->SourceSlotIndex, ItemData->SlotIndex);
		ItemData->InventoryComponent->MoveItem(DragDropOp->SourceSlotIndex, ItemData->SlotIndex);
	}

	bDragStarted = false;
	return true;
}

void UInventoryListRowWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	// Show visual feedback that this row can accept the drop
	UpdateDragVisual(true);

	UE_LOG(LogTemp, Verbose, TEXT("InventoryListRowWidget: Drag enter on row with slot %d"),
		ItemData ? ItemData->SlotIndex : -1);
}

void UInventoryListRowWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	// Remove visual feedback
	UpdateDragVisual(false);

	// Reset drag flag when drag leaves
	bDragStarted = false;

	UE_LOG(LogTemp, Verbose, TEXT("InventoryListRowWidget: Drag leave on row with slot %d"),
		ItemData ? ItemData->SlotIndex : -1);
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

void UInventoryListRowWidget::SetColumnSettings(UInventoryColumnSettings* InColumnSettings)
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
	UE_LOG(LogTemp, Warning, TEXT("===== LIST ROW HandleContextMenuAction ====="));
	UE_LOG(LogTemp, Warning, TEXT("ActionID: %s"), *ActionID.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ItemData valid: %s"), ItemData ? TEXT("YES") : TEXT("NO"));
	if (ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("  Item valid: %s"), ItemData->Item.IsValid() ? TEXT("YES") : TEXT("NO"));
		UE_LOG(LogTemp, Warning, TEXT("  SlotIndex: %d"), ItemData->SlotIndex);
		UE_LOG(LogTemp, Warning, TEXT("  InventoryComponent valid: %s"), ItemData->InventoryComponent ? TEXT("YES") : TEXT("NO"));
		if (ItemData->Item.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("  Item name: %s"), *ItemData->Item.ItemData->ItemName.ToString());
			UE_LOG(LogTemp, Warning, TEXT("  Quantity: %d"), ItemData->Item.Quantity);
		}
	}

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

void UInventoryListRowWidget::HandleSplitItem()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleSplitItem called"));

	if (!ItemData || !ItemData->Item.IsValid() || !ItemData->InventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("HandleSplitItem FAILED: ItemData=%s, Item.IsValid=%s, InvComp=%s"),
			ItemData ? TEXT("YES") : TEXT("NO"),
			(ItemData && ItemData->Item.IsValid()) ? TEXT("YES") : TEXT("NO"),
			(ItemData && ItemData->InventoryComponent) ? TEXT("YES") : TEXT("NO"));
		return;
	}

	if (ItemData->Item.Quantity <= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot split item with quantity 1 or less"));
		return;
	}

	// Find first empty slot
	int32 EmptySlot = ItemData->InventoryComponent->FindEmptySlot();
	if (EmptySlot == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No empty slot available for split"));
		return;
	}

	// Split stack in half
	int32 SplitAmount = ItemData->Item.Quantity / 2;
	UE_LOG(LogTemp, Warning, TEXT("Calling SplitStack: FromSlot=%d, ToSlot=%d, Amount=%d"),
		ItemData->SlotIndex, EmptySlot, SplitAmount);

	ItemData->InventoryComponent->SplitStack(ItemData->SlotIndex, EmptySlot, SplitAmount);

	UE_LOG(LogTemp, Warning, TEXT("Split completed: %d items from slot %d to slot %d"), SplitAmount, ItemData->SlotIndex, EmptySlot);
}

void UInventoryListRowWidget::HandleDestroyItem()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleDestroyItem called"));

	if (!ItemData || !ItemData->Item.IsValid() || !ItemData->InventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("HandleDestroyItem FAILED: ItemData=%s, Item.IsValid=%s, InvComp=%s"),
			ItemData ? TEXT("YES") : TEXT("NO"),
			(ItemData && ItemData->Item.IsValid()) ? TEXT("YES") : TEXT("NO"),
			(ItemData && ItemData->InventoryComponent) ? TEXT("YES") : TEXT("NO"));
		return;
	}

	// Cache the values we need before any widgets are destroyed
	int32 CachedSlotIndex = ItemData->SlotIndex;
	int32 CachedQuantity = ItemData->Item.Quantity;
	FString ItemName = ItemData->Item.ItemData->ItemName.ToString();
	UInventoryComponent* CachedInventoryComponent = ItemData->InventoryComponent;

	UE_LOG(LogTemp, Warning, TEXT("Scheduling destroy for slot %d (%s x%d)"), CachedSlotIndex, *ItemName, CachedQuantity);

	// Defer the actual item removal until the next frame
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick([CachedInventoryComponent, CachedSlotIndex, CachedQuantity, ItemName]()
		{
			if (IsValid(CachedInventoryComponent))
			{
				UE_LOG(LogTemp, Warning, TEXT("Executing deferred destroy: RemoveItemAtSlot(%d, %d)"), CachedSlotIndex, CachedQuantity);
				CachedInventoryComponent->RemoveItemAtSlot(CachedSlotIndex, CachedQuantity);
				UE_LOG(LogTemp, Warning, TEXT("Destroyed %d x %s from slot %d"),
					CachedQuantity,
					*ItemName,
					CachedSlotIndex);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Deferred destroy FAILED: InventoryComponent is no longer valid!"));
			}
		});
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HandleDestroyItem: Could not get World!"));
	}
}

void UInventoryListRowWidget::HandleShowInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleShowInfo called"));

	if (!ItemData || !ItemData->Item.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("HandleShowInfo FAILED: ItemData=%s, Item.IsValid=%s"),
			ItemData ? TEXT("YES") : TEXT("NO"),
			(ItemData && ItemData->Item.IsValid()) ? TEXT("YES") : TEXT("NO"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Calling ShowItemInfoUI Blueprint event for item: %s"),
		*ItemData->Item.ItemData->ItemName.ToString());

	// Call Blueprint event to show UI
	ShowItemInfoUI(ItemData->Item);

	UE_LOG(LogTemp, Warning, TEXT("ShowItemInfoUI Blueprint event called (check if event is implemented in BP)"));
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
		UE_LOG(LogTemp, Warning, TEXT("HandleStackAll called on empty slot - stacking ALL items"));
		HandleStackAllEmpty();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Stack All triggered for slot %d - stacking items of type '%s'"),
		ItemData->SlotIndex, *ItemToStack.ItemData->ItemName.ToString());

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
		UE_LOG(LogTemp, Log, TEXT("Stack All: Only one stack found, nothing to merge"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Stack All: Found %d stacks of '%s'"),
		MatchingSlots.Num(), *ItemToStack.ItemData->ItemName.ToString());

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
				UE_LOG(LogTemp, Log, TEXT("  Merging slot %d (qty=%d) into slot %d (qty=%d)"),
					SourceSlot, SourceItem.Quantity, TargetSlot, TargetItem.Quantity);

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

	UE_LOG(LogTemp, Warning, TEXT("Stack All completed for '%s'"), *ItemToStack.ItemData->ItemName.ToString());
}

void UInventoryListRowWidget::HandleStackAllEmpty()
{
	if (!ItemData || !ItemData->InventoryComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Stack All (Empty) triggered - consolidating all stackable items"));

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
	for (const auto& Group : ItemGroups)
	{
		const TArray<int32>& SlotIndices = Group.Value;
		if (SlotIndices.Num() > 1)
		{
			// Try to merge all items in this group into the first slot
			for (int32 i = 1; i < SlotIndices.Num(); i++)
			{
				ItemData->InventoryComponent->MergeStacks(SlotIndices[i], SlotIndices[0]);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Stack All completed"));
}

void UInventoryListRowWidget::HandleSelectAll()
{
	UE_LOG(LogTemp, Log, TEXT("Select All triggered"));

	// Select all rows
	for (UInventoryListRowWidget* Row : SelectedRows)
	{
		if (Row && Row->IsValidLowLevel())
		{
			Row->SelectRow(true);
		}
	}

	// Also need to select all rows that aren't in the set yet
	// This would require access to all row widgets, which we don't have here
	// For now, just log that it was called
	UE_LOG(LogTemp, Warning, TEXT("Select All for list view - needs access to all rows"));
}

void UInventoryListRowWidget::HandleInvertSelection()
{
	UE_LOG(LogTemp, Log, TEXT("Invert Selection triggered"));

	// Invert selection of all rows
	// This would require access to all row widgets, which we don't have here
	// For now, just log that it was called
	UE_LOG(LogTemp, Warning, TEXT("Invert Selection for list view - needs access to all rows"));
}

void UInventoryListRowWidget::CloseCurrentContextMenu()
{
	if (CurrentContextMenu.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Closing previous context menu"));
		CurrentContextMenu->RemoveFromParent();
		CurrentContextMenu.Reset();
	}
}

void UInventoryListRowWidget::SetCurrentContextMenu(UUserWidget* ContextMenu)
{
	// Close any existing menu first
	CloseCurrentContextMenu();

	if (ContextMenu)
	{
		UE_LOG(LogTemp, Log, TEXT("Setting new current context menu"));
		CurrentContextMenu = ContextMenu;
	}
}

UUserWidget* UInventoryListRowWidget::GetCurrentContextMenu()
{
	return CurrentContextMenu.Get();
}
