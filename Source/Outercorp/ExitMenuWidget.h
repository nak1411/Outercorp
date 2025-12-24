// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExitMenuWidget.generated.h"

class UButton;

/**
 * Exit menu widget with Resume, Settings, and Exit buttons
 * Pauses the game when opened, appears on top of all other UI
 */
UCLASS()
class OUTERCORP_API UExitMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool NativeSupportsKeyboardFocus() const override { return true; }

	/** Resume button - closes menu and resumes game */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> ResumeButton;

	/** Settings button - placeholder for future settings menu */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> SettingsButton;

	/** Exit button - exits the game */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> ExitButton;

public:
	/** Open the exit menu and pause the game */
	UFUNCTION(BlueprintCallable, Category = "Exit Menu")
	void OpenExitMenu();

	/** Close the exit menu and resume the game */
	UFUNCTION(BlueprintCallable, Category = "Exit Menu")
	void CloseExitMenu();

	/** Delegate called when the exit menu is closed */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitMenuClosed);

	UPROPERTY(BlueprintAssignable, Category = "Exit Menu")
	FOnExitMenuClosed OnExitMenuClosed;

protected:
	/** Resume button clicked */
	UFUNCTION()
	void OnResumeButtonClicked();

	/** Settings button clicked */
	UFUNCTION()
	void OnSettingsButtonClicked();

	/** Exit button clicked */
	UFUNCTION()
	void OnExitButtonClicked();

private:
	/** Tracks if this widget instance paused the game */
	bool bDidPauseGame = false;
};
