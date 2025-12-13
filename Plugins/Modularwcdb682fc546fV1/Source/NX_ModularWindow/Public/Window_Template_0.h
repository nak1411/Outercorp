// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Window.h"
#include "Components/VerticalBox.h"
#include "Window_Template_0.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UWindow_Template_0 : public UWindow
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable, Category="Window")
	virtual void WindowSetContent(UWidget* InWidget);

//Data//
	UPROPERTY(BlueprintReadWrite, Category="Window")
	UVerticalBox* VerticalBox;
};
