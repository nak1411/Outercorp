// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ConstructionPart.h"
#include "ConstructionPartData.generated.h"

/**
 * Common socket types for easy selection and filtering
 */
UENUM(BlueprintType)
enum class ESocketTypePreset : uint8
{
	Any				UMETA(DisplayName = "Any (snaps to anything)"),

	// Directional - use these for parts that should only snap in specific orientations
	Top				UMETA(DisplayName = "Top"),
	Bottom			UMETA(DisplayName = "Bottom"),
	Front			UMETA(DisplayName = "Front"),
	Back			UMETA(DisplayName = "Back"),
	Left			UMETA(DisplayName = "Left"),
	Right			UMETA(DisplayName = "Right"),

	// Structural - use these for specific part types
	MountPoint		UMETA(DisplayName = "Mount Point"),
	BeamEnd			UMETA(DisplayName = "Beam End"),
	Corner			UMETA(DisplayName = "Corner"),

	// Custom - if you need a unique type, use the text field below
	Custom			UMETA(DisplayName = "Custom (see Custom Type)")
};

/**
 * Simplified socket definition - just name the socket and what it can snap to
 */
USTRUCT(BlueprintType)
struct FSocketTypeDefinition
{
	GENERATED_BODY()

	/** The name of the socket on this mesh (must match socket name in your 3D model) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

	/** What type is this socket? Use "Any" to snap to everything, or pick a specific type. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket Type")
	ESocketTypePreset SocketTypePreset = ESocketTypePreset::Any;

	/** If you selected "Custom" above, type your custom socket type name here */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket Type", meta = (EditCondition = "SocketTypePreset == ESocketTypePreset::Custom", EditConditionHides))
	FName CustomSocketType;

	/** OPTIONAL: If empty, this socket snaps to ANY other socket. Add types here to filter what this can snap to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snap Filtering", meta = (DisplayName = "Can Snap To (leave empty for 'any')"))
	TArray<ESocketTypePreset> CanSnapTo;

	/** Position offset to apply when snapping (local space, advanced) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
	FVector PositionOffset = FVector::ZeroVector;

	/** Rotation offset in degrees (applied after auto-alignment, advanced) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
	float RotationOffset = 0.0f;

	/** Auto-align rotation based on socket direction? Usually keep this ON. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
	bool bUseAutomaticRotation = true;

	FSocketTypeDefinition()
		: SocketName(NAME_None)
		, SocketTypePreset(ESocketTypePreset::Any)
		, CustomSocketType(NAME_None)
		, PositionOffset(FVector::ZeroVector)
		, RotationOffset(0.0f)
		, bUseAutomaticRotation(true)
	{}

	/** Get the actual socket type name for this socket */
	FName GetSocketType() const
	{
		if (SocketTypePreset == ESocketTypePreset::Custom)
		{
			return CustomSocketType;
		}

		// Convert enum to FName
		const UEnum* EnumPtr = StaticEnum<ESocketTypePreset>();
		if (EnumPtr)
		{
			return FName(*EnumPtr->GetNameStringByValue((int64)SocketTypePreset));
		}

		return FName("Any");
	}

	/** Get the list of allowed socket types this can snap to (as FNames for comparison) */
	TArray<FName> GetAllowedSocketTypes() const
	{
		TArray<FName> AllowedTypes;

		// If CanSnapTo is empty, allow snapping to anything
		if (CanSnapTo.Num() == 0)
		{
			AllowedTypes.Add(FName("Any"));
			return AllowedTypes;
		}

		// Convert preset enums to FNames
		const UEnum* EnumPtr = StaticEnum<ESocketTypePreset>();
		for (ESocketTypePreset Preset : CanSnapTo)
		{
			if (EnumPtr)
			{
				AllowedTypes.Add(FName(*EnumPtr->GetNameStringByValue((int64)Preset)));
			}
		}

		return AllowedTypes;
	}
};

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

	/** Enable debug socket visualization */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bDrawSocketDebug = false;

	/** Enable debug bounding box visualization */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	bool bDrawBoundsDebug = false;

	/** Rotation speed in degrees per second when rotating with A/D keys */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties", meta = (ClampMin = "1.0", ClampMax = "360.0"))
	float RotationSpeed = 90.0f;

	/** Socket type definitions - maps socket names to their types for snap compatibility. Auto-populated from mesh sockets. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Part Properties")
	TArray<FSocketTypeDefinition> SocketTypeDefinitions;

	/** Auto-populate socket definitions from the mesh (called automatically) */
	void AutoPopulateSocketDefinitions();

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
