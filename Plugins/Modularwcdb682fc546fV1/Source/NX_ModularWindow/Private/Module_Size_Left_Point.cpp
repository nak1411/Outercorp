// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_Left_Point.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Size_Left_Point::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	CursorStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X;
	PositionDifference = CursorStart - InWindow->GetPosition().X;
}
void UModule_Size_Left_Point::Move(UWindow* InWindow) {
	Super::Move(InWindow);
	float LCursorPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X;
	InWindow->SetByParameterSize(
		TArray<FName>{"Position_X"},
		TArray<float>{LCursorPosition - PositionDifference});
	InWindow->SetByParameterSize(
		TArray<FName>{"Size_X"},
		TArray<float>{
			CursorStart 
			- 
			LCursorPosition 
			+ 
			static_cast<float>(InWindow->GetSizeStart().X)});
}