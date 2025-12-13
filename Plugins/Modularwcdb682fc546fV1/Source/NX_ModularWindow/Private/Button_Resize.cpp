// Copyright Nyaunix 2025 All Rights Reserved

#include "Button_Resize.h"  
#include "Components/Button.h"  
#include "Styling/SlateBrush.h"
#include "Brushes/SlateNoResource.h"

UButton_Resize::UButton_Resize(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)  
{  
    FSlateNoResource SlateBrush;
    SlateBrush.ImageSize = FVector2D(6, 6);
    const FButtonStyle ButtonStyle = FButtonStyle()
        .SetNormal(SlateBrush)
        .SetHovered(SlateBrush)
        .SetPressed(SlateBrush)
        .SetNormalPadding(FMargin(0, 0, 0, 0))
        .SetPressedPadding(FMargin(0, 0, 0, 0));
    SetStyle(ButtonStyle);
}
