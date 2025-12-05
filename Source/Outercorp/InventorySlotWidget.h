// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryItemData.h"
#include "InventorySlotWidget.generated.h"

class UInventoryComponent;
class UImage;
class UTextBlock;
class UBorder;
class UButton;

/**
 * Drag-drop operation for inventory items
 * Must be declared before UInventorySlotWidget
 */
UCLASS()
class OUTERCORP_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	/** Source slot index */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 SourceSlotIndex = -1;

	/** Item being dragged */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FInventoryItem DraggedItem;

	/** Inventory component */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Visual widget for dragging */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UUserWidget> DraggedVisual;

	/** Is this a split operation (shift-drag) */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool bIsSplitOperation = false;
};

/**
 * Widget representing a single inventory slot
 */
UCLASS()
class OUTERCORP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	/** Item icon image */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	/** Quantity text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> QuantityText;

	/** Rarity border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> RarityBorder;

	/** Background border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> BackgroundBorder;

	/** Slot button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> SlotButton;

	/** Current item in this slot */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FInventoryItem CurrentItem;

	/** Slot index in inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 SlotIndex = -1;

	/** Reference to inventory component */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	/** Default icon for empty slot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSoftObjectPtr<UTexture2D> EmptySlotIcon;

	/** Color for drag hover state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FLinearColor HoverColor = FLinearColor(0.2f, 0.5f, 1.0f, 0.5f);

	/** Normal background color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FLinearColor NormalColor = FLinearColor(0.05f, 0.05f, 0.05f, 0.9f);

public:
	/** Set the item for this slot */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItem(const FInventoryItem& Item);

	/** Set slot index */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSlotIndex(int32 Index) { SlotIndex = Index; }

	/** Set inventory component reference */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryComponent(UInventoryComponent* InInventoryComponent) { InventoryComponent = InInventoryComponent; }

	/** Get current item */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FInventoryItem GetItem() const { return CurrentItem; }

	/** Get slot index */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetSlotIndex() const { return SlotIndex; }

protected:
	/** Called when slot is clicked */
	UFUNCTION()
	void OnSlotClicked();

	/** Update visual appearance based on item */
	void UpdateAppearance();
};
