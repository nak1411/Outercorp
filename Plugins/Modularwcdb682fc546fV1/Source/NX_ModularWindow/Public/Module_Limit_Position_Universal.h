// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Limit_Position_None.h"
#include "Module_Limit_Position_Point.h"
#include "Module_Limit_Position_Line.h"
#include "Module_Limit_Position_Universal.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Limit_Position_Universal : public UModule_Limit_Position_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void SetWindow(UWindow* InWindow) override;

	virtual void Prepare(UWidget* InWindow) override;
	virtual TArray<float> ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues) override;

//Data//
protected:
	UPROPERTY()
	UModule_Limit_Position_Point* LimitPoint;

	UPROPERTY()
	UModule_Limit_Position_Line* LimitLine;
};
