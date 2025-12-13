// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_Up_Line.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Size_Up_Line::Prepare(UWindow* InWindow)
{
	Difference =
		UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y
		-
		InWindow->GetPosition().Y;
}
void UModule_Size_Up_Line::Move(UWindow* InWindow)
{
	InWindow->SetByParameterSize(
		TArray<FName>{"Position_Y"},
		TArray<float>{
			static_cast<float>(UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y)
			-
			Difference}
	);
}

