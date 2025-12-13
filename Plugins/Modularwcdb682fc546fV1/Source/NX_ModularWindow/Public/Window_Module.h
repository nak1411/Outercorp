// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Window.h"
#include "Window_Module.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UWindow_Module : public UObject
{
	GENERATED_BODY()

//Logic//
public:	
	UFUNCTION(BlueprintCallable, Category = "Module")
	UWindow* GetWindow();

	virtual void SetWindow(UWindow* InWindow);
	virtual void RemoveWindow();


	
	UFUNCTION(BlueprintCallable, Category = "Window")
	virtual void Action();

	UFUNCTION(BlueprintCallable, Category = "Window")
	virtual void Deaction();


//Data//
	FName Name;
protected:
	UPROPERTY()
	UWindow* Window;
};