// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemData.h"
#include "WindowContentInterface.h"
#include "ContainerWidget.generated.h"

class UInventoryComponent;
class UInventorySlotWidget;
class UUniformGridPanel;
class UTextBlock;
class UButton;

/**
 * Item category filter for containers
 */
UENUM(BlueprintType)
enum class EContainerFilter : uint8
{
	None UMETA(DisplayName = "No Filter"),
	ToolsOnly UMETA(DisplayName = "Tools Only"),
	ResourcesOnly UMETA(DisplayName = "Resources Only"),
	ConsumablesOnly UMETA(DisplayName = "Consumables Only"),
	Custom UMETA(DisplayName = "Custom Filter")
};

/**
 * Base widget class for container UI (toolbox, material bin, etc.)
 * Displays items from an inventory component with category filtering
 * Implements WindowContentInterface to work with the modular window system
 */
UCLASS()
class OUTERCORP_API UContainerWidget : public UUserWidget, public IWindowContentInterface
{
	GENERATED_BODY()

public:
	// ============================================================================
	// Properties
	// ============================================================================

	/** Maximum number of items this container can hold */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
	int32 MaxSlots = 20;

	/** Number of columns in the grid layout */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
	int32 GridColumns = 5;

	/** Current view mode (true = list view, false = grid view) */
	UPROPERTY(BlueprintReadOnly, Category = "Container")
	bool bIsListView = false;

	/** Filter type for this container */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
	EContainerFilter FilterType = EContainerFilter::None;

	/** Custom category filter (only used when FilterType is Custom) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
	TArray<EItemCategory> AllowedCategories;

	// ============================================================================
	// Window Capabilities
	// ============================================================================

	/** Can the window be moved? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container|Window")
	bool bCanMove = true;

	/** Can the window be resized? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container|Window")
	bool bCanResize = false;  // Containers typically have fixed size

	/** Can the window be fullscreened? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container|Window")
	bool bCanFullscreen = false;

	// ============================================================================
	// Inventory References
	// ============================================================================

	/** Reference to the player's inventory component */
	UPROPERTY(BlueprintReadOnly, Category = "Container")
	UInventoryComponent* PlayerInventory;

	/** Reference to the container's inventory component */
	UPROPERTY(BlueprintReadOnly, Category = "Container")
	UInventoryComponent* ContainerInventory;

	// ============================================================================
	// Widget Components (Set in Blueprint)
	// ============================================================================

	/** Grid panel for displaying items */
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* ItemGrid;

	/** Widget switcher for toggling between grid and list view */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UWidgetSwitcher* ViewModeSwitcher;

	/** Grid view button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UButton* GridViewButton;

	/** List view button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UButton* ListViewButton;

	/** ScrollBox for table-style list view */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UScrollBox* ListViewScrollBox;

	/** Vertical box container inside ScrollBox for rows */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UVerticalBox* ListViewRowContainer;

	/** Header widget for list view columns */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UInventoryListHeaderWidget* ListViewHeader;

	/** Widget class for inventory slots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	/** Class for inventory list row widgets */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Container")
	TSubclassOf<class UInventoryListRowWidget> ListRowWidgetClass;

	/** Class for inventory list header widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Container")
	TSubclassOf<class UInventoryListHeaderWidget> ListHeaderWidgetClass;

	// ============================================================================
	// Methods
	// ============================================================================

	/** Setup the container with player and container inventories */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void SetupContainer(UInventoryComponent* InPlayerInventory, UInventoryComponent* InContainerInventory);

	/** Refresh the container display */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void RefreshContainer();

	/** Toggle between grid view and list view */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void ToggleViewMode();

	/** Switch to a specific view mode */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void SetViewMode(bool bListView);

private:
	/** Internal handler for inventory updates */
	UFUNCTION()
	void OnInventoryUpdate(int32 SlotIndex, const FInventoryItem& Item);

public:

	/** Check if an item passes the container's filter */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Container")
	bool PassesFilter(UInventoryItemData* ItemData) const;

	/** Handle moving item from player inventory to container */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void MoveToContainer(FGuid ItemInstanceID, int32 Quantity);

	/** Handle moving item from container to player inventory */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void MoveToPlayerInventory(FGuid ItemInstanceID, int32 Quantity);

	// ============================================================================
	// IWindowContentInterface Implementation
	// ============================================================================

	/** Get window capabilities for the modular window system */
	virtual FWindowCapabilities GetWindowCapabilities_Implementation() const override;

	/** Blueprint event called when right-clicked on empty area - implement empty context menu in Blueprint */
	UFUNCTION(BlueprintNativeEvent, Category = "Container")
	void OnEmptyAreaRightClicked(FVector2D MousePosition);
	virtual void OnEmptyAreaRightClicked_Implementation(FVector2D MousePosition);

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	/** Grid view button clicked */
	UFUNCTION()
	void OnGridViewButtonClicked();

	/** List view button clicked */
	UFUNCTION()
	void OnListViewButtonClicked();

	/** Populate the table-style list view with items */
	void PopulateTableView();

	/** Array of slot widgets currently displayed */
	UPROPERTY()
	TArray<UInventorySlotWidget*> SlotWidgets;

	/** Cached row widgets for table view */
	UPROPERTY()
	TArray<class UInventoryListRowWidget*> TableViewRows;
};
