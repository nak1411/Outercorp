// Copyright Epic Games, Inc. All Rights Reserved.

#include "NotificationWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "TimerManager.h"
#include "Animation/WidgetAnimation.h"

void UNotificationWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UNotificationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Widget elements will be bound if they exist in Blueprint
	// If not using Blueprint, they'll be null and we'll skip native updates
}

void UNotificationWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// Clear any active timers
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CloseTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
	}
}

void UNotificationWidget::InitializeNotification(const FNotificationData& InNotificationData)
{
	NotificationData = InNotificationData;

	// Try native UI update first
	NativeUpdateUI();

	// Call blueprint event to update UI (if implemented)
	OnNotificationDataUpdated();

	// Try native fade in first
	NativeFadeIn();

	// Call blueprint animation (if implemented)
	PlayFadeInAnimation();

	// Schedule fade out
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			FadeOutTimerHandle,
			this,
			&UNotificationWidget::OnFadeOutTime,
			NotificationData.Duration,
			false
		);
	}
}

void UNotificationWidget::StartClosing()
{
	// Clear the auto-close timer if it's still running
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
	}

	OnFadeOutTime();
}

void UNotificationWidget::ForceClose()
{
	// Clear all timers
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CloseTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
	}

	// Broadcast expiration
	OnNotificationExpired.Broadcast(this);

	// Remove from viewport
	RemoveFromParent();
}

void UNotificationWidget::UpdatePosition(float YOffset)
{
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		// Position from top-left of screen
		FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

		// Anchor to top-left
		CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 0.0f, 0.0f));

		// Position with offset from left edge and from top
		FVector2D Position(10.0f, YOffset); // 10 pixels from left edge
		CanvasSlot->SetPosition(Position);

		// Set alignment to top-left
		CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));
	}
}

void UNotificationWidget::OnFadeOutTime()
{
	// Try native fade out first
	NativeFadeOut();

	// Play blueprint fade out animation (if implemented)
	PlayFadeOutAnimation();

	// Schedule actual removal after fade animation completes
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			CloseTimerHandle,
			this,
			&UNotificationWidget::OnFadeOutComplete,
			FadeOutDuration,
			false
		);
	}
}

void UNotificationWidget::OnFadeOutComplete()
{
	// Broadcast that we're expiring
	OnNotificationExpired.Broadcast(this);

	// Remove from viewport
	RemoveFromParent();
}

void UNotificationWidget::NativeUpdateUI()
{
	// Update UI elements if they're bound from Blueprint
	if (MessageText)
	{
		MessageText->SetText(NotificationData.Message);
	}

	if (BackgroundBorder)
	{
		FLinearColor BorderColor;
		switch (NotificationData.Type)
		{
		case ENotificationType::Info:
			BorderColor = FLinearColor(0.2f, 0.4f, 0.6f, 0.9f); // Blue
			break;
		case ENotificationType::Success:
			BorderColor = FLinearColor(0.2f, 0.6f, 0.2f, 0.9f); // Green
			break;
		case ENotificationType::Warning:
			BorderColor = FLinearColor(0.8f, 0.5f, 0.1f, 0.9f); // Orange
			break;
		case ENotificationType::Error:
			BorderColor = FLinearColor(0.8f, 0.2f, 0.2f, 0.9f); // Red
			break;
		default:
			BorderColor = FLinearColor(0.2f, 0.2f, 0.2f, 0.9f); // Gray
			break;
		}
		BackgroundBorder->SetBrushColor(BorderColor);
	}

	if (IconImage)
	{
		if (NotificationData.Icon)
		{
			IconImage->SetBrushFromTexture(NotificationData.Icon);
			IconImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			IconImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UNotificationWidget::NativeFadeIn()
{
	// Start fade in animation
	SetRenderOpacity(0.0f);
	bIsFadingIn = true;
	bIsFadingOut = false;
	FadeStartTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

	// Target position is always 10.0f from left edge (anchored to top-left)
	InitialXPosition = 10.0f;

	// Set starting position off-screen to the left
	if (bEnableSlideAnimation)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
		{
			FVector2D CurrentPos = CanvasSlot->GetPosition();
			CanvasSlot->SetPosition(FVector2D(InitialXPosition + SlideDistance, CurrentPos.Y));
		}
	}

	// Start animation timer using UFUNCTION callback
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(FadeAnimationTimerHandle, this, &UNotificationWidget::UpdateFadeAnimation, 0.016f, true);
	}
}

void UNotificationWidget::NativeFadeOut()
{
	// Start fade out animation
	bIsFadingIn = false;
	bIsFadingOut = true;
	FadeStartTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

	// Target position is always 10.0f from left edge
	InitialXPosition = 10.0f;

	// Start animation timer using UFUNCTION callback
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(FadeAnimationTimerHandle, this, &UNotificationWidget::UpdateFadeAnimation, 0.016f, true);
	}
}

void UNotificationWidget::UpdateFadeAnimation()
{
	if (!GetWorld())
	{
		if (FadeAnimationTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(FadeAnimationTimerHandle);
		}
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float ElapsedTime = CurrentTime - FadeStartTime;

	if (bIsFadingIn)
	{
		float Alpha = FMath::Clamp(ElapsedTime / FadeInDuration, 0.0f, 1.0f);

		// Apply easing to the alpha for smoother animation
		float EasedAlpha = FMath::InterpEaseOut(0.0f, 1.0f, Alpha, 2.0f);
		SetRenderOpacity(EasedAlpha);

		// Slide animation from left
		if (bEnableSlideAnimation)
		{
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
			{
				FVector2D CurrentPos = CanvasSlot->GetPosition();
				// Slide from (InitialXPosition + SlideDistance) to InitialXPosition
				float TargetX = FMath::Lerp(InitialXPosition + SlideDistance, InitialXPosition, EasedAlpha);
				CanvasSlot->SetPosition(FVector2D(TargetX, CurrentPos.Y));
			}
		}

		if (Alpha >= 1.0f)
		{
			// Fade in complete
			bIsFadingIn = false;
			GetWorld()->GetTimerManager().ClearTimer(FadeAnimationTimerHandle);
		}
	}
	else if (bIsFadingOut)
	{
		float Alpha = FMath::Clamp(ElapsedTime / FadeOutDuration, 0.0f, 1.0f);

		// Apply easing to the alpha for smoother animation
		float EasedAlpha = FMath::InterpEaseIn(0.0f, 1.0f, Alpha, 2.0f);
		SetRenderOpacity(1.0f - EasedAlpha);

		// Slide animation to left
		if (bEnableSlideAnimation)
		{
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
			{
				FVector2D CurrentPos = CanvasSlot->GetPosition();
				// Slide from InitialXPosition to (InitialXPosition + SlideDistance)
				float TargetX = FMath::Lerp(InitialXPosition, InitialXPosition + SlideDistance, EasedAlpha);
				CanvasSlot->SetPosition(FVector2D(TargetX, CurrentPos.Y));
			}
		}

		if (Alpha >= 1.0f)
		{
			// Fade out complete
			bIsFadingOut = false;
			GetWorld()->GetTimerManager().ClearTimer(FadeAnimationTimerHandle);
		}
	}
}
