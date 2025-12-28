// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InventoryItemData.h"
#include "PickupableItem.generated.h"

class UStaticMeshComponent;
class UInteractableComponent;

/**
 * Actor representing an item that can be picked up and added to inventory
 * Implements IInteractableInterface for interaction support
 */
UCLASS()
class OUTERCORP_API APickupableItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	APickupableItem();

protected:
	virtual void BeginPlay() override;

public:
	/** Root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	/** Mesh component for the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	/** Interactable component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractableComponent* InteractableComponent;

	/** Item data asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UInventoryItemData* ItemData;

	/** Quantity of this item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ClampMin = "1"))
	int32 Quantity;

	/** Sound to play when picked up */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Feedback")
	class USoundBase* PickupSound;

	/** Particle effect to spawn when picked up */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Feedback")
	class UParticleSystem* PickupParticle;

	// IInteractableInterface implementation
	virtual void OnLookAt_Implementation(AActor* LookingActor) override;
	virtual void OnLookAway_Implementation(AActor* LookingActor) override;
	virtual void OnInteract_Implementation(AActor* InteractingActor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual FText GetInteractionPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(AActor* InteractingActor) const override;
	virtual float GetInteractionRange_Implementation() const override;
	virtual bool ShouldHighlight_Implementation() const override;

	/** Called when item is successfully picked up */
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void OnPickedUp(AActor* PickedUpBy);

	/** Initialize the item with data */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitializeItem(UInventoryItemData* InItemData, int32 InQuantity = 1);

	/** Start the drop cooldown timer (called when item is dropped from inventory) */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void StartDropCooldown();

	/** Time after drop before item becomes interactable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float DropCooldownTime = 1.0f;

protected:
	/** Whether the item is currently in drop cooldown */
	bool bIsInDropCooldown = false;

	/** Timer handle for drop cooldown */
	FTimerHandle DropCooldownTimerHandle;

	/** Called when drop cooldown expires */
	void OnDropCooldownComplete();
};
