// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CraftingRecipeData.h"
#include "RecipeListRowWidget.generated.h"

class UImage;
class UTextBlock;
class UWorkSurfaceCraftingWidget;
class UCraftingRecipeData;

/**
 * Recipe list row widget for displaying individual recipes in the work surface crafting UI
 * Shows recipe icon, name, and craftability status with visual feedback
 * Implements IUserObjectListEntry for ListView compatibility
 */
UCLASS()
class OUTERCORP_API URecipeListRowWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	// ============================================================================
	// Widget Components (Set in Blueprint)
	// ============================================================================

	/** Recipe icon image */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Recipe Row")
	UImage* Icon;

	/** Recipe name text */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Recipe Row")
	UTextBlock* ItemNameText;

	// ============================================================================
	// Properties
	// ============================================================================

	/** The recipe data this row represents */
	UPROPERTY(BlueprintReadOnly, Category = "Recipe Row")
	UCraftingRecipeData* RecipeData;

	/** Reference to the parent work surface widget */
	UPROPERTY(BlueprintReadOnly, Category = "Recipe Row")
	UWorkSurfaceCraftingWidget* ParentWidget;

	/** Is this recipe currently selected? */
	UPROPERTY(BlueprintReadOnly, Category = "Recipe Row")
	bool bIsSelected = false;

	/** Can this recipe be crafted with current resources? */
	UPROPERTY(BlueprintReadOnly, Category = "Recipe Row")
	bool bIsCraftable = false;

	/** Is this row currently being hovered? */
	UPROPERTY(BlueprintReadOnly, Category = "Recipe Row")
	bool bIsHovered = false;

	// ============================================================================
	// Colors for Visual Feedback
	// ============================================================================

	/** Color tint when recipe is craftable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Row|Visual")
	FLinearColor CraftableColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green

	/** Color tint when recipe is not craftable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Row|Visual")
	FLinearColor NotCraftableColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red

	/** Color tint when row is selected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Row|Visual")
	FLinearColor SelectedColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow

	/** Color tint when row is hovered */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Row|Visual")
	FLinearColor HoverColor = FLinearColor(0.7f, 0.7f, 1.0f, 1.0f); // Light blue

	/** Normal color tint */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe Row|Visual")
	FLinearColor NormalColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); // White

	// ============================================================================
	// Setup and Initialization
	// ============================================================================

	/** Initialize the row with recipe data and parent widget */
	UFUNCTION(BlueprintCallable, Category = "Recipe Row")
	void SetupRow(UCraftingRecipeData* Recipe, UWorkSurfaceCraftingWidget* Parent);

	/** Refresh the row display (icon, name, craftability) */
	UFUNCTION(BlueprintCallable, Category = "Recipe Row")
	void RefreshRow();

	/** Update craftability status (called when inventories change) */
	UFUNCTION(BlueprintCallable, Category = "Recipe Row")
	void UpdateCraftability();

	/** Set the selected state of this row */
	UFUNCTION(BlueprintCallable, Category = "Recipe Row")
	void SetSelected(bool bSelected);

	// ============================================================================
	// Visual Feedback
	// ============================================================================

	/** Update visual state (color tint) based on current state */
	UFUNCTION(BlueprintCallable, Category = "Recipe Row")
	void UpdateVisualState();

	/** Get the current color tint based on state priority (Selected > Hovered > Craftable > Normal) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Recipe Row")
	FLinearColor GetCurrentColorTint() const;

	// ============================================================================
	// Blueprint Events
	// ============================================================================

	/** Called when row is clicked (implement in Blueprint to trigger selection) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Recipe Row")
	void OnRowClicked();

	/** Called when row selection state changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "Recipe Row")
	void OnSelectionChanged(bool bSelected);

	/** Called when craftability changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "Recipe Row")
	void OnCraftabilityChanged(bool bCraftable);

	// ============================================================================
	// IUserObjectListEntry Implementation
	// ============================================================================

	/** Called when this widget is assigned a list item object (for ListView) */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	/** Handle row click (calls parent widget's SelectRecipe) */
	void HandleRowClick();
};
