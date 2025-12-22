// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryListHeaderWidget.generated.h"

class UTextBlock;
class UButton;
class UBorder;
class UInventoryColumnSettings;
class USizeBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColumnHeaderClicked, FName, ColumnName);

/**
 * Header widget for inventory list view
 * Displays column names and allows sorting
 */
UCLASS()
class OUTERCORP_API UInventoryListHeaderWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	/** Header button for Icon column */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> IconHeaderButton;

	/** Header button for Name column */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> NameHeaderButton;

	/** Header button for Quantity column */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> QuantityHeaderButton;

	/** Header button for Weight column */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> WeightHeaderButton;

	/** Header button for Value column */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> ValueHeaderButton;

	/** Header button for Rarity column */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> RarityHeaderButton;

	/** Header button for Category column */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> CategoryHeaderButton;

	/** Icon column spacer (can be Image, Spacer, or SizeBox) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UWidget> IconColumnSpacer;

	/** Name header text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> NameHeaderText;

	/** Quantity header text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityHeaderText;

	/** Weight header text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> WeightHeaderText;

	/** Value header text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueHeaderText;

	/** Rarity header text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> RarityHeaderText;

	/** Category header text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CategoryHeaderText;

	/** Sort arrow text blocks (optional - overlaid on headers) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> NameSortArrow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> QuantitySortArrow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> WeightSortArrow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ValueSortArrow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RaritySortArrow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CategorySortArrow;

	/** Background border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> BackgroundBorder;

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

public:
	/** Current sort column */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FName CurrentSortColumn;

	/** Whether sort is ascending */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	bool bSortAscending = true;

	/** Shared column settings */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UInventoryColumnSettings> ColumnSettings;

	/** Event fired when a column header is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnColumnHeaderClicked OnColumnHeaderClicked;

	/** Set the active sort column and direction */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSortColumn(FName ColumnName, bool bAscending);

	/** Set the column settings and bind to updates */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetColumnSettings(UInventoryColumnSettings* InColumnSettings);

	/** Update column widths from settings */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateColumnWidths();

protected:
	/** Header button clicked handlers */
	UFUNCTION()
	void OnIconHeaderClicked();

	UFUNCTION()
	void OnNameHeaderClicked();

	UFUNCTION()
	void OnQuantityHeaderClicked();

	UFUNCTION()
	void OnWeightHeaderClicked();

	UFUNCTION()
	void OnValueHeaderClicked();

	UFUNCTION()
	void OnRarityHeaderClicked();

	UFUNCTION()
	void OnCategoryHeaderClicked();

	/** Update sort indicators on header texts */
	void UpdateSortIndicators();

	/** Called when column widths change */
	UFUNCTION()
	void OnColumnWidthsChanged();
};
