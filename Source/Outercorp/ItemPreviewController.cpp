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
	TargetRotationQuat = FQuat::Identity;
	CurrentRotationQuat = FQuat::Identity;
	TargetZoomDistance = DefaultZoomDistance;
	CurrentZoomDistance = DefaultZoomDistance;
	CurrentItemMinZoom = MinZoomDistance;
	CurrentItemMaxZoom = MaxZoomDistance;
	CurrentItemInitialZoom = DefaultZoomDistance;
}

void AItemPreviewController::BeginPlay()
{
	Super::BeginPlay();

	// Auto-find scene capture component if not set
	if (!SceneCaptureComponent)
	{
		SceneCaptureComponent = FindComponentByClass<USceneCaptureComponent2D>();
	}

	// Configure scene capture if found
	if (SceneCaptureComponent)
	{
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

		// Set background color
		if (SceneCaptureComponent->TextureTarget)
		{
			SceneCaptureComponent->TextureTarget->ClearColor = BackgroundColor;
		}
	}

	// Add all existing static mesh components to the ShowOnlyList (e.g., background plane)
	if (SceneCaptureComponent)
	{
		TArray<UStaticMeshComponent*> StaticMeshComponents;
		GetComponents<UStaticMeshComponent>(StaticMeshComponents);
		for (UStaticMeshComponent* MeshComp : StaticMeshComponents)
		{
			if (MeshComp != CurrentItemMesh)
			{
				SceneCaptureComponent->ShowOnlyComponents.Add(MeshComp);
			}
		}
	}

	// Set initial camera position
	UpdateCameraPosition();
}

void AItemPreviewController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smooth rotation interpolation using quaternion slerp
	if (!CurrentRotationQuat.Equals(TargetRotationQuat, 0.001f))
	{
		CurrentRotationQuat = FQuat::Slerp(CurrentRotationQuat, TargetRotationQuat, FMath::Clamp(DeltaTime * RotationInterpolationSpeed, 0.0f, 1.0f));
		CurrentRotationQuat.Normalize();

		// Apply rotation to the item mesh if it exists
		if (CurrentItemMesh)
		{
			CurrentItemMesh->SetRelativeRotation(CurrentRotationQuat.Rotator());
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

	if (bUseArcballRotation)
	{
		// Arcball/trackball rotation - rotate around camera-space axes
		// This creates intuitive rotation that follows the mouse cursor

		// Scale input by sensitivity and a base multiplier for good feel
		float ScaledDeltaX = DeltaX * RotationSensitivity * 0.3f;
		float ScaledDeltaY = DeltaY * RotationSensitivity * 0.3f;

		// Create rotation quaternions around camera-relative axes
		// Horizontal mouse movement rotates around the world up axis (Z)
		FQuat YawRotation = FQuat(FVector::UpVector, FMath::DegreesToRadians(-ScaledDeltaX));

		// Vertical mouse movement rotates around the camera's right axis (Y in our coordinate system)
		FQuat PitchRotation = FQuat(FVector::RightVector, FMath::DegreesToRadians(-ScaledDeltaY));

		// Combine rotations: apply pitch first (in object space), then yaw (in world space)
		// This order gives natural trackball-like behavior
		FQuat DeltaRotation = YawRotation * PitchRotation;

		// Apply the delta rotation to the current target
		TargetRotationQuat = DeltaRotation * TargetRotationQuat;
		TargetRotationQuat.Normalize();
	}
	else
	{
		// Simple euler angle rotation (legacy mode)
		FRotator CurrentRotator = TargetRotationQuat.Rotator();
		FRotator DeltaRotation = FRotator::ZeroRotator;

		DeltaRotation.Yaw = DeltaX * RotationSensitivity * 0.5f;
		DeltaRotation.Pitch = -DeltaY * RotationSensitivity * 0.5f;

		CurrentRotator += DeltaRotation;
		CurrentRotator.Normalize();
		TargetRotationQuat = FQuat(CurrentRotator);
	}
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
	TargetRotationQuat = FQuat::Identity;
	CurrentRotationQuat = FQuat::Identity;

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
		return;
	}

	// Reset view when changing items
	ResetView();

	// Create mesh component if needed
	if (!CurrentItemMesh)
	{
		CurrentItemMesh = NewObject<UStaticMeshComponent>(this, TEXT("ItemMesh"));
		CurrentItemMesh->SetupAttachment(PreviewRoot);
		CurrentItemMesh->RegisterComponent();
		CurrentItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Load and set the mesh from item data
	UStaticMesh* LoadedMesh = nullptr;
	if (Item.ItemData->ItemMesh.IsValid())
	{
		LoadedMesh = Item.ItemData->ItemMesh.Get();
		CurrentItemMesh->SetStaticMesh(LoadedMesh);
	}
	else if (!Item.ItemData->ItemMesh.IsNull())
	{
		LoadedMesh = Item.ItemData->ItemMesh.LoadSynchronous();
		if (LoadedMesh)
		{
			CurrentItemMesh->SetStaticMesh(LoadedMesh);
		}
	}

	// Set up the mesh - it will rotate around its pivot point (as set in the mesh asset)
	if (LoadedMesh && CurrentItemMesh)
	{
		CurrentItemMesh->SetRelativeLocation(FVector::ZeroVector);
	}

	// Set initial rotation from item data and calculate zoom
	if (Item.ItemData && LoadedMesh && CurrentItemMesh)
	{
		CurrentItemMesh->SetRelativeRotation(Item.ItemData->DefaultPreviewRotation);
		TargetRotationQuat = FQuat(Item.ItemData->DefaultPreviewRotation);
		CurrentRotationQuat = FQuat(Item.ItemData->DefaultPreviewRotation);

		// Update bounds with the rotation applied
		CurrentItemMesh->UpdateBounds();
		FBoxSphereBounds MeshBounds = CurrentItemMesh->Bounds;

		// Calculate zoom distance
		float ItemZoomDistance = DefaultZoomDistance;

		if (bAutoZoomToExtents)
		{
			// Use automatic zoom-to-extents based on mesh bounds
			ItemZoomDistance = CalculateZoomDistanceForBounds(MeshBounds);

			// Set dynamic zoom range based on the calculated distance
			CurrentItemMinZoom = ItemZoomDistance * 0.5f;
			CurrentItemMaxZoom = ItemZoomDistance * 2.0f;

			// But still respect the absolute min/max if they're more restrictive
			CurrentItemMinZoom = FMath::Max(CurrentItemMinZoom, MinZoomDistance);
			if (!bAutoZoomCanExceedMaxDistance)
			{
				CurrentItemMaxZoom = FMath::Min(CurrentItemMaxZoom, MaxZoomDistance);
			}
		}
		else if (Item.ItemData->PreviewCameraDistance > 0.0f)
		{
			// Use manual distance from item data
			ItemZoomDistance = Item.ItemData->PreviewCameraDistance;
			ItemZoomDistance = FMath::Clamp(ItemZoomDistance, MinZoomDistance, MaxZoomDistance);

			// Use standard zoom range for manual distances
			CurrentItemMinZoom = MinZoomDistance;
			CurrentItemMaxZoom = MaxZoomDistance;
		}
		else
		{
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
	FVector BoxExtent = Bounds.BoxExtent;

	// Calculate the effective extents visible from the camera angle
	float VisibleWidth = BoxExtent.Y;
	float VisibleHeight = BoxExtent.Z;
	float DepthExtent = BoxExtent.X;

	// Calculate distance needed for horizontal fit
	float HalfHorizontalFOV = FMath::DegreesToRadians(HorizontalFOV * 0.5f);
	float DistanceForWidth = (VisibleWidth * AutoZoomPaddingMultiplier) / FMath::Tan(HalfHorizontalFOV);

	// Calculate distance needed for vertical fit
	float HalfVerticalFOV = FMath::DegreesToRadians(VerticalFOV * 0.5f);
	float DistanceForHeight = (VisibleHeight * AutoZoomPaddingMultiplier) / FMath::Tan(HalfVerticalFOV);

	// Use the larger distance to ensure the object fits in both dimensions
	float BaseDistance = FMath::Max(DistanceForWidth, DistanceForHeight);

	// Add depth extent to ensure we're far enough back to see the full object
	BaseDistance += DepthExtent;

	// Clamp to min zoom distance always, but only clamp to max if allowed
	float FinalDistance = FMath::Max(BaseDistance, MinZoomDistance);

	// Only enforce MaxZoomDistance if auto-zoom is not allowed to exceed it
	if (!bAutoZoomCanExceedMaxDistance)
	{
		FinalDistance = FMath::Min(FinalDistance, MaxZoomDistance);
	}

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
		}

		// Clear the mesh
		CurrentItemMesh->SetStaticMesh(nullptr);
	}
}
