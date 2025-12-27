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
	CurrentItemMinZoom = MinZoomDistance;
	CurrentItemMaxZoom = MaxZoomDistance;
	CurrentItemInitialZoom = DefaultZoomDistance;
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

		// Use ShowOnlyList to only render our specific item
		SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
		SceneCaptureComponent->bCaptureEveryFrame = true;
		SceneCaptureComponent->bCaptureOnMovement = true;

		// Completely disable world rendering
		SceneCaptureComponent->ShowFlags.SetAtmosphere(false);
		SceneCaptureComponent->ShowFlags.SetFog(false);
		SceneCaptureComponent->ShowFlags.SetVolumetricFog(false);
		SceneCaptureComponent->ShowFlags.SetSkyLighting(false);
		SceneCaptureComponent->ShowFlags.SetTemporalAA(false);

		// Disable environment effects
		SceneCaptureComponent->ShowFlags.SetAmbientOcclusion(false);
		SceneCaptureComponent->ShowFlags.SetDeferredLighting(true);

		// Enable lighting from local lights in ShowOnlyList
		SceneCaptureComponent->ShowFlags.SetLighting(true);
		SceneCaptureComponent->ShowFlags.SetPostProcessing(false);

		// Capture with proper lighting (not base color)
		SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

		// Check if render target is set and configure background color
		if (SceneCaptureComponent->TextureTarget)
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Render target is set: %s"), *SceneCaptureComponent->TextureTarget->GetName());

			// Set the clear color on the render target
			SceneCaptureComponent->TextureTarget->ClearColor = BackgroundColor;
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Set background color to: %s"), *BackgroundColor.ToString());
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

	// Log warning about light setup - lights must have bAffectsWorld=false set in Blueprint
	UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: IMPORTANT - Make sure all lights in this actor have 'Affects World' set to FALSE in the Blueprint!"));

	// Add all existing static mesh components to the ShowOnlyList (e.g., background plane)
	if (SceneCaptureComponent)
	{
		TArray<UStaticMeshComponent*> StaticMeshComponents;
		GetComponents<UStaticMeshComponent>(StaticMeshComponents);
		int32 BackgroundMeshCount = 0;
		for (UStaticMeshComponent* MeshComp : StaticMeshComponents)
		{
			if (MeshComp != CurrentItemMesh) // Don't add the item mesh yet, it will be added when an item is set
			{
				SceneCaptureComponent->ShowOnlyComponents.Add(MeshComp);
				BackgroundMeshCount++;
				UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Added background mesh to ShowOnlyComponents: %s"), *MeshComp->GetName());
			}
		}

		if (BackgroundMeshCount > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Added %d background mesh(es). IMPORTANT: Background meshes must use EMISSIVE materials to be visible!"), BackgroundMeshCount);
		}
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
	// Use dynamic zoom range based on current item's auto-zoom calculation
	TargetZoomDistance = FMath::Clamp(TargetZoomDistance, CurrentItemMinZoom, CurrentItemMaxZoom);
}

void AItemPreviewController::ResetView()
{
	TargetRotation = FRotator::ZeroRotator;
	CurrentRotation = FRotator::ZeroRotator;

	// Reset to the item's initial calculated zoom distance (not the default)
	TargetZoomDistance = CurrentItemInitialZoom;
	CurrentZoomDistance = CurrentItemInitialZoom;

	if (CurrentItemMesh)
	{
		CurrentItemMesh->SetRelativeRotation(FRotator::ZeroRotator);
	}

	UpdateCameraPosition();
}

void AItemPreviewController::SetPreviewItem(const FInventoryItem& Item)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: SetPreviewItem called"));

	// Ensure background color is set (in case it got reset)
	if (SceneCaptureComponent && SceneCaptureComponent->TextureTarget)
	{
		SceneCaptureComponent->TextureTarget->ClearColor = BackgroundColor;
	}

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

	// Set up the mesh - it will rotate around its pivot point (as set in the mesh asset)
	if (LoadedMesh && CurrentItemMesh)
	{
		// Place the mesh at the origin - its pivot point will be at (0,0,0)
		// This means it will rotate around its pivot naturally
		CurrentItemMesh->SetRelativeLocation(FVector::ZeroVector);

		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Mesh setup - using mesh pivot as rotation center"));
	}

	// Set initial rotation from item data and calculate zoom
	if (Item.ItemData && LoadedMesh && CurrentItemMesh)
	{
		CurrentItemMesh->SetRelativeRotation(Item.ItemData->DefaultPreviewRotation);
		TargetRotation = Item.ItemData->DefaultPreviewRotation;
		CurrentRotation = Item.ItemData->DefaultPreviewRotation;

		// Update bounds with the rotation applied
		CurrentItemMesh->UpdateBounds();
		FBoxSphereBounds MeshBounds = CurrentItemMesh->Bounds;

		UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Mesh bounds after rotation:"));
		UE_LOG(LogTemp, Warning, TEXT("  Bounds Extent: %s"), *MeshBounds.BoxExtent.ToString());
		UE_LOG(LogTemp, Warning, TEXT("  Sphere Radius: %f"), MeshBounds.SphereRadius);

		// Calculate zoom distance
		float ItemZoomDistance = DefaultZoomDistance;

		if (bAutoZoomToExtents)
		{
			// Use automatic zoom-to-extents based on mesh bounds
			ItemZoomDistance = CalculateZoomDistanceForBounds(MeshBounds);
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Using auto zoom-to-extents: %f"), ItemZoomDistance);

			// Set dynamic zoom range based on the calculated distance
			// Allow zooming in to 50% of auto-zoom distance, and out to 200% of auto-zoom distance
			CurrentItemMinZoom = ItemZoomDistance * 0.5f;
			CurrentItemMaxZoom = ItemZoomDistance * 2.0f;

			// But still respect the absolute min/max if they're more restrictive
			CurrentItemMinZoom = FMath::Max(CurrentItemMinZoom, MinZoomDistance);
			if (!bAutoZoomCanExceedMaxDistance)
			{
				CurrentItemMaxZoom = FMath::Min(CurrentItemMaxZoom, MaxZoomDistance);
			}

			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Dynamic zoom range: Min=%f, Max=%f"), CurrentItemMinZoom, CurrentItemMaxZoom);
		}
		else if (Item.ItemData->PreviewCameraDistance > 0.0f)
		{
			// Use manual distance from item data
			ItemZoomDistance = Item.ItemData->PreviewCameraDistance;
			ItemZoomDistance = FMath::Clamp(ItemZoomDistance, MinZoomDistance, MaxZoomDistance);
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Using manual PreviewCameraDistance: %f"), ItemZoomDistance);

			// Use standard zoom range for manual distances
			CurrentItemMinZoom = MinZoomDistance;
			CurrentItemMaxZoom = MaxZoomDistance;
		}
		else
		{
			// Use default distance
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Using DefaultZoomDistance: %f"), DefaultZoomDistance);

			// Use standard zoom range
			CurrentItemMinZoom = MinZoomDistance;
			CurrentItemMaxZoom = MaxZoomDistance;
		}

		// Store the initial zoom distance for reset functionality
		CurrentItemInitialZoom = ItemZoomDistance;

		TargetZoomDistance = ItemZoomDistance;
		CurrentZoomDistance = ItemZoomDistance;
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

float AItemPreviewController::CalculateZoomDistanceForBounds(const FBoxSphereBounds& Bounds) const
{
	if (!SceneCaptureComponent)
	{
		return DefaultZoomDistance;
	}

	// Get the camera's field of view (vertical FOV)
	float VerticalFOV = SceneCaptureComponent->FOVAngle;

	// Get aspect ratio from the render target
	float AspectRatio = 1.0f;
	if (SceneCaptureComponent->TextureTarget)
	{
		AspectRatio = (float)SceneCaptureComponent->TextureTarget->SizeX / (float)SceneCaptureComponent->TextureTarget->SizeY;
	}

	// Calculate horizontal FOV from vertical FOV and aspect ratio
	float HorizontalFOV = FMath::RadiansToDegrees(2.0f * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(VerticalFOV * 0.5f)) * AspectRatio));

	// Transform bounds to camera-local space
	// Camera is at (-distance, 0, 0) looking along +X axis
	// So we need to know the extents as viewed from the camera position
	FVector BoxExtent = Bounds.BoxExtent;

	// Transform the bounds origin to local space of PreviewRoot
	FVector BoundsOriginLocal = FVector::ZeroVector;
	if (PreviewRoot)
	{
		BoundsOriginLocal = PreviewRoot->GetComponentTransform().InverseTransformPosition(Bounds.Origin);
	}

	// The camera looks along the -X axis toward the origin (0,0,0) where the mesh is centered
	// Y extent affects horizontal screen space
	// Z extent affects vertical screen space
	// X extent affects depth (distance from camera to furthest point)

	// Calculate the effective extents visible from the camera angle
	float VisibleWidth = BoxExtent.Y;   // Half-width on screen
	float VisibleHeight = BoxExtent.Z;  // Half-height on screen
	float DepthExtent = BoxExtent.X;    // Depth along camera view direction

	// Calculate distance needed for horizontal fit
	float HalfHorizontalFOV = FMath::DegreesToRadians(HorizontalFOV * 0.5f);
	float DistanceForWidth = (VisibleWidth * AutoZoomPaddingMultiplier) / FMath::Tan(HalfHorizontalFOV);

	// Calculate distance needed for vertical fit
	float HalfVerticalFOV = FMath::DegreesToRadians(VerticalFOV * 0.5f);
	float DistanceForHeight = (VisibleHeight * AutoZoomPaddingMultiplier) / FMath::Tan(HalfVerticalFOV);

	// Use the larger distance to ensure the object fits in both dimensions
	float BaseDistance = FMath::Max(DistanceForWidth, DistanceForHeight);

	// Add depth extent to ensure we're far enough back to see the full object
	// (accounting for the part of the object that extends toward the camera)
	BaseDistance += DepthExtent;

	// Clamp to min zoom distance always, but only clamp to max if allowed
	float FinalDistance = FMath::Max(BaseDistance, MinZoomDistance);

	// Only enforce MaxZoomDistance if auto-zoom is not allowed to exceed it
	if (!bAutoZoomCanExceedMaxDistance)
	{
		FinalDistance = FMath::Min(FinalDistance, MaxZoomDistance);
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Auto-zoom calculation:"));
	UE_LOG(LogTemp, Warning, TEXT("  BoxExtent (world AABB): %s"), *BoxExtent.ToString());
	UE_LOG(LogTemp, Warning, TEXT("  BoundsOrigin (local): %s"), *BoundsOriginLocal.ToString());
	UE_LOG(LogTemp, Warning, TEXT("  VerticalFOV: %f, HorizontalFOV: %f, AspectRatio: %f"), VerticalFOV, HorizontalFOV, AspectRatio);
	UE_LOG(LogTemp, Warning, TEXT("  VisibleWidth: %f, VisibleHeight: %f, DepthExtent: %f"), VisibleWidth, VisibleHeight, DepthExtent);
	UE_LOG(LogTemp, Warning, TEXT("  DistanceForWidth: %f, DistanceForHeight: %f"), DistanceForWidth, DistanceForHeight);
	UE_LOG(LogTemp, Warning, TEXT("  BaseDistance: %f, FinalDistance: %f (CanExceedMax: %s)"),
		BaseDistance, FinalDistance, bAutoZoomCanExceedMaxDistance ? TEXT("true") : TEXT("false"));

	return FinalDistance;
}

void AItemPreviewController::ClearCurrentItem()
{
	if (CurrentItemMesh)
	{
		// Remove only the item mesh from show only list, keep background meshes
		if (SceneCaptureComponent)
		{
			SceneCaptureComponent->ShowOnlyComponents.Remove(CurrentItemMesh);
			UE_LOG(LogTemp, Warning, TEXT("ItemPreviewController: Removed item mesh from ShowOnlyComponents, keeping background meshes"));
		}

		// Clear the mesh
		CurrentItemMesh->SetStaticMesh(nullptr);
	}
}
