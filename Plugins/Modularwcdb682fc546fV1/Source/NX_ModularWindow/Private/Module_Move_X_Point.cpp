// Copyright Nyaunix 2025 All Rights Reserved

#include "Module_Move_X_Point.h"
#include "Blueprint/WidgetLayoutLibrary.h"





void UModule_Move_X_Point::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	StartPosition =
		UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X
		-
		InWindow->GetPosition().X;
}


void UModule_Move_X_Point::Move(UWindow* InWindow) {
	InWindow->SetPositionX(
		UWidgetLayoutLibrary::GetMousePositionOnViewport(InWindow).X
		-
		StartPosition
	);
}



