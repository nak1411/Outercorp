// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_All_Universal.h"
#include "Window.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Button_Resize.h"
#include "Module_Size_AnyAxis_Universal.h"
#include "Blueprint/WidgetTree.h"








void UModule_Size_All_Universal::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	
	//Initialization Fuse
	if (IsValid(Window)) {
		if (IsValid(Module_Left)) {
			return;
		}
	}

	//Validation
	if (!IsValid(GetOuter())) {
		return;
	}
	if (!IsValid(GetOuter()->GetOuter())) {
		return;
	}
	if (!GetOuter()->GetOuter()->GetClass()->IsChildOf(UWindow::StaticClass())) {
		return;
	}

	//Initialization
	Window = Cast<UWindow>(GetOuter()->GetOuter());
	SetWindow(Window);
	

}

void UModule_Size_All_Universal::SetWindow(UWindow* InWidget)
{
	Window = InWidget;
	if (IsValid(Left)) return;


	//Modules//
	//Creating
	Module_Left = NewObject<UModule_Size_AnyAxis_Universal>(this);
	Module_Up = NewObject<UModule_Size_AnyAxis_Universal>(this);
	Module_Right = NewObject<UModule_Size_AnyAxis_Universal>(this);
	Module_Down = NewObject<UModule_Size_AnyAxis_Universal>(this);

	//Settings
	Module_Left->Side = 3;
	Module_Up->Side = 0;
	Module_Right->Side = 1;
	Module_Down->Side = 2;

	//Init
	Module_Left->SetWindow(InWidget);
	Module_Up->SetWindow(InWidget);
	Module_Right->SetWindow(InWidget);
	Module_Down->SetWindow(InWidget);



	//Buttons//
	//Creating
	Left = InWidget->WidgetTree->ConstructWidget<UButton_Resize>(UButton_Resize::StaticClass(), TEXT("Button_Left"));
	Up = InWidget->WidgetTree->ConstructWidget<UButton_Resize>(UButton_Resize::StaticClass(), TEXT("Button_Up"));
	Right = InWidget->WidgetTree->ConstructWidget<UButton_Resize>(UButton_Resize::StaticClass(), TEXT("Button_Right"));
	Down = InWidget->WidgetTree->ConstructWidget<UButton_Resize>(UButton_Resize::StaticClass(), TEXT("Button_Down"));
	LeftUp = InWidget->WidgetTree->ConstructWidget<UButton_Resize>(UButton_Resize::StaticClass(), TEXT("Button_LeftUp"));
	UpRight = InWidget->WidgetTree->ConstructWidget<UButton_Resize>(UButton_Resize::StaticClass(), TEXT("Button_UpRight"));
	RightDown = InWidget->WidgetTree->ConstructWidget<UButton_Resize>(UButton_Resize::StaticClass(), TEXT("Button_RightDown"));
	DownLeft = InWidget->WidgetTree->ConstructWidget<UButton_Resize>(UButton_Resize::StaticClass(), TEXT("Button_DownLeft"));
	
	//Add in overlay
	AddChildToOverlay(Left);
	AddChildToOverlay(Up);
	AddChildToOverlay(Right);
	AddChildToOverlay(Down);
	AddChildToOverlay(LeftUp);
	AddChildToOverlay(UpRight);
	AddChildToOverlay(RightDown);
	AddChildToOverlay(DownLeft);

	//Set cursor icon
	Left->SetCursor(EMouseCursor::ResizeLeftRight);
	Up->SetCursor(EMouseCursor::ResizeUpDown);
	Right->SetCursor(EMouseCursor::ResizeLeftRight);
	Down->SetCursor(EMouseCursor::ResizeUpDown);
	LeftUp->SetCursor(EMouseCursor::ResizeSouthEast);
	UpRight->SetCursor(EMouseCursor::ResizeSouthWest);
	RightDown->SetCursor(EMouseCursor::ResizeSouthEast);
	DownLeft->SetCursor(EMouseCursor::ResizeSouthWest);


	//Set Alignment(in overlay)
	Cast<UOverlaySlot>(Left->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	Cast<UOverlaySlot>(Up->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	Cast<UOverlaySlot>(Right->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
	Cast<UOverlaySlot>(Right->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	Cast<UOverlaySlot>(Down->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	Cast<UOverlaySlot>(Down->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
	
	Cast<UOverlaySlot>(UpRight->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
	Cast<UOverlaySlot>(RightDown->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
	Cast<UOverlaySlot>(RightDown->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
	Cast<UOverlaySlot>(DownLeft->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);


	//Bindings (Button+Module)
	Left->OnPressed.AddDynamic(Module_Left, &UModule_Size_None::Action);
	Left->OnReleased.AddDynamic(Module_Left, &UModule_Size_None::Deaction);

	Up->OnPressed.AddDynamic(Module_Up, &UModule_Size_None::Action);
	Up->OnReleased.AddDynamic(Module_Up, &UModule_Size_None::Deaction);

	Right->OnPressed.AddDynamic(Module_Right, &UModule_Size_None::Action);
	Right->OnReleased.AddDynamic(Module_Right, &UModule_Size_None::Deaction);

	Down->OnPressed.AddDynamic(Module_Down, &UModule_Size_None::Action);
	Down->OnReleased.AddDynamic(Module_Down, &UModule_Size_None::Deaction);


	LeftUp->OnPressed.AddDynamic(Module_Left, &UModule_Size_None::Action);
	LeftUp->OnReleased.AddDynamic(Module_Left, &UModule_Size_None::Deaction);
	LeftUp->OnPressed.AddDynamic(Module_Up, &UModule_Size_None::Action);
	LeftUp->OnReleased.AddDynamic(Module_Up, &UModule_Size_None::Deaction);

	UpRight->OnPressed.AddDynamic(Module_Up, &UModule_Size_None::Action);
	UpRight->OnReleased.AddDynamic(Module_Up, &UModule_Size_None::Deaction);
	UpRight->OnPressed.AddDynamic(Module_Right, &UModule_Size_None::Action);
	UpRight->OnReleased.AddDynamic(Module_Right, &UModule_Size_None::Deaction);

	RightDown->OnPressed.AddDynamic(Module_Right, &UModule_Size_None::Action);
	RightDown->OnReleased.AddDynamic(Module_Right, &UModule_Size_None::Deaction);
	RightDown->OnPressed.AddDynamic(Module_Down, &UModule_Size_None::Action);
	RightDown->OnReleased.AddDynamic(Module_Down, &UModule_Size_None::Deaction);

	DownLeft->OnPressed.AddDynamic(Module_Down, &UModule_Size_None::Action);
	DownLeft->OnReleased.AddDynamic(Module_Down, &UModule_Size_None::Deaction);
	DownLeft->OnPressed.AddDynamic(Module_Left, &UModule_Size_None::Action);
	DownLeft->OnReleased.AddDynamic(Module_Left, &UModule_Size_None::Deaction);

}

