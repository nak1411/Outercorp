// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OutercorpPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UExitMenuWidget;

/**
 *  Simple first person Player Controller
 *  Manages the input mapping context.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract, config="Game")
class OUTERCORP_API AOutercorpPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Constructor */
	AOutercorpPlayerController();

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** If true, the player will use UMG touch controls even if not playing on mobile platforms */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/** Exit menu widget class to spawn when ESC is pressed */
	UPROPERTY(EditAnywhere, Category = "UI|Exit Menu")
	TSubclassOf<UExitMenuWidget> ExitMenuWidgetClass;

	/** Pointer to the currently active exit menu widget */
	UPROPERTY()
	TObjectPtr<UExitMenuWidget> ExitMenuWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	/** Returns true if the player should use UMG touch controls */
	bool ShouldUseTouchControls() const;

public:
	/** Toggle the exit menu on/off */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleExitMenu();

protected:
	/** Called when exit menu is closed */
	UFUNCTION()
	void OnExitMenuClosed();
};
