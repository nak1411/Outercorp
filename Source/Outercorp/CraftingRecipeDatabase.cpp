// Copyright Epic Games, Inc. All Rights Reserved.

#include "CraftingRecipeDatabase.h"

TArray<UCraftingRecipeData*> UCraftingRecipeDatabase::GetAllRecipes() const
{
	TArray<UCraftingRecipeData*> Result;
	for (const TObjectPtr<UCraftingRecipeData>& Recipe : Recipes)
	{
		if (Recipe)
		{
			Result.Add(Recipe);
		}
	}
	return Result;
}

TArray<UCraftingRecipeData*> UCraftingRecipeDatabase::GetRecipesByCategory(ERecipeCategory Category) const
{
	TArray<UCraftingRecipeData*> Result;
	for (const TObjectPtr<UCraftingRecipeData>& Recipe : Recipes)
	{
		if (Recipe && Recipe->Category == Category)
		{
			Result.Add(Recipe);
		}
	}
	return Result;
}

TArray<UCraftingRecipeData*> UCraftingRecipeDatabase::GetRecipesByStation(EStationType StationType) const
{
	TArray<UCraftingRecipeData*> Result;
	for (const TObjectPtr<UCraftingRecipeData>& Recipe : Recipes)
	{
		if (Recipe && Recipe->CanCraftAtStation(StationType))
		{
			Result.Add(Recipe);
		}
	}
	return Result;
}
