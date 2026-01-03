// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingRecipeData.h"
#include "InventoryComponent.h"

bool UCraftingRecipeData::HasRequiredIngredients(UInventoryComponent* Inventory, int32 BatchSize) const
{
	if (!Inventory)
	{
		return false;
	}

	// Check each ingredient
	for (const FCraftingIngredient& Ingredient : Ingredients)
	{
		if (!Ingredient.IsValid())
		{
			continue;
		}

		// Calculate total quantity needed for this batch
		int32 QuantityNeeded = Ingredient.Quantity * BatchSize;

		// Check if inventory has enough of this item
		if (!Inventory->HasItem(Ingredient.Item, QuantityNeeded))
		{
			return false;
		}
	}

	return true;
}

bool UCraftingRecipeData::HasRequiredTools(UInventoryComponent* Inventory) const
{
	if (!Inventory)
	{
		return false;
	}

	// Check each required tool
	for (const FName& ToolName : RequiredTools)
	{
		// TODO: Implement tool checking once we have a tool system
		// For now, assume tools are available if inventory exists
	}

	return true;
}

bool UCraftingRecipeData::CanCraftAtStation(FName StationType) const
{
	// If no stations specified, can be crafted anywhere
	if (AllowedStations.Num() == 0)
	{
		return true;
	}

	// Check if station type is in allowed list
	return AllowedStations.Contains(StationType);
}

float UCraftingRecipeData::GetTotalCraftingTime(float StationSpeedMultiplier) const
{
	return CraftingTime / FMath::Max(0.1f, StationSpeedMultiplier);
}
