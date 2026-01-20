// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemData.h"
#include "CraftingRecipeData.generated.h"

/**
 * Recipe category for organizing and filtering crafting recipes
 */
UENUM(BlueprintType)
enum class ERecipeCategory : uint8
{
	BasicTools UMETA(DisplayName = "Basic Tools"),
	AdvancedTools UMETA(DisplayName = "Advanced Tools"),
	Weapons UMETA(DisplayName = "Weapons"),
	Armor UMETA(DisplayName = "Armor"),
	Ammunition UMETA(DisplayName = "Ammunition"),
	Electronics UMETA(DisplayName = "Electronics"),
	Construction UMETA(DisplayName = "Construction"),
	Consumables UMETA(DisplayName = "Consumables"),
	Components UMETA(DisplayName = "Components"),
	Misc UMETA(DisplayName = "Miscellaneous")
};

/**
 * Fabrication station types for recipe requirements
 */
UENUM(BlueprintType)
enum class EStationType : uint8
{
	Workbench UMETA(DisplayName = "Workbench"),
	Forge UMETA(DisplayName = "Forge"),
	Anvil UMETA(DisplayName = "Anvil"),
	ChemistryStation UMETA(DisplayName = "Chemistry Station"),
	ElectronicsWorkbench UMETA(DisplayName = "Electronics Workbench"),
	GunsmithTable UMETA(DisplayName = "Gunsmith Table"),
	MachineLathe UMETA(DisplayName = "Machine Lathe"),
	AssemblyStation UMETA(DisplayName = "Assembly Station"),
	CookingStation UMETA(DisplayName = "Cooking Station"),
	Smelter UMETA(DisplayName = "Smelter")
};

/**
 * Ingredient for a crafting recipe
 */
USTRUCT(BlueprintType)
struct FCraftingIngredient
{
	GENERATED_BODY()

	/** Item required for this recipe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ingredient")
	TObjectPtr<UInventoryItemData> Item;

	/** Quantity required */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ingredient")
	int32 Quantity = 1;

	FCraftingIngredient()
		: Item(nullptr), Quantity(1)
	{}

	FCraftingIngredient(UInventoryItemData* InItem, int32 InQuantity)
		: Item(InItem), Quantity(InQuantity)
	{}

	bool IsValid() const
	{
		return Item != nullptr && Quantity > 0;
	}
};

/**
 * Data asset defining a crafting recipe
 */
UCLASS(BlueprintType)
class OUTERCORP_API UCraftingRecipeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ============================================================================
	// Recipe Info
	// ============================================================================

	/** Unique identifier for this recipe */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	FName RecipeID;

	/** Display name of the recipe */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	FText RecipeName;

	/** Description of what this recipe produces */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe", meta = (MultiLine = true))
	FText Description;

	/** Category for filtering and organizing recipes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	ERecipeCategory Category = ERecipeCategory::Misc;

	/** Icon to display in UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	UTexture2D* RecipeIcon;

	// ============================================================================
	// Requirements
	// ============================================================================

	/** Items required to craft this recipe */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements")
	TArray<FCraftingIngredient> Ingredients;

	/** Tools required (as item data assets) - checks if player has tool in toolbox inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements")
	TArray<TObjectPtr<UInventoryItemData>> RequiredTools;

	/** Fabrication stations that can craft this recipe (empty = can be crafted at any station) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements")
	TArray<EStationType> AllowedStations;

	/** Minimum skill level required (0 = no requirement) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Requirements")
	int32 RequiredSkillLevel = 0;

	// ============================================================================
	// Output
	// ============================================================================

	/** Item produced by this recipe */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Output")
	TObjectPtr<UInventoryItemData> OutputItem;

	/** Quantity produced */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Output")
	int32 OutputQuantity = 1;

	// ============================================================================
	// Crafting Properties
	// ============================================================================

	/** Time in seconds to craft this recipe */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Crafting")
	float CraftingTime = 1.0f;

	/** Can this recipe be crafted multiple times in one go? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Crafting")
	bool bAllowBatchCrafting = true;

	/** Maximum batch size if batch crafting is allowed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Crafting", meta = (EditCondition = "bAllowBatchCrafting"))
	int32 MaxBatchSize = 10;

	/** Does this recipe consume the required tools? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Crafting")
	bool bConsumeTools = false;

	// ============================================================================
	// Unlock/Discovery
	// ============================================================================

	/** Is this recipe unlocked by default? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Unlock")
	bool bUnlockedByDefault = true;

	/** Items that must be discovered/collected to unlock this recipe */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Unlock")
	TArray<TObjectPtr<UInventoryItemData>> UnlockRequiredItems;

	// ============================================================================
	// Functions
	// ============================================================================

	/** Check if player has all required ingredients in their inventory */
	UFUNCTION(BlueprintPure, Category = "Crafting Recipe")
	bool HasRequiredIngredients(class UInventoryComponent* Inventory, int32 BatchSize = 1) const;

	/** Check if player has all required tools in their inventory */
	UFUNCTION(BlueprintPure, Category = "Crafting Recipe")
	bool HasRequiredTools(class UInventoryComponent* Inventory) const;

	/** Check if this recipe can be crafted at the given station */
	UFUNCTION(BlueprintPure, Category = "Crafting Recipe")
	bool CanCraftAtStation(EStationType StationType) const;

	/** Get total crafting time including station modifiers */
	UFUNCTION(BlueprintPure, Category = "Crafting Recipe")
	float GetTotalCraftingTime(float StationSpeedMultiplier = 1.0f) const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("CraftingRecipe", GetFName());
	}
};
