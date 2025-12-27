// Copyright Nyaunix 2025 All Rights Reserved
#pragma once


#include "CoreMinimal.h"
#include <algorithm>
#include "NX_Delegate.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "WindowContentInterface.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window|Capabilities", meta=(DisplayName="Can Move Window"))
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window|Capabilities", meta=(DisplayName="Can Resize Window"))
	bool bCanResize = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window|Capabilities", meta=(DisplayName="Can Fullscreen Window"))
	bool bCanFullscreen = true;
	
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

	bool bCapabilitiesApplied = false;

	



//Logic//
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	UFUNCTION(BlueprintCallable, Category = "Window")
	bool Init();

	UFUNCTION(BlueprintCallable, Category = "Window")
	void Uninit();

	UFUNCTION(BlueprintCallable, Category = "Window")
	UCanvasPanelSlot* GetCanvasSlot();

	UFUNCTION(BlueprintCallable, Category = "Window")
	void BringToFront();

	UFUNCTION(BlueprintCallable, Category = "Window")
	int32 GetZOrder() const;

	UFUNCTION(BlueprintCallable, Category = "Window")
	void SetZOrder(int32 NewZOrder);





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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Window|Capabilities")
	bool CanMove() const { return bCanMove; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Window|Capabilities")
	bool CanResize() const { return bCanResize; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Window|Capabilities")
	bool CanFullscreen() const { return bCanFullscreen; }

	UFUNCTION(BlueprintCallable, Category = "Window|Capabilities")
	void SetCanMove(bool bNewCanMove) { bCanMove = bNewCanMove; }

	UFUNCTION(BlueprintCallable, Category = "Window|Capabilities")
	void SetCanResize(bool bNewCanResize) { bCanResize = bNewCanResize; }

	UFUNCTION(BlueprintCallable, Category = "Window|Capabilities")
	void SetCanFullscreen(bool bNewCanFullscreen) { bCanFullscreen = bNewCanFullscreen; }

	UFUNCTION(BlueprintCallable, Category = "Window|Capabilities")
	void SetWindowCapabilities(bool bMove, bool bResize, bool bFullscreen)
	{
		bCanMove = bMove;
		bCanResize = bResize;
		bCanFullscreen = bFullscreen;
		UpdateUIForCapabilities();
	}

	UFUNCTION(BlueprintCallable, Category = "Window|Capabilities")
	void ApplyCapabilitiesFromContent(UUserWidget* ContentWidget);

	UFUNCTION(BlueprintCallable, Category = "Window|Capabilities")
	void ApplyCapabilitiesFromStruct(const FWindowCapabilities& Capabilities)
	{
		bCanMove = Capabilities.bCanMove;
		bCanResize = Capabilities.bCanResize;
		bCanFullscreen = Capabilities.bCanFullscreen;
		UpdateUIForCapabilities();
	}

	UFUNCTION(BlueprintCallable, Category = "Window|Capabilities")
	void UpdateUIForCapabilities();

	UFUNCTION(BlueprintImplementableEvent, Category = "Window|Capabilities", meta=(DisplayName="On Capabilities Changed"))
	void OnCapabilitiesChanged();





//EventDispatchers//
public:
	//Clicked
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FED_WindowClicked, UWindow*, ClickedWindow);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FED_WindowClicked ED_WindowClicked;

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

