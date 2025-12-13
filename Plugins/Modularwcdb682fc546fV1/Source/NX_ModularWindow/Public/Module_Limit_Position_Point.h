// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Limit_Position_None.h"
#include "Module_Limit_Position_Point.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Limit_Position_Point : public UModule_Limit_Position_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual TArray<float> ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues) override;
	FVector2D ModifyPosition(UWidget* InWindow, FVector2D InVector);

};
