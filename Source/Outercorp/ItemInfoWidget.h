// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemData.h"
#include "ItemInfoWidget.generated.h"

class UImage;
class UButton;
class AItemPreviewController;

/**
 * Base class for item info/inspect windows
 */
UCLASS(Abstract, Blueprintable)
class OUTERCORP_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeSupportsKeyboardFocus() const override { return true; }

	/** Find the preview controller in the world */
	void FindPreviewController();

	/** Close button (optional) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton;

	/** Item preview image widget (optional - for mouse input) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UUserWidget> ItemPreviewImage;

	/** Current item being displayed */
	UPROPERTY(BlueprintReadOnly, Category = "Item Info")
	FInventoryItem CurrentItem;

	/** Reference to the item preview controller */
	UPROPERTY(BlueprintReadWrite, Category = "Item Info|Preview")
	TObjectPtr<AItemPreviewController> PreviewController;

	/** Is the user currently dragging to rotate? */
	bool bIsDragging = false;

	/** Last mouse position for calculating delta */
	FVector2D LastMousePosition;

public:
	/** Set the item to display (called automatically by InventorySlotWidget) */
	UFUNCTION(BlueprintCallable, Category = "Item Info")
	void SetItemInfo(const FInventoryItem& Item);

	/** Close this window */
	UFUNCTION(BlueprintCallable, Category = "Item Info")
	void CloseWindow();

	/** Blueprint event called when item is set */
	UFUNCTION(BlueprintImplementableEvent, Category = "Item Info")
	void OnItemSet(const FInventoryItem& Item);

protected:
	/** Close button clicked */
	UFUNCTION()
	void OnCloseButtonClicked();
};
