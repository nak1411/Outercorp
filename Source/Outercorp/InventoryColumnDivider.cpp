// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryColumnDivider.h"
#include "InventoryColumnSettings.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Framework/Application/SlateApplication.h"

void UInventoryColumnDivider::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateVisualState(false, false);
}

void UInventoryColumnDivider::SetColumn(FName InColumnName, UInventoryColumnSettings* InColumnSettings)
{
	ColumnName = InColumnName;
	ColumnSettings = InColumnSettings;
}

FReply UInventoryColumnDivider::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && ColumnSettings)
	{
		bIsDragging = true;
		DragStartMousePos = InMouseEvent.GetScreenSpacePosition();
		DragStartColumnWidth = ColumnSettings->GetColumnWidth(ColumnName);

		UpdateVisualState(true, true);

		return FReply::Handled().CaptureMouse(TakeWidget());
	}

	return FReply::Unhandled();
}

FReply UInventoryColumnDivider::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDragging && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = false;
		UpdateVisualState(true, false); // Keep hover state

		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

FReply UInventoryColumnDivider::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDragging && ColumnSettings)
	{
		// Calculate the delta from drag start
		FVector2D CurrentMousePos = InMouseEvent.GetScreenSpacePosition();
		float DeltaX = CurrentMousePos.X - DragStartMousePos.X;

		// Apply delta to the starting column width
		float NewWidth = DragStartColumnWidth + DeltaX;

		// Update the column width (will be clamped to minimum in SetColumnWidth)
		ColumnSettings->SetColumnWidth(ColumnName, NewWidth);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UInventoryColumnDivider::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (!bIsDragging)
	{
		UpdateVisualState(true, false);
	}

	// Change cursor to resize cursor
	// Note: Cursor change is handled by overriding GetCursor() instead
}

void UInventoryColumnDivider::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (!bIsDragging)
	{
		UpdateVisualState(false, false);
	}

	// Reset cursor to default
	// Note: Cursor change is handled by overriding GetCursor() instead
}

FCursorReply UInventoryColumnDivider::NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent)
{
	// Show resize cursor when hovering or dragging
	return FCursorReply::Cursor(EMouseCursor::ResizeLeftRight);
}

void UInventoryColumnDivider::UpdateVisualState(bool bHovered, bool bDragging)
{
	// Update divider line color based on state
	if (DividerLine)
	{
		FLinearColor Color;
		if (bDragging)
		{
			Color = FLinearColor(0.4f, 0.6f, 1.0f, 1.0f); // Bright blue when dragging
		}
		else if (bHovered)
		{
			Color = FLinearColor(0.6f, 0.6f, 0.6f, 1.0f); // Light gray when hovered
		}
		else
		{
			Color = FLinearColor(0.3f, 0.3f, 0.3f, 1.0f); // Dark gray normally
		}

		DividerLine->SetBrushColor(Color);
	}

	// Update drag handle visual if present
	if (DragHandle)
	{
		DragHandle->SetVisibility(bHovered || bDragging ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
