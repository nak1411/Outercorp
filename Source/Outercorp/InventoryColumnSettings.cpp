// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryColumnSettings.h"

UInventoryColumnSettings::UInventoryColumnSettings()
{
	// Default values set in header
}

void UInventoryColumnSettings::SetColumnWidth(FName ColumnName, float NewWidth)
{
	// Clamp to minimum width
	NewWidth = FMath::Max(NewWidth, MinColumnWidth);

	if (ColumnName == TEXT("Icon"))
	{
		IconColumnWidth = NewWidth;
	}
	else if (ColumnName == TEXT("Name"))
	{
		NameColumnWidth = NewWidth;
	}
	else if (ColumnName == TEXT("Quantity"))
	{
		QuantityColumnWidth = NewWidth;
	}
	else if (ColumnName == TEXT("Weight"))
	{
		WeightColumnWidth = NewWidth;
	}
	else if (ColumnName == TEXT("Value"))
	{
		ValueColumnWidth = NewWidth;
	}
	else if (ColumnName == TEXT("Rarity"))
	{
		RarityColumnWidth = NewWidth;
	}
	else if (ColumnName == TEXT("Category"))
	{
		CategoryColumnWidth = NewWidth;
	}

	// Broadcast change
	OnColumnWidthsChanged.Broadcast();
}

float UInventoryColumnSettings::GetColumnWidth(FName ColumnName) const
{
	if (ColumnName == TEXT("Icon"))
	{
		return IconColumnWidth;
	}
	else if (ColumnName == TEXT("Name"))
	{
		return NameColumnWidth;
	}
	else if (ColumnName == TEXT("Quantity"))
	{
		return QuantityColumnWidth;
	}
	else if (ColumnName == TEXT("Weight"))
	{
		return WeightColumnWidth;
	}
	else if (ColumnName == TEXT("Value"))
	{
		return ValueColumnWidth;
	}
	else if (ColumnName == TEXT("Rarity"))
	{
		return RarityColumnWidth;
	}
	else if (ColumnName == TEXT("Category"))
	{
		return CategoryColumnWidth;
	}

	return 100.0f; // Default fallback
}
