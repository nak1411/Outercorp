// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Move_Y_Line.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Move_Y_Line::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	CursorPositionStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y;
	PositionDifference = CursorPositionStart - InWindow->GetPosition().Y;
	WindowSizeStart = InWindow->GetSize().Y;
}

void UModule_Move_Y_Line::Move(UWindow* InWindow) {
	float LCurrentPositionCursor = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y;
	InWindow->SetByParameterPosition(
		TArray<FName>{"Position_Y"},
		TArray<float>{LCurrentPositionCursor - PositionDifference});
	InWindow->SetByParameterPosition(
		TArray<FName>{"Size_Y"},
		TArray<float>{CursorPositionStart - LCurrentPositionCursor + WindowSizeStart});

}
