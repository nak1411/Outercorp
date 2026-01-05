// Copyright Epic Games, Inc. All Rights Reserved.

#include "ContainerWidget.h"
#include "InventoryComponent.h"
#include "InventorySlotWidget.h"
#include "InventoryListRowWidget.h"
#include "InventoryListHeaderWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Base_Window handles title and close button

	// Bind view mode button clicks
	if (GridViewButton)
	{
		GridViewButton->OnClicked.AddDynamic(this, &UContainerWidget::OnGridViewButtonClicked);
	}

	if (ListViewButton)
	{
		ListViewButton->OnClicked.AddDynamic(this, &UContainerWidget::OnListViewButtonClicked);
	}

	// Set initial view mode to grid view (index 0)
	if (ViewModeSwitcher)
	{
		ViewModeSwitcher->SetActiveWidgetIndex(0);
		bIsListView = false;
	}
}

FReply UContainerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Handle right-click on empty area to show context menu
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		APlayerController* PC = GetOwningPlayer();
		if (PC)
		{
			float MouseX, MouseY;
			PC->GetMousePosition(MouseX, MouseY);
			FVector2D MousePosition(MouseX, MouseY);

			OnEmptyAreaRightClicked(MousePosition);

			return FReply::Handled();
		}
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UContainerWidget::SetupContainer(UInventoryComponent* InPlayerInventory, UInventoryComponent* InContainerInventory)
{
	PlayerInventory = InPlayerInventory;
	ContainerInventory = InContainerInventory;

	// Bind to inventory changes
	if (PlayerInventory)
	{
		PlayerInventory->OnInventoryUpdated.AddDynamic(this, &UContainerWidget::OnInventoryUpdate);
	}

	if (ContainerInventory)
	{
		ContainerInventory->OnInventoryUpdated.AddDynamic(this, &UContainerWidget::OnInventoryUpdate);
	}

	// Warn about missing widget classes
	if (!SlotWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ContainerWidget: SlotWidgetClass is not set in Blueprint! Grid view will not work."));
	}
	if (!ListRowWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ContainerWidget: ListRowWidgetClass is not set in Blueprint! List view will not work."));
	}

	// Refresh grid view on setup (list view will be populated when/if user switches to it)
	RefreshContainer();
}

void UContainerWidget::OnInventoryUpdate(int32 SlotIndex, const FInventoryItem& Item)
{
	// Trigger a full refresh when inventory changes
	RefreshContainer();
}

void UContainerWidget::RefreshContainer()
{
	if (!ItemGrid || !SlotWidgetClass)
	{
		return;
	}

	// Clear existing slots
	ItemGrid->ClearChildren();
	SlotWidgets.Empty();

	if (!ContainerInventory)
	{
		return;
	}

	// Get all items from container inventory
	TArray<FInventoryItem> Items = ContainerInventory->GetAllItems();

	int32 Row = 0;
	int32 Column = 0;
	int32 SlotIndexCounter = 0;

	// Create slot widgets - populate all slots up to MaxSlots
	for (int32 i = 0; i < MaxSlots; i++)
	{
		UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (SlotWidget)
		{
			// Set up slot with inventory component and slot index
			SlotWidget->SetInventoryComponent(ContainerInventory);
			SlotWidget->SetSlotIndex(i);

			// If we have an item for this slot, set it
			if (i < Items.Num() && Items[i].IsValid())
			{
				SlotWidget->SetItem(Items[i]);
			}

			ItemGrid->AddChildToUniformGrid(SlotWidget, Row, Column);
			SlotWidgets.Add(SlotWidget);

			Column++;
			if (Column >= GridColumns)
			{
				Column = 0;
				Row++;
			}
		}
	}
}

bool UContainerWidget::PassesFilter(UInventoryItemData* ItemData) const
{
	if (!ItemData)
	{
		return false;
	}

	switch (FilterType)
	{
	case EContainerFilter::None:
		return true;

	case EContainerFilter::ToolsOnly:
		return ItemData->Category == EItemCategory::Tool;

	case EContainerFilter::ResourcesOnly:
		return ItemData->Category == EItemCategory::Resource;

	case EContainerFilter::ConsumablesOnly:
		return ItemData->Category == EItemCategory::Consumable;

	case EContainerFilter::Custom:
		return AllowedCategories.Contains(ItemData->Category);

	default:
		return true;
	}
}

void UContainerWidget::MoveToContainer(FGuid ItemInstanceID, int32 Quantity)
{
	if (!PlayerInventory || !ContainerInventory)
	{
		return;
	}

	// Find the item in player inventory by instance ID
	TArray<FInventoryItem> PlayerItems = PlayerInventory->GetAllItems();
	FInventoryItem* FoundItem = nullptr;
	int32 FoundSlotIndex = -1;

	for (int32 i = 0; i < PlayerItems.Num(); i++)
	{
		if (PlayerItems[i].InstanceID == ItemInstanceID)
		{
			FoundItem = &PlayerItems[i];
			FoundSlotIndex = i;
			break;
		}
	}

	if (!FoundItem || !FoundItem->ItemData)
	{
		return;
	}

	// Check if item passes filter
	if (!PassesFilter(FoundItem->ItemData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item %s does not pass container filter"), *FoundItem->ItemData->ItemName.ToString());
		return;
	}

	// Check if container can accept the item
	if (!ContainerInventory->CanAddItem(FoundItem->ItemData, Quantity))
	{
		UE_LOG(LogTemp, Warning, TEXT("Container cannot accept item"));
		return;
	}

	// Store item data before removing
	UInventoryItemData* ItemData = FoundItem->ItemData;

	// Remove from player inventory
	if (PlayerInventory->RemoveItemByInstanceID(ItemInstanceID, Quantity))
	{
		// Add to container inventory
		int32 OutSlot;
		ContainerInventory->AddItem(ItemData, Quantity, OutSlot);
	}
}

void UContainerWidget::MoveToPlayerInventory(FGuid ItemInstanceID, int32 Quantity)
{
	if (!PlayerInventory || !ContainerInventory)
	{
		return;
	}

	// Find the item in container inventory by instance ID
	TArray<FInventoryItem> ContainerItems = ContainerInventory->GetAllItems();
	FInventoryItem* FoundItem = nullptr;
	int32 FoundSlotIndex = -1;

	for (int32 i = 0; i < ContainerItems.Num(); i++)
	{
		if (ContainerItems[i].InstanceID == ItemInstanceID)
		{
			FoundItem = &ContainerItems[i];
			FoundSlotIndex = i;
			break;
		}
	}

	if (!FoundItem || !FoundItem->ItemData)
	{
		return;
	}

	// Store item data before removing
	UInventoryItemData* ItemData = FoundItem->ItemData;

	// Remove from container inventory
	if (ContainerInventory->RemoveItemByInstanceID(ItemInstanceID, Quantity))
	{
		// Add to player inventory
		int32 OutSlot;
		PlayerInventory->AddItem(ItemData, Quantity, OutSlot);
	}
}

void UContainerWidget::ToggleViewMode()
{
	SetViewMode(!bIsListView);
}

void UContainerWidget::SetViewMode(bool bListView)
{
	bIsListView = bListView;

	UE_LOG(LogTemp, Log, TEXT("SetViewMode called - switching to %s view"), bListView ? TEXT("LIST") : TEXT("GRID"));

	// If using WidgetSwitcher (preferred method)
	if (ViewModeSwitcher)
	{
		int32 TargetIndex = bIsListView ? 1 : 0;
		ViewModeSwitcher->SetActiveWidgetIndex(TargetIndex);

		// Populate list view when switching to it
		if (bIsListView)
		{
			UE_LOG(LogTemp, Log, TEXT("SetViewMode - Calling PopulateTableView"));
			PopulateTableView();
		}
		else
		{
			// Refresh grid view
			UE_LOG(LogTemp, Log, TEXT("SetViewMode - Calling RefreshContainer for grid view"));
			RefreshContainer();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetViewMode - ViewModeSwitcher is NULL!"));
	}
}

void UContainerWidget::OnGridViewButtonClicked()
{
	SetViewMode(false);
}

void UContainerWidget::OnListViewButtonClicked()
{
	SetViewMode(true);
}

void UContainerWidget::PopulateTableView()
{
	if (!ListViewRowContainer || !ListRowWidgetClass || !ContainerInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("PopulateTableView failed - ListViewRowContainer=%s, ListRowWidgetClass=%s, ContainerInventory=%s"),
			ListViewRowContainer ? TEXT("Valid") : TEXT("NULL"),
			ListRowWidgetClass ? TEXT("Valid") : TEXT("NULL"),
			ContainerInventory ? TEXT("Valid") : TEXT("NULL"));
		return;
	}

	// Clear existing rows
	ListViewRowContainer->ClearChildren();
	TableViewRows.Empty();

	// Get all items from container
	TArray<FInventoryItem> Items = ContainerInventory->GetAllItems();

	UE_LOG(LogTemp, Log, TEXT("PopulateTableView - Container has %d items total"), Items.Num());

	// Create a row for each valid item (matching InventoryWidget pattern)
	int32 VisualRowIndex = 0; // Track visual row order (different from slot index)
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		const FInventoryItem& Item = Items[i];

		// Only add valid items (skip empty slots)
		if (!Item.IsValid() || !Item.ItemData)
		{
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("PopulateTableView - Creating row for item: %s (slot %d)"), *Item.ItemData->ItemName.ToString(), i);

		// Create row widget
		UInventoryListRowWidget* RowWidget = CreateWidget<UInventoryListRowWidget>(this, ListRowWidgetClass);
		if (RowWidget)
		{
			// Create data object for this item
			UInventoryListItemData* ItemDataObj = NewObject<UInventoryListItemData>(this);
			ItemDataObj->Item = Item;
			ItemDataObj->SlotIndex = i;
			ItemDataObj->InventoryComponent = ContainerInventory;

			// Set data on row widget
			RowWidget->ItemData = ItemDataObj;
			RowWidget->SetRowIndex(VisualRowIndex); // Set row index for alternating colors

			// IMPORTANT: Set visibility before adding to container
			RowWidget->SetVisibility(ESlateVisibility::Visible);

			RowWidget->RefreshDisplay();

			// Add to vertical box
			UVerticalBoxSlot* RowSlot = ListViewRowContainer->AddChildToVerticalBox(RowWidget);
			if (RowSlot)
			{
				RowSlot->SetPadding(FMargin(0.0f));
				RowSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}

			// Cache the row widget
			TableViewRows.Add(RowWidget);

			UE_LOG(LogTemp, Log, TEXT("PopulateTableView - Successfully added row %d for item %s. RowWidget visible=%d, ListViewRowContainer has %d children"),
				VisualRowIndex,
				*Item.ItemData->ItemName.ToString(),
				RowWidget->GetVisibility() == ESlateVisibility::Visible ? 1 : 0,
				ListViewRowContainer->GetChildrenCount());

			VisualRowIndex++; // Increment visual row counter
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PopulateTableView - Failed to create row widget for item at slot %d"), i);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("PopulateTableView - Finished. Added %d rows to list view"), VisualRowIndex);
}

void UContainerWidget::OnEmptyAreaRightClicked_Implementation(FVector2D MousePosition)
{
	// Empty default implementation - override in Blueprint to show context menu
}

// ============================================================================
// IWindowContentInterface Implementation
// ============================================================================

FWindowCapabilities UContainerWidget::GetWindowCapabilities_Implementation() const
{
	return FWindowCapabilities(bCanMove, bCanResize, bCanFullscreen);
}
