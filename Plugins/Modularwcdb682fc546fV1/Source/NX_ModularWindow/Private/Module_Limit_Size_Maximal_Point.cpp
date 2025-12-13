// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_Size_Maximal_Point.h"
#include "Components/Border.h"

float UModule_Limit_Size_Maximal_Point::ModifyLeft(UWidget* InWindow, float InValue)
{
	float LLocalFloat = 0 - CanvasSizeMinimum.X;
	if (InValue > LLocalFloat) return InValue;
	Block_X = true;
	return LLocalFloat;
}

void UModule_Limit_Size_Maximal_Point::Prepare(UWidget* InWindow)
{
	Super::Prepare(InWindow);
	FVector2D LCanvasSize = InWindow->GetParent()->GetCachedGeometry().GetLocalSize();
	CanvasSizeMinimum = LCanvasSize * Cast<UCanvasPanelSlot>(InWindow->Slot)->GetAnchors().Minimum;
	AnchorModify = LCanvasSize - CanvasSizeMinimum;
}

float UModule_Limit_Size_Maximal_Point::ModifyUp(UWidget* InWindow, float InValue)
{
	float LLocalFloat = 0 - CanvasSizeMinimum.Y;
	if (InValue > LLocalFloat) return InValue;
	Block_Y = true;
	return LLocalFloat;
}



float UModule_Limit_Size_Maximal_Point::ModifyRight(UWidget* InWindow, float InValue)
{
	if (Block_X) {
		Block_X = false;
		return
			Cast<UCanvasPanelSlot>(InWindow->Slot)->GetPosition().X
			+
			Cast<UWindow>(InWindow)->GetPositionStart().X
			+
			Cast<UWindow>(InWindow)->GetSizeStart().X
			+
			(CanvasSizeMinimum.X * 2);
	}
	return FMath::Min(
		InValue,
		AnchorModify.X 
		- 
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetPosition().X);
}

float UModule_Limit_Size_Maximal_Point::ModifyDown(UWidget* InWindow, float InValue)
{
	if (Block_Y) {
		Block_Y = false;
		return
			static_cast<float>(Cast<UCanvasPanelSlot>(InWindow->Slot)->GetPosition().Y)
			+
			Cast<UWindow>(InWindow)->GetPositionStart().Y
			+
			Cast<UWindow>(InWindow)->GetSizeStart().Y
			+
			(CanvasSizeMinimum.Y * 2);
	}
	return FMath::Min(
		InValue,
		AnchorModify.Y
		-
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetPosition().Y);
}
