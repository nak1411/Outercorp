// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemData.h"
#include "TooltipWidget.generated.h"

/**
 * Tooltip positioning mode
 */
UENUM(BlueprintType)
enum class ETooltipPositionMode : uint8
{
	/** Position at mouse cursor with offset */
	MousePosition UMETA(DisplayName = "Mouse Position"),

	/** Position at absolute screen coordinates */
	AbsoluteScreen UMETA(DisplayName = "Absolute Screen Position"),

	/** Snap to the widget that triggered the tooltip */
	SnapToWidget UMETA(DisplayName = "Snap To Widget")
};

/**
 * Anchor position for snap-to-widget mode
 */
UENUM(BlueprintType)
enum class ETooltipSnapAnchor : uint8
{
	/** Snap to bottom of widget */
	Bottom UMETA(DisplayName = "Bottom"),

	/** Snap to top of widget */
	Top UMETA(DisplayName = "Top"),

	/** Snap to left of widget */
	Left UMETA(DisplayName = "Left"),

	/** Snap to right of widget */
	Right UMETA(DisplayName = "Right"),

	/** Snap to bottom-right of widget */
	BottomRight UMETA(DisplayName = "Bottom Right"),

	/** Snap to bottom-left of widget */
	BottomLeft UMETA(DisplayName = "Bottom Left"),

	/** Snap to top-right of widget */
	TopRight UMETA(DisplayName = "Top Right"),

	/** Snap to top-left of widget */
	TopLeft UMETA(DisplayName = "Top Left")
};

/**
 * Configuration structure for tooltip display
 */
USTRUCT(BlueprintType)
struct FTooltipDisplayConfig
{
	GENERATED_BODY()

	/** Positioning mode for the tooltip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	ETooltipPositionMode PositionMode = ETooltipPositionMode::MousePosition;

	/** Offset from mouse position (only used in MousePosition mode) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	FVector2D MousePositionOffset = FVector2D(10.0f, 10.0f);

	/** Absolute screen position (only used in AbsoluteScreen mode) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	FVector2D AbsolutePosition = FVector2D(100.0f, 100.0f);

	/** Anchor position when snapping to widget (only used in SnapToWidget mode) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	ETooltipSnapAnchor SnapAnchor = ETooltipSnapAnchor::Bottom;

	/** Offset from snap anchor position (only used in SnapToWidget mode) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	FVector2D SnapOffset = FVector2D(0.0f, 5.0f);

	/** Delay in seconds before showing the tooltip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip", meta = (ClampMin = "0.0", ClampMax = "5.0"))
	float HoverDelay = 0.5f;

	/** Whether to keep tooltip visible when mouse moves away */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	bool bPersistent = false;

	/** Z-order for tooltip rendering (higher values render on top) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	int32 ZOrder = 1000;
};

/**
 * Widget for displaying tooltips when hovering over items
 */
UCLASS()
class OUTERCORP_API UTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativePreConstruct() override;

public:
	/**
	 * Set the item to display in the tooltip
	 * @param Item The inventory item to display
	 */
	UFUNCTION(BlueprintCallable, Category = "Tooltip")
	void SetItem(const FInventoryItem& Item);

	/**
	 * Set the display configuration for this tooltip
	 * @param Config The configuration settings
	 */
	UFUNCTION(BlueprintCallable, Category = "Tooltip")
	void SetDisplayConfig(const FTooltipDisplayConfig& Config);

	/**
	 * Set the source widget that triggered this tooltip (required for SnapToWidget mode)
	 * @param Widget The widget to snap to
	 */
	UFUNCTION(BlueprintCallable, Category = "Tooltip")
	void SetSourceWidget(UWidget* Widget);

	/**
	 * Update the tooltip position based on current configuration
	 */
	UFUNCTION(BlueprintCallable, Category = "Tooltip")
	void UpdatePosition();

	/**
	 * Show the tooltip (respects hover delay)
	 */
	UFUNCTION(BlueprintCallable, Category = "Tooltip")
	void Show();

	/**
	 * Hide the tooltip immediately
	 */
	UFUNCTION(BlueprintCallable, Category = "Tooltip")
	void Hide();

	/**
	 * Check if tooltip is currently visible
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tooltip")
	bool IsTooltipVisible() const { return bIsVisible; }

	/** Get the override size for the tooltip */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tooltip")
	FVector2D GetOverrideSize() const { return OverrideSize; }

	/** Set the override size for the tooltip */
	UFUNCTION(BlueprintCallable, Category = "Tooltip")
	void SetOverrideSize(const FVector2D& NewSize) { OverrideSize = NewSize; }

protected:
	/** Blueprint event called when tooltip data is updated - implement UI updates here */
	UFUNCTION(BlueprintImplementableEvent, Category = "Tooltip")
	void OnTooltipDataUpdated(const FInventoryItem& Item);

	/** Calculate position based on current configuration */
	FVector2D CalculateTooltipPosition() const;

	/** Calculate position for mouse cursor mode */
	FVector2D CalculateMousePosition() const;

	/** Calculate position for absolute screen mode */
	FVector2D CalculateAbsolutePosition() const;

	/** Calculate position for snap to widget mode */
	FVector2D CalculateSnapPosition() const;

	/** Clamp tooltip position to screen bounds */
	FVector2D ClampToScreen(const FVector2D& Position) const;

protected:
	/** Current item being displayed */
	UPROPERTY(BlueprintReadOnly, Category = "Tooltip")
	FInventoryItem CurrentItem;

	/** Display configuration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	FTooltipDisplayConfig DisplayConfig;

	/** Source widget for snap positioning */
	UPROPERTY(BlueprintReadOnly, Category = "Tooltip")
	TObjectPtr<UWidget> SourceWidget;

	/** Whether tooltip is currently visible */
	UPROPERTY(BlueprintReadOnly, Category = "Tooltip")
	bool bIsVisible = false;

	/** Timer handle for hover delay */
	FTimerHandle HoverDelayTimer;

	/** Whether to update position every tick (used for mouse tracking) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	bool bTrackMousePosition = true;

	/** Cache of current mouse position for mouse tracking mode */
	mutable FVector2D CachedMousePosition;

	/** Override size for tooltip (if set to 0,0 will use desired size) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	FVector2D OverrideSize = FVector2D(300.0f, 200.0f);

	/** Flag to indicate we're waiting for slot to become available */
	bool bWaitingForSlot = false;

	/** Flag to indicate we should show after slot is ready */
	bool bPendingShow = false;
};
