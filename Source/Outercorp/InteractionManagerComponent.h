// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableInterface.h"
#include "InteractionManagerComponent.generated.h"

/**
 * Component that handles interaction for a character
 * Performs line traces to detect interactable objects and manages interaction state
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class OUTERCORP_API UInteractionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionManagerComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** Maximum distance to check for interactables (in centimeters) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionTraceDistance;

	/** How often to check for interactables (0 = every frame) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "0.0"))
	float InteractionCheckInterval;

	/** Trace channel to use for interaction traces */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel;

	/** Should we draw debug lines for interaction traces? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Debug")
	bool bDrawDebugTrace;

	/** Currently focused interactable object */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	TScriptInterface<IInteractableInterface> CurrentInteractable;

	/** Perform interaction with currently focused object */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	/** Get the current interactable actor (if any) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	AActor* GetCurrentInteractableActor() const;

	/** Get the current interactable's display name */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	FText GetCurrentInteractionName() const;

	/** Get the current interactable's prompt text */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	FText GetCurrentInteractionPrompt() const;

	/** Check if we're currently looking at an interactable */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	bool IsLookingAtInteractable() const;

protected:
	/** Perform line trace to check for interactables */
	void CheckForInteractables();

	/** Set the currently focused interactable */
	void SetCurrentInteractable(TScriptInterface<IInteractableInterface> NewInteractable);

	/** Timer for interaction checks */
	float InteractionCheckTimer;

	/** Previous interactable (for detecting changes) */
	TScriptInterface<IInteractableInterface> PreviousInteractable;
};
