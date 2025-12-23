// Copyright Nyaunix 2025 All Rights Reserved


#include "Module_Move_None.h"
#include "TimerManager.h"

void UModule_Move_None::Action()
{
	// Check if window allows moving
	if (Window && !Window->CanMove())
	{
		return; // Don't allow move action
	}

    Super::Action();
    Prepare(Window);
	Window->GetWorld()->GetTimerManager().
		SetTimer(TimerHandle, this, &UModule_Move_None::MoveAuto, 0.01f, true);
}
void UModule_Move_None::Deaction()
{
	Super::Deaction();
	Window->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}


void UModule_Move_None::Prepare(UWindow* InWindow)
{
}
void UModule_Move_None::Move(UWindow* InWindow)
{
}
void UModule_Move_None::MoveAuto()
{
	Move(Window);
}








//Variables//
void UModule_Move_None::SetXWorking(bool bValue)
{
	bXWorking = bValue;
}

void UModule_Move_None::SetYWorking(bool bValue)
{
	bYWorking = bValue;
}

bool UModule_Move_None::GetXWorking()
{
	return bXWorking;
}

bool UModule_Move_None::GetYWorking()
{
	return bYWorking;
}
