// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WindowContentInterface.generated.h"

USTRUCT(BlueprintType)
struct FWindowCapabilities
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window")
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window")
	bool bCanResize = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window")
	bool bCanFullscreen = true;

	FWindowCapabilities()
		: bCanMove(true)
		, bCanResize(true)
		, bCanFullscreen(true)
	{}

	FWindowCapabilities(bool InCanMove, bool InCanResize, bool InCanFullscreen)
		: bCanMove(InCanMove)
		, bCanResize(InCanResize)
		, bCanFullscreen(InCanFullscreen)
	{}
};

UINTERFACE(MinimalAPI, Blueprintable)
class UWindowContentInterface : public UInterface
{
	GENERATED_BODY()
};

class NX_MODULARWINDOW_API IWindowContentInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Window")
	FWindowCapabilities GetWindowCapabilities() const;
	virtual FWindowCapabilities GetWindowCapabilities_Implementation() const
	{
		return FWindowCapabilities();
	}
};
