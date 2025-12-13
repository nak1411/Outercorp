// Copyright Nyaunix 2025 All Rights Reserved
#pragma once


#include "CoreMinimal.h"
#include <algorithm>
#include "NX_Delegate.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Window.generated.h"

class UWindow_Module;
class UModule_Limit_None;
class UWindow_Timer_Event;



UCLASS(BlueprintType, Blueprintable)
class NX_MODULARWINDOW_API UWindow : public UUserWidget
{
	GENERATED_BODY()
	
//Data//
public:
	UPROPERTY(BlueprintReadWrite, Category="Window")
	TArray<UWindow_Module*> Modules;
	
	NX_Delegate_MW<
		UModule_Limit_None,
		TArray<float>(UModule_Limit_None::*)(UWidget*, TArray<FName>, TArray<float>),
		TArray<float>, 
		TArray<FName>,
		UWidget*>
		Delegate_Position;
	NX_Delegate_MW<
		UModule_Limit_None,
		TArray<float>(UModule_Limit_None::*)(UWidget*, TArray<FName>, TArray<float>),
		TArray<float>,
		TArray<FName>,
		UWidget*>
		Delegate_Size;

protected:
	FVector2D PositionStart;
	FVector2D SizeStart;

	UPROPERTY()
	UWindow_Timer_Event* Window_Timer_Event;
	
	UPROPERTY()
	UCanvasPanelSlot* CanvasSlot;

	



//Logic//
protected:
	virtual void NativeOnInitialized() override;
public:
	UFUNCTION(BlueprintCallable, Category = "Window")
	bool Init();

	UFUNCTION(BlueprintCallable, Category = "Window")
	void Uninit();

	UFUNCTION(BlueprintCallable, Category = "Window")
	UCanvasPanelSlot* GetCanvasSlot();





//Position//
	//Getter//
	UFUNCTION(BlueprintCallable, Category = "Window")
	FVector2D GetPosition();

	//Setter//
	UFUNCTION(BlueprintCallable, Category = "Window")
	void SetPosition(FVector2D NewPosition);

	UFUNCTION(BlueprintCallable, Category = "Window")
	void SetPositionX(float NewPosition);

	UFUNCTION(BlueprintCallable, Category = "Window")
	void SetPositionY(float NewPosition);
	
	
//Size//
	//Getter//
	UFUNCTION(BlueprintCallable, Category = "Window")
	FVector2D GetSize();

	//Setter//
	UFUNCTION(BlueprintCallable, Category = "Window")
	void SetSize(FVector2D NewSize);

	UFUNCTION(BlueprintCallable, Category = "Window")
	void SetSizeX(float NewSize);

	UFUNCTION(BlueprintCallable, Category = "Window")
	void SetSizeY(float NewSize);

	

//SetByParameter//
	UFUNCTION(BlueprintCallable, Category="Window")
	void SetByParameter(TArray<FName> InNames, TArray<float> InValues);

	UFUNCTION(BlueprintCallable, Category="Window")
	void SetByParameterPosition(TArray<FName> InNames, TArray<float> InValues);

	UFUNCTION(BlueprintCallable, Category="Window")
	void SetByParameterSize(TArray<FName> InNames, TArray<float> InValues);


	
//Start_Values//
	void SaveWindowParametersStart();
	FVector2D GetPositionStart() const;
	FVector2D GetSizeStart() const;
	

	
//Modules//
	UFUNCTION(BlueprintCallable, Category = "Window")
	void AddModule(UWindow_Module* Module);

	UFUNCTION(BlueprintCallable, Category = "Window")
	void RemoveModule(UWindow_Module* Module);


	UFUNCTION(BlueprintCallable, Category = "Window")
	bool IsAnchorPoint(bool bIsX = true);





//EventDispatchers//
public:
	//Interact
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FED_InteractStart);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FED_InteractStart ED_InteractStart;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FED_InteractEnd);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FED_InteractEnd ED_InteractEnd;



	//Position
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FED_PositionStart);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FED_PositionStart ED_PositionStart;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FED_PositionChanged);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FED_PositionChanged ED_PositionChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FED_PositionEnd);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FED_PositionEnd ED_PositionEnd;



	//Size
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FED_SizeStart);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FED_SizeStart ED_SizeStart;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FED_SizeChanged);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FED_SizeChanged ED_SizeChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FED_SizeEnd);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FED_SizeEnd ED_SizeEnd;
	
};

