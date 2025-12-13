// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Size_None.h"
#include "Module_Size_Right_Line.generated.h"


UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Size_Right_Line : public UModule_Size_None
{
	GENERATED_BODY()

//Logic//
public:
	virtual void Prepare(UWindow* InWindow) override;
	virtual void Move(UWindow* InWindow) override;

//Data//
protected:
	float CursorStart;
	float WindowSizeStart;
};
