// Copyright Nyaunix 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Module_Limit_Size_None.h"
#include "Module_Limit_Size_Universal.generated.h"

UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UModule_Limit_Size_Universal : public UModule_Limit_Size_None
{
	GENERATED_BODY()
	
//Logic//
public:	
	virtual void SetWindow(UWindow* InWindow) override;

	virtual void Prepare(UWidget* InWindow) override;
	virtual TArray<float> ModifyByParameter(UWidget* InWindow, TArray<FName> InNames, TArray<float> InValues) override;

	UFUNCTION(BlueprintCallable, Category = "Window")
	void SetSizeMinimal(FVector2D InSizeMinimal);
	
//Data//
public:
	//if SizeMinimal = 0, the limit class will be "maximum"
	UPROPERTY(BlueprintReadWrite, Category = "Window", meta = (ExposeOnSpawn = "true"))
	FVector2D SizeMinimal = FVector2D::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite, Category = "Window")
	UModule_Limit_None* Point;

	UPROPERTY(BlueprintReadWrite, Category = "Window")
	UModule_Limit_None* Line;
};
