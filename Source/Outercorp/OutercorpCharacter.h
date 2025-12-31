// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InventoryItemData.h"
#include "OutercorpCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UUserWidget;
class UInventoryComponent;
class UInventoryWidget;
class UCharacterWidget;
class UOutercorpSaveGame;
class UWindow;
class UInteractionPromptWidget;
class UInteractionManagerComponent;
class UNotificationComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AOutercorpCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent *FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FirstPersonCameraComponent;

	/** Inventory component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent *InventoryComponent;

	/** Interaction manager component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInteractionManagerComponent *InteractionManagerComponent;

	/** Notification component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UNotificationComponent *NotificationComponent;

	/** Crosshair widget */
	UPROPERTY()
	UUserWidget *CrosshairWidget;

	/** Base HUD widget (contains canvas for modular windows) */
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UUserWidget *BaseHUDWidget;

	/** Interaction prompt widget */
	UPROPERTY()
	UInteractionPromptWidget *InteractionPromptWidget;

protected:
	/** Inventory widget (the actual content widget) */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UInventoryWidget *InventoryWidget;

	/** Modular window that wraps the inventory widget */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget *InventoryWindow;

	/** Character widget (the actual content widget) */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UCharacterWidget *CharacterWidget;

	/** Modular window that wraps the character widget */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget *CharacterWindow;

	/** Item info widget (the actual content widget) */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget *ItemInfoWidget;

	/** Modular window that wraps the item info widget */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget *ItemInfoWindow;

	/** Currently displayed item in the item info window */
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	FInventoryItem CurrentDisplayedItem;

	/** Array tracking all managed windows for Z-order management */
	UPROPERTY()
	TArray<UUserWidget*> ManagedWindows;

	/** Current base Z-order value for windows */
	int32 CurrentWindowZOrder = 100;

	/** Crosshair widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	/** Base HUD widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BaseHUDWidgetClass;

	/** Inventory widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	/** Character widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCharacterWidget> CharacterWidgetClass;

	/** Item info widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemInfoWidgetClass;

	/** Modular window class for wrapping widgets */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ModularWindowClass;

	/** Interaction prompt widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInteractionPromptWidget> InteractionPromptWidgetClass;

	/** Inventory window capabilities */
	UPROPERTY(EditAnywhere, Category = "UI|Window Capabilities")
	bool bInventoryCanMove = true;
	UPROPERTY(EditAnywhere, Category = "UI|Window Capabilities")
	bool bInventoryCanResize = true;
	UPROPERTY(EditAnywhere, Category = "UI|Window Capabilities")
	bool bInventoryCanFullscreen = true;

	/** Character window capabilities */
	UPROPERTY(EditAnywhere, Category = "UI|Window Capabilities")
	bool bCharacterCanMove = true;
	UPROPERTY(EditAnywhere, Category = "UI|Window Capabilities")
	bool bCharacterCanResize = true;
	UPROPERTY(EditAnywhere, Category = "UI|Window Capabilities")
	bool bCharacterCanFullscreen = true;

	/** Item info window capabilities */
	UPROPERTY(EditAnywhere, Category = "UI|Window Capabilities")
	bool bItemInfoCanMove = true;
	UPROPERTY(EditAnywhere, Category = "UI|Window Capabilities")
	bool bItemInfoCanResize = false;
	UPROPERTY(EditAnywhere, Category = "UI|Window Capabilities")
	bool bItemInfoCanFullscreen = false;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction *JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction *MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *MouseLookAction;

	/** Inventory Toggle Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *InventoryAction;

	/** Character Window Toggle Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *CharacterAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *InteractAction;

	/** Construction Mode Toggle Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *ConstructionModeAction;

	/** Construction Place Input Action (left click) */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *ConstructionPlaceAction;

	/** Time player must hold interact key before picking up item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractHoldDelay = 0.5f;

	/** Maximum distance to place items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MaxPlacementDistance = 300.0f;

	/** Material for valid placement ghost (assign in Blueprint - should be blue/green) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UMaterialInterface* ValidPlacementMaterial;

	/** Material for invalid placement ghost (assign in Blueprint - should be red) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	class UMaterialInterface* InvalidPlacementMaterial;

	/** Test construction part class for placement (temporary - for testing) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction")
	TSubclassOf<class AConstructionPart> TestConstructionPartClass;

protected:
	/** Ghost/preview mesh for placement */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	class UStaticMeshComponent* PlacementGhost;

	/** Dynamic material instance for the ghost */
	UPROPERTY()
	class UMaterialInstanceDynamic* GhostMaterialInstance;

	/** Current valid placement location */
	FVector PlacementLocation;

	/** Whether current placement location is valid */
	bool bHasValidPlacement;

	/** Construction mode state */
	UPROPERTY(BlueprintReadOnly, Category = "Construction")
	bool bIsInConstructionMode;

	/** Current construction part ghost (for socket-based placement) */
	UPROPERTY()
	class AConstructionPart* ConstructionGhostPart;

	/** Target socket for snapping */
	FName TargetSocketName;

	/** Target construction part to snap to */
	UPROPERTY()
	class AConstructionPart* TargetConstructionPart;

	/** Socket on the ghost part we're using for attachment */
	FName GhostSocketName;

	/** Stored item data from picked up item (for spawning new item) */
	UPROPERTY()
	class UInventoryItemData* HeldItemData;

	/** Stored quantity from picked up item */
	int32 HeldItemQuantity;

	/** Stored scale from original picked up item */
	FVector HeldItemScale;

	/** Stored location where item was originally picked up (for restore on invalid placement) */
	FVector OriginalItemLocation;

	/** Whether interact key is currently being held */
	bool bIsHoldingInteract;

	/** Timer handle for interact hold */
	FTimerHandle InteractHoldTimerHandle;

public:
	AOutercorpCharacter();

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called when the actor is being destroyed */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

protected:
	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue &Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue &Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	/** Interact with the currently looked at object */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact();

	/** Called when interact key is pressed */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void InteractPressed();

	/** Called when interact key is released */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void InteractReleased();

	/** Pick up and hold the current interactable item */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PickupAndHoldItem();

	/** Drop the currently held item */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void DropHeldItem();

	/** Update the placement preview ghost */
	void UpdatePlacementPreview();

	/** Toggle construction mode */
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void ToggleConstructionMode();

	/** Enter construction mode */
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void EnterConstructionMode();

	/** Exit construction mode */
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void ExitConstructionMode();

	/** Update construction part preview with socket snapping */
	void UpdateConstructionPreview();

	/** Place the construction part */
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void PlaceConstructionPart();

	/** Fasten the construction part (called on second interact) */
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void FastenConstructionPart();

	/** Toggle inventory display */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void ToggleInventory();

	/** Open inventory (BlueprintNativeEvent allows Blueprint override) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void OpenInventory();

	/** Called after inventory widget is created to bind events */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BindInventoryEvents();

	/** Get the HUD canvas panel for adding draggable windows */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	class UCanvasPanel *GetHUDCanvas() const;

	/** Close inventory (to be called from Blueprint or C++) */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventory();

	/** Called when inventory widget is closed */
	UFUNCTION()
	void OnInventoryWidgetClosed();

	/** Toggle character window display */
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void ToggleCharacter();

	/** Open character window (BlueprintNativeEvent allows Blueprint override) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OpenCharacter();

	/** Called after character widget is created to bind events */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void BindCharacterEvents();

	/** Setup character widget in modular window - call this after Create Modular Window */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetupCharacterWidgetInWindow(UUserWidget *ModularWindow);

	/** Setup inventory widget in modular window - call this after Create Modular Window */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetupInventoryWidgetInWindow(UUserWidget *ModularWindow);

	/** Setup item info widget in modular window - call this after Create Modular Window */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupItemInfoWidgetInWindow(UUserWidget *ModularWindow);

	/** Open item info window with item data */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OpenItemInfo(const FInventoryItem& Item);

	/** Update item info widget with current item data - implement this in Blueprint */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateItemInfoDisplay();

	/** Close item info window */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseItemInfo();

	/** Called when item info widget is closed */
	UFUNCTION()
	void OnItemInfoWidgetClosed();

	/** Helper to get child widget from modular window */
	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget *GetModularWindowChild(UUserWidget *ModularWindow) const;

	/** Debug helper to print widget type */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void DebugPrintWidgetType(UUserWidget *Widget) const;

	/** Close character window (to be called from Blueprint or C++) */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void CloseCharacter();

	/** Called when character widget is closed */
	UFUNCTION()
	void OnCharacterWidgetClosed();

	/** Close all open widgets */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseAllWidgets();

	/** Check if any UI widget is currently open */
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsAnyUIWidgetOpen() const;

	/** Save UI layout to disk */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SaveUILayout();

	/** Load UI layout from disk */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void LoadUILayout();

	/** Reset UI layout to defaults and delete save file */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ResetUILayout();

	/** Called when a window is moved or resized */
	UFUNCTION()
	void OnWindowLayoutChanged();

	/** Called when a window is clicked */
	UFUNCTION()
	void OnWindowClicked(UWindow* ClickedWindow);

	/** Called when inventory window interaction starts */
	UFUNCTION()
	void OnInventoryWindowInteractStart();

	/** Called when character window interaction starts */
	UFUNCTION()
	void OnCharacterWindowInteractStart();

	/** Called when item info window interaction starts */
	UFUNCTION()
	void OnItemInfoWindowInteractStart();

	/** Bring a window to the front of all other windows */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void BringWindowToFront(UUserWidget* Window);

	/** Register a window for Z-order management */
	void RegisterWindow(UUserWidget* Window);

	/** Check if a window is currently maximized/fullscreened */
	bool IsWindowMaximized(UUserWidget* Window) const;

	/** Un-maximize a window if it's currently maximized */
	void UnMaximizeWindow(UUserWidget* Window);

protected:
	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent *InputComponent) override;

public:
	/** Returns the first person mesh **/
	USkeletalMeshComponent *GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent *GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Returns inventory component **/
	UInventoryComponent *GetInventoryComponent() const { return InventoryComponent; }

	/** Returns notification component **/
	UNotificationComponent *GetNotificationComponent() const { return NotificationComponent; }

	/** Setup notification canvas to use the HUD canvas */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupNotificationCanvas();
};
