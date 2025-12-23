// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Window.h"
#include "Module_Size_AnyAxis_Universal.h"
#include "Module_Size_All_Universal.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Size_All_Universal : public UOverlay
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void OnWidgetRebuilt() override;

	UFUNCTION(BlueprintCallable, Category="Window")
	virtual void SetWindow(UWindow* InWidget);

	UFUNCTION(BlueprintCallable, Category="Window")
	void SetEnabled(bool bEnabled);

//Data//
public:
	UPROPERTY(BlueprintReadWrite, Category="Window", EditAnywhere, meta = (ExposeOnSpawn = "true"), Instanced)
	UWindow* Window;

//Modules//
	UPROPERTY()
	UModule_Size_AnyAxis_Universal* Module_Left;
	UPROPERTY()
	UModule_Size_AnyAxis_Universal* Module_Up;
	UPROPERTY()
	UModule_Size_AnyAxis_Universal* Module_Right;
	UPROPERTY()
	UModule_Size_AnyAxis_Universal* Module_Down;

//Buttons//
	UPROPERTY()
	UButton* Left;
	UPROPERTY()
	UButton* Up;
	UPROPERTY()
	UButton* Right;
	UPROPERTY()
	UButton* Down;
	UPROPERTY()
	UButton* LeftUp;
	UPROPERTY()
	UButton* UpRight;
	UPROPERTY()
	UButton* RightDown;
	UPROPERTY()
	UButton* DownLeft;
	
};
