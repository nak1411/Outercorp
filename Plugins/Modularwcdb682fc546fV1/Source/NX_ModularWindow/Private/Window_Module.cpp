// Copyright Nyaunix 2025 All Rights Reserved
#include "Window_Module.h"
#include "Window.h"
#include "Blueprint/WidgetLayoutLibrary.h"



UWindow* UWindow_Module::GetWindow()
{
	return Window;
}

void UWindow_Module::SetWindow(UWindow* InWindow)
{
	Window = InWindow;
}

void UWindow_Module::RemoveWindow()
{
	Window = nullptr;
}





//Action
void UWindow_Module::Action()
{
	Window->ED_InteractStart.Broadcast();
}

void UWindow_Module::Deaction()
{
	Window->ED_InteractEnd.Broadcast();
}


