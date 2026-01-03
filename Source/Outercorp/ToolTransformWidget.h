// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolTransformWidget.generated.h"

class AOutercorpCharacter;
class AEquippableTool;
class USlider;
class UCheckBox;
class UButton;

/**
 * Widget for adjusting equipped tool transforms in real-time
 */
UCLASS()
class OUTERCORP_API UToolTransformWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// === Widget Bindings (matching your hierarchy) ===

	/** X Position Slider */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* XSlider;

	/** Y Position Slider */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* YSlider;

	/** Z Position Slider */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* ZSlider;

	/** Pitch Rotation Slider */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* PitchSlider;

	/** Yaw Rotation Slider */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* YawSlider;

	/** Roll Rotation Slider */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* RollSlider;

	/** Auto Apply Checkbox */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* AutoApply;

	/** Toggle Debug Button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ToggleDebugButton;

	/** Apply Button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ApplyButton;

	/** Character reference */
	UPROPERTY(BlueprintReadWrite, Category = "Tool Transform")
	AOutercorpCharacter* CharacterRef;

	/** Current tool being adjusted */
	UPROPERTY(BlueprintReadOnly, Category = "Tool Transform")
	AEquippableTool* CurrentTool;

	// === Transform Values ===

	UPROPERTY(BlueprintReadWrite, Category = "Tool Transform")
	float LocationX = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Tool Transform")
	float LocationY = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Tool Transform")
	float LocationZ = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Tool Transform")
	float RotationPitch = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Tool Transform")
	float RotationYaw = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Tool Transform")
	float RotationRoll = 0.0f;

	// === Blueprint Callable Functions ===

	/** Apply current transform values to the tool */
	UFUNCTION(BlueprintCallable, Category = "Tool Transform")
	void ApplyTransform();

	/** Load current tool's transform into the widget values */
	UFUNCTION(BlueprintCallable, Category = "Tool Transform")
	void LoadCurrentTransform();

	/** Print current transform to log */
	UFUNCTION(BlueprintCallable, Category = "Tool Transform")
	void PrintTransform();

	/** Toggle debug visualization */
	UFUNCTION(BlueprintCallable, Category = "Tool Transform")
	void ToggleDebugViz();

	/** Save transform to item data asset */
	void SaveTransformToItemData();

	/** Reset transform to identity */
	UFUNCTION(BlueprintCallable, Category = "Tool Transform")
	void ResetTransform();

	/** Called when any slider value changes */
	UFUNCTION(BlueprintCallable, Category = "Tool Transform")
	void OnSliderValueChanged(float Value);

protected:
	/** Get the equipped tool from the character */
	AEquippableTool* GetEquippedTool();

	/** Bind widget events */
	void BindWidgetEvents();

	/** Slider callback handlers */
	UFUNCTION()
	void OnXSliderChanged(float Value);

	UFUNCTION()
	void OnYSliderChanged(float Value);

	UFUNCTION()
	void OnZSliderChanged(float Value);

	UFUNCTION()
	void OnPitchSliderChanged(float Value);

	UFUNCTION()
	void OnYawSliderChanged(float Value);

	UFUNCTION()
	void OnRollSliderChanged(float Value);

	/** Button callback handlers */
	UFUNCTION()
	void OnApplyButtonClicked();

	UFUNCTION()
	void OnToggleDebugButtonClicked();

	/** Checkbox callback handler */
	UFUNCTION()
	void OnAutoApplyChanged(bool bIsChecked);
};
