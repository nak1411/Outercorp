// PCGHarvestableManager.h
// Manager for converting PCG instanced meshes to harvestable actors

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarvestableResourceData.h"
#include "PCGHarvestableManager.generated.h"
class AHarvestableResourceActor;
class UInstancedStaticMeshComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UBoxComponent;

/**
 * Reference to a harvestable resource data asset for PCG mapping
 * All PCG-related settings (mesh, actor class, detection range) are stored in the data asset
 */
USTRUCT(BlueprintType)
struct FPCGResourceMapping
{
	GENERATED_BODY()

	/** Resource data asset containing all harvesting and PCG settings (can be base or tree-specific) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mapping")
	TObjectPtr<UHarvestableResourceData> ResourceData = nullptr;

	FPCGResourceMapping() = default;
};

/**
 * Tracks a spawned harvestable actor and its source instance
 */
USTRUCT()
struct FSpawnedResourceInfo
{
	GENERATED_BODY()

	/** The spawned actor */
	UPROPERTY()
	TWeakObjectPtr<AHarvestableResourceActor> SpawnedActor;

	/** Source ISM component */
	UPROPERTY()
	TWeakObjectPtr<UInstancedStaticMeshComponent> SourceComponent;

	/** Original instance index */
	UPROPERTY()
	int32 InstanceIndex = INDEX_NONE;

	/** Original transform */
	UPROPERTY()
	FTransform OriginalTransform;

	/** Whether the instance has been hidden in the ISM */
	UPROPERTY()
	bool bInstanceHidden = false;

	/** Hash of the instance used to track in ConvertedInstances set */
	uint64 InstanceHash = 0;

	FSpawnedResourceInfo()
		: InstanceIndex(INDEX_NONE)
		, bInstanceHidden(false)
		, InstanceHash(0)
	{}
};

/**
 * Manager that handles converting PCG instanced static meshes to harvestable actors
 * Place this actor in the level and configure resource mappings
 */
UCLASS(BlueprintType, Blueprintable)
class OUTERCORP_API APCGHarvestableManager : public AActor
{
	GENERATED_BODY()

public:
	APCGHarvestableManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ============================================
	// CONFIGURATION
	// ============================================

	/** Resource mappings - defines which meshes become which harvestable resources */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TArray<FPCGResourceMapping> ResourceMappings;

	/** How often to check for nearby instances (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (ClampMin = "0.1"))
	float UpdateInterval = 0.25f;

	/** Maximum number of conversions per update tick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (ClampMin = "1"))
	int32 MaxConversionsPerTick = 5;

	/** Distance beyond detection range to despawn actors and restore instances */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (ClampMin = "100.0"))
	float DespawnDistance = 1500.0f;

	/** Should restored instances re-hide when player approaches again */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	bool bAllowReconversion = true;

	/** Tags to search for on ISM components (empty = check all) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	TArray<FName> ISMComponentTags;

	// ============================================
	// DEBUG
	// ============================================

	/** Draw debug visualization */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDrawDebug = false;

	/** Automatically enable collision on discovered ISM/HISM components that have collision disabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	bool bAutoEnableCollision = true;

	/** How often to re-scan for new ISM components (0 = only at BeginPlay).
	 *  Set to a positive value if PCG spawns components after level load. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (ClampMin = "0.0"))
	float RediscoveryInterval = 2.0f;

	// ============================================
	// STATE
	// ============================================

protected:
	/** All tracked ISM components in the level */
	UPROPERTY()
	TArray<TWeakObjectPtr<UInstancedStaticMeshComponent>> TrackedISMComponents;

	/** Timer for rediscovery interval */
	float RediscoveryTimer = 0.0f;

	/** Whether initial discovery has been completed */
	bool bInitialDiscoveryDone = false;

	/** Currently spawned resource actors */
	UPROPERTY()
	TArray<FSpawnedResourceInfo> SpawnedResources;

	/** Set of instance IDs that have been converted (Component + Index hash) */
	TSet<uint64> ConvertedInstances;

	/** Timer for update interval */
	float UpdateTimer;

	/** Cached player pawn */
	UPROPERTY()
	TWeakObjectPtr<APawn> CachedPlayerPawn;

	// ============================================
	// FUNCTIONS
	// ============================================

public:
	/**
	 * Manually register an ISM component for tracking
	 * @param ISMComponent The instanced mesh component to track
	 */
	UFUNCTION(BlueprintCallable, Category = "PCG Harvesting")
	void RegisterISMComponent(UInstancedStaticMeshComponent* ISMComponent);

	/**
	 * Unregister an ISM component from tracking
	 * @param ISMComponent The component to stop tracking
	 */
	UFUNCTION(BlueprintCallable, Category = "PCG Harvesting")
	void UnregisterISMComponent(UInstancedStaticMeshComponent* ISMComponent);

	/**
	 * Find and register all ISM components in the level
	 */
	UFUNCTION(BlueprintCallable, Category = "PCG Harvesting")
	void DiscoverISMComponents();

	/**
	 * Get resource mapping for a specific static mesh
	 * @param Mesh The static mesh to look up
	 * @return Pointer to the mapping if found, nullptr otherwise
	 */
	const FPCGResourceMapping* GetMappingForMesh(UStaticMesh* Mesh) const;

	/**
	 * Get resource mapping for a specific static mesh (Blueprint-friendly version)
	 * @param Mesh The static mesh to look up
	 * @param OutMapping The found mapping if successful
	 * @return True if mapping was found
	 */
	UFUNCTION(BlueprintCallable, Category = "PCG Harvesting")
	bool GetMappingForMeshBP(UStaticMesh* Mesh, FPCGResourceMapping& OutMapping) const;


	/**
	 * Force conversion of a specific instance
	 * @param ISMComponent The source ISM component
	 * @param InstanceIndex The index of the instance to convert
	 * @return The spawned harvestable actor, or nullptr if failed
	 */
	UFUNCTION(BlueprintCallable, Category = "PCG Harvesting")
	AHarvestableResourceActor* ConvertInstance(UInstancedStaticMeshComponent* ISMComponent, int32 InstanceIndex);

protected:
	/** Called each update interval */
	void PerformUpdate();

	/** Check for actors that should be despawned */
	void CheckForDespawn();

	/** Spawn a harvestable actor for an instance */
	AHarvestableResourceActor* SpawnResourceActor(const FPCGResourceMapping& Mapping, UInstancedStaticMeshComponent* ISMComponent, int32 InstanceIndex, const FTransform& InstanceTransform);

	/** Hide an instance in the ISM component */
	void HideInstance(UInstancedStaticMeshComponent* ISMComponent, int32 InstanceIndex);

	/** Restore a hidden instance in the ISM component */
	void RestoreInstance(UInstancedStaticMeshComponent* ISMComponent, int32 InstanceIndex, const FTransform& OriginalTransform);

	/** Generate unique hash for component + instance combination */
	uint64 GenerateInstanceHash(UInstancedStaticMeshComponent* ISMComponent, int32 InstanceIndex) const;

	/** Handle a resource being depleted */
	UFUNCTION()
	void OnResourceDepleted(AHarvestableResourceActor* Resource);

	/** Handle a resource respawning */
	UFUNCTION()
	void OnResourceRespawned(AHarvestableResourceActor* Resource);

	/** Draw debug visualization */
	void DrawDebugInfo();
};
