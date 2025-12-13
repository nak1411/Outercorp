// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_Down_Line.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Size_Down_Line::Prepare(UWindow* InWindow)
{
	CursorStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y;
	WindowSizeStart = InWindow->GetSize().Y;
}
void UModule_Size_Down_Line::Move(UWindow* InWindow)
{
	InWindow->SetByParameterSize(
		TArray<FName>{"Size_Y"},
		TArray<float>{
		CursorStart
		-
		static_cast<float>(UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y)
		+
		WindowSizeStart});
}

