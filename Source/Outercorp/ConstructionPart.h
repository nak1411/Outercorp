// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructionPart.generated.h"

UENUM(BlueprintType)
enum class EConstructionPartState : uint8
{
	InInventory		UMETA(DisplayName = "In Inventory"),
	GhostPreview	UMETA(DisplayName = "Ghost Preview"),
	Placed			UMETA(DisplayName = "Placed"),
	Fastened		UMETA(DisplayName = "Fastened")
};

UENUM(BlueprintType)
enum class EPlacementMode : uint8
{
	SocketSnap		UMETA(DisplayName = "Socket Snap"),			// Snap to sockets only
	GroundSnap		UMETA(DisplayName = "Ground Snap"),			// Snap to floor (no clipping)
	FreePlace		UMETA(DisplayName = "Free Place"),			// Place anywhere in air
	Hybrid			UMETA(DisplayName = "Hybrid")				// Socket snap if available, else ground
};

UENUM(BlueprintType)
enum class EAttachmentType : uint8
{
	Mount			UMETA(DisplayName = "Mount"),		// Structural connection point
	Bolt			UMETA(DisplayName = "Bolt"),		// Fastening point
	Utility			UMETA(DisplayName = "Utility")		// Power/fluid/data connection
};

USTRUCT(BlueprintType)
struct FSocketConnection
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* ConnectedPart = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ConnectedSocket;

	FSocketConnection()
		: ConnectedPart(nullptr)
		, ConnectedSocket(NAME_None)
	{}

	FSocketConnection(AActor* InPart, FName InSocket)
		: ConnectedPart(InPart)
		, ConnectedSocket(InSocket)
	{}
};

USTRUCT(BlueprintType)
struct FAttachmentPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttachmentType AttachmentType;

	// Offset adjustment for this specific socket (editable per socket)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
	FVector SocketOffset = FVector::ZeroVector;

	// Deprecated - kept for backward compatibility
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOccupied = false;

	// Deprecated - kept for backward compatibility
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* ConnectedPart = nullptr;

	// Deprecated - kept for backward compatibility
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ConnectedSocket;

	// New: Multiple connections per socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSocketConnection> Connections;

	FAttachmentPoint()
		: SocketName(NAME_None)
		, AttachmentType(EAttachmentType::Mount)
		, SocketOffset(FVector::ZeroVector)
		, bIsOccupied(false)
		, ConnectedPart(nullptr)
		, ConnectedSocket(NAME_None)
	{}
};

UCLASS()
class OUTERCORP_API AConstructionPart : public AActor
{
	GENERATED_BODY()

public:
	AConstructionPart();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	// Part Data Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction")
	class UConstructionPartData* PartData;

	// Deprecated - kept for backward compatibility during transition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Deprecated")
	FString PartName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Deprecated")
	FString PartType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Deprecated")
	float Mass = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Deprecated")
	bool bRequiresTool = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Deprecated")
	EPlacementMode PlacementMode = EPlacementMode::Hybrid;

	// State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Construction")
	EConstructionPartState CurrentState;

	// Attachment Points (automatically populated from mesh sockets)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Construction")
	TArray<FAttachmentPoint> AttachmentPoints;


	// Functions
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void InitializeAttachmentPoints();

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void InitializeFromData(UConstructionPartData* Data);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void SetPartState(EConstructionPartState NewState);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void SetGhostPreview(bool bIsValid);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	bool FindNearbyAttachmentPoint(const FVector& SearchLocation, AActor*& OutNearbyPart, FName& OutSocketName, FTransform& OutSocketTransform);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	bool CanAttachToSocket(AConstructionPart* OtherPart, FName OtherSocket, FName MySocket);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void AttachToPart(AConstructionPart* OtherPart, FName OtherSocket, FName MySocket);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void DetachFromPart(FName MySocket);

	UFUNCTION(BlueprintCallable, Category = "Construction")
	TArray<FName> GetAvailableSockets() const;

	UFUNCTION(BlueprintCallable, Category = "Construction")
	FTransform GetSocketTransformByName(FName SocketName) const;

	UFUNCTION(BlueprintPure, Category = "Construction")
	bool IsFullyFastened() const;

	UFUNCTION(BlueprintPure, Category = "Construction")
	int32 GetFastenedConnectionCount() const;

private:
	EAttachmentType DetermineAttachmentType(const FName& SocketName);

	UMaterialInterface* OriginalMaterial;
	TArray<UMaterialInterface*> OriginalMaterials;
	void StoreOriginalMaterials();
	void RestoreOriginalMaterials();
};
