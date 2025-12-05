// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button events
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseButtonClicked);
	}

	if (SortByNameButton)
	{
		SortByNameButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnSortByNameClicked);
	}

	if (SortByRarityButton)
	{
		SortByRarityButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnSortByRarityClicked);
	}

	if (SearchText)
	{
		SearchText->OnTextChanged.AddDynamic(this, &UInventoryWidget::OnSearchTextChanged);
	}
}

void UInventoryWidget::NativeDestruct()
{
	// Delegates are automatically cleaned up when the widget is destroyed
	// No need to manually unbind

	Super::NativeDestruct();
}

void UInventoryWidget::InitializeInventory(UInventoryComponent* InInventoryComponent)
{
	if (!InInventoryComponent)
	{
		return;
	}

	InventoryComponent = InInventoryComponent;

	// Bind to inventory events
	InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::OnInventoryUpdated);
	InventoryComponent->OnInventoryCapacityChanged.AddDynamic(this, &UInventoryWidget::OnCapacityChanged);

	// Create slot widgets
	CreateSlotWidgets();

	// Initial refresh
	RefreshInventory();
}

void UInventoryWidget::RefreshInventory()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Refresh all slots
	for (int32 i = 0; i < SlotWidgets.Num(); ++i)
	{
		RefreshSlot(i);
	}

	// Update capacity display
	UpdateCapacityDisplay();
}

void UInventoryWidget::RefreshSlot(int32 SlotIndex)
{
	if (!InventoryComponent || !SlotWidgets.IsValidIndex(SlotIndex))
	{
		return;
	}

	FInventoryItem Item = InventoryComponent->GetItemAtSlot(SlotIndex);

	if (SlotWidgets[SlotIndex])
	{
		// Check if item passes filter
		bool bShouldShow = !Item.IsValid() || PassesFilter(Item);

		SlotWidgets[SlotIndex]->SetItem(Item);
		SlotWidgets[SlotIndex]->SetVisibility(bShouldShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UInventoryWidget::UpdateCapacityDisplay()
{
	if (!InventoryComponent)
	{
		return;
	}

	// Update weight display
	if (WeightText)
	{
		float CurrentWeight = InventoryComponent->GetCurrentWeight();
		float MaxWeight = InventoryComponent->MaxWeight;

		if (MaxWeight > 0.0f)
		{
			WeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1f / %.1f kg"), CurrentWeight, MaxWeight)));

			if (WeightProgressBar)
			{
				WeightProgressBar->SetPercent(CurrentWeight / MaxWeight);
			}
		}
		else
		{
			WeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1f kg"), CurrentWeight)));

			if (WeightProgressBar)
			{
				WeightProgressBar->SetPercent(0.0f);
			}
		}
	}

	// Update capacity display
	if (CapacityText)
	{
		int32 OccupiedSlots = InventoryComponent->GetOccupiedSlots();
		int32 MaxSlots = InventoryComponent->MaxSlots;
		CapacityText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), OccupiedSlots, MaxSlots)));
	}
}

void UInventoryWidget::CloseInventory()
{
	RemoveFromParent();
}

void UInventoryWidget::OnInventoryUpdated(int32 SlotIndex, const FInventoryItem& Item)
{
	RefreshSlot(SlotIndex);
	UpdateCapacityDisplay();
}

void UInventoryWidget::OnCapacityChanged(int32 NewCapacity)
{
	// Recreate slot widgets if capacity changed
	CreateSlotWidgets();
	RefreshInventory();
}

void UInventoryWidget::OnCloseButtonClicked()
{
	CloseInventory();
}

void UInventoryWidget::OnSortByNameClicked()
{
	if (InventoryComponent)
	{
		InventoryComponent->SortInventory(true);
	}
}

void UInventoryWidget::OnSortByRarityClicked()
{
	if (InventoryComponent)
	{
		InventoryComponent->SortInventory(false);
	}
}

void UInventoryWidget::OnSearchTextChanged(const FText& Text)
{
	CurrentFilter = Text.ToString();
	RefreshInventory();
}

void UInventoryWidget::CreateSlotWidgets()
{
	if (!ItemGrid || !SlotWidgetClass || !InventoryComponent)
	{
		return;
	}

	// Clear existing widgets
	ItemGrid->ClearChildren();
	SlotWidgets.Empty();

	// Create new slot widgets
	int32 NumSlots = InventoryComponent->MaxSlots;

	for (int32 i = 0; i < NumSlots; ++i)
	{
		UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (SlotWidget)
		{
			SlotWidget->SetSlotIndex(i);
			SlotWidget->SetInventoryComponent(InventoryComponent);

			int32 Row = i / GridColumns;
			int32 Column = i % GridColumns;

			ItemGrid->AddChildToUniformGrid(SlotWidget, Row, Column);
			SlotWidgets.Add(SlotWidget);
		}
	}
}

bool UInventoryWidget::PassesFilter(const FInventoryItem& Item) const
{
	if (CurrentFilter.IsEmpty() || !Item.IsValid())
	{
		return true;
	}

	// Check if item name contains filter text
	FString ItemName = Item.ItemData->ItemName.ToString();
	return ItemName.Contains(CurrentFilter, ESearchCase::IgnoreCase);
}
