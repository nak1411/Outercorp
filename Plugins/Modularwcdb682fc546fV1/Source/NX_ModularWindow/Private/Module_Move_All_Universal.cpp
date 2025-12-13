// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Move_All_Universal.h"
#include "Module_Move_OneAxis_Universal.h"


void UModule_Move_All_Universal::SetWindow(UWindow* InWindow) {
	Super::SetWindow(InWindow);
	if (!IsValid(Module_X)) {
		Module_X = NewObject<UModule_Move_OneAxis_Universal>(this);
	}
	if (!IsValid(Module_Y)) {
		Module_Y = NewObject<UModule_Move_OneAxis_Universal>(this);
		Cast<UModule_Move_OneAxis_Universal>(Module_Y)->IsY = true;
	}
	Module_X->SetWindow(nullptr);
	Module_Y->SetWindow(nullptr);
}
void UModule_Move_All_Universal::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	Module_X->Prepare(InWindow);
	Module_Y->Prepare(InWindow);
}
void UModule_Move_All_Universal::Move(UWindow* InWindow) {
	Super::Move(InWindow);
	Module_X->Move(InWindow);
	Module_Y->Move(InWindow);
}