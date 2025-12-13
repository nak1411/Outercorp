// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_Left_Line.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Size_Left_Line::Prepare(UWindow* InWindow)
{
	Difference = 
		UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X
		-
		InWindow->GetPosition().X;
}
void UModule_Size_Left_Line::Move(UWindow* InWindow)
{
	InWindow->SetByParameterSize(
		TArray<FName>{"Position_X"},
		TArray<float>{
			static_cast<float>(UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X)
			-
			Difference}
	);
}

