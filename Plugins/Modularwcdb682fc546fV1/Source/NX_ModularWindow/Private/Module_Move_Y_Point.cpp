// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Move_Y_Point.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UModule_Move_Y_Point::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	StartPosition =
		UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y
		-
		InWindow->GetPosition().Y;
}


void UModule_Move_Y_Point::Move(UWindow* InWindow) {
	InWindow->SetPositionY(
		UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).Y
		-
		StartPosition
	);
}
