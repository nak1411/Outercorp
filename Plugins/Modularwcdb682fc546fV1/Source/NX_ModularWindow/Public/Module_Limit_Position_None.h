// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Limit_None.h"
#include "Module_Limit_Position_None.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UModule_Limit_Position_None : public UModule_Limit_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void SetWindow(UWindow* InWindow) override;
	virtual void RemoveWindow() override;

};
