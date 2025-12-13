// Copyright Nyaunix 2025 All Rights Reserved

#include "Module_Move_OneAxis_Universal.h"
#include "Module_Move_X_Point.h"
#include "Module_Move_Y_Point.h"
#include "Module_Move_X_Line.h"
#include "Module_Move_Y_Line.h"

void UModule_Move_OneAxis_Universal::SetWindow(UWindow* InWindow) {
	Super::SetWindow(InWindow);
	if (!IsValid(Point)) {
		if (!IsY) {
			Point = NewObject<UModule_Move_X_Point>(this);
		}
		else {
			Point = NewObject<UModule_Move_Y_Point>(this);
		}
	}
	if (!IsValid(Line)) {
		if (!IsY) {
			Line = NewObject<UModule_Move_X_Line>(this);
		}
		else {
			Line = NewObject<UModule_Move_Y_Line>(this);
		}
	}
}

void UModule_Move_OneAxis_Universal::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	if (InWindow->IsAnchorPoint(!IsY)) {
		Point->Prepare(InWindow);
	}
	else {
		Line->Prepare(InWindow);
	}
}
void UModule_Move_OneAxis_Universal::Move(UWindow* InWindow) {
	Super::Move(InWindow);
	if (InWindow->IsAnchorPoint(!IsY)) {
		Point->Move(InWindow);
	}
	else {
		Line->Move(InWindow);
	}
}

