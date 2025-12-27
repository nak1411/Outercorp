// Copyright Epic Games, Inc. All Rights Reserved.

#include "ItemPreviewController.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/StaticMesh.h"

AItemPreviewController::AItemPreviewController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	PreviewRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PreviewRoot"));
	RootComponent = PreviewRoot;

	// Initialize defaults
	TargetRotation = FRotator::ZeroRotator;
	CurrentRotation = FRotator::ZeroRotator;
	TargetZoomDistance = DefaultZoomDistance;
	CurrentZoomDistance = DefaultZoomDistance;
}

void AItemPreviewController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: BeginPlay called"));

	// Auto-find scene capture component if not set
	if (!SceneCaptureComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Searching for SceneCaptureComponent2D..."));
		SceneCaptureComponent = FindComponentByClass<USceneCaptureComponent2D>();
	}

	// Configure scene capture if found
	if (SceneCaptureComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: SceneCaptureComponent found: %s"), *SceneCaptureComponent->GetName());
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Configuring SceneCaptureComponent..."));
		SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
		SceneCaptureComponent->bCaptureEveryFrame = true;
		SceneCaptureComponent->bCaptureOnMovement = true;

		// Check if render target is set
		if (SceneCaptureComponent->TextureTarget)
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Render target is set: %s"), *SceneCaptureComponent->TextureTarget->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ItemPreviewController: No render target set on SceneCaptureComponent!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ItemPreviewController: SceneCaptureComponent not found!"));
	}

	// Set initial camera position
	UpdateCameraPosition();
}

void AItemPreviewController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smooth rotation interpolation
	if (!CurrentRotation.Equals(TargetRotation, 0.1f))
	{
		CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpolationSpeed);

		// Apply rotation to the item mesh if it exists
		if (CurrentItemMesh)
		{
			CurrentItemMesh->SetRelativeRotation(CurrentRotation);
		}
	}

	// Smooth zoom interpolation
	if (!FMath::IsNearlyEqual(CurrentZoomDistance, TargetZoomDistance, 0.1f))
	{
		CurrentZoomDistance = FMath::FInterpTo(CurrentZoomDistance, TargetZoomDistance, DeltaTime, ZoomInterpolationSpeed);
		UpdateCameraPosition();
	}
}

void AItemPreviewController::AddRotationInput(float DeltaX, float DeltaY)
{
	if (!bEnableRotation)
		return;

	FRotator DeltaRotation = FRotator::ZeroRotator;

	if (bAllowYawRotation)
	{
		float YawMultiplier = bInvertYaw ? -1.0f : 1.0f;
		DeltaRotation.Yaw = DeltaX * RotationSensitivity * YawMultiplier;
	}

	if (bAllowPitchRotation)
	{
		float PitchMultiplier = bInvertPitch ? 1.0f : -1.0f;
		DeltaRotation.Pitch = DeltaY * RotationSensitivity * PitchMultiplier;
	}

	if (bAllowRollRotation)
	{
		float RollMultiplier = bInvertRoll ? -1.0f : 1.0f;
		DeltaRotation.Roll = DeltaX * RotationSensitivity * RollMultiplier;
	}

	TargetRotation += DeltaRotation;
	TargetRotation.Normalize();
}

void AItemPreviewController::AddZoomInput(float Delta)
{
	if (!bEnableZoom)
		return;

	TargetZoomDistance -= Delta * ZoomSensitivity;
	TargetZoomDistance = FMath::Clamp(TargetZoomDistance, MinZoomDistance, MaxZoomDistance);
}

void AItemPreviewController::ResetView()
{
	TargetRotation = FRotator::ZeroRotator;
	CurrentRotation = FRotator::ZeroRotator;
	TargetZoomDistance = DefaultZoomDistance;
	CurrentZoomDistance = DefaultZoomDistance;

	if (CurrentItemMesh)
	{
		CurrentItemMesh->SetRelativeRotation(FRotator::ZeroRotator);
	}

	UpdateCameraPosition();
}

void AItemPreviewController::SetPreviewItem(const FInventoryItem& Item)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: SetPreviewItem called"));

	// Clear previous item
	ClearCurrentItem();

	// Store current item
	CurrentItem = Item;

	// Validate item
	if (!Item.IsValid() || !Item.ItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemPreviewController: Item is invalid or has no ItemData!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Item is valid, ItemData name: %s"), *Item.ItemData->GetName());

	// Reset view when changing items
	ResetView();

	// Create mesh component if needed
	if (!CurrentItemMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Creating new mesh component"));
		CurrentItemMesh = NewObject<UStaticMeshComponent>(this, TEXT("ItemMesh"));
		CurrentItemMesh->SetupAttachment(PreviewRoot);
		CurrentItemMesh->RegisterComponent();
		CurrentItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Load and set the mesh from item data
	UStaticMesh* LoadedMesh = nullptr;
	if (Item.ItemData->ItemMesh.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Mesh already loaded, setting directly"));
		LoadedMesh = Item.ItemData->ItemMesh.Get();
		CurrentItemMesh->SetStaticMesh(LoadedMesh);
	}
	else if (!Item.ItemData->ItemMesh.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Loading mesh synchronously"));
		// Async load if not loaded yet
		LoadedMesh = Item.ItemData->ItemMesh.LoadSynchronous();
		if (LoadedMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Mesh loaded successfully: %s"), *LoadedMesh->GetName());
			CurrentItemMesh->SetStaticMesh(LoadedMesh);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ItemPreviewController: Failed to load mesh!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ItemPreviewController: ItemMesh is null!"));
	}

	// Center the mesh at the origin so it rotates around its center
	if (LoadedMesh && CurrentItemMesh)
	{
		FBoxSphereBounds Bounds = LoadedMesh->GetBounds();
		FVector MeshCenter = Bounds.Origin;

		// Offset the mesh so its center is at the origin
		CurrentItemMesh->SetRelativeLocation(-MeshCenter);

		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Centered mesh. Bounds center: %s"), *MeshCenter.ToString());
	}

	// Set initial rotation from item data
	if (Item.ItemData)
	{
		CurrentItemMesh->SetRelativeRotation(Item.ItemData->DefaultPreviewRotation);
		TargetRotation = Item.ItemData->DefaultPreviewRotation;
		CurrentRotation = Item.ItemData->DefaultPreviewRotation;

		// Set zoom distance from item data, or use default if not set
		float ItemZoomDistance = Item.ItemData->PreviewCameraDistance;
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Item PreviewCameraDistance = %f"), ItemZoomDistance);

		if (ItemZoomDistance <= 0.0f)
		{
			ItemZoomDistance = DefaultZoomDistance;
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: PreviewCameraDistance not set or invalid, using DefaultZoomDistance: %f"), DefaultZoomDistance);
		}

		// Ensure zoom distance is within valid range
		ItemZoomDistance = FMath::Clamp(ItemZoomDistance, MinZoomDistance, MaxZoomDistance);

		TargetZoomDistance = ItemZoomDistance;
		CurrentZoomDistance = ItemZoomDistance;
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Final zoom distance after clamping: %f (Min: %f, Max: %f)"), CurrentZoomDistance, MinZoomDistance, MaxZoomDistance);
		UpdateCameraPosition();
	}

	// Add to scene capture show only list
	if (SceneCaptureComponent && CurrentItemMesh)
	{
		SceneCaptureComponent->ShowOnlyComponents.Add(CurrentItemMesh);
		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Added mesh to ShowOnlyComponents. Total components: %d"), SceneCaptureComponent->ShowOnlyComponents.Num());
	}
	else
	{
		if (!SceneCaptureComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("ItemPreviewController: SceneCaptureComponent is NULL!"));
		}
		if (!CurrentItemMesh)
		{
			UE_LOG(LogTemp, Error, TEXT("ItemPreviewController: CurrentItemMesh is NULL!"));
		}
	}
}

void AItemPreviewController::UpdateCameraPosition()
{
	if (SceneCaptureComponent)
	{
		// Position camera at the current zoom distance
		FVector CameraLocation = FVector(-CurrentZoomDistance, 0.0f, 0.0f);
		SceneCaptureComponent->SetRelativeLocation(CameraLocation);

		// Camera should always look at the center
		SceneCaptureComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
}

void AItemPreviewController::ClearCurrentItem()
{
	if (CurrentItemMesh)
	{
		// Clear show only list
		if (SceneCaptureComponent)
		{
			SceneCaptureComponent->ShowOnlyComponents.Empty();
		}

		// Clear the mesh
		CurrentItemMesh->SetStaticMesh(nullptr);
	}
}
