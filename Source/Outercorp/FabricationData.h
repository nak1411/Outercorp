// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemData.h"
#include "FabricationData.generated.h"

/**
 * Common fabrication zone types
 */
UENUM(BlueprintType)
enum class EFabricationZoneType : uint8
{
	Toolbox UMETA(DisplayName = "Toolbox"),
	MaterialBin UMETA(DisplayName = "Material Bin"),
	WorkSurface UMETA(DisplayName = "Work Surface"),
	PowerSupply UMETA(DisplayName = "Power Supply"),
	Storage UMETA(DisplayName = "Storage"),
	Custom UMETA(DisplayName = "Custom (Use Custom Name)")
};

/**
 * Defines a module that can be installed in a fabrication station zone
 */
USTRUCT(BlueprintType)
struct FFabricationZoneModule
{
	GENERATED_BODY()

	/** Display name of this module shown to players (e.g., "Basic Toolbox", "Material Storage") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module")
	FText ModuleName;

	/** Internal ID for this module type (leave as None to auto-generate from zone type) - only needed for module swapping/upgrades */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module")
	FName ModuleID;

	/** Description of what this module does */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module", meta = (MultiLine = true))
	FText Description;

	/** Icon for UI display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module")
	UTexture2D* ModuleIcon;

	/** Widget class for this module's container (if it has storage) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Container")
	TSubclassOf<class UContainerWidget> ContainerWidgetClass;

	/** Inventory size for this module (0 if not a storage module) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Container")
	int32 InventorySlots = 0;

	/** Item categories allowed in this container (empty = allow all) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Container", meta = (Bitmask, BitmaskEnum = "/Script/Outercorp.EItemCategory"))
	int32 AllowedItemCategories = 0;
};

/**
 * Defines what modules are compatible with a zone TYPE
 * Data asset only knows about zone types, not specific instances
 */
USTRUCT(BlueprintType)
struct FFabricationZoneTypeConfig
{
	GENERATED_BODY()

	/** Preset zone type (select "Custom" to use custom name below) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Type")
	EFabricationZoneType ZoneTypePreset = EFabricationZoneType::Custom;

	/** Custom zone type name (only used if ZoneTypePreset is set to "Custom") - must match Blueprint component naming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Type", meta = (EditCondition = "ZoneTypePreset == EFabricationZoneType::Custom", EditConditionHides))
	FName CustomZoneTypeName;

	/** Default module to install in zones of this type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Type")
	FFabricationZoneModule DefaultModule;

	/** List of modules that CAN be installed in zones of this type (for future upgrades/swapping) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Type")
	TArray<FFabricationZoneModule> CompatibleModules;

	/** Helper function to get the actual zone type name (converts enum to FName or returns custom name) */
	FName GetZoneTypeName() const
	{
		if (ZoneTypePreset == EFabricationZoneType::Custom)
		{
			return CustomZoneTypeName;
		}

		// Convert enum to FName
		switch (ZoneTypePreset)
		{
		case EFabricationZoneType::Toolbox:
			return FName("Toolbox");
		case EFabricationZoneType::MaterialBin:
			return FName("MaterialBin");
		case EFabricationZoneType::WorkSurface:
			return FName("WorkSurface");
		case EFabricationZoneType::PowerSupply:
			return FName("PowerSupply");
		case EFabricationZoneType::Storage:
			return FName("Storage");
		default:
			return NAME_None;
		}
	}
};

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
	// Modular Zones System
	// ============================================================================

	/** Module library - defines what modules are compatible with each zone TYPE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular Zones")
	TArray<FFabricationZoneTypeConfig> ZoneTypeConfigs;

	/** Naming convention for auto-detecting zone components (e.g., "*Zone" will match "ToolboxZone", "MaterialBinZone") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular Zones")
	FString ZoneComponentSuffix = TEXT("Zone");

	/** Naming convention for auto-detecting zone meshes (e.g., "*Mesh" will match "ToolboxMesh", "MaterialBinMesh") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular Zones")
	FString ZoneMeshSuffix = TEXT("Mesh");

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
