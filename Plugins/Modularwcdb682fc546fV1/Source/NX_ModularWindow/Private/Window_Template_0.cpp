// Copyright Nyaunix 2025 All Rights Reserved


#include "Window_Template_0.h"
#include "Components/OverlaySlot.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Module_Move_All_Universal.h"
#include "Module_Limit_Position_Universal.h"
#include "Module_Limit_Size_Universal.h"
#include "Module_Size_All_Universal.h"


void UWindow_Template_0::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	//Overlay
	UModule_Size_All_Universal* Overlay = WidgetTree->ConstructWidget<UModule_Size_All_Universal>(UModule_Size_All_Universal::StaticClass(), TEXT("Overlay_0"));
	WidgetTree->RootWidget = Overlay;
	Overlay->SetWindow(this);

	UBorder* Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Border_0"));
	Overlay->InsertChildAt(0, Border);
	Cast<UOverlaySlot>(Border->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	Cast<UOverlaySlot>(Border->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	Cast<UOverlaySlot>(Border->Slot)->SetPadding(0);
	Border->SetPadding(0);
	

	FSlateBrush SlateBrushBorder;
	SlateBrushBorder.DrawAs = ESlateBrushDrawType::Box;
	SlateBrushBorder.TintColor = FLinearColor(0, 0, 0, 1);
	Border->SetBrush(SlateBrushBorder);


	//VerticalBox
	if (!IsValid(VerticalBox)) {
		VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VerticalBox_0"));
		Border->AddChild(VerticalBox);
		Cast<UBorderSlot>(VerticalBox->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		Cast<UBorderSlot>(VerticalBox->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	}
	

	//Module Move
	UModule_Move_All_Universal* Module_Move = NewObject<UModule_Move_All_Universal>(this);
	AddModule(Module_Move);
	
	//Button//
	UButton* Button_Move = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("Button_Move"));
	VerticalBox->InsertChildAt(0, Button_Move);
	Cast<UVerticalBoxSlot>(Button_Move->Slot)->SetPadding(0);
	
	//Button Style
	FSlateBrush SlateBrushButton = Button_Move->GetStyle().Normal;
	SlateBrushButton.OutlineSettings.CornerRadii = FVector4(0,0,0,0);
	FButtonStyle ButtonStyle = FButtonStyle()
		.SetNormal(SlateBrushButton)
		.SetHovered(SlateBrushButton)
		.SetPressed(SlateBrushButton)
		.SetNormalPadding(FMargin(0, 0, 0, 0))
		.SetPressedPadding(FMargin(0, 0, 0, 0));
	Button_Move->SetStyle(ButtonStyle);

	//Button binding
	Button_Move->OnPressed.AddDynamic(Module_Move, &UModule_Move_None::Action);
	Button_Move->OnReleased.AddDynamic(Module_Move, &UModule_Move_None::Deaction);


	//Modules//
	//Create
	UModule_Limit_None* Limit_Position = NewObject<UModule_Limit_Position_Universal>(this);
	UModule_Limit_None* Limit_Size_Minimal = NewObject<UModule_Limit_Size_Universal>(this);
	UModule_Limit_None* Limit_Size_Maximal = NewObject<UModule_Limit_Size_Universal>(this);
	Cast<UModule_Limit_Size_Universal>(Limit_Size_Minimal)->SetSizeMinimal(FVector2D(256, 256));
	
	//Add
	AddModule(Limit_Position);
	AddModule(Limit_Size_Minimal);
	AddModule(Limit_Size_Maximal);
	

	

}

void UWindow_Template_0::WindowSetContent(UWidget* InWidget)
{
    while (VerticalBox->GetChildrenCount() > 1) {
        VerticalBox->RemoveChildAt(VerticalBox->GetChildrenCount()-1);
    }
    VerticalBox->AddChild(InWidget);
    Cast<UVerticalBoxSlot>(InWidget->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
    Cast<UVerticalBoxSlot>(InWidget->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
    Cast<UVerticalBoxSlot>(InWidget->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
}
