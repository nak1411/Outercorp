// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryItemData.h"
#include "InventoryContextMenuHandler.h"
#include "TooltipWidget.h"
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

	/** Track if the item was successfully dropped on a valid target */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool bWasDroppedOnValidTarget = false;

	/** Override Drop to track if dropped on valid target */
	virtual void Drop_Implementation(const FPointerEvent& PointerEvent) override;

	/** Override DragCancelled to handle dropping outside inventory */
	virtual void DragCancelled_Implementation(const FPointerEvent& PointerEvent) override;

private:
	/** Handle dropping the item into the world */
	void HandleDropToWorld();
};

/**
 * Widget representing a single inventory slot
 */
UCLASS()
class OUTERCORP_API UInventorySlotWidget : public UUserWidget, public IInventoryContextMenuHandler
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent, UDragDropOperation *&OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry &InGeometry, const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent &InDragDropEvent, UDragDropOperation *InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent &InMouseEvent) override;

	/** Track if we started a drag operation */
	bool bDragStarted = false;

	/** Cached item name to avoid unnecessary text updates */
	FText CachedItemName;

public:
	/** Track if mouse is currently hovering over this slot */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	bool bIsHovered = false;

protected:
	/** Item icon image */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	/** Quantity text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> QuantityText;

	/** Item name text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemText;

	/** Rarity border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> RarityBorder;

	/** Quantity border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> QuantityBorder;

	/** Background border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> BackgroundBorder;

	/** Selection border - controlled by Blueprint for hover/selection states */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> SelectionBorder;

	/** Selection highlight image - alternative to SelectionBorder for reliable rendering */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UImage> SelectionHighlight;

	/** Hover overlay - covers the entire slot to provide hover tint effect */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> HoverOverlay;

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

	/** Opacity for item icon when item has an icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Visuals", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ItemIconOpacity = 1.0f;

	/** Color for drag hover state (when dragging over this slot) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Visuals")
	FLinearColor DragHoverColor = FLinearColor(0.2f, 0.5f, 1.0f, 0.5f);

	/** Hover overlay color - shown when slot is hovered (use semi-transparent white to lighten) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Visuals")
	FLinearColor HoverOverlayColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.2f);

	/** Color for selection border when item is selected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Visuals")
	FLinearColor SelectionBorderColor = FLinearColor(1.0f, 0.843f, 0.0f, 1.0f); // Gold color

	/** Whether to show hover overlay on empty slots (default: false) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Visuals")
	bool bShowHoverOnEmptySlots = false;

	/** Whether to enable tooltips on this slot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Tooltip")
	bool bEnableTooltip = true;

	/** Tooltip widget class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Tooltip")
	TSubclassOf<UTooltipWidget> TooltipClass;

	/** Tooltip display configuration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Tooltip")
	FTooltipDisplayConfig TooltipConfig;

	/** Reference to currently active tooltip */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Tooltip")
	TObjectPtr<UTooltipWidget> ActiveTooltip;

public:
	/** Item info window widget class to spawn (optional, if not using Blueprint event) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|ItemInfo")
	TSubclassOf<UUserWidget> ItemInfoWidgetClass;

	/** Reference to currently open item info window */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|ItemInfo")
	TObjectPtr<UUserWidget> ActiveItemInfoWindow;

	/** Quantity input dialog widget class to spawn when dropping items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Drop")
	TSubclassOf<class UQuantityInputDialog> QuantityDialogClass;

	/** Reference to currently open quantity dialog */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Drop")
	TObjectPtr<class UQuantityInputDialog> ActiveQuantityDialog;

	/** Set the item for this slot */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItem(const FInventoryItem &Item);

	/** Set slot index */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSlotIndex(int32 Index) { SlotIndex = Index; }

	/** Set inventory component reference */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryComponent(UInventoryComponent *InInventoryComponent) { InventoryComponent = InInventoryComponent; }

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
	static void SetCurrentContextMenu(UUserWidget *ContextMenu);

private:
	/** Static reference to currently open context menu (shared across all slots) */
	static TWeakObjectPtr<UUserWidget> CurrentOpenContextMenu;

public:
	/** Get the current open context menu (useful for Blueprint IsValid checks) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	static UUserWidget *GetCurrentContextMenu();
	/** Blueprint-callable function to handle context menu actions */
	virtual void HandleContextMenuAction_Implementation(FName ActionID) override;

	/** Handle destroy item action (public for Delete key) */
	void HandleDestroyItem();

protected:
	/** Handle split item action */
	void HandleSplitItem();

	/** Handle split item confirmation after quantity dialog */
	void HandleSplitItemConfirmed(int32 Quantity);

	/** Handle drop item action */
	void HandleDropItem();

	/** Handle drop item confirmation after quantity dialog */
	void HandleDropItemConfirmed(int32 Quantity);

	/** Handle destroy item confirmation after quantity dialog */
	void HandleDestroyItemConfirmed(int32 Quantity);

	/** Handle show info action */
	void HandleShowInfo();

	/** Blueprint event for showing item info UI */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowItemInfoUI(const FInventoryItem &Item);

	/** Handle stack all action - stacks all items of the same type as the clicked slot */
	void HandleStackAll();

	/** Handle stack all items action - stacks all stackable items in the entire inventory */
	void HandleStackAllItems();

	/** Handle select all action - selects all items in inventory */
	void HandleSelectAll();

	/** Handle invert selection action - inverts current selection */
	void HandleInvertSelection();

	/** Blueprint event for notifying selection changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnSelectionChanged(const TArray<int32> &SelectedSlots);

	/** Blueprint event for notifying hover state changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnHoverStateChanged(bool bHovered);

	/** Broadcast selection change to all slots in the inventory */
	void BroadcastSelectionChanged();

	/** Show tooltip for this slot */
	void ShowTooltip();

	/** Hide tooltip for this slot */
	void HideTooltip();

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

	/** Clear all selections for a specific inventory component (static method) */
	static void ClearAllSelectionsForInventory(UInventoryComponent *InInventoryComponent);

	/** Check if any slots are currently selected */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory|Selection")
	bool HasAnySelection() const;

	/** Registry of all slot widgets by inventory component (public for drag drop access) */
	static TMap<UInventoryComponent *, TArray<TWeakObjectPtr<UInventorySlotWidget>>> SlotWidgetRegistry;

protected:
	/** Selection state for this slot */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Selection")
	bool bIsSelected = false;

private:
	/** Shared selection state across all slots in the same inventory */
	static TMap<UInventoryComponent *, TSet<int32>> InventorySelections;
};
