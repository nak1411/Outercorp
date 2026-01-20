// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemData.h"
#include "CraftingRecipeData.h"
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
 * Work surface size determines crafting complexity
 */
UENUM(BlueprintType)
enum class EWorkSurfaceSize : uint8
{
	Small UMETA(DisplayName = "Small"),
	Medium UMETA(DisplayName = "Medium"),
	Large UMETA(DisplayName = "Large")
};

/**
 * Work surface configuration for crafting
 * Determines complexity of items that can be crafted
 */
USTRUCT(BlueprintType)
struct FWorkSurfaceConfig
{
	GENERATED_BODY()

	/** Size of work surface (determines crafting complexity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work Surface")
	EWorkSurfaceSize SurfaceSize = EWorkSurfaceSize::Small;

	/** Maximum number of ingredient slots (e.g., Small=2, Medium=4, Large=6+) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work Surface")
	int32 MaxIngredientSlots = 2;

	/** Maximum number of required tools (e.g., Small=2, Medium=3, Large=5+) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work Surface")
	int32 MaxRequiredTools = 2;

	/** Is this work surface enabled? (allows for modular upgrades) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work Surface")
	bool bIsEnabled = true;
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

	/** Is this a storage module (Toolbox, MaterialBin, Storage)? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module")
	bool bIsStorageModule = false;

	/** Is this a work surface module? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module")
	bool bIsWorkSurface = false;

	/** Widget class for this module's container (only for storage modules) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Container", meta = (EditCondition = "bIsStorageModule", EditConditionHides))
	TSubclassOf<class UContainerWidget> ContainerWidgetClass;

	/** Inventory size for this module (only for storage modules) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Container", meta = (EditCondition = "bIsStorageModule", EditConditionHides))
	int32 InventorySlots = 0;

	/** Item categories allowed in this container (empty = allow all, only for storage modules) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Container", meta = (EditCondition = "bIsStorageModule", EditConditionHides, Bitmask, BitmaskEnum = "/Script/Outercorp.EItemCategory"))
	int32 AllowedItemCategories = 0;

	/** Content widget class for work surface (only for work surface modules) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Work Surface", meta = (EditCondition = "bIsWorkSurface", EditConditionHides))
	TSubclassOf<class UUserWidget> ContentWidgetClass;

	/** Work surface configuration (only for work surface modules) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Work Surface", meta = (EditCondition = "bIsWorkSurface", EditConditionHides))
	FWorkSurfaceConfig WorkSurfaceConfig;

	// ============================================================================
	// Widget Positioning
	// ============================================================================

	/** Should widget position be anchored to the zone mesh location? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Widget Positioning")
	bool bAnchorToMesh = false;

	/** Offset from mesh screen position (if bAnchorToMesh is true) or absolute screen position (if false) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Widget Positioning")
	FVector2D WidgetPositionOffset = FVector2D(200, 150);

	/** Widget window size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Module|Widget Positioning")
	FVector2D WidgetSize = FVector2D(500, 400);
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

	/** Type of fabrication station (used to match with recipe requirements) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Station Info")
	EStationType StationType = EStationType::Workbench;

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

	/** Recipe database reference (optional - provides default recipes for this station) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	TObjectPtr<class UCraftingRecipeDatabase> RecipeDatabase;

	/** Default recipes that are always available at this station (in addition to database) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	TArray<TObjectPtr<UCraftingRecipeData>> DefaultRecipes;

	/** Recipe categories this station can craft (empty = allows all categories) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	TArray<ERecipeCategory> SupportedRecipeCategories;

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
