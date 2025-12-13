// Copyright Nyaunix 2025 All Rights Reserved


#include "Window_Timer_Event.h"
#include "TimerManager.h"

void UWindow_Timer_Event::CarryEventPosition()
{
	if (TickPosition < 1) {
		TickPosition = 10;
		Window->SaveWindowParametersStart();
		Window->ED_PositionStart.Broadcast();
		Window->GetWorld()->GetTimerManager().
			SetTimer(TimerPosition, this, &UWindow_Timer_Event::TimerTickPosition, 0.02f, true);
	}
	else {
		TickPosition = 10;
		Window->ED_PositionChanged.Broadcast();
	}
}

void UWindow_Timer_Event::CarryEventSize()
{
	if (TickSize < 1) {
		TickSize = 10;
		Window->SaveWindowParametersStart();
		Window->ED_SizeStart.Broadcast();
		Window->GetWorld()->GetTimerManager().
			SetTimer(TimerSize, this, &UWindow_Timer_Event::TimerTickSize, 0.02f, true);
	}
	else {
		TickSize = 10;
		Window->ED_SizeChanged.Broadcast();
	}
}

void UWindow_Timer_Event::TimerTickPosition()
{
	TickPosition--;
	if (TickPosition < 1) {
		Window->GetWorld()->GetTimerManager().ClearTimer(TimerPosition);
		Window->ED_PositionEnd.Broadcast();
	}
}

void UWindow_Timer_Event::TimerTickSize()
{
	TickSize--;
	if (TickSize < 1) {
		Window->GetWorld()->GetTimerManager().ClearTimer(TimerSize);
		Window->ED_SizeEnd.Broadcast();
	}
}
