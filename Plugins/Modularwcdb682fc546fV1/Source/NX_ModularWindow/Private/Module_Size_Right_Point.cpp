// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_Right_Point.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Size_Right_Point::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	Difference =
		UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X
		-
		InWindow->GetSize().X;
}
void UModule_Size_Right_Point::Move(UWindow* InWindow) {
	Super::Move(InWindow);
	InWindow->SetByParameterSize(
		TArray<FName>{"Size_X"},
		TArray<float>{
			static_cast<float>(UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X)
			- 
			Difference}
	);
}