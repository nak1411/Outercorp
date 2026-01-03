// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FabricationData.generated.h"

/**
 * Data asset that defines a fabrication station's gameplay properties
 * Visual properties (mesh, materials, positions) are controlled in Blueprint
 */
UCLASS(BlueprintType)
class OUTERCORP_API UFabricationData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ============================================================================
	// Basic Info
	// ============================================================================

	/** Display name of the fabrication station */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Station Info")
	FText StationName;

	/** Description of what this station is used for */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Station Info", meta = (MultiLine = true))
	FText Description;

	/** Type/category of station (e.g., "Workbench", "Forge", "Assembler") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Station Info")
	FString StationType;

	/** Icon for UI display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Station Info")
	UTexture2D* Icon;

	/** The Blueprint class to spawn for this station (Blueprint controls mesh, scale, and components) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Station Blueprint")
	TSubclassOf<class AFabricationBase> StationBlueprintClass;

	// ============================================================================
	// Interaction Properties
	// ============================================================================

	/** Interaction range for using this station (in centimeters) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionRange = 200.0f;

	/** Can multiple players use this station at once? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bAllowMultipleUsers = false;

	/** Does this station require power to function? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bRequiresPower = false;

	// ============================================================================
	// Crafting Properties
	// ============================================================================

	/** List of recipe categories this station can craft (e.g., "BasicTools", "Weapons", "Electronics") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	TArray<FName> SupportedRecipeCategories;

	/** Does this station consume power/fuel while crafting? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	bool bConsumesPowerWhileCrafting = false;

	/** Power consumption rate (units per second) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting", meta = (EditCondition = "bConsumesPowerWhileCrafting"))
	float PowerConsumptionRate = 1.0f;

	/** Speed multiplier for crafting (1.0 = normal speed, 2.0 = twice as fast) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float CraftingSpeedMultiplier = 1.0f;

	// ============================================================================
	// Interactive Crafting Mode
	// ============================================================================

	/** Enable interactive crafting mode (RDR2-style camera blend and zone clicking) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Mode")
	bool bEnableInteractiveMode = true;

	/** Camera blend duration when entering/exiting crafting mode */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Mode", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float CameraBlendDuration = 0.5f;

	/** Material to use for highlighting hovered zones */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Mode")
	UMaterialInterface* ZoneHoverMaterial;

	// ============================================================================
	// Visual/Audio Feedback
	// ============================================================================

	/** Sound to play when station starts being used */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback")
	class USoundBase* UseSound;

	/** Sound to play while crafting (looping) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback")
	class USoundBase* CraftingSound;

	/** Particle effect to play while crafting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback")
	class UParticleSystem* CraftingParticle;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("FabricationStation", GetFName());
	}
};
