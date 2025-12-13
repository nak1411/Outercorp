// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Limit_Size_None.h"
#include "Module_Limit_Size_Maximal_Point.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UModule_Limit_Size_Maximal_Point : public UModule_Limit_Size_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void Prepare(UWidget* InWindow) override;

	virtual float ModifyUp(UWidget* InWindow, float InValue) override;
	virtual float ModifyLeft(UWidget* InWindow, float InValue) override;
	virtual float ModifyRight(UWidget* InWindow, float InValue) override;
	virtual float ModifyDown(UWidget* InWindow, float InValue) override;

//Data//
protected:
	FVector2D CanvasSizeMinimum;
	FVector2D AnchorModify;
	bool Block_X = false;
	bool Block_Y = false;
};
