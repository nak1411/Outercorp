// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Fullscreen_Universal.h"
#include "Module_Fullscreen_Point.h"
#include "Module_Fullscreen_Line.h"

void UModule_Fullscreen_Universal::SetWindow(UWindow* InWindow)
{
	Super::SetWindow(InWindow);
	if (!IsValid(Point)) {
		Point = NewObject<UModule_Fullscreen_Point>(this);
	}
	if (!IsValid(Line)) {
		Line = NewObject<UModule_Fullscreen_Line>(this);
	}
}

void UModule_Fullscreen_Universal::Action()
{
	if (Window->IsAnchorPoint(true)) {
		if (Point->IsFullscreen(Window)) Point->Unscreen(Window);
		else Point->Fullscreen(Window);
	}
	else {
		if (Line->IsFullscreen(Window)) Line->Unscreen(Window);
		else Line->Fullscreen(Window);
	}
}

void UModule_Fullscreen_Universal::Deaction()
{
	if (Window->IsAnchorPoint(true)) {
		if (Point->IsFullscreen(Window)) Point->Unscreen(Window);
	}
	else {
		if (Line->IsFullscreen(Window)) Line->Unscreen(Window);
	}
}
