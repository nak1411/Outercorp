// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Size_None.h"
#include "TimerManager.h"

void UModule_Size_None::Action() {
	// Check if window allows resizing
	if (Window && !Window->CanResize())
	{
		return; // Don't allow resize action
	}

	Super::Action();
	Prepare(Window);
	bIsActivelyResizing = true;

	// Set resizing flag for performance optimization
	if (Window)
	{
		Window->SetResizing(true);
	}

	// Use a slower tick rate for the timer fallback
	Window->GetWorld()->GetTimerManager().
		SetTimer(TimerHandle, this, &UModule_Size_None::MoveAuto, 0.016f, true);
}

void UModule_Size_None::Deaction() {
	Super::Deaction();
	bIsActivelyResizing = false;

	// Clear resizing flag
	if (Window)
	{
		Window->SetResizing(false);

		// Fire final position/size update with delegates to ensure everything is in sync
		Window->SetPosition(Window->GetPosition());
		Window->SetSize(Window->GetSize());
	}

	Window->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UModule_Size_None::Prepare(UWindow* InWindow)
{
	
}
void UModule_Size_None::Move(UWindow* InWindow)
{

}

void UModule_Size_None::PrepareAuto()
{
	Prepare(Window);
}

void UModule_Size_None::MoveAuto()
{
	Move(Window);
}

void UModule_Size_None::TickMove(float DeltaTime)
{
	if (bIsActivelyResizing && Window)
	{
		Move(Window);
	}
}
