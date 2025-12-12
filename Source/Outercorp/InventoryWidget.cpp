// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Input/Reply.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::NativeConstruct: Starting button binding"));

	// Bind button events
	if (CloseButton)
	{
		UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::NativeConstruct: CloseButton found, binding OnClicked"));
		CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::NativeConstruct: CloseButton is NULL! Make sure you have a Button named 'CloseButton' in your widget Blueprint with BindWidget or BindWidgetOptional"));
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

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent)
{
	// Handle ESC or I key to close inventory
	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::I)
	{
		CloseInventory();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInventoryWidget::InitializeInventory(UInventoryComponent *InInventoryComponent)
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
		SlotWidgets[SlotIndex]->SetItem(Item);

		// Show/hide based on whether slot has an item and passes filter
		bool bHasItem = Item.IsValid();
		bool bPassesFilter = !bHasItem || PassesFilter(Item);

		// Empty slots are hidden but still accept drag-drop (HitTestInvisible)
		// Filtered slots are completely collapsed
		if (!bPassesFilter)
		{
			SlotWidgets[SlotIndex]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (!bHasItem)
		{
			SlotWidgets[SlotIndex]->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SlotWidgets[SlotIndex]->SetVisibility(ESlateVisibility::Visible);
		}
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
	UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::CloseInventory: Broadcasting close event"));
	// Just broadcast the close event - let the owning character handle cleanup
	OnInventoryClosed.Broadcast();
	UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::CloseInventory: Broadcast complete"));
}

void UInventoryWidget::OnInventoryUpdated(int32 SlotIndex, const FInventoryItem &Item)
{
	// Just refresh the specific slot - all slots are already created
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
	UE_LOG(LogTemp, Log, TEXT("UInventoryWidget::OnCloseButtonClicked: X button clicked"));
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

void UInventoryWidget::OnSearchTextChanged(const FText &Text)
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

	// Set the slot padding and size on the grid panel
	ItemGrid->SetSlotPadding(SlotPadding);
	ItemGrid->SetMinDesiredSlotWidth(SlotSize);
	ItemGrid->SetMinDesiredSlotHeight(SlotSize);

	// Create all slots - Eve Online style
	// All slots exist but empty ones will be invisible (not collapsed, just invisible)
	int32 NumSlots = InventoryComponent->MaxSlots;

	for (int32 i = 0; i < NumSlots; ++i)
	{
		UInventorySlotWidget *SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (SlotWidget)
		{
			SlotWidget->SetSlotIndex(i);
			SlotWidget->SetInventoryComponent(InventoryComponent);

			int32 Row = i / GridColumns;
			int32 Column = i % GridColumns;

			// Wrap in a SizeBox to enforce the exact slot size
			USizeBox* SizeBox = NewObject<USizeBox>(this);
			SizeBox->SetWidthOverride(SlotSize);
			SizeBox->SetHeightOverride(SlotSize);
			SizeBox->AddChild(SlotWidget);

			// Add to grid
			ItemGrid->AddChildToUniformGrid(SizeBox, Row, Column);
			SlotWidgets.Add(SlotWidget);
		}
	}
}

bool UInventoryWidget::PassesFilter(const FInventoryItem &Item) const
{
	if (CurrentFilter.IsEmpty() || !Item.IsValid())
	{
		return true;
	}

	// Check if item name contains filter text
	FString ItemName = Item.ItemData->ItemName.ToString();
	return ItemName.Contains(CurrentFilter, ESearchCase::IgnoreCase);
}
