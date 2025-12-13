// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Window_Module.h"
#include "Module_Fullscreen_None.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UModule_Fullscreen_None : public UWindow_Module
{
	GENERATED_BODY()
	
//Functions//
public:
	virtual void Action() override;
	virtual void Deaction() override;

	virtual void SaveParameters(UWidget* InWidget);
	virtual bool IsFullscreen(UWidget* InWindow);
	virtual void Fullscreen(UWidget* InWindow);
	virtual void Unscreen(UWidget* InWindow);

//Variables//
public:
	FVector2D PositionSaved;
	FVector2D SizeSaved;

};
