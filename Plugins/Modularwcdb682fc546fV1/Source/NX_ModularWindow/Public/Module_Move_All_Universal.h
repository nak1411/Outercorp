// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Move_None.h"
#include "Module_Move_All_Universal.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Move_All_Universal : public UModule_Move_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void SetWindow(UWindow* InWindow) override;
	virtual void Prepare(UWindow* InWindow) override;
	virtual void Move(UWindow* InWindow) override;

//Data//
public:
	UPROPERTY(BlueprintReadWrite, Category = "Window")
	UModule_Move_None* Module_X;

	UPROPERTY(BlueprintReadWrite, Category = "Window")
	UModule_Move_None* Module_Y;
};
