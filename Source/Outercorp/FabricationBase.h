// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "FabricationBase.generated.h"

UENUM(BlueprintType)
enum class EFabricationState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	InUse			UMETA(DisplayName = "In Use"),
	Crafting		UMETA(DisplayName = "Crafting"),
	Disabled		UMETA(DisplayName = "Disabled")
};

/**
 * Base class for all fabrication stations (workbenches, machines, etc.)
 * Visual/collision/snapping components are set up in Blueprint child classes
 * Gameplay properties are defined in data assets (UFabricationData)
 */
UCLASS()
class OUTERCORP_API AFabricationBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AFabricationBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ============================================================================
	// Components
	// ============================================================================

	/** Root scene component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	/** Main mesh component (set in Blueprint) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	/** Interactable component for player interaction */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInteractableComponent* InteractableComponent;

	/** Camera for crafting mode (position/rotation set in Blueprint) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CraftingCameraPosition;

	// ============================================================================
	// Interaction Zones (Set in Blueprint)
	// ============================================================================

	/** Interaction zone for toolbox */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting Zones")
	class UBoxComponent* ToolboxZone;

	/** Interaction zone for material/hardware bin */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting Zones")
	class UBoxComponent* MaterialBinZone;

	/** Interaction zone for work surface */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting Zones")
	class UBoxComponent* WorkSurfaceZone;

	// ============================================================================
	// Data Asset
	// ============================================================================

	/** Fabrication data asset defining gameplay properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabrication")
	class UFabricationData* FabricationData;

	// ============================================================================
	// State
	// ============================================================================

	/** Current state of this fabrication station */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fabrication")
	EFabricationState CurrentState;

	/** Actor currently using this fabrication station */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fabrication")
	AActor* CurrentUser;

	/** Is the player in interactive crafting mode? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fabrication")
	bool bIsInCraftingMode = false;

	/** Store player's original camera view for restoration */
	UPROPERTY()
	FVector StoredCameraLocation;

	UPROPERTY()
	FRotator StoredCameraRotation;

	/** Store original materials for zones to restore after highlighting */
	UPROPERTY()
	TArray<UMaterialInterface*> ToolboxOriginalMaterials;

	UPROPERTY()
	TArray<UMaterialInterface*> MaterialBinOriginalMaterials;

	UPROPERTY()
	TArray<UMaterialInterface*> WorkSurfaceOriginalMaterials;

	// ============================================================================
	// Functions
	// ============================================================================

	/** Initialize from data asset */
	UFUNCTION(BlueprintCallable, Category = "Fabrication")
	void InitializeFromData(UFabricationData* Data);

	/** Set the fabrication state */
	UFUNCTION(BlueprintCallable, Category = "Fabrication")
	void SetFabricationState(EFabricationState NewState);

	/** Check if this fabrication station can be used */
	UFUNCTION(BlueprintPure, Category = "Fabrication")
	bool CanBeUsed() const;

	/** Start using this fabrication station */
	UFUNCTION(BlueprintCallable, Category = "Fabrication")
	virtual void StartUsing(AActor* User);

	/** Stop using this fabrication station */
	UFUNCTION(BlueprintCallable, Category = "Fabrication")
	virtual void StopUsing();

	/** Open the crafting UI for this station */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Fabrication")
	void OpenCraftingUI(AActor* User);

	/** Close the crafting UI for this station */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Fabrication")
	void CloseCraftingUI();

	// ============================================================================
	// Interactive Crafting Mode
	// ============================================================================

	/** Enter interactive crafting mode (blend camera, show cursor) */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void EnterCraftingMode(APlayerController* PlayerController);

	/** Exit interactive crafting mode (restore camera, hide cursor) */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void ExitCraftingMode();

	/** Called when player clicks on toolbox zone */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Fabrication|Interactive Mode")
	void OnToolboxClicked();
	virtual void OnToolboxClicked_Implementation();

	/** Called when player clicks on material bin zone */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Fabrication|Interactive Mode")
	void OnMaterialBinClicked();
	virtual void OnMaterialBinClicked_Implementation();

	/** Called when player clicks on work surface zone */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Fabrication|Interactive Mode")
	void OnWorkSurfaceClicked();
	virtual void OnWorkSurfaceClicked_Implementation();

	/** Check which zone was clicked based on hit result */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void HandleZoneClick(UPrimitiveComponent* ClickedComponent);

	/** Highlight a zone (for visual feedback) */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void HighlightZone(UPrimitiveComponent* Zone);

	/** Remove highlight from a zone */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void UnhighlightZone(UPrimitiveComponent* Zone);

	/** Flash a zone briefly to show it was clicked */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void FlashZone(UPrimitiveComponent* Zone);

	/** Debug: Draw zone bounds to visualize clickable areas */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void DebugDrawZones();

private:
	/** Internal click handlers bound to component OnClicked events */
	UFUNCTION()
	void OnToolboxZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnMaterialBinZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnWorkSurfaceZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

public:
	// ============================================================================
	// IInteractableInterface Implementation
	// ============================================================================

	virtual void OnLookAt_Implementation(AActor* LookingActor) override;
	virtual void OnLookAway_Implementation(AActor* LookingActor) override;
	virtual void OnInteract_Implementation(AActor* InteractingActor) override;
	virtual FText GetInteractionName_Implementation() const override;
	virtual FText GetInteractionPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(AActor* InteractingActor) const override;
	virtual float GetInteractionRange_Implementation() const override;
	virtual bool ShouldHighlight_Implementation() const override;
};
