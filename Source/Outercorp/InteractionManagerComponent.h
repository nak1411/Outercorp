// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableInterface.h"
#include "InteractionManagerComponent.generated.h"

class APCGHarvestableManager;
class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
class UMaterialInterface;

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
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

	/** Radius of the interaction trace sphere (0 = line trace, >0 = sphere sweep) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "0.0", ClampMax = "50.0"))
	float InteractionTraceRadius;

	/** Should we draw debug lines for interaction traces? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Debug")
	bool bDrawDebugTrace;

	/** Currently focused interactable object */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	TScriptInterface<IInteractableInterface> CurrentInteractable;

	/** Perform interaction with currently focused object */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	/** Perform interaction with non-harvestable objects only (e.g., pickupable items, NPCs) */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractNonHarvestable();

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

	// ============================================
	// ISM/HARVESTABLE SUPPORT
	// ============================================

	/** Cached PCG harvestable manager */
	UPROPERTY()
	mutable TWeakObjectPtr<APCGHarvestableManager> CachedHarvestableManager;

	/** Pending ISM component for conversion */
	UPROPERTY()
	TWeakObjectPtr<UInstancedStaticMeshComponent> PendingISMComponent;

	/** Pending instance index for conversion */
	int32 PendingInstanceIndex;

	/** Pending hit location for ISM interaction */
	FVector PendingISMHitLocation;

	/** Whether we're looking at a harvestable ISM instance */
	bool bLookingAtHarvestableISM;

	/** Highlight mesh component for ISM instances */
	UPROPERTY()
	UStaticMeshComponent* ISMHighlightMesh;

	/** Currently highlighted instance index */
	int32 HighlightedInstanceIndex;

	/** Currently highlighted ISM component */
	TWeakObjectPtr<UInstancedStaticMeshComponent> HighlightedISMComponent;

	/** Create or update the ISM highlight mesh */
	void UpdateISMHighlight(UInstancedStaticMeshComponent* ISMComp, int32 InstanceIndex);

	/** Clear the ISM highlight mesh */
	void ClearISMHighlight();

	/** Overlay material to apply when highlighting ISM instances */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Highlight")
	UMaterialInterface* HighlightOverlayMaterial;

public:
	/** Check if looking at a harvestable ISM instance */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	bool IsLookingAtHarvestableISM() const { return bLookingAtHarvestableISM; }

	/** Get the name of the harvestable being looked at */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	FText GetPendingHarvestableName() const;

	/** Get the PCG harvestable manager */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	APCGHarvestableManager* GetHarvestableManager() const;

	/** Get pending ISM component being looked at */
	TWeakObjectPtr<UInstancedStaticMeshComponent> GetPendingISMComponent() const { return PendingISMComponent; }

	/** Get pending instance index being looked at */
	int32 GetPendingInstanceIndex() const { return PendingInstanceIndex; }

	/** Clear pending ISM state after conversion */
	void ClearPendingISMState();
};
