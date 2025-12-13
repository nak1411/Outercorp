// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Limit_Position_None.h"
#include "Module_Limit_Position_Line.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Limit_Position_Line : public UModule_Limit_Position_None
{
	GENERATED_BODY()
	
//Functions//
public:	
	virtual void Prepare(UWidget* InWindow) override;
	virtual TArray<float> ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues) override;
	FVector4 ModifyPosition(FVector2D InPosition, FVector2D InSize);

//Variables//
protected:
	FVector2D PositionMax;

};
