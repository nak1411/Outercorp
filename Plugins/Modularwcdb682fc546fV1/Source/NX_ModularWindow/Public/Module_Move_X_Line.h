// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Move_None.h"
#include "Module_Move_X_Line.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UModule_Move_X_Line : public UModule_Move_None
{
	GENERATED_BODY()

//Logic//
public:	
	virtual void Prepare(UWindow* InWindow) override;
	virtual void Move(UWindow* InWindow) override;

//Data//
protected:
	float CursorPositionStart;
	float PositionDifference;
	float WindowSizeStart;
};
