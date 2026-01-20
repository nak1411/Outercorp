// Copyright Epic Games, Inc. All Rights Reserved.

#include "RecipeListRowWidget.h"
#include "WorkSurfaceCraftingWidget.h"
#include "RecipeListEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/ListView.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void URecipeListRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initial refresh if recipe data is already set
	if (RecipeData)
	{
		RefreshRow();
	}
}

void URecipeListRowWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Called by ListView when this widget is assigned a data object
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	URecipeListEntry* Entry = Cast<URecipeListEntry>(ListItemObject);
	if (Entry && Entry->RecipeData)
	{
		RecipeData = Entry->RecipeData;

		// Get the parent widget from the owning ListView
		if (!ParentWidget)
		{
			UListViewBase* OwnerListView = GetOwningListView();
			if (OwnerListView)
			{
				// Traverse up to find the WorkSurfaceCraftingWidget
				// Start from the ListView (which is a UWidget)
				UWidget* CurrentWidget = Cast<UWidget>(OwnerListView);
				while (CurrentWidget)
				{
					ParentWidget = Cast<UWorkSurfaceCraftingWidget>(CurrentWidget);
					if (ParentWidget)
					{
						break;
					}

					// Get parent widget
					UWidget* ParentOfCurrent = CurrentWidget->GetParent();
					if (ParentOfCurrent)
					{
						CurrentWidget = ParentOfCurrent;
					}
					else
					{
						// No more parents to traverse
						break;
					}
				}
			}
		}

		RefreshRow();
	}
}

FReply URecipeListRowWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		HandleRowClick();
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void URecipeListRowWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bIsHovered = true;
	UpdateVisualState();
}

void URecipeListRowWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsHovered = false;
	UpdateVisualState();
}

void URecipeListRowWidget::SetupRow(UCraftingRecipeData* Recipe, UWorkSurfaceCraftingWidget* Parent)
{
	if (!Recipe || !Parent)
	{
		UE_LOG(LogTemp, Warning, TEXT("RecipeListRowWidget::SetupRow - Invalid Recipe or Parent widget"));
		return;
	}

	RecipeData = Recipe;
	ParentWidget = Parent;

	RefreshRow();
}

void URecipeListRowWidget::RefreshRow()
{
	if (!RecipeData)
	{
		UE_LOG(LogTemp, Warning, TEXT("RecipeListRowWidget::RefreshRow - No recipe data"));
		return;
	}

	// Set recipe name
	if (ItemNameText)
	{
		ItemNameText->SetText(RecipeData->RecipeName);
	}

	// Set recipe icon
	if (Icon && RecipeData->RecipeIcon)
	{
		Icon->SetBrushFromTexture(RecipeData->RecipeIcon);
	}

	// Update craftability
	UpdateCraftability();
}

void URecipeListRowWidget::UpdateCraftability()
{
	if (!ParentWidget || !RecipeData)
	{
		bIsCraftable = false;
		bIsSelected = false;
		UpdateVisualState();
		return;
	}

	bool bPreviousCraftable = bIsCraftable;
	bIsCraftable = ParentWidget->CanCraftRecipe(RecipeData, 1);

	// Check if this recipe is currently selected
	bool bWasSelected = bIsSelected;
	bIsSelected = (ParentWidget->SelectedRecipe == RecipeData);

	// Trigger events if state changed
	if (bPreviousCraftable != bIsCraftable)
	{
		OnCraftabilityChanged(bIsCraftable);
	}

	if (bWasSelected != bIsSelected)
	{
		OnSelectionChanged(bIsSelected);
	}

	UpdateVisualState();
}

void URecipeListRowWidget::SetSelected(bool bSelected)
{
	if (bIsSelected == bSelected)
	{
		return;
	}

	bIsSelected = bSelected;
	UpdateVisualState();

	// Trigger Blueprint event
	OnSelectionChanged(bSelected);
}

void URecipeListRowWidget::UpdateVisualState()
{
	FLinearColor NewColor = GetCurrentColorTint();

	// Apply color tint to icon
	if (Icon)
	{
		Icon->SetColorAndOpacity(NewColor);
	}

	// Apply color tint to text
	if (ItemNameText)
	{
		ItemNameText->SetColorAndOpacity(FSlateColor(NewColor));
	}
}

FLinearColor URecipeListRowWidget::GetCurrentColorTint() const
{
	// Priority order: Selected > Hovered > Craftable > Normal
	if (bIsSelected)
	{
		return SelectedColor;
	}

	if (bIsHovered)
	{
		return HoverColor;
	}

	if (bIsCraftable)
	{
		return CraftableColor;
	}

	return NotCraftableColor;
}

void URecipeListRowWidget::HandleRowClick()
{
	if (!ParentWidget || !RecipeData)
	{
		UE_LOG(LogTemp, Warning, TEXT("RecipeListRowWidget::HandleRowClick - Invalid parent or recipe data"));
		return;
	}

	// Notify parent widget to select this recipe
	ParentWidget->SelectRecipe(RecipeData);

	// Trigger Blueprint event
	OnRowClicked();
}
