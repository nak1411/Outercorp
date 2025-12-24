// Copyright Epic Games, Inc. All Rights Reserved.


#include "OutercorpPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "OutercorpCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "Outercorp.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "ExitMenuWidget.h"
#include "Components/InputComponent.h"

AOutercorpPlayerController::AOutercorpPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AOutercorpCameraManager::StaticClass();
}

void AOutercorpPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogOutercorp, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AOutercorpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}

		// Bind ESC key to toggle exit menu (bExecuteWhenPaused = true to work when game is paused)
		InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AOutercorpPlayerController::ToggleExitMenu).bExecuteWhenPaused = true;
	}

}

bool AOutercorpPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AOutercorpPlayerController::ToggleExitMenu()
{
	// If exit menu is already open, close it
	if (ExitMenuWidget && ExitMenuWidget->IsInViewport())
	{
		ExitMenuWidget->CloseExitMenu();
		return;
	}

	// Create and open the exit menu
	if (ExitMenuWidgetClass)
	{
		ExitMenuWidget = CreateWidget<UExitMenuWidget>(this, ExitMenuWidgetClass);
		if (ExitMenuWidget)
		{
			// Bind to close event
			ExitMenuWidget->OnExitMenuClosed.AddDynamic(this, &AOutercorpPlayerController::OnExitMenuClosed);

			// Open the menu (this will pause the game and add to viewport)
			ExitMenuWidget->OpenExitMenu();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ExitMenuWidgetClass is not set in PlayerController!"));
	}
}

void AOutercorpPlayerController::OnExitMenuClosed()
{
	// Clear the reference when the menu is closed
	ExitMenuWidget = nullptr;
}
