// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySlotWidget.h"
#include "InventoryComponent.h"
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

// Initialize static member
TWeakObjectPtr<UUserWidget> UInventorySlotWidget::CurrentOpenContextMenu = nullptr;

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
	UE_LOG(LogTemp, Log, TEXT("InventorySlotWidget::NativeOnMouseButtonDown called for slot %d"), SlotIndex);

	// Handle right-click for context menu - call Blueprint event
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Log, TEXT("Right-click detected on slot %d"), SlotIndex);

		// IMPORTANT: Close any existing context menu FIRST
		CloseCurrentContextMenu();

		// Get mouse position in viewport pixel coordinates
		APlayerController* PC = GetOwningPlayer();
		if (PC)
		{
			float MouseX, MouseY;
			PC->GetMousePosition(MouseX, MouseY);
			FVector2D MousePosition(MouseX, MouseY);

			// Call Blueprint implementable event
			OnRightClicked(MousePosition);
		}

		return FReply::Handled();
	}

	// Start drag detection for left-click
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && CurrentItem.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Left-click drag detection"));
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	UE_LOG(LogTemp, Log, TEXT("Calling Super::NativeOnMouseButtonDown"));
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
	// Left-click functionality can be implemented here
	// For now, just broadcast that slot was clicked
	if (CurrentItem.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Slot %d clicked: %s"), SlotIndex, *CurrentItem.ItemData->ItemName.ToString());
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

	// Set background color
	if (BackgroundBorder)
	{
		BackgroundBorder->SetBrushColor(NormalColor);
	}
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
