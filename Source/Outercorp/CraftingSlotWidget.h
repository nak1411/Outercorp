// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemData.h"
#include "CraftingSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UBorder;

/**
 * Type of crafting slot (ingredient or tool)
 */
UENUM(BlueprintType)
enum class ECraftingSlotType : uint8
{
	Ingredient UMETA(DisplayName = "Ingredient"),
	Tool UMETA(DisplayName = "Tool")
};

/**
 * Widget representing a crafting slot in the work surface
 * Accepts drag-drop from toolbox/material bin inventories
 */
UCLASS()
class OUTERCORP_API UCraftingSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	// ============================================================================
	// Widget Components
	// ============================================================================

	/** Item icon image */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	/** Quantity text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> QuantityText;

	/** Background border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> BackgroundBorder;

	/** Selection/highlight border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> HighlightBorder;

	// ============================================================================
	// Properties
	// ============================================================================

	/** Type of slot (ingredient or tool) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting Slot")
	ECraftingSlotType SlotType = ECraftingSlotType::Ingredient;

	/** Currently assigned item */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Slot")
	FInventoryItem AssignedItem;

	/** Source inventory where the assigned item came from */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Slot")
	TObjectPtr<class UInventoryComponent> SourceInventory;

	/** Source slot index in the source inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Slot")
	int32 SourceSlotIndex = -1;

	/** Required item for this slot (from recipe) */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Slot")
	TObjectPtr<UInventoryItemData> RequiredItem;

	/** Required quantity for this slot */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Slot")
	int32 RequiredQuantity = 1;

	/** Is this slot filled? */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Slot")
	bool bIsFilled = false;

	/** Color for drag hover state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting Slot|Visuals")
	FLinearColor DragHoverColor = FLinearColor(0.2f, 0.5f, 1.0f, 0.5f);

	/** Color for valid drop */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting Slot|Visuals")
	FLinearColor ValidDropColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.5f);

	/** Color for invalid drop */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting Slot|Visuals")
	FLinearColor InvalidDropColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.5f);

	// ============================================================================
	// Functions
	// ============================================================================

	/** Set the required item for this slot (from recipe) */
	UFUNCTION(BlueprintCallable, Category = "Crafting Slot")
	void SetRequiredItem(UInventoryItemData* Item, int32 Quantity = 1);

	/** Assign an item to this slot (from drag-drop) */
	UFUNCTION(BlueprintCallable, Category = "Crafting Slot")
	bool AssignItem(const FInventoryItem& Item);

	/** Clear the assigned item */
	UFUNCTION(BlueprintCallable, Category = "Crafting Slot")
	void ClearSlot();

	/** Check if the slot accepts this item */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting Slot")
	bool CanAcceptItem(const FInventoryItem& Item) const;

	/** Check if slot is filled with correct item */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting Slot")
	bool IsSlotFilled() const { return bIsFilled; }

	/** Get the assigned item */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Crafting Slot")
	FInventoryItem GetAssignedItem() const { return AssignedItem; }

	/** Blueprint event when item is assigned */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting Slot")
	void OnItemAssigned(const FInventoryItem& Item);

	/** Blueprint event when slot is cleared */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crafting Slot")
	void OnSlotCleared();

protected:
	/** Update visual appearance based on assigned item */
	void UpdateAppearance();
};
