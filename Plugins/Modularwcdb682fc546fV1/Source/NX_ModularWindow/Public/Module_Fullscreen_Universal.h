// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Fullscreen_None.h"
#include "Module_Fullscreen_Universal.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Fullscreen_Universal : public UModule_Fullscreen_None
{
	GENERATED_BODY()
	
//Logic//
public:
	virtual void SetWindow(UWindow* InWindow) override;
	virtual void Action() override;
	virtual void Deaction() override;

//Data//
public:
	UPROPERTY()
	UModule_Fullscreen_None* Point;

	UPROPERTY()
	UModule_Fullscreen_None* Line;

};
