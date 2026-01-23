// ProceduralMeshSlicer.h
// Utility for slicing static meshes into procedural mesh components

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

class UStaticMesh;

/**
 * Structure to hold sliced mesh data
 */
struct FSlicedMeshData
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> TexCoords;
	TArray<FColor> Colors;
	TArray<FProcMeshTangent> Tangents;

	void Clear()
	{
		Vertices.Empty();
		Triangles.Empty();
		Normals.Empty();
		TexCoords.Empty();
		Colors.Empty();
		Tangents.Empty();
	}
};

/**
 * Utility class for slicing static meshes and creating procedural mesh data
 */
class OUTERCORP_API UProceduralMeshSlicer : public UObject
{

public:
	/**
	 * Slice a static mesh at a plane location and get procedural mesh data for both pieces
	 * @param StaticMesh The mesh to slice
	 * @param SlicePlaneLocation The world location of the slice plane
	 * @param SlicePlaneNormal The normal direction of the slice plane (pointing toward the piece to keep)
	 * @param OutTopPiece Data for the piece in the direction of the plane normal
	 * @param OutBottomPiece Data for the piece opposite the plane normal
	 * @return True if slicing was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Mesh Slicing")
	static bool SliceStaticMesh(
		UStaticMesh* StaticMesh,
		const FVector& SlicePlaneLocation,
		const FVector& SlicePlaneNormal,
		FSlicedMeshData& OutTopPiece,
		FSlicedMeshData& OutBottomPiece
	);

private:
	/**
	 * Get the signed distance from a point to the slice plane
	 */
	static float GetSignedDistanceToPlane(const FVector& Point, const FVector& PlaneLocation, const FVector& PlaneNormal);

	/**
	 * Extract mesh geometry from a static mesh LOD
	 */
	static void ExtractMeshGeometry(
		UStaticMesh* StaticMesh,
		int32 LODIndex,
		FSlicedMeshData& OutData
	);
};
