// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for objects that can be interacted with by the player
 * Implement this interface on actors or components to make them interactable
 */
class OUTERCORP_API IInteractableInterface
{
	GENERATED_BODY()

public:
	/** Called when the player looks at this interactable (when line trace hits) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnLookAt(AActor* LookingActor);
	virtual void OnLookAt_Implementation(AActor* LookingActor) {}

	/** Called when the player stops looking at this interactable */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnLookAway(AActor* LookingActor);
	virtual void OnLookAway_Implementation(AActor* LookingActor) {}

	/** Called when the player interacts with this object (e.g., presses E) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* InteractingActor);
	virtual void OnInteract_Implementation(AActor* InteractingActor) {}

	/** Get the display name shown to the player */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionName() const;
	virtual FText GetInteractionName_Implementation() const { return FText::FromString(TEXT("Interactable")); }

	/** Get the interaction prompt text (e.g., "Press E to Pick Up") */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionPrompt() const;
	virtual FText GetInteractionPrompt_Implementation() const { return FText::FromString(TEXT("Press E to Interact")); }

	/** Check if this object can currently be interacted with */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(AActor* InteractingActor) const;
	virtual bool CanInteract_Implementation(AActor* InteractingActor) const { return true; }

	/** Get the interaction range in centimeters */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	float GetInteractionRange() const;
	virtual float GetInteractionRange_Implementation() const { return 300.0f; }

	/** Should this interactable be highlighted when looked at? */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool ShouldHighlight() const;
	virtual bool ShouldHighlight_Implementation() const { return true; }
};
