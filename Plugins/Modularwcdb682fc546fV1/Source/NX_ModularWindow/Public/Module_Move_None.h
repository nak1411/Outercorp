// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Window_Module.h"
#include "Module_Move_None.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Move_None : public UWindow_Module
{
	GENERATED_BODY()

//Logic//
public:
	virtual void Action() override;
	virtual void Deaction() override;

	UFUNCTION(BlueprintCallable, Category = "Window")
	virtual void SetXWorking(bool bValue);

	UFUNCTION(BlueprintCallable, Category = "Window")
	virtual void SetYWorking(bool bValue);

	UFUNCTION(BlueprintCallable, Category = "Window")
	bool GetXWorking();

	UFUNCTION(BlueprintCallable, Category = "Window")
	bool GetYWorking();

	virtual void Prepare(UWindow* InWindow);
	virtual void Move(UWindow* InWindow);
	virtual void MoveAuto();

	// Tick-based movement for better performance
	virtual void TickMove(float DeltaTime);

//Data//
protected:
	UPROPERTY();
	bool bXWorking = true;
	UPROPERTY();
	bool bYWorking = true;

	// Track if we're actively moving
	bool bIsActivelyMoving = false;

private:
	UPROPERTY();
	FTimerHandle TimerHandle;
};
