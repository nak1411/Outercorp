// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConstructionPart.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructionPartData.h"

AConstructionPart::AConstructionPart()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Overlap bounds are added in Blueprint child classes
	// Add BoxComponents as children of MeshComponent with collision disabled
	// Multiple bounds can be used for complex shapes (e.g., rack with two posts)

	// Create snap root
	SnapRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SnapRoot"));
	SnapRoot->SetupAttachment(MeshComponent);

	// Snap points are now added dynamically in Blueprint child classes
	// This allows each construction part type to have different numbers of snap points

	// Default state
	CurrentState = EConstructionPartState::InInventory;
}

void AConstructionPart::BeginPlay()
{
	Super::BeginPlay();

	// Initialize overlap bounds array from Blueprint-added components
	InitializeOverlapBounds();

	// Initialize snap points array
	InitializeSnapPoints();

	// Initialize from data asset if available
	if (PartData)
	{
		InitializeFromData(PartData);
	}

	// Store original materials for ghost preview
	StoreOriginalMaterials();

	// Update arrow visibility based on debug settings
	bool bShowArrows = (PartData && PartData->bDrawSocketDebug);

	for (UArrowComponent* Arrow : SnapPoints)
	{
		if (Arrow)
		{
			Arrow->SetVisibility(bShowArrows);
			Arrow->SetHiddenInGame(!bShowArrows);
		}
	}
}

void AConstructionPart::InitializeSnapPoints()
{
	// If SnapPoints array is empty, gather all ArrowComponents that are children of SnapRoot
	if (SnapPoints.Num() == 0 && SnapRoot)
	{
		TArray<USceneComponent*> ChildComponents;
		SnapRoot->GetChildrenComponents(false, ChildComponents);

		for (USceneComponent* Child : ChildComponents)
		{
			UArrowComponent* Arrow = Cast<UArrowComponent>(Child);
			if (Arrow)
			{
				SnapPoints.Add(Arrow);
			}
		}

	}

	// Snap points are positioned manually in each Blueprint child class
	// This gives full control over snap point placement for different part geometries
}

void AConstructionPart::InitializeOverlapBounds()
{
	// Gather all BoxComponents that are direct children of MeshComponent
	// This allows Blueprint to add multiple overlap bounds with custom names
	OverlapBounds.Empty();

	if (MeshComponent)
	{
		TArray<USceneComponent*> ChildComponents;
		MeshComponent->GetChildrenComponents(false, ChildComponents);

		// Find all Box Components added in Blueprint
		for (USceneComponent* Child : ChildComponents)
		{
			UBoxComponent* BoxComp = Cast<UBoxComponent>(Child);
			if (BoxComp)
			{
				// Check if this looks like an overlap bounds component
				// (has collision disabled and is hidden in game)
				if (BoxComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
				{
					OverlapBounds.Add(BoxComp);
				}
			}
		}
	}

	// Debug log
	if (OverlapBounds.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Initialized %d overlap bounds"), *GetName(), OverlapBounds.Num());
	}
}

void AConstructionPart::InitializeFromData(UConstructionPartData* Data)
{
	if (!Data)
	{
		return;
	}

	PartData = Data;

	// Visual properties (mesh, scale, materials, arrow positions) are now controlled in Blueprint
	// DataAsset only stores gameplay properties

	// Copy gameplay properties
	PartName = Data->PartName.ToString();
	PartType = Data->PartType;
	Mass = Data->Mass;
	bRequiresTool = Data->bRequiresTool;

	// Re-initialize snap points to gather any snap points added in Blueprint
	InitializeSnapPoints();

	// Store materials for ghost preview (materials are set in Blueprint)
	StoreOriginalMaterials();
}

void AConstructionPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Deprecated - kept for backward compatibility
EAttachmentType AConstructionPart::DetermineAttachmentType(const FName& SocketName)
{
	return EAttachmentType::Mount;
}

void AConstructionPart::SetPartState(EConstructionPartState NewState)
{
	CurrentState = NewState;

	// Toggle arrow visibility based on debug settings
	bool bShowArrows = (PartData && PartData->bDrawSocketDebug);

	for (UArrowComponent* Arrow : SnapPoints)
	{
		if (Arrow)
		{
			Arrow->SetVisibility(bShowArrows);
			Arrow->SetHiddenInGame(!bShowArrows);
		}
	}

	switch (CurrentState)
	{
	case EConstructionPartState::InInventory:
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		break;

	case EConstructionPartState::GhostPreview:
		SetActorHiddenInGame(false);
		SetActorEnableCollision(false);
		break;

	case EConstructionPartState::Placed:
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		RestoreOriginalMaterials();
		if (MeshComponent)
		{
			// Enable physics if the part has bHasPhysics enabled in data asset
			bool bEnablePhysics = (PartData && PartData->bHasPhysics);
			MeshComponent->SetSimulatePhysics(bEnablePhysics);
		}
		break;

	case EConstructionPartState::Fastened:
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		RestoreOriginalMaterials();
		if (MeshComponent)
		{
			// Fastened parts should always have physics disabled for stability
			MeshComponent->SetSimulatePhysics(false);
		}
		break;
	}
}

void AConstructionPart::SetGhostPreview(bool bIsValid)
{
	if (!MeshComponent || !PartData)
	{
		return;
	}

	UMaterialInterface* GhostMaterial = bIsValid ? PartData->ValidPlacementMaterial : PartData->InvalidPlacementMaterial;

	if (GhostMaterial)
	{
		// Apply ghost material to all material slots
		for (int32 i = 0; i < MeshComponent->GetNumMaterials(); ++i)
		{
			MeshComponent->SetMaterial(i, GhostMaterial);
		}
	}
}

void AConstructionPart::StoreOriginalMaterials()
{
	OriginalMaterials.Empty();

	if (MeshComponent)
	{
		for (int32 i = 0; i < MeshComponent->GetNumMaterials(); ++i)
		{
			OriginalMaterials.Add(MeshComponent->GetMaterial(i));
		}
	}
}

void AConstructionPart::RestoreOriginalMaterials()
{
	if (!MeshComponent || OriginalMaterials.Num() == 0)
	{
		return;
	}

	for (int32 i = 0; i < OriginalMaterials.Num() && i < MeshComponent->GetNumMaterials(); ++i)
	{
		if (OriginalMaterials[i])
		{
			MeshComponent->SetMaterial(i, OriginalMaterials[i]);
		}
	}
}

bool AConstructionPart::FindNearbyAttachmentPoint(const FVector& SearchLocation, AActor*& OutNearbyPart, FName& OutSocketName, FTransform& OutSocketTransform)
{
	// Find all construction parts in the world
	TArray<AActor*> FoundParts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AConstructionPart::StaticClass(), FoundParts);

	float ClosestDistance = PartData ? PartData->SnapDistance : 50.0f;
	bool bFoundValidSocket = false;

	for (AActor* Actor : FoundParts)
	{
		AConstructionPart* OtherPart = Cast<AConstructionPart>(Actor);
		if (!OtherPart || OtherPart == this || OtherPart->CurrentState == EConstructionPartState::InInventory)
		{
			continue;
		}

		// Check each socket on the other part
		for (const FAttachmentPoint& AttachPoint : OtherPart->AttachmentPoints)
		{
			// Allow snapping to already occupied sockets (multiple parts can share a connection point)
			FTransform SocketTransform = OtherPart->GetSocketTransformByName(AttachPoint.SocketName);
			float Distance = FVector::Dist(SearchLocation, SocketTransform.GetLocation());

			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				OutNearbyPart = OtherPart;
				OutSocketName = AttachPoint.SocketName;
				OutSocketTransform = SocketTransform;
				bFoundValidSocket = true;
			}
		}
	}

	return bFoundValidSocket;
}

bool AConstructionPart::CanAttachToSocket(AConstructionPart* OtherPart, FName OtherSocket, FName MySocket)
{
	if (!OtherPart)
	{
		return false;
	}

	// Find the attachment points
	FAttachmentPoint* MyAttachPoint = AttachmentPoints.FindByPredicate([MySocket](const FAttachmentPoint& Point) {
		return Point.SocketName == MySocket;
	});

	FAttachmentPoint* OtherAttachPoint = OtherPart->AttachmentPoints.FindByPredicate([OtherSocket](const FAttachmentPoint& Point) {
		return Point.SocketName == OtherSocket;
	});

	if (!MyAttachPoint || !OtherAttachPoint)
	{
		return false;
	}

	// Allow multiple parts to connect at the same socket location
	// This enables building complex structures where multiple beams/parts meet at one point
	// Additional validation can be added here (e.g., matching types, max connections per socket)
	return true;
}

void AConstructionPart::AttachToPart(AConstructionPart* OtherPart, FName OtherSocket, FName MySocket)
{
	if (!CanAttachToSocket(OtherPart, OtherSocket, MySocket))
	{
		return;
	}

	// Find attachment points
	FAttachmentPoint* MyAttachPoint = AttachmentPoints.FindByPredicate([MySocket](const FAttachmentPoint& Point) {
		return Point.SocketName == MySocket;
	});

	FAttachmentPoint* OtherAttachPoint = OtherPart->AttachmentPoints.FindByPredicate([OtherSocket](const FAttachmentPoint& Point) {
		return Point.SocketName == OtherSocket;
	});

	if (MyAttachPoint && OtherAttachPoint)
	{
		// Add to connections array (allows multiple parts at same socket)
		MyAttachPoint->Connections.Add(FSocketConnection(OtherPart, OtherSocket));
		OtherAttachPoint->Connections.Add(FSocketConnection(this, MySocket));

		// Update deprecated fields for backward compatibility (use first connection)
		if (MyAttachPoint->Connections.Num() == 1)
		{
			MyAttachPoint->bIsOccupied = true;
			MyAttachPoint->ConnectedPart = OtherPart;
			MyAttachPoint->ConnectedSocket = OtherSocket;
		}

		if (OtherAttachPoint->Connections.Num() == 1)
		{
			OtherAttachPoint->bIsOccupied = true;
			OtherAttachPoint->ConnectedPart = this;
			OtherAttachPoint->ConnectedSocket = MySocket;
		}

		// DON'T move the part - the placement system already positioned it correctly
		// The part is already positioned so MySocket aligns with OtherSocket
		// Just record the connection and change state

		// Change state to Placed (not yet fastened)
		SetPartState(EConstructionPartState::Placed);
	}
}

void AConstructionPart::DetachFromPart(FName MySocket)
{
	FAttachmentPoint* MyAttachPoint = AttachmentPoints.FindByPredicate([MySocket](const FAttachmentPoint& Point) {
		return Point.SocketName == MySocket;
	});

	if (!MyAttachPoint)
	{
		return;
	}

	// Store all connections to iterate through them
	TArray<FSocketConnection> ConnectionsCopy = MyAttachPoint->Connections;

	// Clear this socket's connections array
	MyAttachPoint->Connections.Empty();

	// Clear deprecated fields
	MyAttachPoint->bIsOccupied = false;
	MyAttachPoint->ConnectedPart = nullptr;
	MyAttachPoint->ConnectedSocket = NAME_None;

	// Clear the connection on the other side for each connected part
	for (const FSocketConnection& Connection : ConnectionsCopy)
	{
		AConstructionPart* ConnectedPart = Cast<AConstructionPart>(Connection.ConnectedPart);
		if (ConnectedPart)
		{
			FAttachmentPoint* OtherAttachPoint = ConnectedPart->AttachmentPoints.FindByPredicate([Connection](const FAttachmentPoint& Point) {
				return Point.SocketName == Connection.ConnectedSocket;
			});

			if (OtherAttachPoint)
			{
				// Remove this part from the other socket's connections
				OtherAttachPoint->Connections.RemoveAll([this, MySocket](const FSocketConnection& Conn) {
					return Conn.ConnectedPart == this && Conn.ConnectedSocket == MySocket;
				});

				// Update deprecated fields if no more connections
				if (OtherAttachPoint->Connections.Num() == 0)
				{
					OtherAttachPoint->bIsOccupied = false;
					OtherAttachPoint->ConnectedPart = nullptr;
					OtherAttachPoint->ConnectedSocket = NAME_None;
				}
			}
		}
	}
}

TArray<FName> AConstructionPart::GetAvailableSockets() const
{
	TArray<FName> AvailableSockets;

	for (const FAttachmentPoint& AttachPoint : AttachmentPoints)
	{
		if (!AttachPoint.bIsOccupied)
		{
			AvailableSockets.Add(AttachPoint.SocketName);
		}
	}

	return AvailableSockets;
}

FTransform AConstructionPart::GetSocketTransformByName(FName SocketName) const
{
	if (MeshComponent)
	{
		return MeshComponent->GetSocketTransform(SocketName);
	}

	return FTransform::Identity;
}

bool AConstructionPart::IsFullyFastened() const
{
	if (AttachmentPoints.Num() == 0)
	{
		return false;
	}

	for (const FAttachmentPoint& AttachPoint : AttachmentPoints)
	{
		if (AttachPoint.AttachmentType == EAttachmentType::Mount && !AttachPoint.bIsOccupied)
		{
			return false;
		}
	}

	return CurrentState == EConstructionPartState::Fastened;
}

int32 AConstructionPart::GetFastenedConnectionCount() const
{
	int32 Count = 0;

	for (const FAttachmentPoint& AttachPoint : AttachmentPoints)
	{
		if (AttachPoint.bIsOccupied)
		{
			Count++;
		}
	}

	return Count;
}

FName AConstructionPart::GetSnapPointTag(UArrowComponent* SnapPoint) const
{
	if (!SnapPoint)
	{
		return NAME_None;
	}

	FName SnapPointName = SnapPoint->GetFName();

	// First check if there's a custom tag override in the map
	const FName* CustomTag = SnapPointTags.Find(SnapPointName);
	if (CustomTag && *CustomTag != NAME_None)
	{
		return *CustomTag;
	}

	// If no custom tag, use the arrow component's name as the tag
	// This allows simple naming like "ShelfMount_01", "PowerPort_A", etc.
	return SnapPointName;
}

bool AConstructionPart::AreSnapPointsCompatible(UArrowComponent* MySnapPoint, AConstructionPart* OtherPart, UArrowComponent* OtherSnapPoint) const
{
	if (!MySnapPoint || !OtherPart || !OtherSnapPoint)
	{
		return false;
	}

	// Get tags for both snap points
	FName MyTag = GetSnapPointTag(MySnapPoint);
	FName OtherTag = OtherPart->GetSnapPointTag(OtherSnapPoint);

	// Get filters for both snap points
	FName MySnapPointName = MySnapPoint->GetFName();
	FName OtherSnapPointName = OtherSnapPoint->GetFName();

	const FSnapPointFilter* MyFilter = SnapPointFilters.Find(MySnapPointName);
	const FSnapPointFilter* OtherFilter = OtherPart->SnapPointFilters.Find(OtherSnapPointName);

	// If no filter is defined on the ghost (this object being placed), reject the connection
	// This requires explicit setup - you must define filters to enable snapping
	if (!MyFilter)
	{
		return false;
	}

	// One-way filtering: Only check if the ghost's filter allows the target's tag
	// This enables directional snapping (e.g., shelf can snap to rack, but rack can't snap to shelf)
	// The target's filter is ignored - only the active/ghost object's filter determines compatibility
	bool bMyFilterAllows = MyFilter->AllowsTag(OtherTag);

	return bMyFilterAllows;
}
