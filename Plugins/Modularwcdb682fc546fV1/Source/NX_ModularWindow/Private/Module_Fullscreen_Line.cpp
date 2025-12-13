// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Fullscreen_Line.h"

bool UModule_Fullscreen_Line::IsFullscreen(UWidget* InWindow)
{
    FVector2D L_Vec = Cast<UCanvasPanelSlot>(InWindow->Slot)->GetPosition();
    float L_Float = L_Vec.X;

    if (!FMath::IsNearlyZero(L_Float, 0.01)) return false;
    L_Float = L_Vec.Y;
    if (!FMath::IsNearlyZero(L_Float, 0.01)) return false;
    

    L_Vec = Cast<UCanvasPanelSlot>(InWindow->Slot)->GetSize();
    L_Float = L_Vec.X;
    if (!FMath::IsNearlyZero(L_Float, 0.01)) return false;
    L_Float = L_Vec.Y;
    if (!FMath::IsNearlyZero(L_Float, 0.01)) return false;
   
    return true;
}

void UModule_Fullscreen_Line::Fullscreen(UWidget* InWindow)
{
    Super::Fullscreen(InWindow);
    Cast<UCanvasPanelSlot>(InWindow->Slot)->SetSize(FVector2D(0, 0));
    Cast<UCanvasPanelSlot>(InWindow->Slot)->SetPosition(FVector2D(0, 0));
}
