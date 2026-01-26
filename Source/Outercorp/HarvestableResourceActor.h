// HarvestableResourceActor.h
// Actor representing a harvestable resource in the world

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "HarvestableResourceData.h"
#include "HarvestableResourceActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UInteractableComponent;
class UHarvestableResourceData;
class UTreeHarvestableResourceData;
class UInventoryItemData;
class APickupableItem;
class AOutercorpCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceHarvested, AHarvestableResourceActor *, Resource, float, DamageDealt);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceDepleted, AHarvestableResourceActor *, Resource);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceRespawned, AHarvestableResourceActor *, Resource);

/**
 * Actor representing a harvestable resource (tree, rock, bush, etc.)
 * Can be spawned from PCG instances or placed directly in the world
 */
UCLASS(BlueprintType, Blueprintable)
class OUTERCORP_API AHarvestableResourceActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AHarvestableResourceActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ============================================
	// COMPONENTS
	// ============================================

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent *RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent *ResourceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent *InteractionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractableComponent *InteractableComponent;

	// ============================================
	// RESOURCE DATA
	// ============================================

public:
	/** Data asset defining this resource's properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	UHarvestableResourceData *ResourceData;

	/** Current health of this resource instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	float CurrentHealth;

	/** Whether this resource has been depleted */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	bool bIsDepleted;

	/** Time remaining before respawn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	float RespawnTimeRemaining;

	// ============================================
	// PCG TRACKING
	// ============================================

	/** If spawned from PCG, the index of the original instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	int32 PCGInstanceIndex;

	/** If spawned from PCG, reference to the ISM component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	TWeakObjectPtr<UInstancedStaticMeshComponent> SourceISMComponent;

	/** Original transform from PCG instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	FTransform OriginalTransform;

	/** Display name from source mesh variant */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	FText MeshDisplayName;

	/** Max health from source mesh variant */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	float MeshMaxHealth;

	/** Harvest yields from source mesh variant */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	TArray<FHarvestYield> MeshHarvestYields;

	/** Depletion bonus yields from source mesh variant */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	TArray<FHarvestYield> MeshDepletionBonusYields;

	/** If true, this resource is picked up directly instead of harvested */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	bool bIsDirectPickup;

	/** Item to give on direct pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	UInventoryItemData *DirectPickupItem;

	/** Quantity to give on direct pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PCG")
	int32 DirectPickupQuantity;

	// ============================================
	// VISUAL STATE
	// ============================================

protected:
	/** Original mesh from resource data */
	UPROPERTY()
	UStaticMesh *OriginalMesh;

	/** Original scale */
	UPROPERTY()
	FVector OriginalScale;

	/** Current destruction stage index */
	UPROPERTY()
	int32 CurrentStageIndex;

	/** Last harvester for determining trunk fall direction */
	UPROPERTY()
	AOutercorpCharacter *LastHarvester;

	/** If true, this is a spawned destruction mesh (trunk/stump) and won't spawn further destruction meshes */
	UPROPERTY()
	bool bIsDestructionMesh;

	// ============================================
	// EVENTS
	// ============================================

public:
	/** Called when resource is harvested (hit) */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceHarvested OnResourceHarvested;

	/** Called when resource is fully depleted */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceDepleted OnResourceDepleted;

	/** Called when resource respawns */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResourceRespawned OnResourceRespawned;

	// ============================================
	// INTERACTABLE INTERFACE
	// ============================================

	virtual void OnLookAt_Implementation(AActor *LookingActor) override;
	virtual void OnLookAway_Implementation(AActor *LookingActor) override;
	virtual void OnInteract_Implementation(AActor *InteractingActor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual FText GetInteractionPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(AActor *InteractingActor) const override;
	virtual float GetInteractionRange_Implementation() const override;
	virtual bool ShouldHighlight_Implementation() const override;

	// ============================================
	// HARVESTING
	// ============================================

	/**
	 * Apply harvest damage to this resource
	 * @param Harvester The character harvesting
	 * @param ToolType Type of tool being used
	 * @param ToolTier Tier/quality of the tool
	 * @param BaseDamage Base damage from the tool
	 * @return True if damage was applied
	 */
	UFUNCTION(BlueprintCallable, Category = "Harvesting")
	bool ApplyHarvestDamage(AOutercorpCharacter *Harvester, EHarvestToolType ToolType, int32 ToolTier, float BaseDamage);

	/**
	 * Spawn yield items at this location
	 * @param Yields Array of yields to spawn
	 * @param SpawnLocation Where to spawn the items
	 */
	UFUNCTION(BlueprintCallable, Category = "Harvesting")
	void SpawnYieldItems(const TArray<FHarvestYield> &Yields, const FVector &SpawnLocation);

	/**
	 * Get current health as a percentage (0.0 - 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Harvesting")
	float GetHealthPercent() const;

	// ============================================
	// INITIALIZATION
	// ============================================

	/**
	 * Initialize this resource from data asset
	 * @param Data The resource data to use
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void InitializeFromData(UHarvestableResourceData *Data);

	/**
	 * Initialize from a PCG instance
	 * @param Data The resource data to use
	 * @param ISMComponent The source instanced mesh component
	 * @param InstanceIndex The index of the instance being replaced
	 * @param InstanceTransform The transform of the original instance
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void InitializeFromPCGInstance(UHarvestableResourceData *Data, UInstancedStaticMeshComponent *ISMComponent, int32 InstanceIndex, const FTransform &InstanceTransform);

	// ============================================
	// STATE MANAGEMENT
	// ============================================

	/**
	 * Update visual state based on current health
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void UpdateVisualState();

	/**
	 * Deplete this resource (called when health reaches 0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void Deplete(AOutercorpCharacter *Harvester = nullptr);

	/**
	 * Respawn this resource (reset to full health)
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void Respawn();

	/**
	 * Restore the original PCG instance (if applicable)
	 */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void RestorePCGInstance();

protected:
	/** Handle respawn timer */
	void HandleRespawnTimer(float DeltaTime);

	/** Play harvest feedback effects */
	void PlayHarvestFeedback(const FVector &HitLocation);

	/** Play depletion feedback effects */
	void PlayDepletionFeedback();

	/** Get pickup item class for spawning */
	TSubclassOf<APickupableItem> GetPickupItemClass() const;

	/** Spawn stump and trunk meshes when fully depleted */
	void SpawnDestructionMeshes(AOutercorpCharacter *Harvester = nullptr);
};
