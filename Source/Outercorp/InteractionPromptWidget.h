// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionPromptWidget.generated.h"

class UTextBlock;

/**
 * Widget that displays interaction prompts to the player
 * Shows the name of the interactable object and the interaction prompt
 * This should be created in Blueprint and placed in the player's HUD
 */
UCLASS()
class OUTERCORP_API UInteractionPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInteractionPromptWidget(const FObjectInitializer& ObjectInitializer);

	/** Text block for the interactable's name */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* InteractionNameText;

	/** Text block for the interaction prompt (e.g., "Press E to Pick Up") */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* InteractionPromptText;

	/** Update the widget with new interaction info */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractionInfo(const FText& Name, const FText& Prompt);

	/** Show the widget */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ShowPrompt();

	/** Hide the widget */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void HidePrompt();

	/** Is the prompt currently visible? */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	bool IsPromptVisible() const;

	/** Initialize the interaction system - call this after creation */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InitializeInteraction();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Reference to the interaction manager component */
	UPROPERTY()
	class UInteractionManagerComponent* InteractionManager;

	/** Timer handle for updating the prompt */
	FTimerHandle UpdateTimerHandle;

	/** Update function called by timer */
	void UpdatePrompt();
};
