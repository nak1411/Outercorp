// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Fullscreen_None.h"

void UModule_Fullscreen_None::Action()
{
	Super::Action();
	if (IsFullscreen(Window)) Unscreen(Window);
	else Fullscreen(Window);
}

void UModule_Fullscreen_None::Deaction()
{
	Unscreen(Window);
	Super::Deaction();
}

void UModule_Fullscreen_None::SaveParameters(UWidget* InWidget)
{
	PositionSaved = Cast<UCanvasPanelSlot>(InWidget->Slot)->GetPosition();
	SizeSaved = Cast<UCanvasPanelSlot>(InWidget->Slot)->GetSize();
}

bool UModule_Fullscreen_None::IsFullscreen(UWidget* InWindow)
{
	return false;
}

void UModule_Fullscreen_None::Fullscreen(UWidget* InWindow)
{
	SaveParameters(InWindow);
}

void UModule_Fullscreen_None::Unscreen(UWidget* InWindow)
{
	Cast<UCanvasPanelSlot>(InWindow->Slot)->SetSize(SizeSaved);
	Cast<UCanvasPanelSlot>(InWindow->Slot)->SetPosition(PositionSaved);
}

