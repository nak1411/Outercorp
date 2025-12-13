// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Window_Module.h"
#include "Module_Limit_None.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Limit_None : public UWindow_Module
{
	GENERATED_BODY()
	
//Functions//
public:	
	virtual void Action() override;

	UFUNCTION(BlueprintCallable, Category="Window")
	virtual void Prepare(UWidget* InWindow);

	UFUNCTION(BlueprintCallable, Category="Window")
	virtual TArray<float> ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues);
	

};
