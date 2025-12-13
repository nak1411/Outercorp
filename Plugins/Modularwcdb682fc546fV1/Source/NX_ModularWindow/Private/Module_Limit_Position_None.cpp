// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_Position_None.h"

void UModule_Limit_Position_None::SetWindow(UWindow* InWindow)
{
	Super::SetWindow(InWindow);
	InWindow->ED_PositionStart.AddDynamic(this, &UModule_Limit_Position_None::Action);
	InWindow->Delegate_Position.Event_Bind_Add(this, &UModule_Limit_None::ModifyByParameter);
}

void UModule_Limit_Position_None::RemoveWindow()
{
	Window->ED_PositionStart.RemoveDynamic(this, &UModule_Limit_Position_None::Action);
	Window->Delegate_Position.Event_Bind_Remove(this, &UModule_Limit_None::ModifyByParameter);

	Super::RemoveWindow();
}
