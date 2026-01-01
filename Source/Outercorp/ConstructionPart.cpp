// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConstructionPart.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructionPartData.h"

AConstructionPart::AConstructionPart()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Default state
	CurrentState = EConstructionPartState::InInventory;
}

void AConstructionPart::BeginPlay()
{
	Super::BeginPlay();

	// Initialize from data asset if available
	if (PartData)
	{
		InitializeFromData(PartData);
	}

	// Store original materials for ghost preview
	StoreOriginalMaterials();
}

void AConstructionPart::InitializeFromData(UConstructionPartData* Data)
{
	if (!Data)
	{
		return;
	}

	PartData = Data;

	// Apply mesh
	if (Data->PartMesh && MeshComponent)
	{
		MeshComponent->SetStaticMesh(Data->PartMesh);
		MeshComponent->SetWorldScale3D(Data->MeshScale);
	}

	// Apply material overrides
	if (Data->MaterialOverrides.Num() > 0 && MeshComponent)
	{
		for (int32 i = 0; i < Data->MaterialOverrides.Num(); ++i)
		{
			if (Data->MaterialOverrides[i])
			{
				MeshComponent->SetMaterial(i, Data->MaterialOverrides[i]);
			}
		}
	}

	// Copy properties
	PartName = Data->PartName.ToString();
	PartType = Data->PartType;
	Mass = Data->Mass;
	bRequiresTool = Data->bRequiresTool;

	// Auto-populate socket definitions if needed
	if (Data->PartMesh && Data->SocketTypeDefinitions.Num() == 0)
	{
		Data->AutoPopulateSocketDefinitions();
	}

	// Initialize attachment points from mesh sockets FIRST
	InitializeAttachmentPoints();

	// Apply socket type definitions to attachment points
	for (const FSocketTypeDefinition& SocketTypeDef : Data->SocketTypeDefinitions)
	{
		// Find the attachment point with this socket name
		FAttachmentPoint* AttachPoint = AttachmentPoints.FindByPredicate([&SocketTypeDef](const FAttachmentPoint& Point) {
			return Point.SocketName == SocketTypeDef.SocketName;
		});

		if (AttachPoint)
		{
			// Use GetSocketType() to resolve the preset enum to FName
			AttachPoint->SocketType = SocketTypeDef.GetSocketType();
		}
	}
}

void AConstructionPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AConstructionPart::InitializeAttachmentPoints()
{
	AttachmentPoints.Empty();

	if (!MeshComponent || !MeshComponent->GetStaticMesh())
	{
		return;
	}

	UStaticMesh* StaticMesh = MeshComponent->GetStaticMesh();
	const TArray<FName>& SocketNames = StaticMesh->Sockets.IsEmpty()
		? TArray<FName>()
		: [&StaticMesh]() {
			TArray<FName> Names;
			for (UStaticMeshSocket* Socket : StaticMesh->Sockets)
			{
				if (Socket)
				{
					Names.Add(Socket->SocketName);
				}
			}
			return Names;
		}();

	for (const FName& SocketName : SocketNames)
	{
		FAttachmentPoint AttachPoint;
		AttachPoint.SocketName = SocketName;
		AttachPoint.AttachmentType = DetermineAttachmentType(SocketName);
		AttachPoint.bIsOccupied = false;
		AttachPoint.ConnectedPart = nullptr;

		AttachmentPoints.Add(AttachPoint);
	}
}

EAttachmentType AConstructionPart::DetermineAttachmentType(const FName& SocketName)
{
	FString SocketNameStr = SocketName.ToString();

	if (SocketNameStr.Contains(TEXT("Mount"), ESearchCase::IgnoreCase))
	{
		return EAttachmentType::Mount;
	}
	else if (SocketNameStr.Contains(TEXT("Bolt"), ESearchCase::IgnoreCase))
	{
		return EAttachmentType::Bolt;
	}
	else if (SocketNameStr.Contains(TEXT("Utility"), ESearchCase::IgnoreCase))
	{
		return EAttachmentType::Utility;
	}

	// Default to Mount if no pattern matches
	return EAttachmentType::Mount;
}

void AConstructionPart::SetPartState(EConstructionPartState NewState)
{
	EConstructionPartState OldState = CurrentState;
	CurrentState = NewState;

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
			MeshComponent->SetSimulatePhysics(false);
		}
		break;

	case EConstructionPartState::Fastened:
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		RestoreOriginalMaterials();
		if (MeshComponent)
		{
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

	if (!MyAttachPoint || !MyAttachPoint->bIsOccupied)
	{
		return;
	}

	// Get connected part info before clearing
	AConstructionPart* ConnectedPart = Cast<AConstructionPart>(MyAttachPoint->ConnectedPart);
	FName ConnectedSocket = MyAttachPoint->ConnectedSocket;

	// Clear this side
	MyAttachPoint->bIsOccupied = false;
	MyAttachPoint->ConnectedPart = nullptr;
	MyAttachPoint->ConnectedSocket = NAME_None;

	// Clear other side
	if (ConnectedPart)
	{
		FAttachmentPoint* OtherAttachPoint = ConnectedPart->AttachmentPoints.FindByPredicate([ConnectedSocket](const FAttachmentPoint& Point) {
			return Point.SocketName == ConnectedSocket;
		});

		if (OtherAttachPoint)
		{
			OtherAttachPoint->bIsOccupied = false;
			OtherAttachPoint->ConnectedPart = nullptr;
			OtherAttachPoint->ConnectedSocket = NAME_None;
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
