// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableInterface.h"
#include "InteractableComponent.generated.h"

// Delegate declaration (must be before UCLASS)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionDelegate, AActor*, InteractingActor);

/**
 * Component that makes an actor interactable
 * Add this to any actor to make it interactable by the player
 * All properties are exposed to Blueprint for easy customization
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class OUTERCORP_API UInteractableComponent : public UActorComponent, public IInteractableInterface
{
	GENERATED_BODY()

public:
	UInteractableComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** Display name shown to the player when looking at this object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionName;

	/** Interaction prompt text (e.g., "Press E to Pick Up") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionPrompt;

	/** Maximum distance from which this can be interacted with (in centimeters) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "0.0"))
	float InteractionRange;

	/** Should this object be highlighted when looked at? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bShouldHighlight;

	/** Can this object currently be interacted with? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract;

	/** Highlight color when looking at this object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Highlight")
	FLinearColor HighlightColor;

	/** Highlight intensity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Highlight", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float HighlightIntensity;

	/** Blueprint event called when player looks at this object */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionDelegate OnLookedAt;

	/** Blueprint event called when player stops looking at this object */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionDelegate OnLookedAway;

	/** Blueprint event called when player interacts with this object */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionDelegate OnInteracted;

	// IInteractableInterface implementation
	virtual void OnLookAt_Implementation(AActor* LookingActor) override;
	virtual void OnLookAway_Implementation(AActor* LookingActor) override;
	virtual void OnInteract_Implementation(AActor* InteractingActor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual FText GetInteractionPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(AActor* InteractingActor) const override;
	virtual float GetInteractionRange_Implementation() const override;
	virtual bool ShouldHighlight_Implementation() const override;

	/** Set custom highlight on mesh */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetHighlighted(bool bHighlighted);

	/** Get the mesh component to apply highlight to */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	UMeshComponent* GetMeshComponent() const;

protected:
	/** Currently highlighted state */
	bool bIsHighlighted;

	/** Original render custom depth state */
	bool bOriginalCustomDepthEnabled;
};
