// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_Size_Minimal_Point.h"


float UModule_Limit_Size_Minimal_Point::ModifyLeft(UWidget* InWindow, float InValue)
{
	return FMath::Min(
		InValue,
		Cast<UWindow>(InWindow)->GetPositionStart().X
		+
		Cast<UWindow>(InWindow)->GetSizeStart().X
		-
		SizeMinimal.X);
}

float UModule_Limit_Size_Minimal_Point::ModifyUp(UWidget* InWindow, float InValue)
{
	return FMath::Min(
		InValue,
		Cast<UWindow>(InWindow)->GetPositionStart().Y
		+
		Cast<UWindow>(InWindow)->GetSizeStart().Y
		-
		SizeMinimal.Y);
}

float UModule_Limit_Size_Minimal_Point::ModifyRight(UWidget* InWindow, float InValue)
{
	return FMath::Max(InValue, SizeMinimal.X);
}

float UModule_Limit_Size_Minimal_Point::ModifyDown(UWidget* InWindow, float InValue)
{
	return FMath::Max(InValue, SizeMinimal.Y);
}
