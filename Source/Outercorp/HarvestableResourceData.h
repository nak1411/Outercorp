// HarvestableResourceData.h
// Data asset defining harvestable resource properties and yields

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemData.h"
#include "HarvestableResourceData.generated.h"

class UInventoryItemData;
class USoundBase;
class UNiagaraSystem;
class UParticleSystem;

/**
 * Type of harvesting tool required
 */
UENUM(BlueprintType)
enum class EHarvestToolType : uint8
{
	None			UMETA(DisplayName = "None (Hand)"),
	Axe				UMETA(DisplayName = "Axe"),
	Pickaxe			UMETA(DisplayName = "Pickaxe"),
	Shovel			UMETA(DisplayName = "Shovel"),
	Sickle			UMETA(DisplayName = "Sickle"),
	Knife			UMETA(DisplayName = "Knife"),
	Any				UMETA(DisplayName = "Any Tool")
};

/**
 * Effectiveness of a tool for harvesting
 */
UENUM(BlueprintType)
enum class EToolEffectiveness : uint8
{
	Incompatible	UMETA(DisplayName = "Incompatible - Cannot Harvest"),
	Poor			UMETA(DisplayName = "Poor - Heavily Penalized"),
	Suboptimal		UMETA(DisplayName = "Suboptimal - Penalized"),
	Optimal			UMETA(DisplayName = "Optimal - Full Effectiveness")
};

/**
 * Tool effectiveness configuration for a specific tool type
 */
USTRUCT(BlueprintType)
struct FToolEffectivenessEntry
{
	GENERATED_BODY()

	/** The tool type this entry applies to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Effectiveness")
	EHarvestToolType ToolType = EHarvestToolType::None;

	/** How effective this tool is */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Effectiveness")
	EToolEffectiveness Effectiveness = EToolEffectiveness::Incompatible;

	/** Damage multiplier (0.0 to 1.0+). Applied to base damage. 0 = incompatible */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Effectiveness", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float DamageMultiplier = 1.0f;

	/** Yield multiplier (0.0 to 1.0+). Applied to harvest yields. Lower = fewer resources */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Effectiveness", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float YieldMultiplier = 1.0f;

	/** Minimum tool tier required for this effectiveness level (0 = any tier) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool Effectiveness", meta = (ClampMin = "0"))
	int32 MinimumToolTier = 0;

	FToolEffectivenessEntry()
		: ToolType(EHarvestToolType::None)
		, Effectiveness(EToolEffectiveness::Incompatible)
		, DamageMultiplier(1.0f)
		, YieldMultiplier(1.0f)
		, MinimumToolTier(0)
	{}
};


/**
 * Single item drop from harvesting
 */
USTRUCT(BlueprintType)
struct FHarvestYield
{
	GENERATED_BODY()

	/** Item to drop */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yield")
	UInventoryItemData* ItemData = nullptr;

	/** Minimum quantity per harvest hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yield", meta = (ClampMin = "0"))
	int32 MinQuantity = 1;

	/** Maximum quantity per harvest hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yield", meta = (ClampMin = "1"))
	int32 MaxQuantity = 1;

	/** Chance to drop this item (0.0 - 1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yield", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DropChance = 1.0f;

	/** Minimum tool tier required for this drop (0 = any) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yield", meta = (ClampMin = "0"))
	int32 RequiredToolTier = 0;

	/** Offset from spawn origin for this yield type (added to base spawn offset) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FVector SpawnOffset = FVector::ZeroVector;

	/** Random offset range to add variation to spawn positions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (ClampMin = "0.0"))
	float SpawnRandomRadius = 0.0f;

	/** Base height offset for spawned items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnHeightOffset = 100.0f;

	/** Whether spawned items should simulate physics and receive impulse */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bEnablePhysics = true;

	FHarvestYield()
		: ItemData(nullptr)
		, MinQuantity(1)
		, MaxQuantity(1)
		, DropChance(1.0f)
		, RequiredToolTier(0)
		, SpawnOffset(FVector::ZeroVector)
		, SpawnRandomRadius(0.0f)
		, SpawnHeightOffset(100.0f)
		, bEnablePhysics(true)
	{}
};

/**
 * Stage of destruction with visual changes
 */
USTRUCT(BlueprintType)
struct FHarvestStage
{
	GENERATED_BODY()

	/** Health percentage threshold to enter this stage (1.0 = full, 0.0 = depleted) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HealthThreshold = 1.0f;

	/** Optional mesh to swap to at this stage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	UStaticMesh* StageMesh = nullptr;

	/** Optional material override at this stage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	UMaterialInterface* StageMaterial = nullptr;

	/** Scale modifier at this stage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FVector ScaleModifier = FVector(1.0f);

	FHarvestStage()
		: HealthThreshold(1.0f)
		, StageMesh(nullptr)
		, StageMaterial(nullptr)
		, ScaleModifier(FVector(1.0f))
	{}
};

/**
 * A source mesh variant with its own settings
 */
USTRUCT(BlueprintType)
struct FHarvestableSourceMesh
{
	GENERATED_BODY()

	/** The static mesh used by PCG instances */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* Mesh = nullptr;

	/** Display name for this variant (e.g., "Small Maple Tree") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FText DisplayName;

	/** Actor class to spawn (leave null to use default HarvestableResourceActor) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSubclassOf<class AHarvestableResourceActor> ActorClass;

	/** Maximum health/durability */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "1.0"))
	float MaxHealth = 100.0f;

	/** If true, interacting picks up the item directly instead of harvesting for yields */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	bool bDirectPickup = false;

	/** Item to give when directly picked up (only used if bDirectPickup is true) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (EditCondition = "bDirectPickup"))
	UInventoryItemData* DirectPickupItem = nullptr;

	/** Quantity to give on direct pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (EditCondition = "bDirectPickup", ClampMin = "1"))
	int32 DirectPickupQuantity = 1;

	/** Items yielded when harvesting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yields", meta = (EditCondition = "!bDirectPickup"))
	TArray<FHarvestYield> HarvestYields;

	/** Bonus yields when fully depleted (final hit) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yields", meta = (EditCondition = "!bDirectPickup"))
	TArray<FHarvestYield> DepletionBonusYields;

	FHarvestableSourceMesh()
		: Mesh(nullptr)
		, MaxHealth(100.0f)
		, bDirectPickup(false)
		, DirectPickupItem(nullptr)
		, DirectPickupQuantity(1)
	{}
};

/**
 * Data asset defining a harvestable type (tree, rock, bush, etc.)
 */
UCLASS(BlueprintType)
class OUTERCORP_API UHarvestableResourceData : public UDataAsset
{
	GENERATED_BODY()

public:
	// ============================================
	// PCG / INSTANCED MESH SETTINGS
	// ============================================

	/** Source mesh variants that map to this harvestable (e.g., small/medium/large tree variants) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PCG")
	TArray<FHarvestableSourceMesh> SourceMeshes;

	// ============================================
	// HARVESTING REQUIREMENTS
	// ============================================

	/** Tool effectiveness configurations. Defines which tools work and how well. If empty, uses legacy RequiredToolType. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvesting")
	TArray<FToolEffectivenessEntry> ToolEffectiveness;

	/** LEGACY: Type of tool required to harvest (deprecated - use ToolEffectiveness instead) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvesting|Legacy", meta = (EditCondition = "ToolEffectiveness.Num() == 0"))
	EHarvestToolType RequiredToolType = EHarvestToolType::None;

	/** LEGACY: Minimum tool tier required (deprecated - use ToolEffectiveness instead) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvesting|Legacy", meta = (ClampMin = "0", EditCondition = "ToolEffectiveness.Num() == 0"))
	int32 MinimumToolTier = 0;

	/** LEGACY: Can be harvested by hand (deprecated - use ToolEffectiveness instead) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvesting|Legacy", meta = (EditCondition = "ToolEffectiveness.Num() == 0"))
	bool bCanHarvestByHand = false;

	/** Damage dealt per harvest hit with bare hands (unarmed) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvesting", meta = (ClampMin = "0.0"))
	float HandHarvestDamage = 5.0f;

	// ============================================
	// HEALTH & DURABILITY
	// ============================================

	/** Armor/resistance reduces incoming damage by this flat amount */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0"))
	float Armor = 0.0f;

	/** Does this resource respawn after being depleted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	bool bCanRespawn = true;

	/** Time in seconds before respawning (if bCanRespawn is true) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0", EditCondition = "bCanRespawn"))
	float RespawnTime = 300.0f;

	// ============================================
	// YIELDS
	// ============================================

	// ============================================
	// VISUALS & FEEDBACK
	// ============================================

	/** Destruction stages for visual feedback */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
	TArray<FHarvestStage> DestructionStages;

	/** Sound played on each harvest hit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback")
	USoundBase* HarvestSound = nullptr;

	/** Sound played when resource is fully depleted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback")
	USoundBase* DepletionSound = nullptr;

	/** Particle effect on harvest hit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback")
	UParticleSystem* HarvestParticle = nullptr;

	/** Niagara effect on harvest hit (preferred over legacy particles) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback")
	UNiagaraSystem* HarvestNiagaraEffect = nullptr;

	/** Camera shake on harvest hit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback")
	TSubclassOf<UCameraShakeBase> HarvestCameraShake;

	/** Intensity of camera shake */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float CameraShakeIntensity = 0.5f;

	// ============================================
	// INTERACTION
	// ============================================

	/** Text shown when looking at this resource */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText InteractionPrompt;

	/** Range at which player can interact */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "50.0"))
	float InteractionRange = 300.0f;

	/** Should highlight when looked at */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bHighlightOnLookAt = true;

	// ============================================
	// DEBUG SETTINGS
	// ============================================

	/** Show collision debug visualization for this resource type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bDebugShowCollision = false;

	/** Show health bar above this resource type when damaged */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bDebugShowHealth = false;

	/** Show tool raycast debug when harvesting this resource type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bDebugShowToolRaycast = false;

	// ============================================
	// HELPER FUNCTIONS
	// ============================================

	/** Get the destruction stage for a given health percentage */
	const FHarvestStage* GetStageForHealth(float HealthPercent) const;

	/** Get the destruction stage for a given health percentage (Blueprint-friendly version) */
	UFUNCTION(BlueprintCallable, Category = "Harvestable Resource")
	bool GetStageForHealthBP(float HealthPercent, FHarvestStage& OutStage) const;

	/** Calculate total damage after armor */
	UFUNCTION(BlueprintCallable, Category = "Harvestable Resource")
	float CalculateDamage(float IncomingDamage) const;

	/** LEGACY: Check if a tool type is valid for this resource (deprecated - use GetToolEffectiveness instead) */
	UFUNCTION(BlueprintCallable, Category = "Harvestable Resource")
	bool IsToolTypeValid(EHarvestToolType ToolType, int32 ToolTier) const;

	/** Get the effectiveness of a specific tool for this resource. Returns multipliers and compatibility. */
	UFUNCTION(BlueprintCallable, Category = "Harvestable Resource")
	bool GetToolEffectiveness(EHarvestToolType ToolType, int32 ToolTier, float& OutDamageMultiplier, float& OutYieldMultiplier) const;

	/** Check if a tool can harvest this resource at all (not incompatible) */
	UFUNCTION(BlueprintCallable, Category = "Harvestable Resource")
	bool CanToolHarvest(EHarvestToolType ToolType, int32 ToolTier) const;

	/** Check if this data asset contains the given mesh */
	bool ContainsMesh(UStaticMesh* Mesh) const;

	/** Get the source mesh entry for a given mesh (returns nullptr if not found) */
	const FHarvestableSourceMesh* GetSourceMeshEntry(UStaticMesh* Mesh) const;
};
