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

	// === Rotation Settings ===

	/** Rotation sensitivity (degrees per pixel of mouse movement) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	float RotationSensitivity = 0.5f;

	/** Enable/disable rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	bool bEnableRotation = true;

	/** Rotation axis constraints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	bool bAllowYawRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	bool bAllowPitchRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	bool bAllowRollRotation = false;

	/** Invert rotation axes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	bool bInvertYaw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	bool bInvertPitch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preview|Rotation")
	bool bInvertRoll = false;

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

	/** Target rotation (interpolated to) */
	FRotator TargetRotation;

	/** Current rotation (smoothly interpolated) */
	FRotator CurrentRotation;

	/** Target zoom distance */
	float TargetZoomDistance;

	/** Current zoom distance */
	float CurrentZoomDistance;

	/** Update camera position based on zoom */
	void UpdateCameraPosition();

	/** Clear the current preview item */
	void ClearCurrentItem();
};
