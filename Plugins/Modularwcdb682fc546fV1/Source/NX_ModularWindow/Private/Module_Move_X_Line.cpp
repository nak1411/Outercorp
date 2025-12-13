// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Move_X_Line.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Move_X_Line::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	CursorPositionStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X;
	PositionDifference = CursorPositionStart - InWindow->GetPosition().X;
	WindowSizeStart = InWindow->GetSize().X;
}

void UModule_Move_X_Line::Move(UWindow* InWindow) {
	float LCurrentPositionCursor = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X;
	InWindow->SetByParameterPosition(
		TArray<FName>{"Position_X"},
		TArray<float>{LCurrentPositionCursor - PositionDifference});
	InWindow->SetByParameterPosition(
		TArray<FName>{"Size_X"},
		TArray<float>{CursorPositionStart - LCurrentPositionCursor + WindowSizeStart});
}