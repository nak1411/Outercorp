// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Limit_Size_None.h"
#include "Module_Limit_Size_Maximal_Line.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UModule_Limit_Size_Maximal_Line : public UModule_Limit_Size_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual TArray<float> ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues) override;
};
