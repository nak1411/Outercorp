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
	Window->GetWorld()->GetTimerManager().
		SetTimer(TimerHandle, this, &UModule_Size_None::MoveAuto, 0.01f, true);
}

void UModule_Size_None::Deaction() {
	Super::Deaction();
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

