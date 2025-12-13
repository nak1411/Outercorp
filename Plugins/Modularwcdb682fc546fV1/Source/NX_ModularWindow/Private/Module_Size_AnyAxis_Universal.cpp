// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_AnyAxis_Universal.h"
#include "Module_Size_Up_Point.h"
#include "Module_Size_Up_Line.h"
#include "Module_Size_Right_Point.h"
#include "Module_Size_Right_Line.h"
#include "Module_Size_Down_Point.h"
#include "Module_Size_Down_Line.h"
#include "Module_Size_Left_Point.h"
#include "Module_Size_Left_Line.h"

void UModule_Size_AnyAxis_Universal::SetWindow(UWindow* InWindow) {
	Super::SetWindow(InWindow);

	TSubclassOf<UModule_Size_None> LPoint;
	TSubclassOf<UModule_Size_None> LLine;
	if (Side == 0) {
		LPoint = UModule_Size_Up_Point::StaticClass();
		LLine = UModule_Size_Up_Line::StaticClass();
	}
	else if (Side == 1) { 
		LPoint = UModule_Size_Right_Point::StaticClass();
		LLine = UModule_Size_Right_Line::StaticClass();
	}
	else if (Side == 2) {
		LPoint = UModule_Size_Down_Point::StaticClass();
		LLine = UModule_Size_Down_Line::StaticClass();
	}
	else if (Side == 3) {
		LPoint = UModule_Size_Left_Point::StaticClass();
		LLine = UModule_Size_Left_Line::StaticClass();
	}

	Point = NewObject<UModule_Size_None>(this, LPoint);
	Line = NewObject<UModule_Size_None>(this, LLine);

}

void UModule_Size_AnyAxis_Universal::Prepare(UWindow* InWindow) {
	Super::Prepare(InWindow);
	if (InWindow->IsAnchorPoint(Side == 1 || Side == 3)) {
		Point->Prepare(InWindow);
	}
	else {
		Line->Prepare(InWindow);
	}
}
void UModule_Size_AnyAxis_Universal::Move(UWindow* InWindow) {
	Super::Move(InWindow);
	if (InWindow->IsAnchorPoint(Side == 1 || Side == 3)) {
		Point->Move(InWindow);
	}
	else {
		Line->Move(InWindow);
	}
}

void UModule_Size_AnyAxis_Universal::SetSide(uint8 InData)
{
	Side = InData;
	if (IsValid(Window)) {
		SetWindow(Window);
	}
}
