// Copyright Epic Games, Inc. All Rights Reserved.

#include "ItemPreviewImage.h"
#include "ItemPreviewController.h"
#include "Components/Image.h"
#include "EngineUtils.h"

UItemPreviewImage::UItemPreviewImage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetVisibility(ESlateVisibility::Visible);
}

void UItemPreviewImage::NativeConstruct()
{
	Super::NativeConstruct();

	// Set up the preview image to be clickable
	if (PreviewImage)
	{
		PreviewImage->SetVisibility(ESlateVisibility::Visible);
	}

	// Find preview controller
	FindPreviewController();
}

void UItemPreviewImage::FindPreviewController()
{
	if (PreviewController)
		return;

	// Find ItemPreviewController in the world
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemPreviewImage: No World!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemPreviewImage: Searching for ItemPreviewController..."));

	int32 Count = 0;
	for (TActorIterator<AItemPreviewController> It(World); It; ++It)
	{
		Count++;
		PreviewController = *It;
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewImage: Found ItemPreviewController: %s"), *It->GetName());
		break;
	}

	if (Count == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemPreviewImage: No ItemPreviewController actors found in world!"));
		UE_LOG(LogTemp, Error, TEXT("ItemPreviewImage: Make sure BP_ItemInfoPreview is placed in the level and inherits from ItemPreviewController"));
	}
}

FReply UItemPreviewImage::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemPreviewImage: Mouse button down"));

	// Find controller if not found yet
	if (!PreviewController)
	{
		FindPreviewController();
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewImage: PreviewController = %s"), PreviewController ? TEXT("Found") : TEXT("NULL"));
	}

	// Start dragging on left mouse button
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (PreviewController)
		{
			bIsDragging = true;
			LastMousePosition = InMouseEvent.GetScreenSpacePosition();

			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewImage: Starting drag"));
			// Capture mouse so we continue receiving events even if cursor leaves widget
			return FReply::Handled().CaptureMouse(this->TakeWidget());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ItemPreviewImage: No PreviewController!"));
		}
	}

	return FReply::Unhandled();
}

FReply UItemPreviewImage::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Stop dragging
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bIsDragging)
	{
		bIsDragging = false;
		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

FReply UItemPreviewImage::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

FReply UItemPreviewImage::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Find controller if not found yet
	if (!PreviewController)
	{
		FindPreviewController();
	}

	// Handle zoom
	if (PreviewController)
	{
		float WheelDelta = InMouseEvent.GetWheelDelta();
		PreviewController->AddZoomInput(WheelDelta);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}
