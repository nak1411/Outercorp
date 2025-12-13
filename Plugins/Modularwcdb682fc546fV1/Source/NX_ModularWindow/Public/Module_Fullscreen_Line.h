// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Fullscreen_None.h"
#include "Module_Fullscreen_Line.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UModule_Fullscreen_Line : public UModule_Fullscreen_None
{
	GENERATED_BODY()
	
//Functions//
public:	
	virtual bool IsFullscreen(UWidget* InWindow) override;
	virtual void Fullscreen(UWidget* InWindow) override;

};
