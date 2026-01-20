// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CraftingRecipeData.h"
#include "RecipeListEntry.generated.h"

/**
 * Data object for recipe list entries in ListView
 * Wraps UCraftingRecipeData for use with UListView widget
 */
UCLASS(BlueprintType)
class OUTERCORP_API URecipeListEntry : public UObject
{
	GENERATED_BODY()

public:
	/** The recipe data this entry represents */
	UPROPERTY(BlueprintReadOnly, Category = "Recipe List")
	UCraftingRecipeData* RecipeData;

	/** Initialize this entry with recipe data */
	UFUNCTION(BlueprintCallable, Category = "Recipe List")
	void Initialize(UCraftingRecipeData* InRecipeData)
	{
		RecipeData = InRecipeData;
	}
};
