// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Window.h"
#include "Window_Timer_Event.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UWindow_Timer_Event : public UObject
{
	GENERATED_BODY()
	
public:	
	void CarryEventPosition();
	void CarryEventSize();
	void TimerTickPosition();
	void TimerTickSize();

	//Data//
public:
	UPROPERTY()
	UWindow* Window = nullptr;
protected:
	uint8_t TickPosition = 0;
	uint8_t TickSize = 0;
	UPROPERTY()
	FTimerHandle TimerPosition;
	
	UPROPERTY()
	FTimerHandle TimerSize;

};
