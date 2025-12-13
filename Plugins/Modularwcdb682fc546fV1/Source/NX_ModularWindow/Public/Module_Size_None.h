// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Window_Module.h"
#include "Module_Size_None.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Size_None : public UWindow_Module
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void Action() override;
	virtual void Deaction() override;

	virtual void Prepare(UWindow* InWindow);
	virtual void Move(UWindow* InWindow);
	virtual void PrepareAuto();
	virtual void MoveAuto();


//Data//
private:
	UPROPERTY()
	FTimerHandle TimerHandle;
};
