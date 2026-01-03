// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WrenchTool.h"
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

UENUM(BlueprintType)
enum class ESnapFilterMode : uint8
{
	NoFilter		UMETA(DisplayName = "No Filter"),		// Accept any snap point (default)
	Whitelist		UMETA(DisplayName = "Whitelist"),		// Only accept tags in the list
	Blacklist		UMETA(DisplayName = "Blacklist")		// Reject tags in the list
};

USTRUCT(BlueprintType)
struct FSnapPointFilter
{
	GENERATED_BODY()

	/** Filter mode: No filter, whitelist, or blacklist */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snap Filter")
	ESnapFilterMode FilterMode = ESnapFilterMode::NoFilter;

	/** Tags to filter by. In whitelist mode, only these tags are accepted. In blacklist mode, these tags are rejected. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snap Filter")
	TArray<FName> FilterTags;

	FSnapPointFilter()
		: FilterMode(ESnapFilterMode::NoFilter)
	{}

	/** Check if this filter allows the given tag */
	bool AllowsTag(FName Tag) const
	{
		if (FilterMode == ESnapFilterMode::NoFilter)
		{
			return true; // No filtering
		}

		bool TagExists = FilterTags.Contains(Tag);

		if (FilterMode == ESnapFilterMode::Whitelist)
		{
			// Whitelist: Only allow if tag is in the list
			// If filter list is empty, reject everything (must explicitly whitelist tags)
			return TagExists;
		}
		else // Blacklist
		{
			// Blacklist: Reject if tag is in the list
			return !TagExists;
		}
	}
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

	// Socket type/tag for matching compatible sockets (e.g., "MountPoint", "PowerPort", etc.)
	// Only sockets with matching types can snap together
	// Leave empty to allow snapping to any socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
	FName SocketType = NAME_None;

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
class OUTERCORP_API AConstructionPart : public AActor, public IConnectableInterface
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

	// Overlap bounds visualization (for overlap detection)
	// Multiple bounds can be used for complex shapes (e.g., rack with two posts)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TArray<class UBoxComponent*> OverlapBounds;

	// Snap point system using Arrow components (following snapping.txt guide)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SnapRoot;

	// Dynamic array of snap points - can be populated in Blueprint or C++
	// Add ArrowComponents as children of SnapRoot in your Blueprint child class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TArray<class UArrowComponent*> SnapPoints;

	// OPTIONAL: Custom tag overrides for snap points
	// By default, the arrow component's NAME is used as its tag (e.g., "ShelfMount_01" arrow = "ShelfMount_01" tag)
	// Only use this map if you want to override the default name with a different tag
	// Key = Arrow component name, Value = Custom tag name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snap Filtering")
	TMap<FName, FName> SnapPointTags;

	// Snap point filters - controls what this snap point will accept
	// Key = Arrow component name, Value = Filter settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snap Filtering")
	TMap<FName, FSnapPointFilter> SnapPointFilters;

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

	// Deprecated - kept for backward compatibility
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Construction|Deprecated")
	TArray<FAttachmentPoint> AttachmentPoints;


	// Functions
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void InitializeSnapPoints();

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void InitializeOverlapBounds();

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

	UFUNCTION(BlueprintCallable, Category = "Construction")
	void RestoreOriginalMaterials();

	// Snap Filtering Functions

	/** Get the tag for a specific snap point (arrow component) */
	UFUNCTION(BlueprintPure, Category = "Snap Filtering")
	FName GetSnapPointTag(UArrowComponent* SnapPoint) const;

	/** Check if two snap points are compatible based on their tags and filters */
	UFUNCTION(BlueprintPure, Category = "Snap Filtering")
	bool AreSnapPointsCompatible(UArrowComponent* MySnapPoint, AConstructionPart* OtherPart, UArrowComponent* OtherSnapPoint) const;

	// === IConnectableInterface Implementation ===

	/** Check if this construction part can be connected/fastened with wrench */
	virtual bool CanBeConnected_Implementation() const override;

	/** Get the prompt text for wrench interaction */
	virtual FText GetConnectionPrompt_Implementation() const override;

	/** Called when wrench connects/fastens this part */
	virtual void OnConnected_Implementation() override;

	/** Check if this part is already connected/fastened */
	virtual bool IsConnected_Implementation() const override;

	/** Called when wrench disconnects/unfastens this part */
	virtual void OnDisconnected_Implementation() override;

private:
	EAttachmentType DetermineAttachmentType(const FName& SocketName);

	UMaterialInterface* OriginalMaterial;
	TArray<UMaterialInterface*> OriginalMaterials;
	void StoreOriginalMaterials();
};
