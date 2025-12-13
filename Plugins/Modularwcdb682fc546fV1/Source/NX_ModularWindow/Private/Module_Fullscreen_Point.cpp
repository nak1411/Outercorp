// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Fullscreen_Point.h"
#include "Components/Border.h"

bool UModule_Fullscreen_Point::IsFullscreen(UWidget* InWindow)
{
    float a, b;

    a = InWindow->GetParent()->GetCachedGeometry().GetLocalSize().X;
    b = Cast<UCanvasPanelSlot>(InWindow->Slot)->GetSize().X;
    if (!FMath::IsNearlyEqual(a, b, 0.01f)) return false;

    a = InWindow->GetParent()->GetCachedGeometry().GetLocalSize().Y;
    b = Cast<UCanvasPanelSlot>(InWindow->Slot)->GetSize().Y;
    if (!FMath::IsNearlyEqual(a, b, 0.01f)) return false;

    return true;
}

void UModule_Fullscreen_Point::Fullscreen(UWidget* InWindow)
{
    Super::Fullscreen(InWindow);
    Cast<UWindow>(InWindow)->SetPosition(
        -(Cast<UCanvasPanelSlot>(InWindow->Slot)->GetAnchors().Minimum
        *
        InWindow->GetParent()->GetCachedGeometry().GetLocalSize()));
    Cast<UWindow>(InWindow)->SetSize(InWindow->GetParent()->GetCachedGeometry().GetLocalSize());
}
