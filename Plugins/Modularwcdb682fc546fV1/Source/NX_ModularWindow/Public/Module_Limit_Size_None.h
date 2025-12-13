// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Limit_None.h"
#include "Module_Limit_Size_None.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UModule_Limit_Size_None : public UModule_Limit_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void SetWindow(UWindow* InWindow) override;
	virtual void RemoveWindow() override;

	virtual TArray<float> ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues) override;

	UFUNCTION(BlueprintCallable, Category="Window")
	virtual float ModifyUp(UWidget* InWindow, float InValue);

	UFUNCTION(BlueprintCallable, Category="Window")
	virtual float ModifyLeft(UWidget* InWindow, float InValue);

	UFUNCTION(BlueprintCallable, Category="Window")
	virtual float ModifyRight(UWidget* InWindow, float InValue);

	UFUNCTION(BlueprintCallable, Category="Window")
	virtual float ModifyDown(UWidget* InWindow, float InValue);

};
