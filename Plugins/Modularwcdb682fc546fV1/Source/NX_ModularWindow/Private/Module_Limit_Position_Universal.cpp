// Copyright Nyaunix 2025 All Rights Reserved
#include "Module_Limit_Position_Universal.h"
#include "Window.h"

void UModule_Limit_Position_Universal::SetWindow(UWindow* InWindow)
{
    Super::SetWindow(InWindow);

    // Creating modules_limit
    if (!IsValid(LimitPoint)) {
        LimitPoint = NewObject<UModule_Limit_Position_Point>(this);
    }
    if (!IsValid(LimitLine)) {
        LimitLine = NewObject<UModule_Limit_Position_Line>(this);
    }

 }



void UModule_Limit_Position_Universal::Prepare(UWidget* InWindow)
{
	Super::Prepare(InWindow);
	LimitPoint->Prepare(InWindow);
	LimitLine->Prepare(InWindow);
}

TArray<float> UModule_Limit_Position_Universal::ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues)
{
	TArray<float> ReturnValues;
	

	//For load in Limiters
	TArray<FName> LAName = { " " };
	TArray<float> LAFloat = { 0 };
	int I = 0;
	for (FName CurrentName : InNames) {

		//For load in Limiters
		LAName[0] = CurrentName;
		LAFloat[0] = InValues[I];

		//Select LimiterType for "ModifyByParameter"
		if (Cast<UWindow>(InWindow)->IsAnchorPoint(CurrentName.ToString().EndsWith("X"))) {
			ReturnValues.Append(
				LimitPoint->ModifyByParameter(InWindow, LAName, LAFloat));
		}
		else {
			ReturnValues.Append(
				LimitLine->ModifyByParameter(InWindow, LAName, LAFloat));
		}
		I++;
	}
	return ReturnValues;
}
