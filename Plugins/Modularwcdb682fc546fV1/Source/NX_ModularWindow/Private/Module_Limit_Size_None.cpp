// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_Size_None.h"

void UModule_Limit_Size_None::SetWindow(UWindow* InWindow)
{
	Super::SetWindow(InWindow);
	InWindow->ED_SizeStart.AddDynamic(this, &UModule_Limit_None::Action);
	InWindow->Delegate_Size.Event_Bind_Add(this, &UModule_Limit_None::ModifyByParameter);
}

void UModule_Limit_Size_None::RemoveWindow()
{
	Window->ED_SizeStart.RemoveDynamic(this, &UModule_Limit_None::Action);
	Super::RemoveWindow();
}


TArray<float> UModule_Limit_Size_None::ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues)
{
	TArray<float> ToReturn;
	int i = 0;
	for (FName CurrentName : InNames) {
		if (CurrentName == "Position_X") {
			ToReturn.Add(ModifyLeft(InWindow, InValues[i]));
		}
		else if (CurrentName == "Position_Y") {
			ToReturn.Add(ModifyUp(InWindow, InValues[i]));
		}
		else if (CurrentName == "Size_X") {
			ToReturn.Add(ModifyRight(InWindow, InValues[i]));
		}
		else if (CurrentName == "Size_Y") {
			ToReturn.Add(ModifyDown(InWindow, InValues[i]));
		}
		i++;
	}
	return ToReturn;
}

float UModule_Limit_Size_None::ModifyUp(UWidget* InWindow, float InValue)
{
	return InValue;
}

float UModule_Limit_Size_None::ModifyLeft(UWidget* InWindow, float InValue)
{
	return InValue;
}

float UModule_Limit_Size_None::ModifyRight(UWidget* InWindow, float InValue)
{
	return InValue;
}

float UModule_Limit_Size_None::ModifyDown(UWidget* InWindow, float InValue)
{
	return InValue;
}
