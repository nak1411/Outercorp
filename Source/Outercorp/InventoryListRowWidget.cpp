// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryListRowWidget.h"
#include "InventoryColumnSettings.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "InventoryItemData.h"
#include "Engine/Texture2D.h"
#include "Input/Reply.h"
#include "GameFramework/PlayerController.h"

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

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: ItemData set successfully, calling RefreshDisplay"));
	RefreshDisplay();
}

void UInventoryListRowWidget::NativeOnItemSelectionChanged(bool bIsItemSelected)
{
	bIsSelected = bIsItemSelected;

	// Update background color based on selection
	if (BackgroundBorder)
	{
		FLinearColor BackgroundColor = bIsItemSelected
			? FLinearColor(0.2f, 0.4f, 0.8f, 0.5f)  // Blue highlight when selected
			: FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Transparent when not selected

		BackgroundBorder->SetBrushColor(BackgroundColor);
	}
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

	// For other mouse buttons, only handle if row has an item
	if (!ItemData || !ItemData->Item.IsValid())
	{
		return FReply::Unhandled();
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// Check if Shift or Ctrl is held for multi-selection
		bool bAddToSelection = InMouseEvent.IsControlDown() || InMouseEvent.IsShiftDown();

		ToggleSelection(bAddToSelection);

		return FReply::Handled();
	}

	return FReply::Unhandled();
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
	if (BackgroundBorder)
	{
		FLinearColor BackgroundColor = bIsSelected
			? FLinearColor(0.2f, 0.4f, 0.8f, 0.5f)  // Blue highlight when selected
			: FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Transparent when not selected

		BackgroundBorder->SetBrushColor(BackgroundColor);
	}
}

void UInventoryListRowWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bIsHovered = true;

	// TODO: Add hover visual feedback if desired
}

void UInventoryListRowWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsHovered = false;

	// TODO: Remove hover visual feedback if desired
}

void UInventoryListRowWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// TODO: Implement drag and drop
	// This will be similar to the inventory slot drag and drop system
	// For now, this is a placeholder for future implementation

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: Drag detected on row with slot %d"),
		ItemData ? ItemData->SlotIndex : -1);
}

bool UInventoryListRowWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// TODO: Implement drag and drop handling
	// This will handle dropping items onto this row

	UpdateDragVisual(false);

	UE_LOG(LogTemp, Log, TEXT("InventoryListRowWidget: Drop on row with slot %d"),
		ItemData ? ItemData->SlotIndex : -1);

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
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

	UE_LOG(LogTemp, Verbose, TEXT("InventoryListRowWidget: Drag leave on row with slot %d"),
		ItemData ? ItemData->SlotIndex : -1);
}

void UInventoryListRowWidget::UpdateDragVisual(bool bIsDragTarget)
{
	// TODO: Add visual feedback for drag and drop
	// For now, we can use a different border color or opacity

	if (BackgroundBorder && !bIsSelected)
	{
		if (bIsDragTarget)
		{
			// Show a different color when being dragged over
			BackgroundBorder->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.0f, 0.3f)); // Yellow tint
		}
		else
		{
			// Clear the drag visual (but keep selection visual if selected)
			UpdateSelectionVisual();
		}
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

void UInventoryListRowWidget::HandleContextMenuAction(FName ActionID)
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

	UE_LOG(LogTemp, Log, TEXT("Stack All triggered for slot %d"), ItemData->SlotIndex);
	UE_LOG(LogTemp, Warning, TEXT("Stack All functionality - to be implemented"));
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
