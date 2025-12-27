// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItemData.h"
#include "ItemPreviewController.generated.h"

class USceneCaptureComponent2D;
class USceneComponent;

/**
 * Controller for the item preview viewport - handles rotation, zoom, and item swapping
 * Designed to work with BP_ItemInfoPreview blueprint
 */
UCLASS(Blueprintable)
class OUTERCORP_API AItemPreviewController : public AActor
{
	GENERATED_BODY()

public:
	AItemPreviewController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** Root component for the preview setup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Preview")
	TObjectPtr<USceneComponent> PreviewRoot;

	/** Scene capture component (set from Blueprint - use your existing one) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview")
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;

	/** Current spawned item mesh component */
	UPROPERTY(BlueprintReadOnly, Category = "Preview")
	TObjectPtr<UStaticMeshComponent> CurrentItemMesh;

	/** Background color for the preview (change this for different background colors) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview")
	FLinearColor BackgroundColor = FLinearColor(0.02f, 0.02f, 0.02f, 1.0f); // Very dark gray

	// === Rotation Settings ===

	/** Rotation sensitivity (multiplier for mouse movement) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	float RotationSensitivity = 1.0f;

	/** Enable/disable rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	bool bEnableRotation = true;

	/** Use arcball/trackball rotation (more intuitive) vs simple euler angle rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	bool bUseArcballRotation = true;

	/** Smooth rotation interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	float RotationInterpolationSpeed = 10.0f;

	// === Zoom Settings ===

	/** Enable/disable zoom */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Zoom")
	bool bEnableZoom = true;

	/** Zoom sensitivity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Zoom")
	float ZoomSensitivity = 10.0f;

	/** Minimum zoom distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Zoom")
	float MinZoomDistance = 50.0f;

	/** Maximum zoom distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Zoom")
	float MaxZoomDistance = 500.0f;

	/** Default zoom distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Zoom")
	float DefaultZoomDistance = 200.0f;

	/** Smooth zoom interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Zoom")
	float ZoomInterpolationSpeed = 10.0f;

	// === Auto Zoom-to-Extents Settings ===

	/** Automatically calculate zoom distance based on item bounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Zoom")
	bool bAutoZoomToExtents = true;

	/** Padding multiplier for auto-zoom (1.5 = 50% padding around item) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Zoom")
	float AutoZoomPaddingMultiplier = 1.5f;

	/** Allow auto-zoom to exceed MaxZoomDistance if needed to fit the object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Zoom")
	bool bAutoZoomCanExceedMaxDistance = true;

	// === Public Functions ===

	/** Handle mouse/touch input for rotation (call this from Blueprint on mouse drag) */
	UFUNCTION(BlueprintCallable, Category = "Preview")
	void AddRotationInput(float DeltaX, float DeltaY);

	/** Handle zoom input (call this from Blueprint on mouse wheel) */
	UFUNCTION(BlueprintCallable, Category = "Preview")
	void AddZoomInput(float Delta);

	/** Reset rotation and zoom to defaults */
	UFUNCTION(BlueprintCallable, Category = "Preview")
	void ResetView();

	/** Swap the currently displayed item */
	UFUNCTION(BlueprintCallable, Category = "Preview")
	void SetPreviewItem(const FInventoryItem& Item);

	/** Get the current item being previewed */
	UFUNCTION(BlueprintPure, Category = "Preview")
	FInventoryItem GetCurrentItem() const { return CurrentItem; }

protected:
	/** Current item being previewed */
	UPROPERTY(BlueprintReadOnly, Category = "Preview")
	FInventoryItem CurrentItem;

	/** Target rotation quaternion (for arcball rotation) */
	FQuat TargetRotationQuat;

	/** Current rotation quaternion (smoothly interpolated) */
	FQuat CurrentRotationQuat;

	/** Target zoom distance */
	float TargetZoomDistance;

	/** Current zoom distance */
	float CurrentZoomDistance;

	/** Dynamic min/max zoom bounds calculated from auto-zoom (for current item) */
	float CurrentItemMinZoom;
	float CurrentItemMaxZoom;
	float CurrentItemInitialZoom; // The calculated auto-zoom distance for reset

	/** Update camera position based on zoom */
	void UpdateCameraPosition();

	/** Calculate optimal zoom distance based on mesh bounds */
	float CalculateZoomDistanceForBounds(const FBoxSphereBounds& Bounds) const;

	/** Clear the current preview item */
	void ClearCurrentItem();
};
