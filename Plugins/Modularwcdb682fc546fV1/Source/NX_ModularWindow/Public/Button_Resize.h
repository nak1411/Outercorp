// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Button_Resize.generated.h"

UCLASS()
class NX_MODULARWINDOW_API UButton_Resize : public UButton
{
	GENERATED_BODY()
	
public:	
	UButton_Resize(const FObjectInitializer& ObjectInitializer);
};
