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

	// Spawn or find preview controller
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

	// Destroy the spawned preview controller actor
	if (PreviewController && PreviewController->IsValidLowLevel())
	{
		PreviewController->Destroy();
		PreviewController = nullptr;
	}

	Super::NativeDestruct();
}

void UItemInfoWidget::SetItemInfo(const FInventoryItem& Item)
{
	CurrentItem = Item;

	// Find controller if not found yet
	if (!PreviewController)
	{
		FindPreviewController();
	}

	// Update preview controller
	if (PreviewController)
	{
		PreviewController->SetPreviewItem(Item);
	}

	// Call Blueprint event
	OnItemSet(Item);
}

void UItemInfoWidget::FindPreviewController()
{
	if (PreviewController)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// First, try to find an existing controller in the world (for backwards compatibility)
	for (TActorIterator<AItemPreviewController> It(World); It; ++It)
	{
		PreviewController = *It;
		return;
	}

	// If not found and we have a class set, spawn a new one
	if (PreviewControllerClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Name = FName(TEXT("ItemPreviewController_Spawned"));
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		PreviewController = World->SpawnActor<AItemPreviewController>(PreviewControllerClass, PreviewSpawnLocation, FRotator::ZeroRotator, SpawnParams);
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
