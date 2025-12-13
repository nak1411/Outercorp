// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_Size_Universal.h"
#include "Module_Limit_Size_Minimal_Point.h"
#include "Module_Limit_Size_Minimal_Line.h"
#include "Module_Limit_Size_Maximal_Point.h"
#include "Module_Limit_Size_Maximal_Line.h"

void UModule_Limit_Size_Universal::SetWindow(UWindow* InWindow)
{
	Super::SetWindow(InWindow);

	if (!IsValid(Point)) {
		if (!SizeMinimal.IsNearlyZero(0.1)) {
			Point = NewObject<UModule_Limit_Size_Minimal_Point>(this);
			Cast<UModule_Limit_Size_Minimal_Point>(Point)->SizeMinimal = SizeMinimal;
		}
		else {
			Point = NewObject<UModule_Limit_Size_Maximal_Point>(this);
		}
	}
	if (!IsValid(Line)) {
		if (!SizeMinimal.IsNearlyZero(0.1)) {
			Line = NewObject<UModule_Limit_Size_Minimal_Line>(this);
			Cast<UModule_Limit_Size_Minimal_Line>(Line)->SizeMinimal = SizeMinimal;
		}
		else {
			Line = NewObject<UModule_Limit_Size_Maximal_Line>(this);
		}
	}
}


void UModule_Limit_Size_Universal::Prepare(UWidget* InWindow)
{
	Point->Prepare(InWindow);
	Line->Prepare(InWindow);
}

TArray<float> UModule_Limit_Size_Universal::ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues)
{
	TArray<float> ToReturn;
	UModule_Limit_None* Limiter;
	int i = 0;
	for (FName CurrentName : InNames) {
		if (Cast<UWindow>(InWindow)->IsAnchorPoint(CurrentName.ToString().EndsWith("X"))) {
			Limiter = Point;
		}
		else {
			Limiter = Line;
		}

		ToReturn.Append(
			Limiter->ModifyByParameter(
				InWindow,
				TArray<FName>{CurrentName},
				TArray<float>{InValues[i]})
		);
		i++;
	}
	return ToReturn;
}

void UModule_Limit_Size_Universal::SetSizeMinimal(FVector2D InSizeMinimal)
{
	SizeMinimal = InSizeMinimal;
	if (!IsValid(Point)) return;
	if (Point->GetClass()->IsChildOf(UModule_Limit_Size_Minimal_Point::StaticClass())){
		Cast<UModule_Limit_Size_Minimal_Point>(Point)->SizeMinimal = SizeMinimal;
	}
	if (!IsValid(Line)) return;
	if (Line->GetClass()->IsChildOf(UModule_Limit_Size_Minimal_Line::StaticClass())){
		Cast<UModule_Limit_Size_Minimal_Line>(Line)->SizeMinimal = SizeMinimal;
	}
}
