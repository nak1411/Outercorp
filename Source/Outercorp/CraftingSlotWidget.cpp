// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingSlotWidget.h"
#include "InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UCraftingSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateAppearance();
}

bool UCraftingSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (!DragOp)
	{
		return false;
	}

	// Mark as dropped on valid target to prevent dropping to world
	// Even if we can't accept the item, we handle the drop here
	DragOp->bWasDroppedOnValidTarget = true;

	// Check if we can accept this item
	if (!CanAcceptItem(DragOp->DraggedItem))
	{
		// Reset highlight
		if (HighlightBorder)
		{
			HighlightBorder->SetBrushColor(FLinearColor::Transparent);
		}
		return true; // Return true to indicate we handled the drop (prevents world drop)
	}

	// Assign the item (this doesn't remove it from source inventory, just references it)
	if (AssignItem(DragOp->DraggedItem))
	{
		// Store source inventory info for consumption later
		SourceInventory = DragOp->InventoryComponent;
		SourceSlotIndex = DragOp->SourceSlotIndex;

		// Mark as successfully dropped
		DragOp->bWasDroppedOnValidTarget = true;

		// Reset highlight
		if (HighlightBorder)
		{
			HighlightBorder->SetBrushColor(FLinearColor::Transparent);
		}

		return true;
	}

	return false;
}

void UCraftingSlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (!DragOp || !HighlightBorder)
	{
		return;
	}

	// Show valid/invalid drop indicator
	if (CanAcceptItem(DragOp->DraggedItem))
	{
		HighlightBorder->SetBrushColor(ValidDropColor);
	}
	else
	{
		HighlightBorder->SetBrushColor(InvalidDropColor);
	}
}

void UCraftingSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Reset highlight
	if (HighlightBorder)
	{
		HighlightBorder->SetBrushColor(FLinearColor::Transparent);
	}
}

FReply UCraftingSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Right-click to clear slot
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton && bIsFilled)
	{
		ClearSlot();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UCraftingSlotWidget::SetRequiredItem(UInventoryItemData* Item, int32 Quantity)
{
	RequiredItem = Item;
	RequiredQuantity = Quantity;

	// Clear current assignment when requirements change
	ClearSlot();
}

bool UCraftingSlotWidget::AssignItem(const FInventoryItem& Item)
{
	if (!CanAcceptItem(Item))
	{
		return false;
	}

	AssignedItem = Item;
	bIsFilled = true;

	UpdateAppearance();

	// Notify Blueprint
	OnItemAssigned(Item);

	return true;
}

void UCraftingSlotWidget::ClearSlot()
{
	AssignedItem = FInventoryItem();
	SourceInventory = nullptr;
	SourceSlotIndex = -1;
	bIsFilled = false;

	UpdateAppearance();

	// Notify Blueprint
	OnSlotCleared();
}

bool UCraftingSlotWidget::CanAcceptItem(const FInventoryItem& Item) const
{
	if (!Item.IsValid() || !Item.ItemData)
	{
		return false;
	}

	// Check slot type compatibility
	if (SlotType == ECraftingSlotType::Tool)
	{
		// Tool slots only accept items with Tool category
		if (Item.ItemData->Category != EItemCategory::Tool)
		{
			return false;
		}
	}
	else if (SlotType == ECraftingSlotType::Ingredient)
	{
		// Ingredient/Material slots only accept Resource category items
		if (Item.ItemData->Category != EItemCategory::Resource)
		{
			return false;
		}
	}

	// If a specific item is required, check it matches
	if (RequiredItem)
	{
		// Check if this is the required item
		if (Item.ItemData != RequiredItem)
		{
			return false;
		}

		// Check quantity
		if (Item.Quantity < RequiredQuantity)
		{
			return false;
		}
	}

	return true;
}

void UCraftingSlotWidget::UpdateAppearance()
{
	if (!ItemIcon)
	{
		return;
	}

	if (bIsFilled && AssignedItem.IsValid())
	{
		// Show item icon
		if (AssignedItem.ItemData && AssignedItem.ItemData->ItemIcon)
		{
			UTexture2D* LoadedTexture = AssignedItem.ItemData->ItemIcon.LoadSynchronous();
			if (LoadedTexture)
			{
				ItemIcon->SetBrushFromTexture(LoadedTexture);
				ItemIcon->SetOpacity(1.0f);
			}
		}

		// Show quantity
		if (QuantityText)
		{
			if (AssignedItem.Quantity > 1)
			{
				QuantityText->SetText(FText::AsNumber(AssignedItem.Quantity));
				QuantityText->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				QuantityText->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	else
	{
		// Show placeholder or required item icon
		if (RequiredItem && RequiredItem->ItemIcon)
		{
			// Show required item as faded placeholder
			UTexture2D* LoadedTexture = RequiredItem->ItemIcon.LoadSynchronous();
			if (LoadedTexture)
			{
				ItemIcon->SetBrushFromTexture(LoadedTexture);
				ItemIcon->SetOpacity(0.3f);
			}
		}
		else
		{
			// Empty slot
			ItemIcon->SetBrushFromTexture(nullptr);
			ItemIcon->SetOpacity(0.0f);
		}

		// Hide quantity
		if (QuantityText)
		{
			QuantityText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
