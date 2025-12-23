// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Fullscreen_None.h"
#include "Window.h"

void UModule_Fullscreen_None::Action()
{
	// Check if window allows fullscreen
	UWindow* WindowPtr = Cast<UWindow>(Window);
	if (WindowPtr && !WindowPtr->CanFullscreen())
	{
		return; // Don't allow fullscreen action
	}

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

