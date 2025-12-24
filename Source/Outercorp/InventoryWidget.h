// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "InventoryContextMenuHandler.h"
#include "InventoryWidget.generated.h"

class UInventorySlotWidget;
class UInventoryListRowWidget;
class UInventoryListHeaderWidget;
class UInventoryColumnSettings;
class UUniformGridPanel;
class UScrollBox;
class USizeBox;
class UTextBlock;
class UProgressBar;
class UButton;
class UEditableText;
class UListView;

/**
 * Main inventory window widget (Eve Online style)
 */
UCLASS()
class OUTERCORP_API UInventoryWidget : public UUserWidget, public IInventoryContextMenuHandler
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeSupportsKeyboardFocus() const override { return true; }

	/** Reference to the inventory component */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Grid panel containing item slots */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemGrid;

	/** Outer scroll box for vertical scrolling (optional - will be used if present) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> ItemScrollBox;

	/** Inner scroll box for horizontal scrolling (optional - for 2D scrolling) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> HorizontalScrollBox;

	/** SizeBox wrapper for ItemGrid (optional - created at runtime if needed) */
	UPROPERTY()
	TObjectPtr<USizeBox> GridSizeBox;

	/** Weight display text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> WeightText;

	/** Weight progress bar */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> WeightProgressBar;

	/** Capacity display text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CapacityText;

	/** Title text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TitleText;

	/** Close button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton;

	/** Sort by name button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> SortByNameButton;

	/** Sort by rarity button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> SortByRarityButton;

	/** Search/filter text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UEditableText> SearchText;

	/** Grid view button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> GridViewButton;

	/** List view button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> ListViewButton;

	/** ListView widget for list view mode (legacy) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UListView> ItemListView;

	/** ScrollBox for table-style list view (recommended) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> ListViewScrollBox;

	/** Vertical box container inside ScrollBox for rows */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<class UVerticalBox> ListViewRowContainer;

	/** Header widget for list view columns */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UInventoryListHeaderWidget> ListViewHeader;

	/** Widget switcher for toggling between grid and list view (optional, more reliable than show/hide) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<class UWidgetSwitcher> ViewModeSwitcher;

	/** Class for inventory slot widgets */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	/** Class for inventory list row widgets */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UInventoryListRowWidget> ListRowWidgetClass;

	/** Class for inventory list header widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UInventoryListHeaderWidget> ListHeaderWidgetClass;

	/** Number of columns in grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 GridColumns = 10;

	/** Padding between inventory slots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FMargin SlotPadding = FMargin(2.0f);

	/** Size for each slot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (ClampMin = "32.0"))
	float SlotSize = 64.0f;

public:
	/** Array of slot widgets */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<TObjectPtr<UInventorySlotWidget>> SlotWidgets;

protected:

	/** Current search filter */
	UPROPERTY()
	FString CurrentFilter;

	/** Cached size of the grid container for detecting resize */
	FVector2D CachedGridSize;

	/** Current number of visible columns in the grid */
	int32 CurrentVisibleColumns;

	/** Current number of visible rows in the grid */
	int32 CurrentVisibleRows;

	/** Current total number of visible slots */
	int32 CurrentVisibleSlots;

	/** Maximum columns to pre-create (prevents recreation on resize) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxPreCreatedColumns = 20;

	/** Maximum rows to pre-create (prevents recreation on resize) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxPreCreatedRows = 20;

	/** Whether we're waiting for valid geometry to create slots */
	bool bWaitingForGeometry;

	/** Track which slots were occupied before the last update (for detecting new items) */
	TSet<int32> PreviouslyOccupiedSlots;

	/** Input processor for detecting clicks outside inventory */
	TSharedPtr<class IInputProcessor> InputProcessor;

	/** Whether click capture is currently active */
	bool bIsClickCaptureActive = false;

	/** Timer handle for delayed click detection setup */
	FTimerHandle SetupTimerHandle;

	/** Timer handle for debounced reflow after resize */
	FTimerHandle ReflowDebounceTimerHandle;

	/** Pending column count for debounced reflow */
	int32 PendingColumnCount;

	/** Current view mode (true = list view, false = grid view) */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	bool bIsListView = false;

	/** Shared column settings for list view */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryColumnSettings> ColumnSettings;

	/** Cached row widgets for table view */
	UPROPERTY()
	TArray<TObjectPtr<UInventoryListRowWidget>> TableViewRows;

	/** Setup click capture for deselecting on outside clicks */
	void SetupClickCapture();

	/** Execute the debounced reflow */
	void ExecuteDebouncedReflow();

public:
	/** Initialize widget with inventory component */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventory(UInventoryComponent* InInventoryComponent);

	/** Refresh all inventory slots */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RefreshInventory();

	/** Refresh specific slot */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RefreshSlot(int32 SlotIndex);

	/** Update capacity display */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateCapacityDisplay();

	/** Close inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventory();

	/** Apply clipping settings to prevent slot compression and enable 2D scrolling */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ApplyClippingSettings();

	/** Enable horizontal scrolling in the ScrollBox */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EnableHorizontalScrolling();

	/** Update scrollbar visibility based on items */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateScrollbarVisibility();

	/** Compress items to fit within visible area by moving them to earliest available slots */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CompressItems();

	/** Scroll to make a specific slot visible */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ScrollToSlot(int32 TargetSlotIndex);

	/** Get the current visible column count */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetCurrentVisibleColumns() const { return (CurrentVisibleColumns > 0) ? CurrentVisibleColumns : GridColumns; }

	/** Reflow items to fit the current grid column count */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ReflowItemsToGrid();

	/** Toggle between grid view and list view */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleViewMode();

	/** Switch to a specific view mode */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetViewMode(bool bListView);

	/** Populate the list view with items (legacy ListView) */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PopulateListView();

	/** Populate the table-style list view with items (ScrollBox version) */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PopulateTableView();

	/** Synchronize selections from grid view to list view */
	void SyncSelectionsGridToList();

	/** Synchronize selections from list view to grid view */
	void SyncSelectionsListToGrid();

	/** Blueprint-callable function to handle context menu actions from empty area */
	virtual void HandleContextMenuAction_Implementation(FName ActionID) override;

	/** Delegate called when inventory is closed */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryClosed);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryClosed OnInventoryClosed;

protected:
	/** Called when inventory is updated */
	UFUNCTION()
	void OnInventoryUpdated(int32 SlotIndex, const FInventoryItem& Item);

	/** Called when capacity changes */
	UFUNCTION()
	void OnCapacityChanged(int32 NewCapacity);

	/** Close button clicked */
	UFUNCTION()
	void OnCloseButtonClicked();

	/** Sort by name clicked */
	UFUNCTION()
	void OnSortByNameClicked();

	/** Sort by rarity clicked */
	UFUNCTION()
	void OnSortByRarityClicked();

	/** Search text changed */
	UFUNCTION()
	void OnSearchTextChanged(const FText& Text);

	/** Grid view button clicked */
	UFUNCTION()
	void OnGridViewButtonClicked();

	/** List view button clicked */
	UFUNCTION()
	void OnListViewButtonClicked();

	/** Column header clicked */
	UFUNCTION()
	void OnColumnHeaderClicked(FName ColumnName);

	/** Blueprint event called when right-clicked on empty area in list view - implement empty context menu in Blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnEmptyAreaRightClicked(FVector2D MousePosition);

	/** Create slot widgets */
	void CreateSlotWidgets();

	/** Recalculate grid layout based on available space */
	void RecalculateGridLayout(const FVector2D& AvailableSize);

	/** Update slot visibility based on current grid layout */
	void UpdateSlotVisibility();

	/** Calculate how many columns can fit in the given width */
	int32 CalculateColumnsFromWidth(float AvailableWidth) const;

	/** Check if item passes filter */
	bool PassesFilter(const FInventoryItem& Item) const;

	/** Handle stack all (from empty area) action */
	void HandleStackAllEmpty();

	/** Handle select all action */
	void HandleSelectAll();

	/** Handle invert selection action */
	void HandleInvertSelection();
};
