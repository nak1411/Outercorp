// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_Position_Point.h"
#include "Components/Border.h"

TArray<float> UModule_Limit_Position_Point::ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues)
{
	TArray<float> LMaking;
	int i = 0;
	for (FName n : InNames) {
		if (n == "Position_X") {
			LMaking.Add(ModifyPosition(InWindow, FVector2D(InValues[i], InValues[i])).X);
		}
		else if (n == "Position_Y") {
			LMaking.Add(ModifyPosition(InWindow, FVector2D(InValues[i], InValues[i])).Y);
		}
		i++;
	}

	return LMaking;
}

FVector2D UModule_Limit_Position_Point::ModifyPosition(UWidget* InWindow, FVector2D InVector)
{
	FVector2D LCanvasSize =
		InWindow->GetParent()->GetCachedGeometry().GetLocalSize();
	FVector2D LCanvasSizeMinimum =
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetAnchors().Minimum
		* 
		LCanvasSize;



	FVector2D ClampMinimum =
		LCanvasSize
		-
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetSize()
		-
		LCanvasSizeMinimum;
	FVector2D ClampMaximum =  -LCanvasSizeMinimum;
	

	InVector.X = FMath::Clamp(InVector.X, ClampMaximum.X, ClampMinimum.X);
	InVector.Y = FMath::Clamp(InVector.Y, ClampMaximum.Y, ClampMinimum.Y);
	return InVector;
}