// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Limit_Size_Minimal_Line.h"
#include "Components/Border.h"

void UModule_Limit_Size_Minimal_Line::Prepare(UWidget* InWindow)
{
	Super::Prepare(InWindow);
	CanvasSize = 
		(
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetAnchors().Maximum
		-
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetAnchors().Minimum
		)
		*
		InWindow->GetParent()->GetCachedGeometry().GetLocalSize();
}



float UModule_Limit_Size_Minimal_Line::ModifyLeft(UWidget* InWindow, float InValue)
{
	return FMath::Min(InValue,
		CanvasSize.X - 
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetSize().X - 
		SizeMinimal.X
	);
}

float UModule_Limit_Size_Minimal_Line::ModifyUp(UWidget* InWindow, float InValue)
{
	return FMath::Min(InValue,
		CanvasSize.Y -
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetSize().Y -
		SizeMinimal.Y
	);
}
float UModule_Limit_Size_Minimal_Line::ModifyRight(UWidget* InWindow, float InValue)
{
	return FMath::Min(InValue,
		CanvasSize.X -
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetPosition().X -
		SizeMinimal.X
	);
}
float UModule_Limit_Size_Minimal_Line::ModifyDown(UWidget* InWindow, float InValue)
{
	return FMath::Min(InValue,
		CanvasSize.Y -
		Cast<UCanvasPanelSlot>(InWindow->Slot)->GetPosition().Y -
		SizeMinimal.Y
	);
}
