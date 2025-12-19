// Copyright Epic Games, Inc. All Rights Reserved.

#include "ContextMenuWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/IInputProcessor.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

/**
 * Input processor that captures clicks outside the context menu
 */
class FContextMenuInputProcessor : public IInputProcessor
{
public:
	FContextMenuInputProcessor(UContextMenuWidget* InContextMenu)
		: ContextMenu(InContextMenu)
	{
	}

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		if (!ContextMenu.IsValid())
		{
			return false;
		}

		// Only handle left and right mouse buttons for closing
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton ||
		    MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
		{
			// Get the widget under the cursor
			FWidgetPath WidgetPath = SlateApp.LocateWindowUnderMouse(MouseEvent.GetScreenSpacePosition(), SlateApp.GetInteractiveTopLevelWindows());

			if (WidgetPath.IsValid())
			{
				// Check if the click is inside the context menu widget
				TSharedPtr<SWidget> ContextMenuSlateWidget = ContextMenu->TakeWidget();

				bool bClickedInside = false;
				for (int32 i = 0; i < WidgetPath.Widgets.Num(); ++i)
				{
					if (WidgetPath.Widgets[i].Widget == ContextMenuSlateWidget)
					{
						bClickedInside = true;
						break;
					}
				}

				// If clicked outside, close the menu - call public function
				if (!bClickedInside)
				{
					UE_LOG(LogTemp, Log, TEXT("Click detected outside context menu, closing..."));
					ContextMenu->RequestClose();
				}
			}
		}

		// IMPORTANT: Don't consume the event - let it pass through to other widgets
		// This allows the inventory slot to receive the right-click event
		return false;
	}

private:
	TWeakObjectPtr<UContextMenuWidget> ContextMenu;
};

void UContextMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Set up click capture after a brief delay
	// This prevents the right-click that opened the menu from immediately closing it
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			SetupTimerHandle,
			this,
			&UContextMenuWidget::SetupClickCapture,
			0.1f,  // 100ms delay
			false
		);
	}
}

void UContextMenuWidget::NativeDestruct()
{
	// Clean up timer
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SetupTimerHandle);
	}

	// Remove input processor
	if (InputProcessor.IsValid() && FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
		InputProcessor.Reset();
	}

	bIsClickCaptureActive = false;

	Super::NativeDestruct();
}

FReply UContextMenuWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Handle clicks on the menu itself - don't close
	UE_LOG(LogTemp, Log, TEXT("Click on context menu itself - keeping open"));
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UContextMenuWidget::SetupClickCapture()
{
	if (bIsClickCaptureActive)
	{
		return;
	}

	// Create and register input processor for detecting clicks outside
	// Use a very low priority (999) so other widgets process input first
	if (FSlateApplication::IsInitialized())
	{
		InputProcessor = MakeShared<FContextMenuInputProcessor>(this);
		FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor, 999);
		bIsClickCaptureActive = true;

		UE_LOG(LogTemp, Log, TEXT("Context menu click capture activated"));
	}
}

void UContextMenuWidget::RequestClose()
{
	CloseMenu();
}

void UContextMenuWidget::CloseMenu()
{
	if (!IsValid(this))
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Closing context menu"));

	// CRITICAL: Remove input processor FIRST before doing anything else
	// This ensures we don't interfere with subsequent clicks
	if (InputProcessor.IsValid() && FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
		InputProcessor.Reset();
	}

	bIsClickCaptureActive = false;

	// Call Blueprint event
	OnRequestClose();

	// Small delay before removing from parent to ensure proper cleanup
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick([this]()
		{
			if (IsValid(this))
			{
				RemoveFromParent();
			}
		});
	}
	else
	{
		RemoveFromParent();
	}
}
