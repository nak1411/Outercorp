// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryComponent.h"
#include "InventoryContextMenuHandler.h"
#include "TooltipWidget.h"
#include "InventoryListRowWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
class UBorder;
class UInventoryColumnSettings;
class USizeBox;
class UInventorySlotWidget;

/**
 * Data object for list view items
 */
UCLASS(BlueprintType)
class OUTERCORP_API UInventoryListItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FInventoryItem Item;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 SlotIndex = -1;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;
};

/**
 * List row widget for displaying inventory items in spreadsheet/list view
 * Similar to EVE Online's "Details" view
 */
UCLASS()
class OUTERCORP_API UInventoryListRowWidget : public UUserWidget, public IUserObjectListEntry, public IInventoryContextMenuHandler
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	/** Icon image */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	/** Item name text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	/** Quantity text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityText;

	/** Weight text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> WeightText;

	/** Value text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText;

	/** Rarity text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> RarityText;

	/** Category text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CategoryText;

	/** Row button for interaction */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> RowButton;

	/** Background border for selection highlight */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> BackgroundBorder;

	/** Color when row is selected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Style")
	FLinearColor SelectedColor = FLinearColor(0.2f, 0.4f, 0.8f, 0.5f);

	/** Color when row is not selected (if not using alternating rows) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Style")
	FLinearColor UnselectedColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/** Color when hovering over row with mouse */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Style")
	FLinearColor HoverColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.05f);

	/** Color when dragging an item over this row (drop target highlight) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection Style")
	FLinearColor DragDropHighlightColor = FLinearColor(0.2f, 0.5f, 1.0f, 0.5f);

	/** Enable alternating row background colors */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alternating Rows")
	bool bUseAlternatingRowColors = false;

	/** Background color for even rows (when alternating is enabled) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alternating Rows", meta = (EditCondition = "bUseAlternatingRowColors"))
	FLinearColor EvenRowColor = FLinearColor(0.1f, 0.1f, 0.1f, 0.3f);

	/** Background color for odd rows (when alternating is enabled) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alternating Rows", meta = (EditCondition = "bUseAlternatingRowColors"))
	FLinearColor OddRowColor = FLinearColor(0.15f, 0.15f, 0.15f, 0.3f);

	/** SizeBoxes for each column (optional - for dynamic width control) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> IconColumnBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> NameColumnBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> QuantityColumnBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> WeightColumnBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> ValueColumnBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> RarityColumnBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> CategoryColumnBox;

	/** Whether this row is currently selected */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	bool bIsSelected = false;

	/** The row index in the list (for alternating colors) */
	int32 RowIndex = 0;

	/** Shared column settings */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UInventoryColumnSettings> ColumnSettings;

	/** Widget class to use for drag visual (should be set to WBP_InventorySlot in Blueprint) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventorySlotWidget> DragVisualClass;

	/** Whether to enable tooltips on this row */
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

	/** Quantity input dialog widget class to spawn when dropping/destroying/splitting items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Drop")
	TSubclassOf<class UQuantityInputDialog> QuantityDialogClass;

	/** Reference to currently open quantity dialog */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Drop")
	TObjectPtr<class UQuantityInputDialog> ActiveQuantityDialog;

	/** Static set of all selected rows across all inventory widgets */
	static TSet<UInventoryListRowWidget*> SelectedRows;

	/** Static pointer to the currently open context menu (shared across all rows) */
	static TWeakObjectPtr<UUserWidget> CurrentContextMenu;

	/** Static pointer to the last clicked row (for shift-click range selection) */
	static TWeakObjectPtr<UInventoryListRowWidget> LastClickedRow;

public:
	/** The data for this row */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryListItemData> ItemData;

	/** Refresh the row display with current data */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RefreshDisplay();

	/** Set the row index for alternating row colors */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetRowIndex(int32 Index);

	/** Get the slot index for this row */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetSlotIndex() const { return ItemData ? ItemData->SlotIndex : -1; }

	/** Get the inventory item for this row */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	const FInventoryItem& GetItem() const;

	/** Set the column settings and bind to updates */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetColumnSettings(UInventoryColumnSettings* InColumnSettings);

	/** Update column widths from settings */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateColumnWidths();

	/** Select this row */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SelectRow(bool bAddToSelection = false);

	/** Deselect this row */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DeselectRow();

	/** Toggle selection of this row */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleSelection(bool bAddToSelection = false);

	/** Handle range selection from last clicked row to this row (for shift-click) */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SelectRange();

	/** Get all rows from the same list view container */
	TArray<UInventoryListRowWidget*> GetAllRowsInList() const;

	/** Clear all row selections */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void ClearAllRowSelections();

	/** Get all selected rows */
	static const TSet<UInventoryListRowWidget*>& GetSelectedRows() { return SelectedRows; }

	/** Check if this row is selected */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool IsRowSelected() const { return bIsSelected; }

	/** Whether the mouse is currently hovering over this row (for drag preview and Delete key) */
	bool bIsHovered = false;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	/** Row button clicked */
	UFUNCTION()
	void OnRowButtonClicked();

	/** Update visual state based on selection */
	void UpdateSelectionVisual();

	/** Track if we started a drag operation */
	bool bDragStarted = false;

	/** Visual state for drag and drop feedback */
	void UpdateDragVisual(bool bIsDragTarget);

	/** Get color for rarity */
	FLinearColor GetRarityColor(EItemRarity Rarity) const;

	/** Get text for rarity */
	FText GetRarityText(EItemRarity Rarity) const;

	/** Get text for category */
	FText GetCategoryText(EItemCategory Category) const;

	/** Called when column widths change */
	UFUNCTION()
	void OnColumnWidthsChanged();

	/** Blueprint event called when right-clicked on occupied row - implement context menu in Blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnRightClicked(FVector2D MousePosition);

	/** Blueprint event called when right-clicked on empty row - implement empty slot context menu in Blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnEmptyRowRightClicked(FVector2D MousePosition);

	/** Blueprint-callable function to handle context menu actions */
	virtual void HandleContextMenuAction_Implementation(FName ActionID) override;

	/** Blueprint event to show item info UI */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowItemInfoUI(const FInventoryItem& Item);

	/** Close any currently open context menu before opening a new one */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void CloseCurrentContextMenu();

	/** Set the currently open context menu (call this after creating the menu) */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void SetCurrentContextMenu(UUserWidget* ContextMenu);

	/** Get the current open context menu (useful for Blueprint IsValid checks) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	static UUserWidget* GetCurrentContextMenu();

private:
	/** Handle split item action */
	void HandleSplitItem();

	/** Handle split item confirmation after quantity dialog */
	void HandleSplitItemConfirmed(int32 Quantity);

	/** Handle destroy item action */
	void HandleDestroyItem();

	/** Handle destroy item confirmation after quantity dialog */
	void HandleDestroyItemConfirmed(int32 Quantity);

	/** Handle drop item action */
	void HandleDropItem();

	/** Handle drop item confirmation after quantity dialog */
	void HandleDropItemConfirmed(int32 Quantity);

	/** Handle show info action */
	void HandleShowInfo();

	/** Handle stack all action */
	void HandleStackAll();

	/** Handle stack all (from empty slot) action */
	void HandleStackAllEmpty();

	/** Handle select all action */
	void HandleSelectAll();

	/** Handle invert selection action */
	void HandleInvertSelection();

	/** Show tooltip for this row */
	void ShowTooltip();

	/** Hide tooltip for this row */
	void HideTooltip();
};
