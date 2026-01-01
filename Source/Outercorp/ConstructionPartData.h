// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ConstructionPart.h"
#include "ConstructionPartData.generated.h"

/**
 * Data asset that defines a construction part's gameplay properties
 * Visual properties (mesh, materials, arrow positions) are controlled in Blueprint
 */
UCLASS(BlueprintType)
class OUTERCORP_API UConstructionPartData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Display name of the part */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Info")
	FText PartName;

	/** Description of what this part is used for */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Info", meta = (MultiLine = true))
	FText Description;

	/** Type/category of part (e.g., "Beam", "Platform", "Module") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Info")
	FString PartType;

	/** Icon for UI display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Info")
	UTexture2D* Icon;

	/** The Blueprint class to spawn for this part (Blueprint controls mesh, scale, and arrow positions) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Blueprint")
	TSubclassOf<class AConstructionPart> PartBlueprintClass;

	/** Mass of the part (kg) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	float Mass = 10.0f;

	/** Does this part require a tool to place/fasten? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bRequiresTool = false;

	/** Snap distance for socket attachment */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	float SnapDistance = 50.0f;

	/** Snap angle tolerance in degrees */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	float SnapAngleTolerance = 15.0f;

	/** Maximum view angle in degrees to allow snapping (0-180). Lower = must look directly at socket, Higher = can look away more. Default 70 degrees = ~120 degree cone */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties", meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float SnapViewAngleMax = 70.0f;

	/** Minimum placement distance from camera when using mousewheel adjustment */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties", meta = (ClampMin = "50.0"))
	float MinPlacementDistance = 100.0f;

	/** Maximum placement distance from camera when using mousewheel adjustment */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties", meta = (ClampMin = "100.0"))
	float MaxPlacementDistance = 500.0f;

	/** Maximum distance from camera to highlight and delete this part in delete mode */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties", meta = (ClampMin = "100.0"))
	float MaxDeleteDistance = 500.0f;

	/** Maximum allowed overlap percentage (0.0-1.0) before rejecting snap point */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MaxOverlapThreshold = 0.25f;

	/** If true, allows this part to overlap with other parts. If false, rejects any overlap completely. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bAllowOverlap = true;

	/** Can this part be moved after placement? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bCanRelocate = true;

	/** Enable physics simulation when part is placed or moved */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bHasPhysics = false;

	/** Enable debug socket visualization */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bDrawSocketDebug = false;

	/** Enable debug bounding box visualization */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bDrawBoundsDebug = false;

	/** Rotation speed in degrees per second when rotating with A/D keys */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties", meta = (ClampMin = "1.0", ClampMax = "360.0"))
	float RotationSpeed = 90.0f;

	/** Material for valid placement ghost preview (blue/green) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Appearance")
	UMaterialInterface* ValidPlacementMaterial;

	/** Material for invalid placement ghost preview (red) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Appearance")
	UMaterialInterface* InvalidPlacementMaterial;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ConstructionPart", GetFName());
	}
};
