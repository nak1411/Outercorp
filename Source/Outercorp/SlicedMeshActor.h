// SlicedMeshActor.h
// Actor for handling sliced procedural mesh destruction effects

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshSlicer.h"
#include "SlicedMeshActor.generated.h"

class UStaticMesh;
class UMaterialInterface;

/**
 * Actor that spawns and manages sliced procedural meshes from harvested resources
 * Used for destruction effects when harvesting resources with procedural mesh slicing
 */
UCLASS()
class OUTERCORP_API ASlicedMeshActor : public AActor
{
	GENERATED_BODY()

public:
	ASlicedMeshActor();

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	virtual void BeginPlay() override;

	// ============================================
	// PROPERTIES
	// ============================================

	/** Time in seconds before this actor despawns (0 = never despawn) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Mesh", meta = (ClampMin = "0.0"))
	float DespawnTime = 30.0f;

	/** The static mesh to slice */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Mesh")
	UStaticMesh* MeshToSlice;

	/** Material to apply to procedural mesh pieces */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Mesh")
	UMaterialInterface* SlicedMeshMaterial;

	/** Location of the slice plane (world space) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Mesh")
	FVector SlicePlaneLocation = FVector::ZeroVector;

	/** Normal direction of the slice plane (pointing toward the top piece) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Mesh")
	FVector SlicePlaneNormal = FVector::UpVector;

	/** Velocity to apply to the fallen piece */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Mesh")
	FVector FallingVelocity = FVector(0, 0, -500);

	/** Whether to spawn a cap mesh at the slice point (disabled for stability) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliced Mesh")
	bool bCreateCapMesh = false;

	// ============================================
	// COMPONENTS
	// ============================================

	/** Root component for the stump (remains in place) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* RootSceneComponent;

	/** Procedural mesh for the top/falling piece */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProceduralMeshComponent* FallingPieceMesh;

	/** Procedural mesh for the bottom/stump piece */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProceduralMeshComponent* StumpMesh;

	/** Procedural mesh for the cut cap surface */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProceduralMeshComponent* CapMesh;

	// ============================================
	// INITIALIZATION
	// ============================================

	/**
	 * Initialize with a static mesh to slice at a specific location
	 * @param StaticMesh The mesh to slice
	 * @param InSlicePlaneLocation World location of the slice plane
	 * @param InSlicePlaneNormal Normal of the slice plane (pointing toward the piece to keep)
	 * @param InFallingVelocity Velocity for the falling piece
	 * @param InMaterial Material to apply to the sliced meshes
	 */
	UFUNCTION(BlueprintCallable, Category = "Sliced Mesh")
	void InitializeSlicedMesh(
		UStaticMesh* StaticMesh,
		const FVector& InSlicePlaneLocation,
		const FVector& InSlicePlaneNormal,
		const FVector& InFallingVelocity = FVector(0, 0, -500),
		UMaterialInterface* InMaterial = nullptr
	);

	/**
	 * Create the sliced mesh components from the stored data
	 */
	UFUNCTION(BlueprintCallable, Category = "Sliced Mesh")
	void CreateSlicedMeshes();

protected:
	/** Timer handle for despawn */
	FTimerHandle DespawnTimerHandle;

	/** Stored slice data */
	FSlicedMeshData TopPieceData;
	FSlicedMeshData BottomPieceData;
	FSlicedMeshData CapData;

	/** Whether slicing was successful */
	bool bSliceSuccessful = false;

	/**
	 * Setup the despawn timer
	 */
	void SetupDespawnTimer();

	/**
	 * Setup physics for the falling piece
	 */
	void SetupFallingPhysics();
};
