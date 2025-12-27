// Copyright Epic Games, Inc. All Rights Reserved.

#include "ItemInfoWidget.h"
#include "Components/Button.h"
#include "ItemPreviewController.h"
#include "ItemPreviewImage.h"
#include "EngineUtils.h"

void UItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Enable mouse input
	SetVisibility(ESlateVisibility::Visible);

	// Bind close button
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UItemInfoWidget::OnCloseButtonClicked);
	}

	// Find preview controller in world
	FindPreviewController();

	// If ItemPreviewImage widget is bound, forward its preview controller reference
	if (ItemPreviewImage)
	{
		UItemPreviewImage* PreviewImageWidget = Cast<UItemPreviewImage>(ItemPreviewImage);
		if (PreviewImageWidget && PreviewController)
		{
			PreviewImageWidget->PreviewController = PreviewController;
		}
	}
}

void UItemInfoWidget::NativeDestruct()
{
	// Clean up
	if (bIsDragging)
	{
		bIsDragging = false;
	}

	Super::NativeDestruct();
}

void UItemInfoWidget::SetItemInfo(const FInventoryItem& Item)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemInfoWidget: SetItemInfo called"));

	CurrentItem = Item;

	// Find controller if not found yet
	if (!PreviewController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemInfoWidget: PreviewController is null, attempting to find it..."));
		FindPreviewController();
	}

	// Update preview controller
	if (PreviewController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemInfoWidget: Calling SetPreviewItem on controller"));
		PreviewController->SetPreviewItem(Item);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ItemInfoWidget: PreviewController is still null!"));
	}

	// Call Blueprint event
	OnItemSet(Item);
}

void UItemInfoWidget::FindPreviewController()
{
	if (PreviewController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemInfoWidget: PreviewController already set"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemInfoWidget: Searching for ItemPreviewController in world..."));

	// Find ItemPreviewController in the world
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemInfoWidget: No World!"));
		return;
	}

	int32 Count = 0;
	for (TActorIterator<AItemPreviewController> It(World); It; ++It)
	{
		Count++;
		PreviewController = *It;
		UE_LOG(LogTemp, Warning, TEXT("ItemInfoWidget: Found ItemPreviewController: %s"), *It->GetName());
		break;
	}

	if (Count == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemInfoWidget: No ItemPreviewController actors found in world!"));
	}
}

void UItemInfoWidget::CloseWindow()
{
	RemoveFromParent();
}

void UItemInfoWidget::OnCloseButtonClicked()
{
	CloseWindow();
}

FReply UItemInfoWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Start dragging on left mouse button
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (PreviewController)
		{
			bIsDragging = true;
			LastMousePosition = InMouseEvent.GetScreenSpacePosition();

			// Capture mouse so we continue receiving events even if cursor leaves widget
			return FReply::Handled().CaptureMouse(this->TakeWidget());
		}
	}

	return FReply::Unhandled();
}

FReply UItemInfoWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Stop dragging
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bIsDragging)
	{
		bIsDragging = false;
		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

FReply UItemInfoWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Handle rotation while dragging
	if (bIsDragging && PreviewController)
	{
		FVector2D CurrentMousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D MouseDelta = CurrentMousePosition - LastMousePosition;

		// Send rotation input to preview controller
		PreviewController->AddRotationInput(MouseDelta.X, MouseDelta.Y);

		LastMousePosition = CurrentMousePosition;
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply UItemInfoWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Handle zoom
	if (PreviewController)
	{
		float WheelDelta = InMouseEvent.GetWheelDelta();
		PreviewController->AddZoomInput(WheelDelta);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}
