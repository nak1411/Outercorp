// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CraftingRecipeData.h"
#include "CraftingRecipeDatabase.generated.h"

/**
 * Database of all crafting recipes in the game
 * Create one data asset and populate it with all available recipes
 */
UCLASS(BlueprintType)
class OUTERCORP_API UCraftingRecipeDatabase : public UDataAsset
{
	GENERATED_BODY()

public:
	/** All recipes available in the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipes")
	TArray<TObjectPtr<UCraftingRecipeData>> Recipes;

	/** Get all recipes in the database */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	TArray<UCraftingRecipeData*> GetAllRecipes() const;

	/** Get recipes by category */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	TArray<UCraftingRecipeData*> GetRecipesByCategory(ERecipeCategory Category) const;

	/** Get recipes by station type */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	TArray<UCraftingRecipeData*> GetRecipesByStation(EStationType StationType) const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("RecipeDatabase", GetFName());
	}
};
