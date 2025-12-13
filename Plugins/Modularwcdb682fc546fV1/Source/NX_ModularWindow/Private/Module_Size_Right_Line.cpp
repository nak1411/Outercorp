// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_Right_Line.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Size_Right_Line::Prepare(UWindow* InWindow)
{
	CursorStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X;
	WindowSizeStart = InWindow->GetSize().X;
}
void UModule_Size_Right_Line::Move(UWindow* InWindow)
{
	InWindow->SetByParameterSize(
		TArray<FName>{"Size_X"},
		TArray<float>{
			CursorStart
			-
			static_cast<float>(UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X)
			+
			WindowSizeStart}
	);
}

