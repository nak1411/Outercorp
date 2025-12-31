// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ConstructionPart.h"
#include "ConstructionPartData.generated.h"

/**
 * Data asset that defines a construction part's properties
 * Similar to InventoryItemData but for construction parts
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

	/** The mesh to use for this part */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Mesh")
	UStaticMesh* PartMesh;

	/** Default scale of the part */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Mesh")
	FVector MeshScale = FVector::OneVector;

	/** Mass of the part (kg) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	float Mass = 10.0f;

	/** Does this part require a tool to place/fasten? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bRequiresTool = false;

	/** Placement mode for this part */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	EPlacementMode PlacementMode = EPlacementMode::Hybrid;

	/** Snap distance for socket attachment */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	float SnapDistance = 50.0f;

	/** Snap angle tolerance in degrees */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	float SnapAngleTolerance = 15.0f;

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

	/** Enable debug socket visualization */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bDrawSocketDebug = false;

	/** Materials to override on the mesh (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Appearance")
	TArray<UMaterialInterface*> MaterialOverrides;

	/** Material for valid placement ghost preview (blue/green) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Appearance")
	UMaterialInterface* ValidPlacementMaterial;

	/** Material for invalid placement ghost preview (red) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Appearance")
	UMaterialInterface* InvalidPlacementMaterial;

	/** Socket names that should exist on the mesh for validation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Validation")
	TArray<FName> RequiredSockets;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ConstructionPart", GetFName());
	}
};
