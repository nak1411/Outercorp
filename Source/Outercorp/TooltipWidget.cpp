// Copyright Epic Games, Inc. All Rights Reserved.

#include "TooltipWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void UTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Set initial visibility to hidden but NOT Collapsed - use HitTestInvisible so widget still ticks
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRenderOpacity(0.0f); // Make it invisible via opacity instead
	bIsVisible = false;

	// Ensure ticking is enabled so we can detect when slot becomes available
	SetIsFocusable(false);
}

void UTooltipWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTooltipWidget::NativeDestruct()
{
	// Clear any pending timers
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HoverDelayTimer);
	}

	Super::NativeDestruct();
}

void UTooltipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update position every frame if tracking mouse and in mouse position mode
	if (bIsVisible && bTrackMousePosition && DisplayConfig.PositionMode == ETooltipPositionMode::MousePosition)
	{
		UpdatePosition();
	}
}

void UTooltipWidget::SetItem(const FInventoryItem& Item)
{
	CurrentItem = Item;

	// Notify Blueprint to update UI
	OnTooltipDataUpdated(CurrentItem);
}

void UTooltipWidget::SetDisplayConfig(const FTooltipDisplayConfig& Config)
{
	DisplayConfig = Config;

	// If already visible, update position immediately
	if (bIsVisible)
	{
		UpdatePosition();
	}
}

void UTooltipWidget::SetSourceWidget(UWidget* Widget)
{
	SourceWidget = Widget;

	// If already visible and in snap mode, update position immediately
	if (bIsVisible && DisplayConfig.PositionMode == ETooltipPositionMode::SnapToWidget)
	{
		UpdatePosition();
	}
}

void UTooltipWidget::Show()
{
	// Set the desired size for the tooltip
	FVector2D TooltipSize = OverrideSize;
	if (TooltipSize.X <= 0.0f || TooltipSize.Y <= 0.0f)
	{
		TooltipSize = FVector2D(300.0f, 200.0f);
	}
	SetDesiredSizeInViewport(TooltipSize);

	// Calculate initial position
	FVector2D InitialPosition = CalculateTooltipPosition();
	SetPositionInViewport(InitialPosition);

	// Clear any existing timer
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HoverDelayTimer);

		// If hover delay is set, start timer
		if (DisplayConfig.HoverDelay > 0.0f)
		{
			World->GetTimerManager().SetTimer(
				HoverDelayTimer,
				[this]()
				{
					SetVisibility(ESlateVisibility::HitTestInvisible);
					SetRenderOpacity(1.0f); // Restore full opacity
					bIsVisible = true;
				},
				DisplayConfig.HoverDelay,
				false
			);
		}
		else
		{
			SetVisibility(ESlateVisibility::HitTestInvisible);
			SetRenderOpacity(1.0f); // Restore full opacity
			bIsVisible = true;
		}
	}
}

void UTooltipWidget::Hide()
{
	// Clear any pending show timer
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(HoverDelayTimer);
	}

	// Hide immediately using opacity (keep HitTestInvisible so we can still tick if needed)
	SetRenderOpacity(0.0f);
	bIsVisible = false;
	bWaitingForSlot = false;
	bPendingShow = false;
}

void UTooltipWidget::UpdatePosition()
{
	if (!bIsVisible)
	{
		return;
	}

	FVector2D NewPosition = CalculateTooltipPosition();

	// Use SetPositionInViewport for widgets added via AddToViewport
	SetPositionInViewport(NewPosition);
}

FVector2D UTooltipWidget::CalculateTooltipPosition() const
{
	FVector2D Position;

	switch (DisplayConfig.PositionMode)
	{
	case ETooltipPositionMode::MousePosition:
		Position = CalculateMousePosition();
		break;

	case ETooltipPositionMode::AbsoluteScreen:
		Position = CalculateAbsolutePosition();
		break;

	case ETooltipPositionMode::SnapToWidget:
		Position = CalculateSnapPosition();
		break;

	default:
		Position = FVector2D::ZeroVector;
		break;
	}

	// Clamp to screen bounds to prevent tooltip from going off-screen
	return ClampToScreen(Position);
}

FVector2D UTooltipWidget::CalculateMousePosition() const
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return FVector2D::ZeroVector;
	}

	// Get current mouse position
	float MouseX, MouseY;
	PC->GetMousePosition(MouseX, MouseY);
	CachedMousePosition = FVector2D(MouseX, MouseY);

	// Apply offset
	return CachedMousePosition + DisplayConfig.MousePositionOffset;
}

FVector2D UTooltipWidget::CalculateAbsolutePosition() const
{
	// Simply return the configured absolute position
	return DisplayConfig.AbsolutePosition;
}

FVector2D UTooltipWidget::CalculateSnapPosition() const
{
	if (!SourceWidget)
	{
		return FVector2D::ZeroVector;
	}

	// Get the geometry of the source widget
	FGeometry SourceGeometry = SourceWidget->GetCachedGeometry();

	// Get position in absolute pixel space
	// LocalToViewport converts from widget local space to viewport pixel space
	FVector2D PixelPosition;
	FVector2D ParentScale;
	USlateBlueprintLibrary::LocalToViewport(this, SourceGeometry, FVector2D::ZeroVector, PixelPosition, ParentScale);

	// Get the absolute pixel size of the source widget (accounting for DPI scale)
	FVector2D SourceSizePixels = SourceGeometry.GetAbsoluteSize();

	// Get DPI scale from viewport
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);

	// Get tooltip size - OverrideSize is in unscaled coordinates, so we need to scale it
	FVector2D TooltipSize = OverrideSize;
	if (TooltipSize.X <= 0.0f || TooltipSize.Y <= 0.0f)
	{
		TooltipSize = FVector2D(300.0f, 200.0f);
	}

	// Scale tooltip size to match the absolute coordinate space
	FVector2D TooltipSizePixels = TooltipSize * ViewportScale;

	FVector2D Position;

	// Calculate position based on snap anchor
	switch (DisplayConfig.SnapAnchor)
	{
	case ETooltipSnapAnchor::Bottom:
		Position = FVector2D(
			PixelPosition.X + (SourceSizePixels.X - TooltipSizePixels.X) * 0.5f, // Center horizontally
			PixelPosition.Y + SourceSizePixels.Y // Below widget
		);
		break;

	case ETooltipSnapAnchor::Top:
		Position = FVector2D(
			PixelPosition.X + (SourceSizePixels.X - TooltipSizePixels.X) * 0.5f, // Center horizontally
			PixelPosition.Y - TooltipSizePixels.Y // Above widget
		);
		break;

	case ETooltipSnapAnchor::Left:
		Position = FVector2D(
			PixelPosition.X - TooltipSizePixels.X, // Left of widget
			PixelPosition.Y + (SourceSizePixels.Y - TooltipSizePixels.Y) * 0.5f // Center vertically
		);
		break;

	case ETooltipSnapAnchor::Right:
		Position = FVector2D(
			PixelPosition.X + SourceSizePixels.X, // Right of widget
			PixelPosition.Y + (SourceSizePixels.Y - TooltipSizePixels.Y) * 0.5f // Center vertically
		);
		break;

	case ETooltipSnapAnchor::BottomRight:
		Position = FVector2D(
			PixelPosition.X + SourceSizePixels.X, // Right edge
			PixelPosition.Y + SourceSizePixels.Y // Bottom edge
		);
		break;

	case ETooltipSnapAnchor::BottomLeft:
		Position = FVector2D(
			PixelPosition.X - TooltipSizePixels.X, // Left of widget
			PixelPosition.Y + SourceSizePixels.Y // Bottom edge
		);
		break;

	case ETooltipSnapAnchor::TopRight:
		Position = FVector2D(
			PixelPosition.X + SourceSizePixels.X, // Right edge
			PixelPosition.Y - TooltipSizePixels.Y // Above widget
		);
		break;

	case ETooltipSnapAnchor::TopLeft:
		Position = FVector2D(
			PixelPosition.X - TooltipSizePixels.X, // Left of widget
			PixelPosition.Y - TooltipSizePixels.Y // Above widget
		);
		break;

	default:
		Position = PixelPosition;
		break;
	}

	// Apply offset
	return Position + DisplayConfig.SnapOffset;
}

FVector2D UTooltipWidget::ClampToScreen(const FVector2D& Position) const
{
	// Get viewport size
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	FVector2D TooltipSize = GetDesiredSize();

	FVector2D ClampedPosition = Position;

	// Clamp X
	ClampedPosition.X = FMath::Clamp(ClampedPosition.X, 0.0f, ViewportSize.X - TooltipSize.X);

	// Clamp Y
	ClampedPosition.Y = FMath::Clamp(ClampedPosition.Y, 0.0f, ViewportSize.Y - TooltipSize.Y);

	return ClampedPosition;
}
