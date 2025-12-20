// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryItemData.h"
#include "InventorySlotWidget.generated.h"

class UInventoryComponent;
class UImage;
class UTextBlock;
class UBorder;
class UButton;

/**
 * Drag-drop operation for inventory items
 * Must be declared before UInventorySlotWidget
 */
UCLASS()
class OUTERCORP_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	/** Source slot index */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 SourceSlotIndex = -1;

	/** Item being dragged */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FInventoryItem DraggedItem;

	/** Inventory component */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Visual widget for dragging */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UUserWidget> DraggedVisual;

	/** Is this a split operation (shift-drag) */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool bIsSplitOperation = false;
};

/**
 * Widget representing a single inventory slot
 */
UCLASS()
class OUTERCORP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	/** Track if we started a drag operation */
	bool bDragStarted = false;

	/** Item icon image */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	/** Quantity text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> QuantityText;

	/** Rarity border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> RarityBorder;

	/** Quantity border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> QuantityBorder;

	/** Background border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> BackgroundBorder;

	/** Slot button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> SlotButton;

	/** Current item in this slot */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FInventoryItem CurrentItem;

	/** Slot index in inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 SlotIndex = -1;

	/** Reference to inventory component */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Opacity for item icon when item has no icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Visuals", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ItemNoIconOpacity = 0.3f;

	/** Opacity for item icon when item has an icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Visuals", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ItemIconOpacity = 1.0f;

	/** Color for drag hover state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Visuals")
	FLinearColor HoverColor = FLinearColor(0.2f, 0.5f, 1.0f, 0.5f);

	/** Normal background color */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Visuals")
	FLinearColor NormalColor = FLinearColor(0.05f, 0.05f, 0.05f, 0.9f);

public:
	/** Set the item for this slot */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItem(const FInventoryItem& Item);

	/** Set slot index */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSlotIndex(int32 Index) { SlotIndex = Index; }

	/** Set inventory component reference */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryComponent(UInventoryComponent* InInventoryComponent) { InventoryComponent = InInventoryComponent; }

	/** Get current item */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FInventoryItem GetItem() const { return CurrentItem; }

	/** Get slot index */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetSlotIndex() const { return SlotIndex; }

	/** Check if slot is empty */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool IsSlotEmpty() const { return !CurrentItem.IsValid(); }

protected:
	/** Called when slot is clicked */
	UFUNCTION()
	void OnSlotClicked();

	/** Update visual appearance based on item */
	void UpdateAppearance();

	/** Blueprint event called when right-clicked on occupied slot - implement context menu in Blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnRightClicked(FVector2D MousePosition);

	/** Blueprint event called when right-clicked on empty slot - implement empty slot context menu in Blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnEmptySlotRightClicked(FVector2D MousePosition);

	/** Close any currently open context menu before opening a new one */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void CloseCurrentContextMenu();

	/** Set the currently open context menu (call this after creating the menu) */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void SetCurrentContextMenu(UUserWidget* ContextMenu);

private:
	/** Static reference to currently open context menu (shared across all slots) */
	static TWeakObjectPtr<UUserWidget> CurrentOpenContextMenu;

public:
	/** Get the current open context menu (useful for Blueprint IsValid checks) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	static UUserWidget* GetCurrentContextMenu();
	/** Blueprint-callable function to handle context menu actions */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HandleContextMenuAction(FName ActionID);

protected:
	/** Handle split item action */
	void HandleSplitItem();

	/** Handle destroy item action */
	void HandleDestroyItem();

	/** Handle show info action */
	void HandleShowInfo();

	/** Blueprint event for showing item info UI */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowItemInfoUI(const FInventoryItem& Item);

	/** Handle stack all action */
	void HandleStackAll();

	/** Handle stack all (empty slot version) - stacks all items of any type */
	void HandleStackAllEmpty();

	/** Handle select all action - selects all items in inventory */
	void HandleSelectAll();

	/** Handle invert selection action - inverts current selection */
	void HandleInvertSelection();

	/** Blueprint event for notifying selection changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnSelectionChanged(const TArray<int32>& SelectedSlots);

	/** Broadcast selection change to all slots in the inventory */
	void BroadcastSelectionChanged();

public:
	/** Get current selection state for this slot */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Selection")
	bool IsSelected() const { return bIsSelected; }

	/** Set selection state for this slot */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Selection")
	void SetSelected(bool bSelected);

	/** Toggle selection state for this slot */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Selection")
	void ToggleSelection();

	/** Get currently selected slots from the inventory */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Selection")
	TArray<int32> GetSelectedSlots() const;

	/** Clear all selections in the inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Selection")
	void ClearAllSelections();

	/** Check if any slots are currently selected */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Selection")
	bool HasAnySelection() const;

protected:
	/** Selection state for this slot */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Selection")
	bool bIsSelected = false;

private:
	/** Shared selection state across all slots in the same inventory */
	static TMap<UInventoryComponent*, TSet<int32>> InventorySelections;

	/** Registry of all slot widgets by inventory component */
	static TMap<UInventoryComponent*, TArray<TWeakObjectPtr<UInventorySlotWidget>>> SlotWidgetRegistry;
};
