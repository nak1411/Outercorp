// HarvestingTool.h
// Tool for harvesting resources (axes, pickaxes, etc.)

#pragma once

#include "CoreMinimal.h"
#include "EquippableTool.h"
#include "HarvestableResourceData.h"
#include "HarvestingTool.generated.h"

class AHarvestableResourceActor;
class USoundBase;
class UNiagaraSystem;

/**
 * Tool used for harvesting resources from the world
 * Examples: Axe for trees, Pickaxe for rocks, Sickle for plants
 */
UCLASS(BlueprintType, Blueprintable)
class OUTERCORP_API AHarvestingTool : public AEquippableTool
{
	GENERATED_BODY()

public:
	AHarvestingTool();

protected:
	virtual void BeginPlay() override;

public:
	// ============================================
	// HARVESTING PROPERTIES
	// ============================================

	/** Type of harvesting this tool can perform */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting")
	EHarvestToolType HarvestToolType = EHarvestToolType::Axe;

	/** Tier/quality of this tool (affects damage and yield) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting", meta = (ClampMin = "1"))
	int32 ToolTier = 1;

	/** Base damage dealt per harvest hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting", meta = (ClampMin = "1.0"))
	float HarvestDamage = 20.0f;

	/** Range at which this tool can harvest resources */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting", meta = (ClampMin = "50.0"))
	float HarvestRange = 250.0f;

	/** Swing time / time between harvests */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting", meta = (ClampMin = "0.1"))
	float SwingTime = 0.5f;

	/** Time into swing when hit occurs (for harvest detection). If 0, uses ToolHit anim notify instead */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Harvesting", meta = (ClampMin = "0.0"))
	float HitTime = 0.0f;

	// ============================================
	// FEEDBACK
	// ============================================

	/** Sound played when swinging the tool */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
	USoundBase *SwingSound;

	/** Sound played on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
	USoundBase *HitSound;

	/** Sound played when hitting nothing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
	USoundBase *MissSound;

	/** Niagara effect on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
	UNiagaraSystem *HitEffect;

	/** Camera shake on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
	TSubclassOf<UCameraShakeBase> HitCameraShake;

	/** Camera shake intensity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float HitCameraShakeIntensity = 0.3f;

	// ============================================
	// STATE
	// ============================================

protected:
	/** Currently targeted resource (if any) */
	UPROPERTY(BlueprintReadOnly, Category = "Harvesting")
	AHarvestableResourceActor *TargetedResource;

	/** Whether we're currently in a swing */
	UPROPERTY(BlueprintReadOnly, Category = "Harvesting")
	bool bIsSwinging;

	/** Whether the hit has been delivered for the current swing */
	bool bHitDelivered;

	/** Timer for swing cooldown */
	FTimerHandle SwingTimerHandle;

	/** Timer for hit detection (fallback if anim notify doesn't work) */
	FTimerHandle HitTimerHandle;

	// ============================================
	// TOOL OVERRIDES
	// ============================================

public:
	virtual void PerformPrimaryAction_Implementation() override;
	virtual void StartPrimaryUse_Implementation() override;
	virtual void StopPrimaryUse_Implementation() override;
	virtual void OnToolHitNotify_Implementation() override;

	/** Initialize harvesting properties from item data */
	virtual void InitializeFromItemData(UInventoryItemData *InItemData) override;

	// ============================================
	// HARVESTING FUNCTIONS
	// ============================================

	/**
	 * Perform a harvest swing
	 * Raycast to find resource and apply damage
	 */
	UFUNCTION(BlueprintCallable, Category = "Harvesting")
	void PerformHarvestSwing();

	/**
	 * Find harvestable resource in front of player
	 * @return The harvestable resource actor if found, nullptr otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Harvesting")
	AHarvestableResourceActor *FindTargetResource();

protected:
	/** Called when swing animation completes */
	void OnSwingComplete();

	/** Play swing feedback effects */
	void PlaySwingFeedback();

	/** Play hit feedback effects */
	void PlayHitFeedback(const FVector &HitLocation);

	/** Play miss feedback effects */
	void PlayMissFeedback();

	/** Get the actual impact point on the target resource */
	FVector GetImpactPoint(AHarvestableResourceActor *Resource) const;
};
