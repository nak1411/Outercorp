// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_Up_Point.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Size_Up_Point::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	CursorStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y;
	PositionDifference = CursorStart - InWindow->GetPosition().Y;
}
void UModule_Size_Up_Point::Move(UWindow* InWindow) {
	Super::Move(InWindow);
	float LCursorPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y;
	InWindow->SetByParameterSize(
		TArray<FName>{"Position_Y"},
		TArray<float>{LCursorPosition - PositionDifference});
	InWindow->SetByParameterSize(
		TArray<FName>{"Size_Y"},
		TArray<float>{
			CursorStart - 
			LCursorPosition + 
			static_cast<float>(InWindow->GetSizeStart().Y)});
}