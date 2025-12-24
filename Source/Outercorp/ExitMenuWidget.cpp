// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExitMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

void UExitMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button events
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UExitMenuWidget::OnResumeButtonClicked);
	}

	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &UExitMenuWidget::OnSettingsButtonClicked);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UExitMenuWidget::OnExitButtonClicked);
	}
}

void UExitMenuWidget::NativeDestruct()
{
	// If we paused the game, make sure to unpause when widget is destroyed
	if (bDidPauseGame)
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
		}
		bDidPauseGame = false;
	}

	Super::NativeDestruct();
}

void UExitMenuWidget::OpenExitMenu()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		// Add to viewport first with high z-order to appear on top of everything
		AddToViewport(1000);

		// Pause the game
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bDidPauseGame = true;

		// Show mouse cursor and set input mode to UI only
		PC->SetShowMouseCursor(true);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(GetCachedWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);

		// Set focus to this widget
		SetFocus();
	}
}

void UExitMenuWidget::CloseExitMenu()
{
	if (bDidPauseGame)
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			// Unpause the game
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			bDidPauseGame = false;

			// Hide mouse cursor and set input mode back to game only
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
		}
	}

	// Broadcast close event
	OnExitMenuClosed.Broadcast();

	// Remove from viewport
	RemoveFromParent();
}

void UExitMenuWidget::OnResumeButtonClicked()
{
	CloseExitMenu();
}

void UExitMenuWidget::OnSettingsButtonClicked()
{
	// Placeholder for settings menu
	// TODO: Implement settings menu
	UE_LOG(LogTemp, Log, TEXT("Settings button clicked - not yet implemented"));
}

void UExitMenuWidget::OnExitButtonClicked()
{
	// Exit the game
	if (APlayerController* PC = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, false);
	}
}
