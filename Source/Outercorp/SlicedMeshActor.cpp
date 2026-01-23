// SlicedMeshActor.cpp

#include "SlicedMeshActor.h"
#include "ProceduralMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"

ASlicedMeshActor::ASlicedMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	// Create procedural mesh for falling piece (with physics)
	FallingPieceMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("FallingPieceMesh"));
	FallingPieceMesh->SetupAttachment(RootSceneComponent);
	FallingPieceMesh->bUseAsyncCooking = true;
	FallingPieceMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FallingPieceMesh->SetCollisionResponseToAllChannels(ECR_Block);
	FallingPieceMesh->SetSimulatePhysics(false);
	// Use simple collision for physics simulation
	FallingPieceMesh->SetCollisionObjectType(ECC_WorldDynamic);

	// Create procedural mesh for stump (no physics)
	StumpMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("StumpMesh"));
	StumpMesh->SetupAttachment(RootSceneComponent);
	StumpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create procedural mesh for cap
	CapMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CapMesh"));
	CapMesh->SetupAttachment(RootSceneComponent);
	CapMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Default settings
	DespawnTime = 30.0f;
	SlicePlaneNormal = FVector::UpVector;
	FallingVelocity = FVector(0, 0, -500);
	bCreateCapMesh = true;
}

#if WITH_EDITOR
void ASlicedMeshActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Editor preview disabled - slicing is complex and can crash editor
	// Slicing will happen at runtime instead
}
#endif

void ASlicedMeshActor::BeginPlay()
{
	Super::BeginPlay();

	// If mesh is set but not initialized, initialize it now
	if (!bSliceSuccessful && MeshToSlice)
	{
		InitializeSlicedMesh(
			MeshToSlice,
			SlicePlaneLocation,
			SlicePlaneNormal,
			FallingVelocity,
			SlicedMeshMaterial
		);

		// Create the meshes from the sliced data
		CreateSlicedMeshes();
	}

	// Setup physics and timers
	if (bSliceSuccessful)
	{
		SetupFallingPhysics();
	}

	SetupDespawnTimer();
}

void ASlicedMeshActor::InitializeSlicedMesh(
	UStaticMesh* StaticMesh,
	const FVector& InSlicePlaneLocation,
	const FVector& InSlicePlaneNormal,
	const FVector& InFallingVelocity,
	UMaterialInterface* InMaterial)
{
	MeshToSlice = StaticMesh;
	SlicePlaneLocation = InSlicePlaneLocation;
	SlicePlaneNormal = InSlicePlaneNormal.GetSafeNormal();
	FallingVelocity = InFallingVelocity;
	SlicedMeshMaterial = InMaterial;

	// Perform the slicing
	bSliceSuccessful = UProceduralMeshSlicer::SliceStaticMesh(
		MeshToSlice,
		SlicePlaneLocation,
		SlicePlaneNormal,
		TopPieceData,
		BottomPieceData
	);

	// Cap mesh creation disabled
}

void ASlicedMeshActor::CreateSlicedMeshes()
{
	if (!bSliceSuccessful)
	{
		return;
	}

	// Safety check - ensure we have valid data
	if (TopPieceData.Triangles.Num() < 3 || BottomPieceData.Triangles.Num() < 3)
	{
		return;
	}

	// Create the falling piece mesh (the top part that falls)
	if (FallingPieceMesh && TopPieceData.Vertices.Num() > 0 && TopPieceData.Triangles.Num() > 0)
	{
		if (TopPieceData.Normals.Num() < TopPieceData.Vertices.Num())
		{
			TopPieceData.Normals.Init(FVector::UpVector, TopPieceData.Vertices.Num());
		}
		if (TopPieceData.TexCoords.Num() < TopPieceData.Vertices.Num())
		{
			TopPieceData.TexCoords.Init(FVector2D::ZeroVector, TopPieceData.Vertices.Num());
		}

		FallingPieceMesh->CreateMeshSection_LinearColor(
			0,
			TopPieceData.Vertices,
			TopPieceData.Triangles,
			TopPieceData.Normals,
			TopPieceData.TexCoords,
			TArray<FLinearColor>(),
			TopPieceData.Tangents,
			false  // Use simple collision, not complex
		);

		if (SlicedMeshMaterial)
		{
			FallingPieceMesh->SetMaterial(0, SlicedMeshMaterial);
		}
	}

	// Create the stump mesh (the bottom part that stays)
	if (StumpMesh && BottomPieceData.Vertices.Num() > 0 && BottomPieceData.Triangles.Num() > 0)
	{
		if (BottomPieceData.Normals.Num() < BottomPieceData.Vertices.Num())
		{
			BottomPieceData.Normals.Init(FVector::UpVector, BottomPieceData.Vertices.Num());
		}
		if (BottomPieceData.TexCoords.Num() < BottomPieceData.Vertices.Num())
		{
			BottomPieceData.TexCoords.Init(FVector2D::ZeroVector, BottomPieceData.Vertices.Num());
		}

		StumpMesh->CreateMeshSection_LinearColor(
			0,
			BottomPieceData.Vertices,
			BottomPieceData.Triangles,
			BottomPieceData.Normals,
			BottomPieceData.TexCoords,
			TArray<FLinearColor>(),
			BottomPieceData.Tangents,
			false
		);

		if (SlicedMeshMaterial)
		{
			StumpMesh->SetMaterial(0, SlicedMeshMaterial);
		}
	}
}

void ASlicedMeshActor::SetupFallingPhysics()
{
	if (!FallingPieceMesh)
	{
		return;
	}

	// Enable physics simulation on the falling piece
	FallingPieceMesh->SetSimulatePhysics(true);
	FallingPieceMesh->SetPhysicsLinearVelocity(FallingVelocity);

	// Randomize rotation slightly for more natural effect
	if (TopPieceData.Vertices.Num() > 0)
	{
		FVector RandomRotation = FVector(
			FMath::RandRange(-180, 180),
			FMath::RandRange(-180, 180),
			FMath::RandRange(-180, 180)
		);
		FallingPieceMesh->SetPhysicsAngularVelocityInDegrees(RandomRotation * 0.5f);
	}
}

void ASlicedMeshActor::SetupDespawnTimer()
{
	if (DespawnTime <= 0.0f)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().SetTimer(
		DespawnTimerHandle,
		FTimerDelegate::CreateLambda([this]()
		{
			if (this && !IsActorBeingDestroyed())
			{
				Destroy();
			}
		}),
		DespawnTime,
		false
	);
}
