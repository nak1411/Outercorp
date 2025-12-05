// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySlotWidget.h"
#include "InventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/Texture2D.h"
#include "Input/Reply.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

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
	// Start drag detection
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && CurrentItem.IsValid())
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!CurrentItem.IsValid() || !InventoryComponent)
	{
		return;
	}

	// Create drag-drop operation
	UInventoryDragDropOperation* DragDropOp = NewObject<UInventoryDragDropOperation>();
	DragDropOp->SourceSlotIndex = SlotIndex;
	DragDropOp->DraggedItem = CurrentItem;
	DragDropOp->InventoryComponent = InventoryComponent;

	// Check if shift is held for split operation
	DragDropOp->bIsSplitOperation = InMouseEvent.IsShiftDown() && CurrentItem.Quantity > 1;

	// Create visual widget for dragging
	UInventorySlotWidget* DragVisual = CreateWidget<UInventorySlotWidget>(this, GetClass());
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
		return false;
	}

	// Reset background color
	if (BackgroundBorder)
	{
		BackgroundBorder->SetBrushColor(NormalColor);
	}

	// Don't drop on same slot
	if (DragDropOp->SourceSlotIndex == SlotIndex)
	{
		return false;
	}

	// Handle split operation
	if (DragDropOp->bIsSplitOperation)
	{
		int32 SplitAmount = DragDropOp->DraggedItem.Quantity / 2;
		if (SplitAmount > 0)
		{
			InventoryComponent->SplitStack(DragDropOp->SourceSlotIndex, SlotIndex, SplitAmount);
		}
		return true;
	}

	// Handle normal move/swap
	InventoryComponent->MoveItem(DragDropOp->SourceSlotIndex, SlotIndex);

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

	// Reset background color
	if (BackgroundBorder)
	{
		BackgroundBorder->SetBrushColor(NormalColor);
	}
}

void UInventorySlotWidget::SetItem(const FInventoryItem& Item)
{
	CurrentItem = Item;
	UpdateAppearance();
}

void UInventorySlotWidget::OnSlotClicked()
{
	// Right-click or use functionality can be implemented here
	// For now, just broadcast that slot was clicked
	if (CurrentItem.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Slot %d clicked: %s"), SlotIndex, *CurrentItem.ItemData->ItemName.ToString());
	}
}

void UInventorySlotWidget::UpdateAppearance()
{
	if (CurrentItem.IsValid() && CurrentItem.ItemData)
	{
		// Set item icon
		if (ItemIcon)
		{
			if (CurrentItem.ItemData->ItemIcon.IsNull())
			{
				ItemIcon->SetOpacity(0.3f);
				ItemIcon->SetBrushFromTexture(nullptr);
			}
			else
			{
				ItemIcon->SetOpacity(1.0f);
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

		// Set rarity border color
		if (RarityBorder)
		{
			FLinearColor RarityColor = CurrentItem.ItemData->GetRarityColor();
			RarityBorder->SetBrushColor(RarityColor);
		}
	}
	else
	{
		// Empty slot
		if (ItemIcon)
		{
			if (EmptySlotIcon.IsNull())
			{
				ItemIcon->SetOpacity(0.1f);
				ItemIcon->SetBrushFromTexture(nullptr);
			}
			else
			{
				ItemIcon->SetOpacity(0.3f);
				UTexture2D* EmptyTexture = EmptySlotIcon.LoadSynchronous();
				ItemIcon->SetBrushFromTexture(EmptyTexture);
			}
		}

		if (QuantityText)
		{
			QuantityText->SetVisibility(ESlateVisibility::Hidden);
		}

		if (RarityBorder)
		{
			RarityBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.5f));
		}
	}

	// Set background color
	if (BackgroundBorder)
	{
		BackgroundBorder->SetBrushColor(NormalColor);
	}
}
