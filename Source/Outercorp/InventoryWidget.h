// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "InventoryWidget.generated.h"

class UInventorySlotWidget;
class UUniformGridPanel;
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
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual bool NativeSupportsKeyboardFocus() const override { return true; }

	/** Reference to the inventory component */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Grid panel containing item slots */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemGrid;

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
	int32 GridColumns = 6;

	/** Array of slot widgets */
	UPROPERTY()
	TArray<TObjectPtr<UInventorySlotWidget>> SlotWidgets;

	/** Current search filter */
	UPROPERTY()
	FString CurrentFilter;

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

	/** Check if item passes filter */
	bool PassesFilter(const FInventoryItem& Item) const;

private:
	/** Timer to delay focus reclaim to avoid interfering with button clicks */
	float FocusReclaimTimer = 0.0f;
};
