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
	// Dynamic Zone System
	// ============================================================================

	/**
	 * Discovered zones at runtime
	 * Key = Zone type (e.g., "Toolbox", "MaterialBin")
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Zones")
	TMap<FName, class UBoxComponent*> DiscoveredZones;

	/**
	 * Meshes associated with each zone
	 * Key = Zone type (e.g., "Toolbox", "MaterialBin")
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Zones")
	TMap<FName, class UStaticMeshComponent*> ZoneMeshes;

	/**
	 * Inventory components for each zone (dynamically created)
	 * Key = Zone type (e.g., "Toolbox", "MaterialBin")
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Zones")
	TMap<FName, class UInventoryComponent*> ZoneInventories;

	/**
	 * Currently installed module for each zone
	 * Key = Zone type, Value = Module ID
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Crafting Zones")
	TMap<FName, FName> InstalledModules;

	// ============================================================================
	// Data Asset
	// ============================================================================

	/** Fabrication data asset defining gameplay properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fabrication")
	class UFabricationData* FabricationData;

	// ============================================================================
	// Container Widgets
	// ============================================================================

	/** Base window class (WBP_Base_Window) - used for all zone container windows */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting Zones|Widgets")
	TSubclassOf<class UUserWidget> BaseWindowClass;

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

	/** Currently hovered mesh component */
	UPROPERTY()
	UStaticMeshComponent* CurrentlyHoveredMesh;

	/** Cache for storing original materials during highlighting (non-UPROPERTY, transient runtime data) */
	TMap<UStaticMeshComponent*, TArray<UMaterialInterface*>> OriginalMaterialsCache;

	// DEPRECATED: Legacy cached pointers for backward compatibility
	// These are populated from DiscoveredZones at runtime to support old code
	UPROPERTY(Transient)
	class UBoxComponent* ToolboxZone;

	UPROPERTY(Transient)
	class UBoxComponent* MaterialBinZone;

	UPROPERTY(Transient)
	class UBoxComponent* WorkSurfaceZone;

	UPROPERTY(Transient)
	class UStaticMeshComponent* ToolboxMesh;

	UPROPERTY(Transient)
	class UStaticMeshComponent* MaterialBinMesh;

	UPROPERTY(Transient)
	class UInventoryComponent* ToolboxInventory;

	UPROPERTY(Transient)
	class UInventoryComponent* MaterialBinInventory;

	/** Currently open toolbox window instance */
	UPROPERTY()
	class UUserWidget* CurrentToolboxWindow;

	/** Currently open material bin window instance */
	UPROPERTY()
	class UUserWidget* CurrentMaterialBinWindow;

	/** Currently open work surface window instance */
	UPROPERTY()
	class UUserWidget* CurrentWorkSurfaceWindow;

	/** Saved window positions for each zone type */
	UPROPERTY()
	TMap<FName, FVector2D> SavedWindowPositions;

	/** Saved window sizes for each zone type */
	UPROPERTY()
	TMap<FName, FVector2D> SavedWindowSizes;

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

	/** Create toolbox container content widget */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	class UContainerWidget* CreateToolboxContent();

	/** Create material bin container content widget */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	class UContainerWidget* CreateMaterialBinContent();

	/** Create work surface content widget */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	class UUserWidget* CreateWorkSurfaceContent();

	/** Close the toolbox window */
	UFUNCTION()
	void CloseToolboxWindow();

	/** Close the material bin window */
	UFUNCTION()
	void CloseMaterialBinWindow();

	/** Close the work surface window */
	UFUNCTION()
	void CloseWorkSurfaceWindow();

	/** Get work surface configuration for the currently installed module */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Work Surface")
	bool GetWorkSurfaceConfig(FWorkSurfaceConfig& OutConfig) const;

	/** Check if a recipe can be crafted on the current work surface (based on size constraints) */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Work Surface")
	bool CanCraftRecipe(const class UCraftingRecipeData* Recipe) const;

	/** Get station type for recipe validation */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fabrication")
	EStationType GetStationType() const;

	/** Get crafting speed multiplier */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fabrication")
	float GetCraftingSpeedMultiplier() const;

	/** Get player inventory (current user's inventory) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fabrication")
	class UInventoryComponent* GetPlayerInventory() const;

	/** Get toolbox inventory */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fabrication")
	class UInventoryComponent* GetToolboxInventory() const { return ToolboxInventory; }

	/** Get material bin inventory */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fabrication")
	class UInventoryComponent* GetMaterialBinInventory() const { return MaterialBinInventory; }

	/** Get station data asset */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fabrication")
	class UFabricationData* GetStationData() const { return FabricationData; }

	/** Add test items to toolbox for debugging */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Debug")
	void AddTestItemsToToolbox();

	// ============================================================================
	// Auto-Discovery System
	// ============================================================================

	/** Discover and configure all zones from Blueprint components */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Zone System")
	void DiscoverZones();

	/** Extract zone type from component name (e.g., "ToolboxZone" -> "Toolbox") */
	FName ExtractZoneType(const FString& ComponentName) const;

	/** Find mesh component associated with a zone by proximity or naming */
	UStaticMeshComponent* FindAssociatedMesh(UBoxComponent* ZoneComponent, FName ZoneType) const;

	/** Install a module in a zone */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Zone System")
	void InstallModule(FName ZoneType, FName ModuleID);

	/** Get zone type config from data asset */
	const struct FFabricationZoneTypeConfig* GetZoneTypeConfig(FName ZoneType) const;

	/** Setup zone events (hover, click) */
	void SetupZoneEvents(UBoxComponent* ZoneComponent, FName ZoneType);

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

	/** Highlight a static mesh component (for hover feedback) */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void HighlightMesh(UStaticMeshComponent* Mesh);

	/** Remove highlight from a static mesh component */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void UnhighlightMesh(UStaticMeshComponent* Mesh);

	/** Check for mesh hover using cursor ray trace (called on tick when in crafting mode) */
	UFUNCTION(BlueprintCallable, Category = "Fabrication|Interactive Mode")
	void CheckMeshHover();

private:
	/** Generic zone event handlers (used by auto-discovery system) */
	UFUNCTION()
	void OnZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnZoneBeginHover(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnZoneEndHover(UPrimitiveComponent* TouchedComponent);

	/** DEPRECATED: Legacy zone-specific handlers for backward compatibility */
	UFUNCTION()
	void OnToolboxZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnMaterialBinZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnWorkSurfaceZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnToolboxZoneBeginHover(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnToolboxZoneEndHover(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnMaterialBinZoneBeginHover(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnMaterialBinZoneEndHover(UPrimitiveComponent* TouchedComponent);

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
