// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_Position_Line.h"


void UModule_Limit_Position_Line::Prepare(UWidget* InWindow)
{
	PositionMax = 
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetPosition() 
		+ 
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetSize();
}

TArray<float> UModule_Limit_Position_Line::ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues)
{
	TArray<float> Making;
	int i = 0;
	for (FName n : InNames) {
		if (n == "Position_X") {
			Making.Add(
				ModifyPosition(
					FVector2D(InValues[i], 0),
					FVector2D(0, 0)
				).X);
		}
		else if (n == "Position_Y") {
			Making.Add(
				ModifyPosition(
					FVector2D(0, InValues[i]),
					FVector2D(0, 0)
				).Y);
		}
		else if (n == "Size_X") {
			Making.Add(
				ModifyPosition(
					FVector2D(0, 0),
					FVector2D(InValues[i], 0)
				).Z);
		}
		else if (n == "Size_Y") {
			Making.Add(
				ModifyPosition(
					FVector2D(0, 0),
					FVector2D(0, InValues[i])
				).W);
		}
		else Making.Add(0);
		i++;
	}
	return Making;
}

FVector4 UModule_Limit_Position_Line::ModifyPosition(FVector2D InPosition, FVector2D InSize)
{
	return FVector4(
			FMath::Clamp(InPosition.X, 0, PositionMax.X),
			FMath::Clamp(InPosition.Y, 0, PositionMax.Y),
			FMath::Clamp(InSize.X, 0, PositionMax.X),
			FMath::Clamp(InSize.Y, 0, PositionMax.Y)
	);
}


