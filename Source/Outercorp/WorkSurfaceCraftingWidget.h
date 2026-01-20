// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WindowContentInterface.h"
#include "CraftingRecipeData.h"
#include "FabricationData.h"
#include "WorkSurfaceCraftingWidget.generated.h"

class AFabricationBase;
class UInventoryComponent;
class UCraftingRecipeData;

/**
 * Crafting state for tracking current operation
 */
UENUM(BlueprintType)
enum class ECraftingState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Crafting UMETA(DisplayName = "Crafting"),
	Completed UMETA(DisplayName = "Completed"),
	Failed UMETA(DisplayName = "Failed")
};

/**
 * Work surface crafting widget for RDR2-style crafting interface
 * Handles recipe selection, validation, and crafting execution
 * Implements WindowContentInterface to work with the modular window system
 */
UCLASS()
class OUTERCORP_API UWorkSurfaceCraftingWidget : public UUserWidget, public IWindowContentInterface
{
	GENERATED_BODY()

public:
	// ============================================================================
	// Window Capabilities
	// ============================================================================

	/** Can the window be moved? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting|Window")
	bool bCanMove = true;

	/** Can the window be resized? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting|Window")
	bool bCanResize = true;

	/** Can the window be fullscreened? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting|Window")
	bool bCanFullscreen = false;

	// ============================================================================
	// References
	// ============================================================================

	/** Reference to the fabrication station */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	AFabricationBase* FabricationStation;

	/** Reference to the player's inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	UInventoryComponent* PlayerInventory;

	/** Reference to the toolbox inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	UInventoryComponent* ToolboxInventory;

	/** Reference to the material bin inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	UInventoryComponent* MaterialBinInventory;

	// ============================================================================
	// Current State
	// ============================================================================

	/** Currently selected recipe */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	UCraftingRecipeData* SelectedRecipe;

	/** Current crafting state */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	ECraftingState CraftingState;

	/** Current crafting progress (0.0 to 1.0) */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	float CraftingProgress;

	/** Current batch size for crafting */
	UPROPERTY(BlueprintReadWrite, Category = "Crafting")
	int32 BatchSize = 1;

	/** Timer handle for crafting */
	FTimerHandle CraftingTimerHandle;

	/** Work surface configuration */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	FWorkSurfaceConfig WorkSurfaceConfig;

	// ============================================================================
	// Recipe Management
	// ============================================================================

	/** All recipes in the game (loaded from data assets) */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	TArray<UCraftingRecipeData*> AllRecipes;

	/** Recipes the player has learned/unlocked (via training or blueprints) */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	TArray<UCraftingRecipeData*> LearnedRecipes;

	/** All available recipes (populated on setup) */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	TArray<UCraftingRecipeData*> AvailableRecipes;

	/** Filtered recipes based on current criteria */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	TArray<UCraftingRecipeData*> FilteredRecipes;

	/** Current recipe category filter */
	UPROPERTY(BlueprintReadWrite, Category = "Crafting")
	ERecipeCategory CategoryFilter = ERecipeCategory::Misc;

	/** Should we only show craftable recipes? */
	UPROPERTY(BlueprintReadWrite, Category = "Crafting")
	bool bShowOnlyCraftable = false;

	// ============================================================================
	// UI Components
	// ============================================================================

	/** ListView for recipe list (bind in Blueprint) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Crafting")
	class UListView* MaterialList;

	/** Crafting ingredient slots (bind in Blueprint or create dynamically) */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	TArray<class UCraftingSlotWidget*> IngredientSlots;

	/** Crafting tool slots (bind in Blueprint or create dynamically) */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting")
	TArray<class UCraftingSlotWidget*> ToolSlots;

	// ============================================================================
	// Setup and Initialization
	// ============================================================================

	/** Initialize the crafting widget with station and inventories */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void SetupCrafting(AFabricationBase* Station, const FWorkSurfaceConfig& Config);

	/** Load all available recipes (override in Blueprint to specify recipe sources) */
	UFUNCTION(BlueprintNativeEvent, Category = "Crafting")
	void LoadAvailableRecipes();
	virtual void LoadAvailableRecipes_Implementation();

	/** Refresh the recipe list based on filters */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void RefreshRecipeList();

	/** Populate the UI with recipe row widgets */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void PopulateRecipeListUI();

	/** Update craftability status for all recipe rows (call when inventory changes) */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void UpdateAllRecipeRows();

	/** Learn a new recipe (from training or blueprint) */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void LearnRecipe(UCraftingRecipeData* Recipe);

	/** Check if a recipe has been learned */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	bool IsRecipeLearned(UCraftingRecipeData* Recipe) const;

	/** Unlearn a recipe (for debugging) */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void UnlearnRecipe(UCraftingRecipeData* Recipe);

	/** Learn all available recipes (for debugging/testing) */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void LearnAllRecipes();

	// ============================================================================
	// Recipe Selection and Validation
	// ============================================================================

	/** Select a recipe to craft */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void SelectRecipe(UCraftingRecipeData* Recipe);

	/** Check if a recipe can be crafted (has ingredients, tools, and fits work surface) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	bool CanCraftRecipe(UCraftingRecipeData* Recipe, int32 Batches = 1) const;

	/** Check if recipe fits the work surface constraints */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	bool RecipeFitsWorkSurface(UCraftingRecipeData* Recipe) const;

	/** Check if player has required ingredients */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	bool HasRequiredIngredients(UCraftingRecipeData* Recipe, int32 Batches = 1) const;

	/** Check if toolbox has required tools */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	bool HasRequiredTools(UCraftingRecipeData* Recipe) const;

	/** Get missing ingredients for a recipe */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	TArray<FCraftingIngredient> GetMissingIngredients(UCraftingRecipeData* Recipe, int32 Batches = 1) const;

	/** Get missing tools for a recipe */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	TArray<UInventoryItemData*> GetMissingTools(UCraftingRecipeData* Recipe) const;

	/** Calculate maximum craftable batches based on available ingredients */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	int32 GetMaxCraftableBatches(UCraftingRecipeData* Recipe) const;

	// ============================================================================
	// Crafting Execution
	// ============================================================================

	/** Start crafting the selected recipe */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	bool StartCrafting();

	/** Cancel current crafting operation */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void CancelCrafting();

	/** Called each frame during crafting to update progress */
	UFUNCTION()
	void UpdateCraftingProgress();

	/** Called when crafting completes successfully */
	UFUNCTION()
	void CompleteCrafting();

	/** Consume ingredients and tools for crafting */
	UFUNCTION()
	bool ConsumeResources();

	/** Add crafted items to player inventory */
	UFUNCTION()
	void AddCraftedItems();

	// ============================================================================
	// Crafting Slot Management
	// ============================================================================

	/** Setup crafting slots for the selected recipe */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void SetupCraftingSlots();

	/** Clear all crafting slots */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void ClearCraftingSlots();

	/** Check if all required crafting slots are filled */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting")
	bool AreAllSlotsFilled() const;

	/** Register an ingredient slot (call this from Blueprint for each slot) */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void RegisterIngredientSlot(class UCraftingSlotWidget* CraftingSlot);

	/** Register a tool slot (call this from Blueprint for each slot) */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void RegisterToolSlot(class UCraftingSlotWidget* CraftingSlot);

	// ============================================================================
	// Filter Management
	// ============================================================================

	/** Set recipe category filter */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void SetCategoryFilter(ERecipeCategory Category);

	/** Toggle showing only craftable recipes */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void ToggleShowOnlyCraftable();

	/** Clear all filters */
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void ClearFilters();

	// ============================================================================
	// Blueprint Events
	// ============================================================================

	/** Called when a recipe is selected (implement in Blueprint to update UI) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnRecipeSelected(UCraftingRecipeData* Recipe);

	/** Called when recipe list is refreshed (implement in Blueprint to update UI) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnRecipeListRefreshed();

	/** Called when crafting starts */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnCraftingStarted();

	/** Called when crafting progress updates */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnCraftingProgressUpdated(float Progress);

	/** Called when crafting completes */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnCraftingCompleted(bool bSuccess);

	/** Called when crafting is cancelled */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting")
	void OnCraftingCancelled();

	// ============================================================================
	// IWindowContentInterface Implementation
	// ============================================================================

	/** Get window capabilities for the modular window system */
	virtual FWindowCapabilities GetWindowCapabilities_Implementation() const override;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	/** Elapsed time for current crafting operation */
	float CraftingElapsedTime;

	/** Total time required for current crafting operation */
	float CraftingTotalTime;

	/** Helper function to remove items from inventory by ItemData */
	bool RemoveItemFromInventory(UInventoryComponent* Inventory, UInventoryItemData* ItemData, int32 Quantity);
};
