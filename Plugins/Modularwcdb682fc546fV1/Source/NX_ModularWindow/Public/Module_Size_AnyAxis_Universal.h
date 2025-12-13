// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Size_None.h"
#include "Module_Size_AnyAxis_Universal.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Size_AnyAxis_Universal : public UModule_Size_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void SetWindow(UWindow* InWindow) override;

	virtual void Prepare(UWindow* InWindow) override;
	virtual void Move(UWindow* InWindow) override;

	//0 Up, 1 Right, 2 Down, 3 Left
	UFUNCTION(BlueprintCallable, Category="Window")
	virtual void SetSide(uint8 InData);
	
//Data//
public:
	//0 Up, 1 Right, 2 Down, 3 Left.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window", meta = (ExposeOnSpawn = true, ToolTip="0 Up, 1 Right, 2 Down, 3 Left"))
	uint8 Side;
protected:
	bool bXWorking = true;

	UPROPERTY()
	UModule_Size_None* Point;

	UPROPERTY()
	UModule_Size_None* Line;
};
