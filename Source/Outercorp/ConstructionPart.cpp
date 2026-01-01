// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConstructionPart.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructionPartData.h"

AConstructionPart::AConstructionPart()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Create snap root
	SnapRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SnapRoot"));
	SnapRoot->SetupAttachment(MeshComponent);

	// Create 6 snap point arrows (following snapping.txt Step 1)
	SnapPoint_Top = CreateDefaultSubobject<UArrowComponent>(TEXT("SnapPoint_Top"));
	SnapPoint_Top->SetupAttachment(SnapRoot);
	SnapPoint_Top->SetArrowColor(FLinearColor::Green);
	SnapPoint_Top->ArrowSize = 2.0f;
	SnapPoint_Top->bIsScreenSizeScaled = true;
	SnapPoint_Top->bTreatAsASprite = true;
	SnapPoint_Top->SetHiddenInGame(false);
	SnapPoint_Top->SetVisibility(true);

	SnapPoint_Bottom = CreateDefaultSubobject<UArrowComponent>(TEXT("SnapPoint_Bottom"));
	SnapPoint_Bottom->SetupAttachment(SnapRoot);
	SnapPoint_Bottom->SetArrowColor(FLinearColor::Green);
	SnapPoint_Bottom->ArrowSize = 2.0f;
	SnapPoint_Bottom->bIsScreenSizeScaled = true;
	SnapPoint_Bottom->bTreatAsASprite = true;
	SnapPoint_Bottom->SetHiddenInGame(false);
	SnapPoint_Bottom->SetVisibility(true);

	SnapPoint_Front = CreateDefaultSubobject<UArrowComponent>(TEXT("SnapPoint_Front"));
	SnapPoint_Front->SetupAttachment(SnapRoot);
	SnapPoint_Front->SetArrowColor(FLinearColor::Red);
	SnapPoint_Front->ArrowSize = 2.0f;
	SnapPoint_Front->bIsScreenSizeScaled = true;
	SnapPoint_Front->bTreatAsASprite = true;
	SnapPoint_Front->SetHiddenInGame(false);
	SnapPoint_Front->SetVisibility(true);

	SnapPoint_Back = CreateDefaultSubobject<UArrowComponent>(TEXT("SnapPoint_Back"));
	SnapPoint_Back->SetupAttachment(SnapRoot);
	SnapPoint_Back->SetArrowColor(FLinearColor::Red);
	SnapPoint_Back->ArrowSize = 2.0f;
	SnapPoint_Back->bIsScreenSizeScaled = true;
	SnapPoint_Back->bTreatAsASprite = true;
	SnapPoint_Back->SetHiddenInGame(false);
	SnapPoint_Back->SetVisibility(true);

	SnapPoint_Right = CreateDefaultSubobject<UArrowComponent>(TEXT("SnapPoint_Right"));
	SnapPoint_Right->SetupAttachment(SnapRoot);
	SnapPoint_Right->SetArrowColor(FLinearColor::Blue);
	SnapPoint_Right->ArrowSize = 2.0f;
	SnapPoint_Right->bIsScreenSizeScaled = true;
	SnapPoint_Right->bTreatAsASprite = true;
	SnapPoint_Right->SetHiddenInGame(false);
	SnapPoint_Right->SetVisibility(true);

	SnapPoint_Left = CreateDefaultSubobject<UArrowComponent>(TEXT("SnapPoint_Left"));
	SnapPoint_Left->SetupAttachment(SnapRoot);
	SnapPoint_Left->SetArrowColor(FLinearColor::Blue);
	SnapPoint_Left->ArrowSize = 2.0f;
	SnapPoint_Left->bIsScreenSizeScaled = true;
	SnapPoint_Left->bTreatAsASprite = true;
	SnapPoint_Left->SetHiddenInGame(false);
	SnapPoint_Left->SetVisibility(true);

	// Default state
	CurrentState = EConstructionPartState::InInventory;
}

void AConstructionPart::BeginPlay()
{
	Super::BeginPlay();

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
	UE_LOG(LogTemp, Warning, TEXT("ConstructionPart BeginPlay: PartData=%s, bDrawSocketDebug=%d, bShowArrows=%d, SnapPoints count=%d"),
		PartData ? *PartData->GetName() : TEXT("NULL"),
		PartData ? PartData->bDrawSocketDebug : false,
		bShowArrows,
		SnapPoints.Num());

	for (UArrowComponent* Arrow : SnapPoints)
	{
		if (Arrow)
		{
			Arrow->SetVisibility(bShowArrows);
			Arrow->SetHiddenInGame(!bShowArrows);
			UE_LOG(LogTemp, Warning, TEXT("  Arrow %s: Visibility=%d, HiddenInGame=%d"),
				*Arrow->GetName(),
				Arrow->IsVisible(),
				Arrow->bHiddenInGame);
		}
	}
}

void AConstructionPart::InitializeSnapPoints()
{
	// Fill the SnapPoints array for easy iteration (snapping.txt Step 1)
	SnapPoints.Empty();
	SnapPoints.Add(SnapPoint_Top);
	SnapPoints.Add(SnapPoint_Bottom);
	SnapPoints.Add(SnapPoint_Front);
	SnapPoints.Add(SnapPoint_Back);
	SnapPoints.Add(SnapPoint_Right);
	SnapPoints.Add(SnapPoint_Left);

	UE_LOG(LogTemp, Warning, TEXT("InitializeSnapPoints: bAutoPositionSnapPoints = %d"), bAutoPositionSnapPoints);

	// Only auto-position if enabled (allows manual positioning in Blueprint)
	if (!bAutoPositionSnapPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitializeSnapPoints: Auto-positioning disabled, using manual positions"));
		return;
	}

	// Get mesh bounds to position snap points at face centers
	if (MeshComponent && MeshComponent->GetStaticMesh())
	{
		FBox MeshBounds = MeshComponent->GetStaticMesh()->GetBoundingBox();
		FVector Extent = MeshBounds.GetExtent();

		UE_LOG(LogTemp, Warning, TEXT("InitializeSnapPoints: Auto-positioning enabled, mesh extent = %s"), *Extent.ToString());

		// Position and orient each snap point
		// Top: Forward=+Z, Up=+Y (toward front)
		SnapPoint_Top->SetRelativeLocation(FVector(0, 0, Extent.Z));
		SnapPoint_Top->SetRelativeRotation(FRotator(90, 0, 0)); // Pitch up

		// Bottom: Forward=-Z, Up=+Y
		SnapPoint_Bottom->SetRelativeLocation(FVector(0, 0, -Extent.Z));
		SnapPoint_Bottom->SetRelativeRotation(FRotator(-90, 0, 0)); // Pitch down

		// Front: Forward=+X, Up=+Z
		SnapPoint_Front->SetRelativeLocation(FVector(Extent.X, 0, 0));
		SnapPoint_Front->SetRelativeRotation(FRotator(0, 0, 0)); // Default orientation

		// Back: Forward=-X, Up=+Z
		SnapPoint_Back->SetRelativeLocation(FVector(-Extent.X, 0, 0));
		SnapPoint_Back->SetRelativeRotation(FRotator(0, 180, 0)); // Turn around

		// Right: Forward=+Y, Up=+Z
		SnapPoint_Right->SetRelativeLocation(FVector(0, Extent.Y, 0));
		SnapPoint_Right->SetRelativeRotation(FRotator(0, 90, 0)); // Turn right

		// Left: Forward=-Y, Up=+Z
		SnapPoint_Left->SetRelativeLocation(FVector(0, -Extent.Y, 0));
		SnapPoint_Left->SetRelativeRotation(FRotator(0, -90, 0)); // Turn left
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

	// Re-initialize snap points with correct mesh bounds (only if auto-positioning is enabled)
	// bAutoPositionSnapPoints is controlled in Blueprint Class Defaults
	// Mesh is already set in Blueprint, so we can auto-position based on it
	if (bAutoPositionSnapPoints)
	{
		InitializeSnapPoints();
	}

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
	EConstructionPartState OldState = CurrentState;
	CurrentState = NewState;

	// Toggle arrow visibility based on debug settings
	bool bShowArrows = (PartData && PartData->bDrawSocketDebug);
	UE_LOG(LogTemp, Warning, TEXT("SetPartState: %d -> %d, bShowArrows=%d"),
		(int32)OldState, (int32)NewState, bShowArrows);

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
