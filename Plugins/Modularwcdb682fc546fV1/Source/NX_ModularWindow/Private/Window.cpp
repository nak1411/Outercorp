// Copyright Nyaunix 2025 All Rights Reserved


#include "Window.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Window_Module.h"
#include "Module_Limit_None.h"
#include "Module_Limit_Position_Universal.h"
#include "Module_Limit_Size_Universal.h"


#include "Window_Timer_Event.h"
#include "Engine/Engine.h"






void UWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Init();
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


