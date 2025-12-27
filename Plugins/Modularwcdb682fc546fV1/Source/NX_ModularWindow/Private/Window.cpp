// Copyright Nyaunix 2025 All Rights Reserved


#include "Window.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Window_Module.h"
#include "Module_Limit_None.h"
#include "Module_Limit_Position_Universal.h"
#include "Module_Limit_Size_Universal.h"
#include "Module_Fullscreen_None.h"
#include "Module_Size_All_Universal.h"
#include "Blueprint/WidgetTree.h"


#include "Window_Timer_Event.h"
#include "Engine/Engine.h"






void UWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Init();
}

void UWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Apply capabilities on the first tick after construction
	// This ensures all child widgets (like Module_Size_All_Universal) are fully constructed
	if (!bCapabilitiesApplied)
	{
		UpdateUIForCapabilities();
		bCapabilitiesApplied = true;
	}
}

FReply UWindow::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Bring window to front when clicked
	BringToFront();

	// Allow the event to continue propagating to child widgets
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UWindow::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// This is called before child widgets get the event
	// Bring window to front when clicked anywhere on it
	BringToFront();

	// Return unhandled so the event continues to child widgets
	return FReply::Unhandled();
}



bool UWindow::Init()
{
	if (!IsValid(Window_Timer_Event)) {
		Window_Timer_Event = NewObject<UWindow_Timer_Event>(this);
		Window_Timer_Event->Window = this;
	}
	if (!IsValid(Slot)) return false;
	CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	if (!IsValid(CanvasSlot)) return false;
	if (!GEngine) return false;

	
	return true;
}

void UWindow::Uninit()
{
	CanvasSlot = nullptr;
}

UCanvasPanelSlot* UWindow::GetCanvasSlot()
{
	return Cast<UCanvasPanelSlot>(Slot);
}

void UWindow::BringToFront()
{
	if (CanvasSlot)
	{
		// Broadcast the event so the window manager can handle Z-ordering
		ED_WindowClicked.Broadcast(this);
	}
}

int32 UWindow::GetZOrder() const
{
	if (CanvasSlot)
	{
		return CanvasSlot->GetZOrder();
	}
	return 0;
}

void UWindow::SetZOrder(int32 NewZOrder)
{
	if (CanvasSlot)
	{
		CanvasSlot->SetZOrder(NewZOrder);
	}
}










//Modules---------------------------------------
void UWindow::AddModule(UWindow_Module* Module)
{
	Module->SetWindow(this);
	Modules.Add(Module);
}

void UWindow::RemoveModule(UWindow_Module* Module)
{
	Module->RemoveWindow();
	Modules.Remove(Module);
}









//Getter-------------------------------------------------
//Position
FVector2D UWindow::GetPosition()
{
	return CanvasSlot->GetPosition();
}


//Size
FVector2D UWindow::GetSize()
{
	return CanvasSlot->GetSize();
}







//SetByParameter-----------------------------------------
void UWindow::SetByParameter(TArray<FName> InNames, TArray<float> InValues)
{
	float LValue;
	int32 I = 0;
	//Set ForEachLoop on FNames.
	for (FName LName : InNames)
	{
		LValue = InValues[I];
		if (LName == "Position_X") {
			CanvasSlot->SetPosition(FVector2D(LValue, GetPosition().Y));
		}
		else if (LName == "Position_Y") {
			CanvasSlot->SetPosition(FVector2D(GetPosition().X, LValue));
		}
		else if (LName == "Size_X") {
			CanvasSlot->SetSize(FVector2D(LValue, GetSize().Y));
		}
		else if (LName == "Size_Y") {
			CanvasSlot->SetSize(FVector2D(GetSize().X, LValue));
		}
		I++;
	}

	
}

void UWindow::SetByParameterPosition(TArray<FName> InNames, TArray<float> InValues)
{
	Window_Timer_Event->CarryEventPosition();
	SetByParameter(
		InNames,
		Delegate_Position.Event_Call_IO(this, InNames, InValues)
	);
}

void UWindow::SetByParameterSize(TArray<FName> InNames, TArray<float> InValues)
{
	Window_Timer_Event->CarryEventSize();
	SetByParameter(
		InNames, 
		Delegate_Size.Event_Call_IO(this, InNames, InValues)
	);
}














//Setter-----------------------------------------
//Position
void UWindow::SetPosition(FVector2D NewPosition)
{
	CanvasSlot->SetPosition(NewPosition);
	SetByParameterPosition(
		TArray<FName>{"Position_X", "Position_Y"}, 
		TArray<float>{
			static_cast<float>(
			NewPosition.X), 
			static_cast<float>(
			NewPosition.Y)
		}
	);
}

void UWindow::SetPositionX(float NewPosition)
{
	SetPosition(FVector2D(NewPosition, GetPosition().Y));
}

void UWindow::SetPositionY(float NewPosition)
{
	SetPosition(FVector2D(GetPosition().X, NewPosition));
}





//Size
void UWindow::SetSize(FVector2D NewSize)
{
	CanvasSlot->SetSize(NewSize);
}


void UWindow::SetSizeX(float NewSize)
{
	SetSize(FVector2D(NewSize, GetSize().Y));
}

void UWindow::SetSizeY(float NewSize)
{
	SetSize(FVector2D(GetSize().X, NewSize));
}












//Anchor---------------------------
bool UWindow::IsAnchorPoint(bool bIsX)
{
	if (bIsX) {
		return FMath::IsNearlyEqual(
			CanvasSlot->GetAnchors().Minimum.X,
			CanvasSlot->GetAnchors().Maximum.X,
			0.1f);
	}
	else {
		return FMath::IsNearlyEqual(
			CanvasSlot->GetAnchors().Minimum.Y,
			CanvasSlot->GetAnchors().Maximum.Y,
			0.1f);
	}
}





void UWindow::SaveWindowParametersStart()
{
	PositionStart = GetPosition();
	SizeStart = GetSize();
}

FVector2D UWindow::GetPositionStart() const
{
	return PositionStart;
}

FVector2D UWindow::GetSizeStart() const
{
	return SizeStart;
}

void UWindow::ApplyCapabilitiesFromContent(UUserWidget* ContentWidget)
{
	if (!IsValid(ContentWidget))
	{
		return;
	}

	// Check if the content widget implements the interface
	if (ContentWidget->GetClass()->ImplementsInterface(UWindowContentInterface::StaticClass()))
	{
		FWindowCapabilities Capabilities = IWindowContentInterface::Execute_GetWindowCapabilities(ContentWidget);
		ApplyCapabilitiesFromStruct(Capabilities);
	}
}

void UWindow::UpdateUIForCapabilities()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateUIForCapabilities called - CanResize: %s, CanFullscreen: %s"),
		bCanResize ? TEXT("true") : TEXT("false"),
		bCanFullscreen ? TEXT("true") : TEXT("false"));

	// Hide/show resize handles based on CanResize
	// Search through widget tree to find Module_Size_All_Universal by type
	UModule_Size_All_Universal* SizeModule = nullptr;

	if (WidgetTree)
	{
		TArray<UWidget*> ChildWidgets;
		WidgetTree->GetAllWidgets(ChildWidgets);

		for (UWidget* Widget : ChildWidgets)
		{
			if (UModule_Size_All_Universal* FoundModule = Cast<UModule_Size_All_Universal>(Widget))
			{
				SizeModule = FoundModule;
				UE_LOG(LogTemp, Warning, TEXT("Found Module_Size_All_Universal: %s"), *Widget->GetName());
				break;
			}
		}
	}

	if (SizeModule)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calling SetEnabled(%s) on Module_Size_All_Universal"), bCanResize ? TEXT("true") : TEXT("false"));
		SizeModule->SetEnabled(bCanResize);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Module_Size_All_Universal found in widget tree!"));
	}

	// Hide/show fullscreen button based on CanFullscreen
	// Search through widget tree to find button by name
	UWidget* FullscreenBtn = nullptr;

	if (WidgetTree)
	{
		TArray<UWidget*> AllWidgets;
		WidgetTree->GetAllWidgets(AllWidgets);

		for (UWidget* Widget : AllWidgets)
		{
			FString WidgetName = Widget->GetName().ToLower();
			// Check if widget name contains "fullscreen", "maximize", or "max"
			if (WidgetName.Contains(TEXT("fullscreen")) ||
			    WidgetName.Contains(TEXT("maximize")) ||
			    WidgetName.Contains(TEXT("max")))
			{
				FullscreenBtn = Widget;
				UE_LOG(LogTemp, Warning, TEXT("Found fullscreen button: %s"), *Widget->GetName());
				break;
			}
		}
	}

	if (FullscreenBtn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting fullscreen button visibility to: %s"), bCanFullscreen ? TEXT("Visible") : TEXT("Collapsed"));
		FullscreenBtn->SetVisibility(bCanFullscreen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No fullscreen button found in widget tree!"));
	}

	// If fullscreen is disabled and we're currently fullscreened, exit fullscreen
	if (!bCanFullscreen)
	{
		// Try to find and call unscreen on fullscreen modules
		for (UWindow_Module* Module : Modules)
		{
			if (UModule_Fullscreen_None* FullscreenModule = Cast<UModule_Fullscreen_None>(Module))
			{
				if (FullscreenModule->IsFullscreen(this))
				{
					FullscreenModule->Unscreen(this);
				}
			}
		}
	}

	// Call Blueprint event to allow custom UI updates
	OnCapabilitiesChanged();
}


