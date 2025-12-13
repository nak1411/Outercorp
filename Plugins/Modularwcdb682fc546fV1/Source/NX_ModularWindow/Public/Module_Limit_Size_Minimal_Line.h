// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Limit_Size_None.h"
#include "Module_Limit_Size_Minimal_Line.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UModule_Limit_Size_Minimal_Line : public UModule_Limit_Size_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void Prepare(UWidget* InWindow) override;

	virtual float ModifyLeft(UWidget* InWindow, float InValue) override;
	virtual float ModifyUp(UWidget* InWindow, float InValue) override;
	virtual float ModifyRight(UWidget* InWindow, float InValue) override;
	virtual float ModifyDown(UWidget* InWindow, float InValue) override;

//Data//
public:
	UPROPERTY(BlueprintReadWrite, Category = "Window", meta = (ExposeOnSpawn = "true"))
	FVector2D SizeMinimal = { 128, 128 };
protected:
	FVector2D CanvasSize;
};
