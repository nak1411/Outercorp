// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToolTransformWidget.h"
#include "OutercorpCharacter.h"
#include "EquippableTool.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"

void UToolTransformWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Get player character
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		CharacterRef = Cast<AOutercorpCharacter>(PlayerController->GetPawn());
	}

	// Bind widget events
	BindWidgetEvents();

	// Load current tool's transform if one is equipped
	LoadCurrentTransform();
}

void UToolTransformWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update current tool reference
	CurrentTool = GetEquippedTool();
}

AEquippableTool* UToolTransformWidget::GetEquippedTool()
{
	if (!CharacterRef)
	{
		return nullptr;
	}

	return CharacterRef->GetEquippedTool();
}

void UToolTransformWidget::BindWidgetEvents()
{
	// Bind slider events
	if (XSlider)
	{
		XSlider->OnValueChanged.AddDynamic(this, &UToolTransformWidget::OnXSliderChanged);
	}

	if (YSlider)
	{
		YSlider->OnValueChanged.AddDynamic(this, &UToolTransformWidget::OnYSliderChanged);
	}

	if (ZSlider)
	{
		ZSlider->OnValueChanged.AddDynamic(this, &UToolTransformWidget::OnZSliderChanged);
	}

	if (PitchSlider)
	{
		PitchSlider->OnValueChanged.AddDynamic(this, &UToolTransformWidget::OnPitchSliderChanged);
	}

	if (YawSlider)
	{
		YawSlider->OnValueChanged.AddDynamic(this, &UToolTransformWidget::OnYawSliderChanged);
	}

	if (RollSlider)
	{
		RollSlider->OnValueChanged.AddDynamic(this, &UToolTransformWidget::OnRollSliderChanged);
	}

	// Bind button events
	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UToolTransformWidget::OnApplyButtonClicked);
	}

	if (ToggleDebugButton)
	{
		ToggleDebugButton->OnClicked.AddDynamic(this, &UToolTransformWidget::OnToggleDebugButtonClicked);
	}

	// Bind checkbox event
	if (AutoApply)
	{
		AutoApply->OnCheckStateChanged.AddDynamic(this, &UToolTransformWidget::OnAutoApplyChanged);
	}
}

void UToolTransformWidget::OnXSliderChanged(float Value)
{
	LocationX = Value;
	OnSliderValueChanged(Value);
}

void UToolTransformWidget::OnYSliderChanged(float Value)
{
	LocationY = Value;
	OnSliderValueChanged(Value);
}

void UToolTransformWidget::OnZSliderChanged(float Value)
{
	LocationZ = Value;
	OnSliderValueChanged(Value);
}

void UToolTransformWidget::OnPitchSliderChanged(float Value)
{
	RotationPitch = Value;
	OnSliderValueChanged(Value);
}

void UToolTransformWidget::OnYawSliderChanged(float Value)
{
	RotationYaw = Value;
	OnSliderValueChanged(Value);
}

void UToolTransformWidget::OnRollSliderChanged(float Value)
{
	RotationRoll = Value;
	OnSliderValueChanged(Value);
}

void UToolTransformWidget::OnSliderValueChanged(float Value)
{
	// If auto apply is enabled, apply the transform immediately
	if (AutoApply && AutoApply->IsChecked())
	{
		ApplyTransform();
	}
}

void UToolTransformWidget::OnApplyButtonClicked()
{
	ApplyTransform();
}

void UToolTransformWidget::OnToggleDebugButtonClicked()
{
	ToggleDebugViz();
}

void UToolTransformWidget::OnAutoApplyChanged(bool bIsChecked)
{
	// Auto apply state changed - no additional action needed
	// The OnSliderValueChanged function will check AutoApply->IsChecked()
}

void UToolTransformWidget::ApplyTransform()
{
	AEquippableTool* Tool = GetEquippedTool();
	if (!Tool)
	{
		UE_LOG(LogTemp, Warning, TEXT("No tool equipped to adjust"));
		return;
	}

	FVector Location(LocationX, LocationY, LocationZ);
	FRotator Rotation(RotationPitch, RotationYaw, RotationRoll);
	FVector Scale(1.0f, 1.0f, 1.0f);

	Tool->SetToolRelativeTransform(Location, Rotation, Scale);

	// Save the transform to the item data so it persists
	SaveTransformToItemData();
}

void UToolTransformWidget::SaveTransformToItemData()
{
	AEquippableTool* Tool = GetEquippedTool();
	if (!Tool || !CharacterRef)
	{
		return;
	}

	// Get the equipped tool's item data from the character
	UInventoryItemData* ItemData = CharacterRef->GetEquippedToolItemData();
	if (ItemData)
	{
		// Save the current transform
		FTransform CurrentTransform;
		CurrentTransform.SetLocation(FVector(LocationX, LocationY, LocationZ));
		CurrentTransform.SetRotation(FRotator(RotationPitch, RotationYaw, RotationRoll).Quaternion());
		CurrentTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		ItemData->EquippedRelativeTransform = CurrentTransform;

		// Mark the package as dirty so it can be saved
		ItemData->MarkPackageDirty();
	}
}

void UToolTransformWidget::LoadCurrentTransform()
{
	AEquippableTool* Tool = GetEquippedTool();
	if (!Tool)
	{
		return;
	}

	FTransform CurrentTransform = Tool->GetToolRelativeTransform();

	FVector Loc = CurrentTransform.GetLocation();
	LocationX = Loc.X;
	LocationY = Loc.Y;
	LocationZ = Loc.Z;

	FRotator Rot = CurrentTransform.GetRotation().Rotator();
	RotationPitch = Rot.Pitch;
	RotationYaw = Rot.Yaw;
	RotationRoll = Rot.Roll;

	// Update slider values to match loaded transform
	if (XSlider) XSlider->SetValue(LocationX);
	if (YSlider) YSlider->SetValue(LocationY);
	if (ZSlider) ZSlider->SetValue(LocationZ);
	if (PitchSlider) PitchSlider->SetValue(RotationPitch);
	if (YawSlider) YawSlider->SetValue(RotationYaw);
	if (RollSlider) RollSlider->SetValue(RotationRoll);
}

void UToolTransformWidget::PrintTransform()
{
	AEquippableTool* Tool = GetEquippedTool();
	if (!Tool)
	{
		UE_LOG(LogTemp, Warning, TEXT("No tool equipped"));
		return;
	}

	Tool->PrintCurrentTransform();
}

void UToolTransformWidget::ToggleDebugViz()
{
	AEquippableTool* Tool = GetEquippedTool();
	if (!Tool)
	{
		UE_LOG(LogTemp, Warning, TEXT("No tool equipped"));
		return;
	}

	Tool->ToggleDebugVisualization();
}

void UToolTransformWidget::ResetTransform()
{
	LocationX = 0.0f;
	LocationY = 0.0f;
	LocationZ = 0.0f;
	RotationPitch = 0.0f;
	RotationYaw = 0.0f;
	RotationRoll = 0.0f;

	// Update slider values
	if (XSlider) XSlider->SetValue(0.0f);
	if (YSlider) YSlider->SetValue(0.0f);
	if (ZSlider) ZSlider->SetValue(0.0f);
	if (PitchSlider) PitchSlider->SetValue(0.0f);
	if (YawSlider) YawSlider->SetValue(0.0f);
	if (RollSlider) RollSlider->SetValue(0.0f);

	if (AutoApply && AutoApply->IsChecked())
	{
		ApplyTransform();
	}
}
