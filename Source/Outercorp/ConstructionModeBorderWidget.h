// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "ConstructionModeBorderWidget.generated.h"

class AOutercorpCharacter;

/**
 * Visual border widget that changes color based on the current construction mode
 * Displays at screen edges to provide clear visual feedback of active mode
 */
UCLASS()
class OUTERCORP_API UConstructionModeBorderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Top border edge
	UPROPERTY(meta = (BindWidget))
	UBorder* TopBorder;

	// Bottom border edge
	UPROPERTY(meta = (BindWidget))
	UBorder* BottomBorder;

	// Left border edge
	UPROPERTY(meta = (BindWidget))
	UBorder* LeftBorder;

	// Right border edge
	UPROPERTY(meta = (BindWidget))
	UBorder* RightBorder;

	// Color for normal gameplay mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode Colors")
	FLinearColor NormalModeColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f); // Transparent

	// Color for construction mode (placing new parts)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode Colors")
	FLinearColor ConstructionModeColor = FLinearColor(0.0f, 0.5f, 1.0f, 0.6f); // Blue

	// Color for delete mode (removing parts)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode Colors")
	FLinearColor DeleteModeColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.6f); // Red

	// Color for move mode (relocating parts)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode Colors")
	FLinearColor MoveModeColor = FLinearColor(1.0f, 0.8f, 0.0f, 0.6f); // Yellow/Orange

	// Border thickness in pixels
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Border Settings")
	float BorderThickness = 8.0f;

	// Smooth color transition speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Border Settings")
	float ColorTransitionSpeed = 5.0f;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// Reference to the player character
	UPROPERTY()
	AOutercorpCharacter* OwnerCharacter;

	// Current color being displayed (for smooth transitions)
	FLinearColor CurrentColor;

	// Target color to transition to
	FLinearColor TargetColor;

	// Updates the border color based on current mode
	void UpdateBorderColor(float DeltaTime);

	// Gets the target color based on active mode
	FLinearColor GetTargetColorForCurrentMode() const;
};
