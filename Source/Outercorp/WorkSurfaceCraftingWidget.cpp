// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorkSurfaceCraftingWidget.h"
#include "FabricationBase.h"
#include "FabricationData.h"
#include "CraftingRecipeDatabase.h"
#include "CraftingSlotWidget.h"
#include "InventoryComponent.h"
#include "InventoryItemData.h"
#include "RecipeListEntry.h"
#include "Components/ListView.h"
#include "TimerManager.h"

void UWorkSurfaceCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize state
	CraftingState = ECraftingState::Idle;
	CraftingProgress = 0.0f;
	CraftingElapsedTime = 0.0f;
	CraftingTotalTime = 0.0f;
	BatchSize = 1;
}

bool UWorkSurfaceCraftingWidget::RemoveItemFromInventory(UInventoryComponent* Inventory, UInventoryItemData* ItemData, int32 Quantity)
{
	if (!Inventory || !ItemData || Quantity <= 0)
	{
		return false;
	}

	int32 RemainingToRemove = Quantity;
	TArray<FInventoryItem> AllItems = Inventory->GetAllItems();

	// Find all slots containing this item and remove from them
	for (int32 i = 0; i < AllItems.Num() && RemainingToRemove > 0; i++)
	{
		const FInventoryItem& Item = AllItems[i];
		if (Item.IsValid() && Item.ItemData == ItemData)
		{
			int32 ToRemove = FMath::Min(Item.Quantity, RemainingToRemove);
			if (Inventory->RemoveItemAtSlot(i, ToRemove))
			{
				RemainingToRemove -= ToRemove;
			}
		}
	}

	return RemainingToRemove == 0;
}

void UWorkSurfaceCraftingWidget::NativeDestruct()
{
	// Clean up timer if widget is destroyed during crafting
	if (CraftingTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CraftingTimerHandle);
	}

	Super::NativeDestruct();
}

void UWorkSurfaceCraftingWidget::SetupCrafting(AFabricationBase* Station, const FWorkSurfaceConfig& Config)
{
	if (!Station)
	{
		UE_LOG(LogTemp, Error, TEXT("WorkSurfaceCraftingWidget::SetupCrafting - Station is null!"));
		return;
	}

	FabricationStation = Station;
	WorkSurfaceConfig = Config;

	// Get inventory references from the station
	PlayerInventory = Station->GetPlayerInventory();
	ToolboxInventory = Station->GetToolboxInventory();
	MaterialBinInventory = Station->GetMaterialBinInventory();

	UE_LOG(LogTemp, Log, TEXT("WorkSurfaceCraftingWidget::SetupCrafting - Station: %s, SurfaceSize: %s"),
		*Station->GetName(),
		*UEnum::GetValueAsString(Config.SurfaceSize));

	// Load available recipes
	LoadAvailableRecipes();

	// Initial recipe list refresh
	RefreshRecipeList();
}

void UWorkSurfaceCraftingWidget::LoadAvailableRecipes_Implementation()
{
	AvailableRecipes.Empty();

	if (!FabricationStation)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadAvailableRecipes - No fabrication station set"));
		return;
	}

	UFabricationData* StationData = FabricationStation->GetStationData();
	if (!StationData)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadAvailableRecipes - Station has no data asset"));
		return;
	}

	// Load from recipe database if available
	if (StationData->RecipeDatabase)
	{
		TArray<UCraftingRecipeData*> DatabaseRecipes = StationData->RecipeDatabase->GetAllRecipes();
		for (UCraftingRecipeData* Recipe : DatabaseRecipes)
		{
			if (Recipe && !AvailableRecipes.Contains(Recipe))
			{
				AvailableRecipes.Add(Recipe);
			}
		}
		UE_LOG(LogTemp, Log, TEXT("LoadAvailableRecipes - Loaded %d recipes from database"), DatabaseRecipes.Num());
	}

	// Load default recipes from station data
	for (const TObjectPtr<UCraftingRecipeData>& Recipe : StationData->DefaultRecipes)
	{
		if (Recipe && !AvailableRecipes.Contains(Recipe))
		{
			AvailableRecipes.Add(Recipe);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("LoadAvailableRecipes - Total available recipes: %d"), AvailableRecipes.Num());
}

void UWorkSurfaceCraftingWidget::RefreshRecipeList()
{
	FilteredRecipes.Empty();

	if (!FabricationStation)
	{
		return;
	}

	// Only show learned recipes
	for (UCraftingRecipeData* Recipe : LearnedRecipes)
	{
		if (!Recipe)
		{
			continue;
		}

		// Apply category filter (skip if filter is Misc, which means "all")
		if (CategoryFilter != ERecipeCategory::Misc && Recipe->Category != CategoryFilter)
		{
			continue;
		}

		// Check if recipe can be crafted at this station
		if (!Recipe->CanCraftAtStation(FabricationStation->GetStationType()))
		{
			continue;
		}

		// Check if recipe fits work surface
		if (!RecipeFitsWorkSurface(Recipe))
		{
			continue;
		}

		// Apply "show only craftable" filter
		if (bShowOnlyCraftable && !CanCraftRecipe(Recipe, 1))
		{
			continue;
		}

		FilteredRecipes.Add(Recipe);
	}

	UE_LOG(LogTemp, Log, TEXT("RefreshRecipeList - Filtered %d recipes from %d learned recipes"),
		FilteredRecipes.Num(), LearnedRecipes.Num());

	// Notify Blueprint
	OnRecipeListRefreshed();

	// Populate the UI with recipe rows
	PopulateRecipeListUI();
}

void UWorkSurfaceCraftingWidget::LearnRecipe(UCraftingRecipeData* Recipe)
{
	if (!Recipe)
	{
		UE_LOG(LogTemp, Warning, TEXT("LearnRecipe - Invalid recipe"));
		return;
	}

	if (LearnedRecipes.Contains(Recipe))
	{
		UE_LOG(LogTemp, Warning, TEXT("LearnRecipe - Recipe '%s' already learned"), *Recipe->RecipeName.ToString());
		return;
	}

	LearnedRecipes.Add(Recipe);
	UE_LOG(LogTemp, Log, TEXT("LearnRecipe - Learned recipe: %s"), *Recipe->RecipeName.ToString());

	// Refresh the list to show the new recipe
	RefreshRecipeList();
}

bool UWorkSurfaceCraftingWidget::IsRecipeLearned(UCraftingRecipeData* Recipe) const
{
	return LearnedRecipes.Contains(Recipe);
}

void UWorkSurfaceCraftingWidget::UnlearnRecipe(UCraftingRecipeData* Recipe)
{
	if (!Recipe)
	{
		return;
	}

	if (LearnedRecipes.Remove(Recipe) > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("UnlearnRecipe - Unlearned recipe: %s"), *Recipe->RecipeName.ToString());

		// Deselect if this was the selected recipe
		if (SelectedRecipe == Recipe)
		{
			SelectRecipe(nullptr);
		}

		// Refresh the list
		RefreshRecipeList();
	}
}

void UWorkSurfaceCraftingWidget::LearnAllRecipes()
{
	LearnedRecipes.Empty();

	for (UCraftingRecipeData* Recipe : AvailableRecipes)
	{
		if (Recipe && !LearnedRecipes.Contains(Recipe))
		{
			LearnedRecipes.Add(Recipe);
			UE_LOG(LogTemp, Log, TEXT("LearnAllRecipes - Learned recipe: %s"), *Recipe->RecipeName.ToString());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("LearnAllRecipes - Learned %d recipes"), LearnedRecipes.Num());

	// Refresh the list to show the new recipes
	RefreshRecipeList();
}

void UWorkSurfaceCraftingWidget::PopulateRecipeListUI()
{
	if (!MaterialList)
	{
		UE_LOG(LogTemp, Warning, TEXT("PopulateRecipeListUI - MaterialList ListView not bound!"));
		return;
	}

	// Clear existing list items
	MaterialList->ClearListItems();

	// Create URecipeListEntry objects for each filtered recipe
	for (UCraftingRecipeData* Recipe : FilteredRecipes)
	{
		if (!Recipe)
		{
			continue;
		}

		// Create a data object for this recipe
		URecipeListEntry* Entry = NewObject<URecipeListEntry>(this);
		Entry->Initialize(Recipe);

		// Add to the ListView
		MaterialList->AddItem(Entry);
	}

	UE_LOG(LogTemp, Log, TEXT("PopulateRecipeListUI - Added %d recipe entries to ListView"), FilteredRecipes.Num());
}

void UWorkSurfaceCraftingWidget::UpdateAllRecipeRows()
{
	if (!MaterialList)
	{
		return;
	}

	// Get all list items and update their craftability
	TArray<UObject*> ListItems = MaterialList->GetListItems();
	for (UObject* Item : ListItems)
	{
		URecipeListEntry* Entry = Cast<URecipeListEntry>(Item);
		if (Entry && Entry->RecipeData)
		{
			// Regenerate the widget for this entry to trigger UpdateCraftability
			MaterialList->RegenerateAllEntries();
			return; // RegenerateAllEntries handles all items, so we can return after calling it once
		}
	}
}

void UWorkSurfaceCraftingWidget::SelectRecipe(UCraftingRecipeData* Recipe)
{
	SelectedRecipe = Recipe;

	if (Recipe)
	{
		UE_LOG(LogTemp, Log, TEXT("SelectRecipe - Selected: %s"), *Recipe->RecipeName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SelectRecipe - Deselected"));
	}

	// Update all recipe rows to reflect selection state
	if (MaterialList)
	{
		MaterialList->RegenerateAllEntries();
	}

	// Notify Blueprint to update the recipe details panel
	OnRecipeSelected(Recipe);

	// Setup crafting slots for the new recipe
	SetupCraftingSlots();
}

void UWorkSurfaceCraftingWidget::RegisterIngredientSlot(UCraftingSlotWidget* CraftingSlot)
{
	if (CraftingSlot && !IngredientSlots.Contains(CraftingSlot))
	{
		IngredientSlots.Add(CraftingSlot);
	}
}

void UWorkSurfaceCraftingWidget::RegisterToolSlot(UCraftingSlotWidget* CraftingSlot)
{
	if (CraftingSlot && !ToolSlots.Contains(CraftingSlot))
	{
		ToolSlots.Add(CraftingSlot);
	}
}

void UWorkSurfaceCraftingWidget::SetupCraftingSlots()
{
	// Clear all slots first
	ClearCraftingSlots();

	if (!SelectedRecipe)
	{
		UE_LOG(LogTemp, Log, TEXT("SetupCraftingSlots - No recipe selected, slots cleared"));
		return;
	}

	// Setup ingredient slots
	int32 IngredientIndex = 0;
	for (const FCraftingIngredient& Ingredient : SelectedRecipe->Ingredients)
	{
		if (IngredientIndex >= IngredientSlots.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("SetupCraftingSlots - Not enough ingredient slots! Need %d, have %d"),
				SelectedRecipe->Ingredients.Num(), IngredientSlots.Num());
			break;
		}

		if (IngredientSlots[IngredientIndex])
		{
			IngredientSlots[IngredientIndex]->SetRequiredItem(Ingredient.Item, Ingredient.Quantity);
		}

		IngredientIndex++;
	}

	// Setup tool slots
	int32 ToolIndex = 0;
	for (const TObjectPtr<UInventoryItemData>& Tool : SelectedRecipe->RequiredTools)
	{
		if (ToolIndex >= ToolSlots.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("SetupCraftingSlots - Not enough tool slots! Need %d, have %d"),
				SelectedRecipe->RequiredTools.Num(), ToolSlots.Num());
			break;
		}

		if (ToolSlots[ToolIndex])
		{
			ToolSlots[ToolIndex]->SetRequiredItem(Tool, 1);
		}

		ToolIndex++;
	}

	UE_LOG(LogTemp, Log, TEXT("SetupCraftingSlots - Setup %d ingredient slots and %d tool slots for recipe: %s"),
		IngredientIndex, ToolIndex, *SelectedRecipe->RecipeName.ToString());
}

void UWorkSurfaceCraftingWidget::ClearCraftingSlots()
{
	// Clear all ingredient slots
	for (UCraftingSlotWidget* CraftingSlot : IngredientSlots)
	{
		if (CraftingSlot)
		{
			CraftingSlot->ClearSlot();
		}
	}

	// Clear all tool slots
	for (UCraftingSlotWidget* CraftingSlot : ToolSlots)
	{
		if (CraftingSlot)
		{
			CraftingSlot->ClearSlot();
		}
	}
}

bool UWorkSurfaceCraftingWidget::AreAllSlotsFilled() const
{
	if (!SelectedRecipe)
	{
		return false;
	}

	// Check ingredient slots (only check slots that are required by the recipe)
	int32 RequiredIngredients = SelectedRecipe->Ingredients.Num();
	for (int32 i = 0; i < RequiredIngredients && i < IngredientSlots.Num(); i++)
	{
		if (!IngredientSlots[i] || !IngredientSlots[i]->IsSlotFilled())
		{
			return false;
		}
	}

	// Check tool slots (only check slots that are required by the recipe)
	int32 RequiredTools = SelectedRecipe->RequiredTools.Num();
	for (int32 i = 0; i < RequiredTools && i < ToolSlots.Num(); i++)
	{
		if (!ToolSlots[i] || !ToolSlots[i]->IsSlotFilled())
		{
			return false;
		}
	}

	return true;
}

bool UWorkSurfaceCraftingWidget::CanCraftRecipe(UCraftingRecipeData* Recipe, int32 Batches) const
{
	if (!Recipe || !FabricationStation)
	{
		return false;
	}

	// Check work surface constraints
	if (!RecipeFitsWorkSurface(Recipe))
	{
		return false;
	}

	// Check station type
	if (!Recipe->CanCraftAtStation(FabricationStation->GetStationType()))
	{
		return false;
	}

	// Check ingredients
	if (!HasRequiredIngredients(Recipe, Batches))
	{
		return false;
	}

	// Check tools
	if (!HasRequiredTools(Recipe))
	{
		return false;
	}

	return true;
}

bool UWorkSurfaceCraftingWidget::RecipeFitsWorkSurface(UCraftingRecipeData* Recipe) const
{
	if (!Recipe)
	{
		return false;
	}

	// Check ingredient slot count
	if (Recipe->Ingredients.Num() > WorkSurfaceConfig.MaxIngredientSlots)
	{
		return false;
	}

	// Check required tool count
	if (Recipe->RequiredTools.Num() > WorkSurfaceConfig.MaxRequiredTools)
	{
		return false;
	}

	return true;
}

bool UWorkSurfaceCraftingWidget::HasRequiredIngredients(UCraftingRecipeData* Recipe, int32 Batches) const
{
	if (!Recipe)
	{
		return false;
	}

	// Check material bin inventory first, then player inventory
	for (const FCraftingIngredient& Ingredient : Recipe->Ingredients)
	{
		if (!Ingredient.IsValid())
		{
			continue;
		}

		int32 QuantityNeeded = Ingredient.Quantity * Batches;
		int32 QuantityAvailable = 0;

		// Check material bin
		if (MaterialBinInventory)
		{
			QuantityAvailable += MaterialBinInventory->GetItemQuantity(Ingredient.Item);
		}

		// Check player inventory
		if (PlayerInventory)
		{
			QuantityAvailable += PlayerInventory->GetItemQuantity(Ingredient.Item);
		}

		if (QuantityAvailable < QuantityNeeded)
		{
			return false;
		}
	}

	return true;
}

bool UWorkSurfaceCraftingWidget::HasRequiredTools(UCraftingRecipeData* Recipe) const
{
	if (!Recipe || !ToolboxInventory)
	{
		return false;
	}

	// Check toolbox inventory for required tools
	return Recipe->HasRequiredTools(ToolboxInventory);
}

TArray<FCraftingIngredient> UWorkSurfaceCraftingWidget::GetMissingIngredients(UCraftingRecipeData* Recipe, int32 Batches) const
{
	TArray<FCraftingIngredient> MissingIngredients;

	if (!Recipe)
	{
		return MissingIngredients;
	}

	for (const FCraftingIngredient& Ingredient : Recipe->Ingredients)
	{
		if (!Ingredient.IsValid())
		{
			continue;
		}

		int32 QuantityNeeded = Ingredient.Quantity * Batches;
		int32 QuantityAvailable = 0;

		// Check material bin
		if (MaterialBinInventory)
		{
			QuantityAvailable += MaterialBinInventory->GetItemQuantity(Ingredient.Item);
		}

		// Check player inventory
		if (PlayerInventory)
		{
			QuantityAvailable += PlayerInventory->GetItemQuantity(Ingredient.Item);
		}

		if (QuantityAvailable < QuantityNeeded)
		{
			FCraftingIngredient MissingIngredient;
			MissingIngredient.Item = Ingredient.Item;
			MissingIngredient.Quantity = QuantityNeeded - QuantityAvailable;
			MissingIngredients.Add(MissingIngredient);
		}
	}

	return MissingIngredients;
}

TArray<UInventoryItemData*> UWorkSurfaceCraftingWidget::GetMissingTools(UCraftingRecipeData* Recipe) const
{
	TArray<UInventoryItemData*> MissingTools;

	if (!Recipe || !ToolboxInventory)
	{
		return MissingTools;
	}

	for (const TObjectPtr<UInventoryItemData>& Tool : Recipe->RequiredTools)
	{
		if (!Tool)
		{
			continue;
		}

		if (!ToolboxInventory->HasItem(Tool, 1))
		{
			MissingTools.Add(Tool);
		}
	}

	return MissingTools;
}

int32 UWorkSurfaceCraftingWidget::GetMaxCraftableBatches(UCraftingRecipeData* Recipe) const
{
	if (!Recipe || Recipe->Ingredients.Num() == 0)
	{
		return 0;
	}

	int32 MaxBatches = INT32_MAX;

	for (const FCraftingIngredient& Ingredient : Recipe->Ingredients)
	{
		if (!Ingredient.IsValid() || Ingredient.Quantity <= 0)
		{
			continue;
		}

		int32 QuantityAvailable = 0;

		// Check material bin
		if (MaterialBinInventory)
		{
			QuantityAvailable += MaterialBinInventory->GetItemQuantity(Ingredient.Item);
		}

		// Check player inventory
		if (PlayerInventory)
		{
			QuantityAvailable += PlayerInventory->GetItemQuantity(Ingredient.Item);
		}

		int32 BatchesForThisIngredient = QuantityAvailable / Ingredient.Quantity;
		MaxBatches = FMath::Min(MaxBatches, BatchesForThisIngredient);
	}

	// Clamp to recipe's max batch size
	if (Recipe->bAllowBatchCrafting)
	{
		MaxBatches = FMath::Min(MaxBatches, Recipe->MaxBatchSize);
	}
	else
	{
		MaxBatches = FMath::Min(MaxBatches, 1);
	}

	return FMath::Max(0, MaxBatches);
}

bool UWorkSurfaceCraftingWidget::StartCrafting()
{
	if (!SelectedRecipe || !FabricationStation)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartCrafting - No recipe selected or station invalid"));
		return false;
	}

	if (CraftingState == ECraftingState::Crafting)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartCrafting - Already crafting"));
		return false;
	}

	// Check if all crafting slots are filled
	if (!AreAllSlotsFilled())
	{
		UE_LOG(LogTemp, Warning, TEXT("StartCrafting - Not all crafting slots are filled"));
		CraftingState = ECraftingState::Failed;
		OnCraftingCompleted(false);
		return false;
	}

	// Validate can craft
	if (!CanCraftRecipe(SelectedRecipe, BatchSize))
	{
		UE_LOG(LogTemp, Warning, TEXT("StartCrafting - Cannot craft recipe '%s'"), *SelectedRecipe->RecipeName.ToString());
		CraftingState = ECraftingState::Failed;
		OnCraftingCompleted(false);
		return false;
	}

	// Consume resources
	if (!ConsumeResources())
	{
		UE_LOG(LogTemp, Error, TEXT("StartCrafting - Failed to consume resources"));
		CraftingState = ECraftingState::Failed;
		OnCraftingCompleted(false);
		return false;
	}

	// Initialize crafting
	CraftingState = ECraftingState::Crafting;
	CraftingProgress = 0.0f;
	CraftingElapsedTime = 0.0f;
	CraftingTotalTime = SelectedRecipe->GetTotalCraftingTime(FabricationStation->GetCraftingSpeedMultiplier());

	UE_LOG(LogTemp, Log, TEXT("StartCrafting - Started crafting '%s' (x%d) for %.2f seconds"),
		*SelectedRecipe->RecipeName.ToString(), BatchSize, CraftingTotalTime);

	// Start timer
	GetWorld()->GetTimerManager().SetTimer(
		CraftingTimerHandle,
		this,
		&UWorkSurfaceCraftingWidget::UpdateCraftingProgress,
		0.1f, // Update every 0.1 seconds
		true
	);

	// Notify Blueprint
	OnCraftingStarted();

	return true;
}

void UWorkSurfaceCraftingWidget::CancelCrafting()
{
	if (CraftingState != ECraftingState::Crafting)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("CancelCrafting - Cancelled crafting '%s'"),
		SelectedRecipe ? *SelectedRecipe->RecipeName.ToString() : TEXT("Unknown"));

	// Clear timer
	if (CraftingTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CraftingTimerHandle);
		CraftingTimerHandle.Invalidate();
	}

	// Reset state
	CraftingState = ECraftingState::Idle;
	CraftingProgress = 0.0f;
	CraftingElapsedTime = 0.0f;

	// Notify Blueprint
	OnCraftingCancelled();

	// TODO: Optionally refund resources (partial refund system)
}

void UWorkSurfaceCraftingWidget::UpdateCraftingProgress()
{
	if (CraftingState != ECraftingState::Crafting || CraftingTotalTime <= 0.0f)
	{
		return;
	}

	CraftingElapsedTime += 0.1f;
	CraftingProgress = FMath::Clamp(CraftingElapsedTime / CraftingTotalTime, 0.0f, 1.0f);

	// Notify Blueprint of progress update
	OnCraftingProgressUpdated(CraftingProgress);

	// Check if completed
	if (CraftingProgress >= 1.0f)
	{
		CompleteCrafting();
	}
}

void UWorkSurfaceCraftingWidget::CompleteCrafting()
{
	// Clear timer
	if (CraftingTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CraftingTimerHandle);
		CraftingTimerHandle.Invalidate();
	}

	// Add crafted items to player inventory
	AddCraftedItems();

	// Update state
	CraftingState = ECraftingState::Completed;
	CraftingProgress = 1.0f;

	UE_LOG(LogTemp, Log, TEXT("CompleteCrafting - Completed crafting '%s' (x%d)"),
		SelectedRecipe ? *SelectedRecipe->RecipeName.ToString() : TEXT("Unknown"),
		BatchSize);

	// Notify Blueprint
	OnCraftingCompleted(true);

	// Reset after a short delay
	FTimerHandle ResetTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		ResetTimerHandle,
		[this]()
		{
			CraftingState = ECraftingState::Idle;
			CraftingProgress = 0.0f;
			RefreshRecipeList(); // Refresh to update craftable status
		},
		1.0f,
		false
	);
}

bool UWorkSurfaceCraftingWidget::ConsumeResources()
{
	if (!SelectedRecipe)
	{
		UE_LOG(LogTemp, Error, TEXT("ConsumeResources - No recipe selected"));
		return false;
	}

	// Consume from ingredient slots
	for (int32 i = 0; i < IngredientSlots.Num(); i++)
	{
		UCraftingSlotWidget* CraftingSlot = IngredientSlots[i];
		if (!CraftingSlot || !CraftingSlot->IsSlotFilled())
		{
			continue;
		}

		FInventoryItem AssignedItem = CraftingSlot->GetAssignedItem();
		UInventoryComponent* SourceInv = CraftingSlot->SourceInventory;

		if (!SourceInv || !AssignedItem.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("ConsumeResources - Ingredient slot %d has no valid source inventory or item"), i);
			return false;
		}

		// Remove the required quantity from the source inventory
		int32 QuantityToRemove = CraftingSlot->RequiredQuantity * BatchSize;
		if (!RemoveItemFromInventory(SourceInv, AssignedItem.ItemData, QuantityToRemove))
		{
			UE_LOG(LogTemp, Error, TEXT("ConsumeResources - Failed to remove %d x '%s' from source inventory"),
				QuantityToRemove, *AssignedItem.ItemData->ItemName.ToString());
			return false;
		}

		UE_LOG(LogTemp, Log, TEXT("ConsumeResources - Consumed %d x '%s' from ingredient slot %d"),
			QuantityToRemove, *AssignedItem.ItemData->ItemName.ToString(), i);
	}

	// Consume from tool slots (if recipe consumes tools)
	if (SelectedRecipe->bConsumeTools)
	{
		for (int32 i = 0; i < ToolSlots.Num(); i++)
		{
			UCraftingSlotWidget* CraftingSlot = ToolSlots[i];
			if (!CraftingSlot || !CraftingSlot->IsSlotFilled())
			{
				continue;
			}

			FInventoryItem AssignedItem = CraftingSlot->GetAssignedItem();
			UInventoryComponent* SourceInv = CraftingSlot->SourceInventory;

			if (!SourceInv || !AssignedItem.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("ConsumeResources - Tool slot %d has no valid source inventory or item"), i);
				return false;
			}

			// Remove the tool from the source inventory (tools are typically quantity 1)
			int32 QuantityToRemove = 1 * BatchSize;
			if (!RemoveItemFromInventory(SourceInv, AssignedItem.ItemData, QuantityToRemove))
			{
				UE_LOG(LogTemp, Error, TEXT("ConsumeResources - Failed to remove %d x '%s' from source inventory"),
					QuantityToRemove, *AssignedItem.ItemData->ItemName.ToString());
				return false;
			}

			UE_LOG(LogTemp, Log, TEXT("ConsumeResources - Consumed %d x '%s' from tool slot %d"),
				QuantityToRemove, *AssignedItem.ItemData->ItemName.ToString(), i);
		}
	}

	// Clear all crafting slots after successful consumption
	ClearCraftingSlots();

	UE_LOG(LogTemp, Log, TEXT("ConsumeResources - Successfully consumed resources for '%s' (x%d)"),
		*SelectedRecipe->RecipeName.ToString(), BatchSize);

	return true;
}

void UWorkSurfaceCraftingWidget::AddCraftedItems()
{
	if (!SelectedRecipe || !PlayerInventory)
	{
		return;
	}

	int32 TotalQuantity = SelectedRecipe->OutputQuantity * BatchSize;

	int32 OutSlotIndex;
	bool bAdded = PlayerInventory->AddItem(SelectedRecipe->OutputItem, TotalQuantity, OutSlotIndex);

	if (bAdded)
	{
		UE_LOG(LogTemp, Log, TEXT("AddCraftedItems - Added %d x '%s' to player inventory at slot %d"),
			TotalQuantity, *SelectedRecipe->OutputItem->ItemName.ToString(), OutSlotIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AddCraftedItems - Failed to add items to player inventory (inventory full?)"));
		// TODO: Drop items on ground or show error message
	}
}

void UWorkSurfaceCraftingWidget::SetCategoryFilter(ERecipeCategory Category)
{
	CategoryFilter = Category;
	RefreshRecipeList();
}

void UWorkSurfaceCraftingWidget::ToggleShowOnlyCraftable()
{
	bShowOnlyCraftable = !bShowOnlyCraftable;
	RefreshRecipeList();
}

void UWorkSurfaceCraftingWidget::ClearFilters()
{
	CategoryFilter = ERecipeCategory::Misc;
	bShowOnlyCraftable = false;
	RefreshRecipeList();
}

FWindowCapabilities UWorkSurfaceCraftingWidget::GetWindowCapabilities_Implementation() const
{
	FWindowCapabilities Capabilities;
	Capabilities.bCanMove = bCanMove;
	Capabilities.bCanResize = bCanResize;
	Capabilities.bCanFullscreen = bCanFullscreen;
	return Capabilities;
}
