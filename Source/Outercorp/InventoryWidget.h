// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "InventoryWidget.generated.h"

class UInventorySlotWidget;
class UUniformGridPanel;
class UScrollBox;
class USizeBox;
class UTextBlock;
class UProgressBar;
class UButton;
class UEditableText;

/**
 * Main inventory window widget (Eve Online style)
 */
UCLASS()
class OUTERCORP_API UInventoryWidget : public UUserWidget
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

	/** Class for inventory slot widgets */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

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

	/** Input processor for detecting clicks outside inventory */
	TSharedPtr<class IInputProcessor> InputProcessor;

	/** Whether click capture is currently active */
	bool bIsClickCaptureActive = false;

	/** Timer handle for delayed click detection setup */
	FTimerHandle SetupTimerHandle;

	/** Setup click capture for deselecting on outside clicks */
	void SetupClickCapture();

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

	/** Create slot widgets */
	void CreateSlotWidgets();

	/** Recalculate grid layout based on available space */
	void RecalculateGridLayout(const FVector2D& AvailableSize);

	/** Update slot visibility based on current grid layout */
	void UpdateSlotVisibility();

	/** Check if item passes filter */
	bool PassesFilter(const FInventoryItem& Item) const;
};
