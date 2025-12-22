// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryColumnDivider.generated.h"

class UInventoryColumnSettings;
class UBorder;
class UImage;

/**
 * Column divider widget with drag handle for resizing columns
 */
UCLASS()
class OUTERCORP_API UInventoryColumnDivider : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FCursorReply NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent) override;

	/** The visual divider line */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UBorder> DividerLine;

	/** Drag handle visual (optional, can be same as DividerLine) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UImage> DragHandle;

	/** Whether we're currently dragging */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	bool bIsDragging = false;

	/** Mouse position when drag started */
	FVector2D DragStartMousePos;

	/** Column width when drag started */
	float DragStartColumnWidth;

public:
	/** The column this divider controls (to the left of the divider) */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FName ColumnName;

	/** Reference to the shared column settings */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UInventoryColumnSettings> ColumnSettings;

	/** Set the column this divider controls */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetColumn(FName InColumnName, UInventoryColumnSettings* InColumnSettings);

protected:
	/** Update visual state based on hover/drag */
	void UpdateVisualState(bool bHovered, bool bDragging);
};
