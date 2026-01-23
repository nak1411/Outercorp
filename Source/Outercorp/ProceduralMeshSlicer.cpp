// ProceduralMeshSlicer.cpp

#include "ProceduralMeshSlicer.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshResources.h"

bool UProceduralMeshSlicer::SliceStaticMesh(
	UStaticMesh* StaticMesh,
	const FVector& SlicePlaneLocation,
	const FVector& SlicePlaneNormal,
	FSlicedMeshData& OutTopPiece,
	FSlicedMeshData& OutBottomPiece)
{
	if (!StaticMesh)
	{
		return false;
	}

	OutTopPiece.Clear();
	OutBottomPiece.Clear();

	// Extract base geometry
	FSlicedMeshData BaseMeshData;
	ExtractMeshGeometry(StaticMesh, 0, BaseMeshData);

	if (BaseMeshData.Vertices.Num() < 3 || BaseMeshData.Triangles.Num() < 3)
	{
		return false;
	}

	// Safety check on triangle indices
	int32 MaxVertexIndex = BaseMeshData.Vertices.Num() - 1;
	for (int32 TriIdx : BaseMeshData.Triangles)
	{
		if (TriIdx < 0 || TriIdx > MaxVertexIndex)
		{
			return false; // Invalid triangle index
		}
	}

	// Find the actual Z range of the mesh to better calibrate epsilon
	float MinZ = FLT_MAX, MaxZ = -FLT_MAX;
	for (const FVector& Vertex : BaseMeshData.Vertices)
	{
		MinZ = FMath::Min(MinZ, Vertex.Z);
		MaxZ = FMath::Max(MaxZ, Vertex.Z);
	}
	float MeshZRange = MaxZ - MinZ;
	float AdaptiveEpsilon = FMath::Max(0.1f, MeshZRange * 0.001f); // 0.1% of mesh height

	// Process each triangle - simple approach: only keep whole triangles on each side
	for (int32 i = 0; i < BaseMeshData.Triangles.Num(); i += 3)
	{
		int32 V0 = BaseMeshData.Triangles[i];
		int32 V1 = BaseMeshData.Triangles[i + 1];
		int32 V2 = BaseMeshData.Triangles[i + 2];

		float Distance0 = GetSignedDistanceToPlane(BaseMeshData.Vertices[V0], SlicePlaneLocation, SlicePlaneNormal);
		float Distance1 = GetSignedDistanceToPlane(BaseMeshData.Vertices[V1], SlicePlaneLocation, SlicePlaneNormal);
		float Distance2 = GetSignedDistanceToPlane(BaseMeshData.Vertices[V2], SlicePlaneLocation, SlicePlaneNormal);

		// Skip very small distances (treat as on the plane) using adaptive epsilon
		if (FMath::Abs(Distance0) < AdaptiveEpsilon) Distance0 = 0.0f;
		if (FMath::Abs(Distance1) < AdaptiveEpsilon) Distance1 = 0.0f;
		if (FMath::Abs(Distance2) < AdaptiveEpsilon) Distance2 = 0.0f;

		// All vertices on positive side - add to top piece
		if (Distance0 >= 0.0f && Distance1 >= 0.0f && Distance2 >= 0.0f)
		{
			int32 BaseIdx = OutTopPiece.Vertices.Num();
			OutTopPiece.Vertices.Add(BaseMeshData.Vertices[V0]);
			OutTopPiece.Vertices.Add(BaseMeshData.Vertices[V1]);
			OutTopPiece.Vertices.Add(BaseMeshData.Vertices[V2]);

			OutTopPiece.Triangles.Add(BaseIdx);
			OutTopPiece.Triangles.Add(BaseIdx + 1);
			OutTopPiece.Triangles.Add(BaseIdx + 2);

			OutTopPiece.Normals.Add(BaseMeshData.Normals.IsValidIndex(V0) ? BaseMeshData.Normals[V0] : FVector::UpVector);
			OutTopPiece.Normals.Add(BaseMeshData.Normals.IsValidIndex(V1) ? BaseMeshData.Normals[V1] : FVector::UpVector);
			OutTopPiece.Normals.Add(BaseMeshData.Normals.IsValidIndex(V2) ? BaseMeshData.Normals[V2] : FVector::UpVector);

			OutTopPiece.TexCoords.Add(BaseMeshData.TexCoords.IsValidIndex(V0) ? BaseMeshData.TexCoords[V0] : FVector2D::ZeroVector);
			OutTopPiece.TexCoords.Add(BaseMeshData.TexCoords.IsValidIndex(V1) ? BaseMeshData.TexCoords[V1] : FVector2D::ZeroVector);
			OutTopPiece.TexCoords.Add(BaseMeshData.TexCoords.IsValidIndex(V2) ? BaseMeshData.TexCoords[V2] : FVector2D::ZeroVector);
		}
		// All vertices on negative side - add to bottom piece
		else if (Distance0 <= 0.0f && Distance1 <= 0.0f && Distance2 <= 0.0f)
		{
			int32 BaseIdx = OutBottomPiece.Vertices.Num();
			OutBottomPiece.Vertices.Add(BaseMeshData.Vertices[V0]);
			OutBottomPiece.Vertices.Add(BaseMeshData.Vertices[V1]);
			OutBottomPiece.Vertices.Add(BaseMeshData.Vertices[V2]);

			OutBottomPiece.Triangles.Add(BaseIdx);
			OutBottomPiece.Triangles.Add(BaseIdx + 1);
			OutBottomPiece.Triangles.Add(BaseIdx + 2);

			OutBottomPiece.Normals.Add(BaseMeshData.Normals.IsValidIndex(V0) ? BaseMeshData.Normals[V0] : FVector::UpVector);
			OutBottomPiece.Normals.Add(BaseMeshData.Normals.IsValidIndex(V1) ? BaseMeshData.Normals[V1] : FVector::UpVector);
			OutBottomPiece.Normals.Add(BaseMeshData.Normals.IsValidIndex(V2) ? BaseMeshData.Normals[V2] : FVector::UpVector);

			OutBottomPiece.TexCoords.Add(BaseMeshData.TexCoords.IsValidIndex(V0) ? BaseMeshData.TexCoords[V0] : FVector2D::ZeroVector);
			OutBottomPiece.TexCoords.Add(BaseMeshData.TexCoords.IsValidIndex(V1) ? BaseMeshData.TexCoords[V1] : FVector2D::ZeroVector);
			OutBottomPiece.TexCoords.Add(BaseMeshData.TexCoords.IsValidIndex(V2) ? BaseMeshData.TexCoords[V2] : FVector2D::ZeroVector);
		}
		// Triangle crosses the plane - discard it (no splitting)
		// This creates a gap at the cut but avoids mesh corruption
	}

	return OutTopPiece.Vertices.Num() > 0 && OutBottomPiece.Vertices.Num() > 0;
}

float UProceduralMeshSlicer::GetSignedDistanceToPlane(const FVector& Point, const FVector& PlaneLocation, const FVector& PlaneNormal)
{
	return FVector::DotProduct(Point - PlaneLocation, PlaneNormal);
}

void UProceduralMeshSlicer::ExtractMeshGeometry(
	UStaticMesh* StaticMesh,
	int32 LODIndex,
	FSlicedMeshData& OutData)
{
	OutData.Clear();

	if (!StaticMesh || !StaticMesh->GetRenderData())
	{
		return;
	}

	const FStaticMeshRenderData* RenderData = StaticMesh->GetRenderData();
	if (!RenderData->LODResources.IsValidIndex(LODIndex))
	{
		return;
	}

	const FStaticMeshLODResources& LODResource = RenderData->LODResources[LODIndex];

	// Extract vertices using the vertex buffer
	int32 NumVertices = LODResource.GetNumVertices();
	OutData.Vertices.Reserve(NumVertices);
	OutData.Normals.Reserve(NumVertices);
	OutData.TexCoords.Reserve(NumVertices);

	for (int32 i = 0; i < NumVertices; ++i)
	{
		// Get position
		FVector Position = FVector(LODResource.VertexBuffers.PositionVertexBuffer.VertexPosition(i));
		OutData.Vertices.Add(Position);

		// Get normal
		FVector4f TangentZ = LODResource.VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(i);
		OutData.Normals.Add(FVector(TangentZ.X, TangentZ.Y, TangentZ.Z));

		// Get UV
		if (LODResource.VertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords() > 0)
		{
			FVector2f UV = LODResource.VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(i, 0);
			OutData.TexCoords.Add(FVector2D(UV.X, UV.Y));
		}
		else
		{
			OutData.TexCoords.Add(FVector2D::ZeroVector);
		}
	}

	// Extract indices
	uint32 NumIndices = LODResource.IndexBuffer.GetNumIndices();
	OutData.Triangles.Reserve(NumIndices);
	for (uint32 i = 0; i < NumIndices; ++i)
	{
		OutData.Triangles.Add(LODResource.IndexBuffer.GetIndex(i));
	}
}

