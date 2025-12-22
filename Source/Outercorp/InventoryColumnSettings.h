// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryColumnSettings.generated.h"

/**
 * Shared column width settings for inventory list view
 * This allows header and rows to stay in sync when columns are resized
 */
UCLASS(BlueprintType)
class OUTERCORP_API UInventoryColumnSettings : public UObject
{
	GENERATED_BODY()

public:
	UInventoryColumnSettings();

	/** Column widths (in pixels) */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	float IconColumnWidth = 32.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	float NameColumnWidth = 200.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	float QuantityColumnWidth = 60.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	float WeightColumnWidth = 80.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	float ValueColumnWidth = 80.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	float RarityColumnWidth = 100.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	float CategoryColumnWidth = 120.0f;

	/** Minimum column width to prevent columns from becoming too small */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	float MinColumnWidth = 40.0f;

	/** Delegate called when column widths change */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnColumnWidthsChanged);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnColumnWidthsChanged OnColumnWidthsChanged;

	/** Set a column width and broadcast change */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetColumnWidth(FName ColumnName, float NewWidth);

	/** Get a column width by name */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	float GetColumnWidth(FName ColumnName) const;
};
