// Fill out your copyright notice in the Description page of Project Settings.

#include "ConstructionModeBorderWidget.h"
#include "OutercorpCharacter.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

void UConstructionModeBorderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Get reference to the player character
	OwnerCharacter = Cast<AOutercorpCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConstructionModeBorderWidget: Failed to get OwnerCharacter reference"));
	}

	// Initialize colors
	CurrentColor = NormalModeColor;
	TargetColor = NormalModeColor;

	// Verify all border widgets are bound
	int32 BoundBorders = 0;
	if (TopBorder) BoundBorders++;
	if (BottomBorder) BoundBorders++;
	if (LeftBorder) BoundBorders++;
	if (RightBorder) BoundBorders++;

	UE_LOG(LogTemp, Log, TEXT("ConstructionModeBorderWidget: Found %d/4 border widgets"), BoundBorders);

	// Set initial colors for all borders
	if (TopBorder) TopBorder->SetBrushColor(CurrentColor);
	if (BottomBorder) BottomBorder->SetBrushColor(CurrentColor);
	if (LeftBorder) LeftBorder->SetBrushColor(CurrentColor);
	if (RightBorder) RightBorder->SetBrushColor(CurrentColor);

	if (BoundBorders == 4)
	{
		UE_LOG(LogTemp, Log, TEXT("ConstructionModeBorderWidget: All borders set up successfully. Border thickness: %f"), BorderThickness);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ConstructionModeBorderWidget: Missing border widgets! Make sure TopBorder, BottomBorder, LeftBorder, and RightBorder are all named correctly in Blueprint"));
	}
}

void UConstructionModeBorderWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateBorderColor(InDeltaTime);
}

void UConstructionModeBorderWidget::UpdateBorderColor(float DeltaTime)
{
	if (!OwnerCharacter)
	{
		return;
	}

	// Get the target color based on current mode
	FLinearColor NewTargetColor = GetTargetColorForCurrentMode();

	// Log when target color changes (mode changed)
	if (!TargetColor.Equals(NewTargetColor, 0.01f))
	{
		TargetColor = NewTargetColor;
		UE_LOG(LogTemp, Log, TEXT("ConstructionModeBorderWidget: Mode changed, new color: R=%f G=%f B=%f A=%f"),
			TargetColor.R, TargetColor.G, TargetColor.B, TargetColor.A);
	}

	// Smoothly interpolate to target color
	CurrentColor = FMath::LerpStable(CurrentColor, TargetColor, DeltaTime * ColorTransitionSpeed);

	// Update all border colors
	if (TopBorder) TopBorder->SetBrushColor(CurrentColor);
	if (BottomBorder) BottomBorder->SetBrushColor(CurrentColor);
	if (LeftBorder) LeftBorder->SetBrushColor(CurrentColor);
	if (RightBorder) RightBorder->SetBrushColor(CurrentColor);
}

FLinearColor UConstructionModeBorderWidget::GetTargetColorForCurrentMode() const
{
	if (!OwnerCharacter)
	{
		return NormalModeColor;
	}

	// Check modes in priority order
	// Delete mode has highest visual priority (red warning)
	if (OwnerCharacter->IsInDeleteMode())
	{
		return DeleteModeColor;
	}

	// Move mode second priority (yellow/orange)
	if (OwnerCharacter->IsInMoveMode())
	{
		return MoveModeColor;
	}

	// Construction mode third priority (blue)
	if (OwnerCharacter->IsInConstructionMode())
	{
		return ConstructionModeColor;
	}

	// Normal mode (transparent)
	return NormalModeColor;
}
