// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_None.h"



void UModule_Limit_None::Action()
{
	Prepare(Window);
}

void UModule_Limit_None::Prepare(UWidget* InWindow)
{

}

TArray<float> UModule_Limit_None::ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues)
{
    return InValues;
}

