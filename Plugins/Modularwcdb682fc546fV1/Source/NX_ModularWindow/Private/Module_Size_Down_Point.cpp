// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_Down_Point.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Size_Down_Point::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	Difference =
		UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y
		-
		InWindow->GetSize().Y;
}
void UModule_Size_Down_Point::Move(UWindow* InWindow) {
	Super::Move(InWindow);
	InWindow->SetByParameterSize(
		TArray<FName>{"Size_Y"},
		TArray<float>{
			static_cast<float>(UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y)
			-
			Difference});
}