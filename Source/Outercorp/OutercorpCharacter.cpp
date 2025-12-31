// Copyright Epic Games, Inc. All Rights Reserved.

#include "OutercorpCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "CharacterWidget.h"
#include "ItemInfoWidget.h"
#include "InteractionPromptWidget.h"
#include "InteractionManagerComponent.h"
#include "NotificationComponent.h"
#include "PickupableItem.h"
#include "Outercorp.h"
#include "Window.h"
#include "OutercorpSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Module_Fullscreen_None.h"
#include "Module_Fullscreen_Point.h"
#include "Module_Fullscreen_Line.h"
#include "Window_Module.h"
#include "ConstructionPart.h"
#include "ConstructionPartData.h"
#include "Engine/OverlapResult.h"

AOutercorpCharacter::AOutercorpCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Create inventory component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	// Create interaction manager component
	InteractionManagerComponent = CreateDefaultSubobject<UInteractionManagerComponent>(TEXT("Interaction Manager"));

	// Create notification component
	NotificationComponent = CreateDefaultSubobject<UNotificationComponent>(TEXT("Notification Component"));

	// Create placement ghost component
	PlacementGhost = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placement Ghost"));
	PlacementGhost->SetupAttachment(RootComponent);
	PlacementGhost->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlacementGhost->SetVisibility(false);
	PlacementGhost->SetCastShadow(false);

	// Initialize interaction hold variables
	HeldItemData = nullptr;
	HeldItemQuantity = 0;
	HeldItemScale = FVector::OneVector;
	OriginalItemLocation = FVector::ZeroVector;
	bIsHoldingInteract = false;
	bHasValidPlacement = false;
	bPreviousValidPlacement = false;
	ValidPlacementMaterial = nullptr;
	InvalidPlacementMaterial = nullptr;
	GhostMaterialInstance = nullptr;

	// Initialize construction mode variables
	bIsInConstructionMode = false;
	bSnapModeEnabled = true; // Default to snapping enabled
	CurrentPlacementDistance = 300.0f; // Start at max placement distance
	bIsInDeleteMode = false;
	HighlightedPartForDeletion = nullptr;
	DeleteHighlightMaterial = nullptr;
	ConstructionGhostPart = nullptr;
	TargetConstructionPart = nullptr;
	TargetSocketName = NAME_None;
	GhostSocketName = NAME_None;
}

void AOutercorpCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update delete mode highlighting if in delete mode
	if (bIsInDeleteMode && FirstPersonCameraComponent)
	{
		UpdateDeleteModeHighlight();
	}
	// Update construction preview if in construction mode
	else if (bIsInConstructionMode && ConstructionGhostPart && FirstPersonCameraComponent)
	{
		UpdateConstructionPreview();
	}
	// Update placement preview if holding an item (checking HeldItemData instead of HeldItem)
	else if (HeldItemData && FirstPersonCameraComponent)
	{
		UpdatePlacementPreview();
	}
}

void AOutercorpCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		// Create and display the base HUD widget first
		if (BaseHUDWidgetClass)
		{
			BaseHUDWidget = CreateWidget<UUserWidget>(GetWorld(), BaseHUDWidgetClass);
			if (BaseHUDWidget)
			{
				BaseHUDWidget->AddToViewport(0); // Base layer

				// Get the WindowCanvas to add our modular windows to
				UCanvasPanel *WindowCanvas = GetHUDCanvas();
				if (WindowCanvas && ModularWindowClass)
				{
					// Create the inventory window and add it to the canvas
					InventoryWindow = CreateWidget<UUserWidget>(GetWorld(), ModularWindowClass);
					if (InventoryWindow)
					{
						UCanvasPanelSlot *Slot = WindowCanvas->AddChildToCanvas(InventoryWindow);
						if (Slot)
						{
							// Set position and size for the window
							Slot->SetPosition(FVector2D(100, 100));
							Slot->SetSize(FVector2D(600, 400));
							Slot->SetAnchors(FAnchors(0, 0, 0, 0));
						}

						// Call Init() on the window to initialize the modular window system
						UWindow *Window = Cast<UWindow>(InventoryWindow);
						if (Window)
						{
							Window->Init();

							// Set window capabilities from Blueprint-configurable properties
							Window->SetWindowCapabilities(bInventoryCanMove, bInventoryCanResize, bInventoryCanFullscreen);

							// Bind to position and size end events for auto-save
							Window->ED_PositionEnd.AddDynamic(this, &AOutercorpCharacter::OnWindowLayoutChanged);
							Window->ED_SizeEnd.AddDynamic(this, &AOutercorpCharacter::OnWindowLayoutChanged);

							// Bind to interact start event to bring window to front when dragging starts
							Window->ED_InteractStart.AddDynamic(this, &AOutercorpCharacter::OnInventoryWindowInteractStart);
						}

						// Register window for Z-order management
						RegisterWindow(InventoryWindow);

						// Pre-create the inventory widget to avoid first-open delay
						SetupInventoryWidgetInWindow(InventoryWindow);

						InventoryWindow->SetVisibility(ESlateVisibility::Hidden);
					}

					// Create the character window and add it to the canvas
					CharacterWindow = CreateWidget<UUserWidget>(GetWorld(), ModularWindowClass);
					if (CharacterWindow)
					{
						UCanvasPanelSlot *Slot = WindowCanvas->AddChildToCanvas(CharacterWindow);
						if (Slot)
						{
							// Set position and size for the window
							Slot->SetPosition(FVector2D(750, 100));
							Slot->SetSize(FVector2D(400, 500));
							Slot->SetAnchors(FAnchors(0, 0, 0, 0));
						}

						// Call Init() on the window to initialize the modular window system
						UWindow *Window = Cast<UWindow>(CharacterWindow);
						if (Window)
						{
							Window->Init();

							// Set window capabilities from Blueprint-configurable properties
							Window->SetWindowCapabilities(bCharacterCanMove, bCharacterCanResize, bCharacterCanFullscreen);

							// Bind to position and size end events for auto-save
							Window->ED_PositionEnd.AddDynamic(this, &AOutercorpCharacter::OnWindowLayoutChanged);
							Window->ED_SizeEnd.AddDynamic(this, &AOutercorpCharacter::OnWindowLayoutChanged);

							// Bind to interact start event to bring window to front when dragging starts
							Window->ED_InteractStart.AddDynamic(this, &AOutercorpCharacter::OnCharacterWindowInteractStart);
						}

						// Register window for Z-order management
						RegisterWindow(CharacterWindow);

						CharacterWindow->SetVisibility(ESlateVisibility::Hidden);
					}

					// Create the item info window and add it to the canvas
					ItemInfoWindow = CreateWidget<UUserWidget>(GetWorld(), ModularWindowClass);
					if (ItemInfoWindow)
					{
						UCanvasPanelSlot *Slot = WindowCanvas->AddChildToCanvas(ItemInfoWindow);
						if (Slot)
						{
							// Set position and size for the window
							Slot->SetPosition(FVector2D(400, 200));
							Slot->SetSize(FVector2D(600, 500));
							Slot->SetAnchors(FAnchors(0, 0, 0, 0));
						}

						// Call Init() on the window to initialize the modular window system
						UWindow *Window = Cast<UWindow>(ItemInfoWindow);
						if (Window)
						{
							Window->Init();

							// Set window capabilities from Blueprint-configurable properties
							Window->SetWindowCapabilities(bItemInfoCanMove, bItemInfoCanResize, bItemInfoCanFullscreen);

							// Bind to position and size end events for auto-save
							Window->ED_PositionEnd.AddDynamic(this, &AOutercorpCharacter::OnWindowLayoutChanged);
							Window->ED_SizeEnd.AddDynamic(this, &AOutercorpCharacter::OnWindowLayoutChanged);

							// Bind to interact start event to bring window to front when dragging starts
							Window->ED_InteractStart.AddDynamic(this, &AOutercorpCharacter::OnItemInfoWindowInteractStart);
						}

						// Register window for Z-order management
						RegisterWindow(ItemInfoWindow);

						ItemInfoWindow->SetVisibility(ESlateVisibility::Hidden);
					}

					// Load saved UI layout after all windows are created
					LoadUILayout();

					// Setup notification canvas
					SetupNotificationCanvas();
				}
				else
				{
					if (!WindowCanvas)
					{					}
					if (!ModularWindowClass)
					{					}
				}
			}
		}

		// Create and display the crosshair widget
		if (CrosshairWidgetClass)
		{
			CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
			if (CrosshairWidget)
			{
				CrosshairWidget->AddToViewport(1); // Above HUD
			}
		}

		// Create and display the interaction prompt widget
		if (InteractionPromptWidgetClass)
		{			InteractionPromptWidget = CreateWidget<UInteractionPromptWidget>(GetWorld(), InteractionPromptWidgetClass);
			if (InteractionPromptWidget)
			{				InteractionPromptWidget->AddToViewport(2); // Above crosshair

				// Manually initialize the interaction system
				InteractionPromptWidget->InitializeInteraction();
			}
			else
			{			}
		}
		else
		{		}

		// Set initial input mode to game-only (no UI, no cursor)
		APlayerController *PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
		}

		// Ensure we start with construction mode and delete mode both OFF
		bIsInConstructionMode = false;
		bIsInDeleteMode = false;
		ConstructionGhostPart = nullptr;
		HighlightedPartForDeletion = nullptr;
	}
}

void AOutercorpCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// FORCE windows back to default sizes before saving
	// This prevents saving maximized state
	if (InventoryWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(InventoryWindow->Slot);
		if (Slot)
		{
			FVector2D CurrentSize = Slot->GetSize();
			// If window is larger than reasonable threshold, reset to default
			if (CurrentSize.X > 1500.0f || CurrentSize.Y > 1500.0f)
			{
				Slot->SetSize(FVector2D(600, 400));
				Slot->SetPosition(FVector2D(100, 100));
			}
		}
	}

	if (CharacterWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(CharacterWindow->Slot);
		if (Slot)
		{
			FVector2D CurrentSize = Slot->GetSize();
			if (CurrentSize.X > 1500.0f || CurrentSize.Y > 1500.0f)
			{
				Slot->SetSize(FVector2D(400, 500));
				Slot->SetPosition(FVector2D(750, 100));
			}
		}
	}

	if (ItemInfoWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(ItemInfoWindow->Slot);
		if (Slot)
		{
			FVector2D CurrentSize = Slot->GetSize();
			if (CurrentSize.X > 1500.0f || CurrentSize.Y > 1500.0f)
			{
				Slot->SetSize(FVector2D(350, 450));
				Slot->SetPosition(FVector2D(400, 200));
			}
		}
	}

	// Save the layout with all windows at normal/default sizes
	SaveUILayout();
}

void AOutercorpCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AOutercorpCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AOutercorpCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOutercorpCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOutercorpCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AOutercorpCharacter::LookInput);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AOutercorpCharacter::ToggleInventory);

		// Character
		EnhancedInputComponent->BindAction(CharacterAction, ETriggerEvent::Started, this, &AOutercorpCharacter::ToggleCharacter);

		// Construction Mode
		EnhancedInputComponent->BindAction(ConstructionModeAction, ETriggerEvent::Started, this, &AOutercorpCharacter::ToggleConstructionMode);

		// Construction Place (left click)
		EnhancedInputComponent->BindAction(ConstructionPlaceAction, ETriggerEvent::Started, this, &AOutercorpCharacter::PlaceConstructionPart);

		// Toggle Snap Mode
		EnhancedInputComponent->BindAction(ToggleSnapModeAction, ETriggerEvent::Started, this, &AOutercorpCharacter::ToggleSnapMode);

		// Adjust Placement Distance (mousewheel)
		if (AdjustPlacementDistanceAction)
		{
			EnhancedInputComponent->BindAction(AdjustPlacementDistanceAction, ETriggerEvent::Triggered, this, &AOutercorpCharacter::AdjustPlacementDistance);
		}

		// Delete Mode
		if (DeleteModeAction)
		{
			EnhancedInputComponent->BindAction(DeleteModeAction, ETriggerEvent::Started, this, &AOutercorpCharacter::ToggleDeleteMode);
		}

		// Delete Item (left click in delete mode)
		if (DeleteItemAction)
		{
			EnhancedInputComponent->BindAction(DeleteItemAction, ETriggerEvent::Started, this, &AOutercorpCharacter::DeleteHighlightedPart);
		}

		// Interact - bind both press and release
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AOutercorpCharacter::InteractPressed);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AOutercorpCharacter::InteractReleased);
	}
	else
	{	}
}

void AOutercorpCharacter::MoveInput(const FInputActionValue &Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AOutercorpCharacter::LookInput(const FInputActionValue &Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void AOutercorpCharacter::DoAim(float Yaw, float Pitch)
{
	// Don't process look input if any UI widget is open
	if (IsAnyUIWidgetOpen())
	{
		return;
	}

	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AOutercorpCharacter::DoMove(float Right, float Forward)
{
	// Don't process movement input if any UI widget is open
	if (IsAnyUIWidgetOpen())
	{
		return;
	}

	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AOutercorpCharacter::DoJumpStart()
{
	// Don't process jump input if any UI widget is open
	if (IsAnyUIWidgetOpen())
	{
		return;
	}

	// pass Jump to the character
	Jump();
}

void AOutercorpCharacter::DoJumpEnd()
{
	// Don't process jump input if any UI widget is open
	if (IsAnyUIWidgetOpen())
	{
		return;
	}

	// pass StopJumping to the character
	StopJumping();
}

void AOutercorpCharacter::Interact()
{
	// Don't process interact input if any UI widget is open
	if (IsAnyUIWidgetOpen())
	{
		return;
	}

	// Call interact on the interaction manager component
	if (InteractionManagerComponent)
	{
		InteractionManagerComponent->Interact();
	}
}

void AOutercorpCharacter::InteractPressed()
{
	// Don't process interact input if any UI widget is open
	if (IsAnyUIWidgetOpen())
	{
		return;
	}

	// Don't allow interact while in construction mode (use left click instead)
	if (bIsInConstructionMode)
	{
		return;
	}

	// If already holding an item, place it
	if (HeldItemData)
	{		DropHeldItem();
		return;
	}
	// Set the flag that we're holding interact
	bIsHoldingInteract = true;

	// Start a timer to check when we've held long enough to pick up the item
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			InteractHoldTimerHandle,
			this,
			&AOutercorpCharacter::PickupAndHoldItem,
			InteractHoldDelay,
			false
		);
	}
}

void AOutercorpCharacter::InteractReleased()
{
	// If we were holding the interact key, handle the release
	if (bIsHoldingInteract)
	{
		bIsHoldingInteract = false;
		// If no held item and timer hasn't fired, do instant interact
		if (!HeldItemData)
		{
			// Timer hasn't fired yet (quick tap), do instant interact			if (GetWorld())
			{
				GetWorld()->GetTimerManager().ClearTimer(InteractHoldTimerHandle);
			}
			Interact();
		}
		// If we have held item data, just keep holding it (don't drop)
		// Next interact press will place it
	}
}

void AOutercorpCharacter::PickupAndHoldItem()
{
	// Make sure we're still holding the interact key
	if (!bIsHoldingInteract)
	{
		return;
	}

	// Don't process if UI is open
	if (IsAnyUIWidgetOpen())
	{
		return;
	}

	// Get the current interactable from the interaction manager
	if (!InteractionManagerComponent)
	{
		return;
	}

	AActor* InteractableActor = InteractionManagerComponent->GetCurrentInteractableActor();
	if (!InteractableActor)
	{
		return;
	}

	// Check if it's a pickupable item
	APickupableItem* PickupItem = Cast<APickupableItem>(InteractableActor);
	if (!PickupItem)
	{
		return;
	}

	// Check if we can interact with it
	if (!PickupItem->Execute_CanInteract(PickupItem, this))
	{
		return;
	}

	// Store the item data and quantity before destroying
	if (!PickupItem->ItemData)
	{		return;
	}

	// IMMEDIATELY disable physics, collision, and hide the item to prevent any flicker/movement
	if (PickupItem->ItemMesh)
	{
		PickupItem->ItemMesh->SetSimulatePhysics(false);
		PickupItem->ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PickupItem->ItemMesh->SetEnableGravity(false);
		PickupItem->ItemMesh->SetVisibility(false);
	}
	PickupItem->SetActorEnableCollision(false);
	PickupItem->SetActorHiddenInGame(true);

	HeldItemData = PickupItem->ItemData;
	HeldItemQuantity = PickupItem->Quantity;
	OriginalItemLocation = PickupItem->GetActorLocation();
	// Copy the item's mesh to the ghost for preview BEFORE destroying the item
	if (PickupItem->ItemMesh && PlacementGhost)
	{
		// Store the original scale
		HeldItemScale = PickupItem->ItemMesh->GetComponentScale();

		PlacementGhost->SetStaticMesh(PickupItem->ItemMesh->GetStaticMesh());
		PlacementGhost->SetWorldScale3D(HeldItemScale);

		// Position ghost at camera forward BEFORE making it visible to prevent flicker
		if (FirstPersonCameraComponent)
		{
			FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
			FVector CameraForward = FirstPersonCameraComponent->GetForwardVector();
			FVector InitialGhostPosition = CameraLocation + (CameraForward * 150.0f); // Middle distance
			PlacementGhost->SetWorldLocation(InitialGhostPosition);
			PlacementGhost->SetWorldRotation(FRotator::ZeroRotator);
		}

		// Apply the valid placement material
		if (ValidPlacementMaterial)
		{
			int32 NumMaterials = PlacementGhost->GetNumMaterials();
			for (int32 i = 0; i < NumMaterials; i++)
			{
				PlacementGhost->SetMaterial(i, ValidPlacementMaterial);
			}		}
		else
		{		}

		// NOW make it visible after positioning
		PlacementGhost->SetVisibility(true);	}

	// DESTROY the original item (already hidden, so no flicker)
	PickupItem->Destroy();}

void AOutercorpCharacter::DropHeldItem()
{
	if (!HeldItemData)
	{		return;
	}

	if (!bHasValidPlacement)
	{
		// Spawn the item back at the original pickup location
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APickupableItem* RestoredItem = GetWorld()->SpawnActor<APickupableItem>(
			APickupableItem::StaticClass(),
			OriginalItemLocation,
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (RestoredItem)
		{
			// Initialize with the stored data
			RestoredItem->InitializeItem(HeldItemData, HeldItemQuantity);

			// Restore the original scale
			if (RestoredItem->ItemMesh)
			{
				RestoredItem->ItemMesh->SetWorldScale3D(HeldItemScale);
			}

			// Enable physics after a delay
			if (RestoredItem->ItemMesh)
			{
				RestoredItem->ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				RestoredItem->ItemMesh->SetEnableGravity(true);

				FTimerHandle PhysicsEnableTimer;
				FTimerDelegate PhysicsDelegate;
				PhysicsDelegate.BindLambda([CapturedMesh = RestoredItem->ItemMesh]()
				{
					if (CapturedMesh && CapturedMesh->IsValidLowLevel())
					{
						CapturedMesh->SetSimulatePhysics(true);
					}
				});
				GetWorld()->GetTimerManager().SetTimer(PhysicsEnableTimer, PhysicsDelegate, 0.1f, false);
			}

			RestoredItem->StartDropCooldown();		}

		// Hide the ghost
		if (PlacementGhost)
		{
			PlacementGhost->SetVisibility(false);
		}

		// Clear the held item data
		HeldItemData = nullptr;
		HeldItemQuantity = 0;
		return;
	}
	// HIDE the ghost BEFORE spawning the new item to prevent flicker
	if (PlacementGhost)
	{
		PlacementGhost->SetVisibility(false);
	}

	// Spawn a new item at the ghost location
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APickupableItem* NewItem = GetWorld()->SpawnActor<APickupableItem>(
		APickupableItem::StaticClass(),
		PlacementLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (NewItem)
	{
		// Initialize the new item with the stored data
		NewItem->InitializeItem(HeldItemData, HeldItemQuantity);

		// Apply the stored scale to match the original item
		if (NewItem->ItemMesh)
		{
			NewItem->ItemMesh->SetWorldScale3D(HeldItemScale);		}
		// Check if physics should be enabled based on item data and placement type
		bool bShouldEnablePhysics = false;
		if (HeldItemData->bCanFreePlacement)
		{
			// For free placement items, use the bEnablePhysicsOnPlacement setting
			bShouldEnablePhysics = HeldItemData->bEnablePhysicsOnPlacement;		}
		else
		{
			// For surface-placed items, always enable physics after a delay
			bShouldEnablePhysics = true;		}

		// Handle physics setup
		if (NewItem->ItemMesh)
		{
			if (bShouldEnablePhysics)
			{
				// Enable physics after a small delay to ensure position is stable
				NewItem->ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				NewItem->ItemMesh->SetEnableGravity(true);

				FTimerHandle PhysicsEnableTimer;
				FTimerDelegate PhysicsDelegate;
				PhysicsDelegate.BindLambda([CapturedMesh = NewItem->ItemMesh]()
				{
					if (CapturedMesh && CapturedMesh->IsValidLowLevel())
					{
						CapturedMesh->SetSimulatePhysics(true);					}
				});
				GetWorld()->GetTimerManager().SetTimer(PhysicsEnableTimer, PhysicsDelegate, 0.1f, false);
			}
			else
			{
				// Disable physics - item will be static in mid-air
				NewItem->ItemMesh->SetSimulatePhysics(false);
				NewItem->ItemMesh->SetEnableGravity(false);
				NewItem->ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);			}
		}

		// Start drop cooldown so player can't immediately pick it up again
		NewItem->StartDropCooldown();
	}
	else
	{	}

	// Clear the held item data (ghost already hidden above before spawning)
	HeldItemData = nullptr;
	HeldItemQuantity = 0;
	bHasValidPlacement = false;
}

void AOutercorpCharacter::UpdatePlacementPreview()
{
	if (!PlacementGhost || !FirstPersonCameraComponent || !HeldItemData)
	{
		return;
	}

	// Check if this item can be freely placed anywhere
	bool bCanFreePlacement = HeldItemData->bCanFreePlacement;

	// Perform trace from camera forward
	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector CameraForward = FirstPersonCameraComponent->GetForwardVector();
	FVector TraceEnd = CameraLocation + (CameraForward * MaxPlacementDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Trace for placement surface
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		// Valid placement location found - offset up by item bounds so it sits on surface
		FVector BottomOffset = FVector::ZeroVector;
		if (PlacementGhost && PlacementGhost->GetStaticMesh())
		{
			// Get the unscaled bounds
			FBox ItemBounds = PlacementGhost->GetStaticMesh()->GetBoundingBox();

			// Apply the scale to the bounds to get the actual scaled bounds
			FVector ScaledMin = ItemBounds.Min * HeldItemScale;
			FVector ScaledMax = ItemBounds.Max * HeldItemScale;

			// Offset by the scaled bottom of the mesh so it sits on the surface
			BottomOffset = FVector(0, 0, -ScaledMin.Z);		}

		PlacementLocation = HitResult.Location + BottomOffset;
		bHasValidPlacement = true;

		// Position ghost at hit location
		PlacementGhost->SetWorldLocation(PlacementLocation);
		PlacementGhost->SetWorldRotation(FRotator::ZeroRotator);

		// Apply valid (blue/green) material
		if (ValidPlacementMaterial)
		{
			int32 NumMaterials = PlacementGhost->GetNumMaterials();
			for (int32 i = 0; i < NumMaterials; i++)
			{
				PlacementGhost->SetMaterial(i, ValidPlacementMaterial);
			}
		}
	}
	else
	{
		// No surface hit
		if (bCanFreePlacement)
		{
			// Free placement allowed - can place anywhere within max distance
			PlacementLocation = TraceEnd;
			bHasValidPlacement = true;  // Valid because free placement is allowed

			PlacementGhost->SetWorldLocation(PlacementLocation);
			PlacementGhost->SetWorldRotation(FRotator::ZeroRotator);

			// Apply valid (blue/green) material - free placement is valid
			if (ValidPlacementMaterial)
			{
				int32 NumMaterials = PlacementGhost->GetNumMaterials();
				for (int32 i = 0; i < NumMaterials; i++)
				{
					PlacementGhost->SetMaterial(i, ValidPlacementMaterial);
				}
			}
		}
		else
		{
			// No valid placement - item requires surface and none found
			PlacementLocation = TraceEnd;
			bHasValidPlacement = false;

			PlacementGhost->SetWorldLocation(PlacementLocation);
			PlacementGhost->SetWorldRotation(FRotator::ZeroRotator);

			// Apply invalid (red) material
			if (InvalidPlacementMaterial)
			{
				int32 NumMaterials = PlacementGhost->GetNumMaterials();
				for (int32 i = 0; i < NumMaterials; i++)
				{
					PlacementGhost->SetMaterial(i, InvalidPlacementMaterial);
				}
			}
		}
	}
}

void AOutercorpCharacter::ToggleInventory()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	// Check if inventory is open by checking visibility
	if (InventoryWindow != nullptr && InventoryWindow->GetVisibility() == ESlateVisibility::Visible)
	{
		CloseInventory();
	}
	else
	{
		OpenInventory();
	}
}

void AOutercorpCharacter::OpenInventory_Implementation()
{
	// If the window already exists (was pre-created in HUD)
	if (InventoryWindow)
	{
		InventoryWindow->SetVisibility(ESlateVisibility::Visible);

		// Bring the window to front when opened
		BringWindowToFront(InventoryWindow);
	}
	else
	{	}

	APlayerController *PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		// Set input mode to Game and UI
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}

void AOutercorpCharacter::BindInventoryEvents()
{
	if (!InventoryWidget)
	{		return;
	}

	if (!InventoryComponent)
	{		return;
	}

	// Bind to close event
	InventoryWidget->OnInventoryClosed.AddDynamic(this, &AOutercorpCharacter::CloseInventory);

	// Initialize the inventory widget
	InventoryWidget->InitializeInventory(InventoryComponent);
}

UCanvasPanel *AOutercorpCharacter::GetHUDCanvas() const
{
	if (!BaseHUDWidget)
	{		return nullptr;
	}

	// Try to get the canvas panel named "WindowCanvas" from the HUD widget
	UCanvasPanel *Canvas = Cast<UCanvasPanel>(BaseHUDWidget->GetWidgetFromName(FName("WindowCanvas")));

	if (!Canvas)
	{	}

	return Canvas;
}

void AOutercorpCharacter::CloseInventory()
{
	// Hide the modular window instead of removing it (to keep it in canvas hierarchy)
	if (InventoryWindow)
	{
		InventoryWindow->SetVisibility(ESlateVisibility::Hidden);
	}

	// Don't null out references - we'll reuse them
	// The window and widget stay intact for next time

	// Call the closed callback
	OnInventoryWidgetClosed();
}

void AOutercorpCharacter::OnInventoryWidgetClosed()
{
	// Only restore game input if no other UI widgets are open
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		// Check if character window is still open (by visibility)
		bool bAnyWidgetOpen = (CharacterWindow != nullptr && CharacterWindow->GetVisibility() == ESlateVisibility::Visible);

		if (bAnyWidgetOpen)
		{
			// Keep UI mode - don't lock focus to specific widget
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}
		else
		{
			// No widgets open, restore game input
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
		}
	}
}

void AOutercorpCharacter::ToggleCharacter()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	// Check if character window is open by checking visibility
	if (CharacterWindow != nullptr && CharacterWindow->GetVisibility() == ESlateVisibility::Visible)
	{
		CloseCharacter();
	}
	else
	{
		OpenCharacter();
	}
}

void AOutercorpCharacter::OpenCharacter_Implementation()
{
	// If the window already exists (was pre-created in HUD)
	if (CharacterWindow)
	{
		// If the character widget hasn't been created yet, create it now
		if (!CharacterWidget)
		{
			SetupCharacterWidgetInWindow(CharacterWindow);
		}

		CharacterWindow->SetVisibility(ESlateVisibility::Visible);

		// Bring the window to front when opened
		BringWindowToFront(CharacterWindow);
	}

	APlayerController *PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		// Set input mode to Game and UI
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}

void AOutercorpCharacter::BindCharacterEvents()
{
	if (!CharacterWidget)
	{		return;
	}

	// Bind to close event
	CharacterWidget->OnCharacterClosed.AddDynamic(this, &AOutercorpCharacter::CloseCharacter);
}

void AOutercorpCharacter::CloseCharacter()
{
	// Hide the modular window instead of removing it (to keep it in canvas hierarchy)
	if (CharacterWindow)
	{
		CharacterWindow->SetVisibility(ESlateVisibility::Hidden);
	}

	// Don't null out references - we'll reuse them
	// The window and widget stay intact for next time

	// Call the closed callback
	OnCharacterWidgetClosed();
}

void AOutercorpCharacter::OnCharacterWidgetClosed()
{
	// Only restore game input if no other UI widgets are open
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		// Check if inventory window is still open (by visibility)
		bool bAnyWidgetOpen = (InventoryWindow != nullptr && InventoryWindow->GetVisibility() == ESlateVisibility::Visible);

		if (bAnyWidgetOpen)
		{
			// Keep UI mode - don't lock focus to specific widget
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}
		else
		{
			// No widgets open, restore game input
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
		}
	}
}

void AOutercorpCharacter::CloseAllWidgets()
{
	// Close inventory if open
	if (InventoryWindow != nullptr)
	{
		CloseInventory();
	}

	// Close character window if open
	if (CharacterWindow != nullptr)
	{
		CloseCharacter();
	}

	// Close item info window if open
	if (ItemInfoWindow != nullptr)
	{
		CloseItemInfo();
	}

	// Restore input mode to game only
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
	}
}

bool AOutercorpCharacter::IsAnyUIWidgetOpen() const
{
	bool bInventoryOpen = (InventoryWindow != nullptr && InventoryWindow->GetVisibility() == ESlateVisibility::Visible);
	bool bCharacterOpen = (CharacterWindow != nullptr && CharacterWindow->GetVisibility() == ESlateVisibility::Visible);
	bool bItemInfoOpen = (ItemInfoWindow != nullptr && ItemInfoWindow->GetVisibility() == ESlateVisibility::Visible);
	return bInventoryOpen || bCharacterOpen || bItemInfoOpen;
}

UUserWidget *AOutercorpCharacter::GetModularWindowChild(UUserWidget *ModularWindow) const
{
	if (!ModularWindow)
	{
		return nullptr;
	}

	// First, try getting the root widget and traversing from there
	UPanelWidget *RootWidget = ModularWindow->GetRootWidget() ? Cast<UPanelWidget>(ModularWindow->GetRootWidget()) : nullptr;

	if (RootWidget)
	{
		// Iterate through all children of root widget
		for (int32 i = 0; i < RootWidget->GetChildrenCount(); ++i)
		{
			UWidget *Child = RootWidget->GetChildAt(i);
			if (Child)
			{
				// Check if this child is a UUserWidget
				if (UUserWidget *UserWidgetChild = Cast<UUserWidget>(Child))
				{
					// Check if it's our target widget type
					if (UserWidgetChild->IsA(UCharacterWidget::StaticClass()) || UserWidgetChild->IsA(UInventoryWidget::StaticClass()))
					{
						return UserWidgetChild;
					}
				}

				// If it's a panel, check its children too
				if (UPanelWidget *Panel = Cast<UPanelWidget>(Child))
				{
					for (int32 j = 0; j < Panel->GetChildrenCount(); ++j)
					{
						UWidget *PanelChild = Panel->GetChildAt(j);
						if (PanelChild)
						{
							if (UUserWidget *UserWidgetChild = Cast<UUserWidget>(PanelChild))
							{
								if (UserWidgetChild->IsA(UCharacterWidget::StaticClass()) || UserWidgetChild->IsA(UInventoryWidget::StaticClass()))
								{
									return UserWidgetChild;
								}
							}
						}
					}
				}
			}
		}
	}

	// Try legacy name-based lookup as fallback
	TArray<FName> PossibleNames = {
		FName("Content"),
		FName("ChildWidget"),
		FName("ContentWidget"),
		FName("WindowContent"),
		FName("WidgetContent")};

	for (const FName &Name : PossibleNames)
	{
		if (UWidget *Widget = ModularWindow->GetWidgetFromName(Name))
		{
			if (UUserWidget *UserWidget = Cast<UUserWidget>(Widget))
			{
				return UserWidget;
			}
		}
	}

	return nullptr;
}

void AOutercorpCharacter::DebugPrintWidgetType(UUserWidget *Widget) const
{
	// This function is kept for compatibility but no longer logs debug information
}

void AOutercorpCharacter::SetupCharacterWidgetInWindow(UUserWidget *ModularWindow)
{
	if (!ModularWindow)
	{		return;
	}

	// Store the modular window reference
	CharacterWindow = ModularWindow;

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel *ChildCanvas = Cast<UCanvasPanel>(ModularWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	if (!ChildCanvas)
	{		return;
	}

	// Create the character widget
	if (!CharacterWidgetClass)
	{		return;
	}

	CharacterWidget = CreateWidget<UCharacterWidget>(GetWorld(), CharacterWidgetClass);
	if (!CharacterWidget)
	{		return;
	}

	// Add the character widget to the child canvas
	UCanvasPanelSlot *Slot = ChildCanvas->AddChildToCanvas(CharacterWidget);
	if (Slot)
	{
		// Make the widget fill the entire canvas
		Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		Slot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
	}

	// Set the window title on the modular window
	UTextBlock *WindowTitleText = Cast<UTextBlock>(ModularWindow->GetWidgetFromName(FName("TitleText")));
	if (WindowTitleText)
	{
		WindowTitleText->SetText(FText::FromString(TEXT("Character")));
	}

	// Bind events
	BindCharacterEvents();

	// Try to bind to the modular window's close button
	UButton *WindowCloseButton = Cast<UButton>(ModularWindow->GetWidgetFromName(FName("CloseBtn")));
	if (WindowCloseButton)
	{
		WindowCloseButton->OnClicked.AddDynamic(this, &AOutercorpCharacter::CloseCharacter);
	}

	// Set keyboard focus to the character widget so it can receive ESC/I key events
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		CharacterWidget->SetKeyboardFocus();

		// Ensure input mode is set to Game and UI with mouse visible
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}

void AOutercorpCharacter::SetupInventoryWidgetInWindow(UUserWidget *ModularWindow)
{
	if (!ModularWindow)
	{		return;
	}

	// Store the modular window reference
	InventoryWindow = ModularWindow;

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel *ChildCanvas = Cast<UCanvasPanel>(ModularWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	if (!ChildCanvas)
	{		return;
	}

	// Create the inventory widget
	if (!InventoryWidgetClass)
	{		return;
	}

	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	if (!InventoryWidget)
	{		return;
	}

	// Add the inventory widget to the child canvas
	UCanvasPanelSlot *Slot = ChildCanvas->AddChildToCanvas(InventoryWidget);
	if (Slot)
	{
		// Make the widget fill the entire canvas
		Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		Slot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
	}

	// Set the window title on the modular window
	UTextBlock *WindowTitleText = Cast<UTextBlock>(ModularWindow->GetWidgetFromName(FName("TitleText")));
	if (WindowTitleText)
	{
		WindowTitleText->SetText(FText::FromString(TEXT("Inventory")));
	}

	// Bind events
	BindInventoryEvents();

	// Try to bind to the modular window's close button
	UButton *WindowCloseButton = Cast<UButton>(ModularWindow->GetWidgetFromName(FName("CloseBtn")));
	if (WindowCloseButton)
	{
		WindowCloseButton->OnClicked.AddDynamic(this, &AOutercorpCharacter::CloseInventory);
	}

	// Set keyboard focus to the inventory widget so it can receive ESC/C key events
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		InventoryWidget->SetKeyboardFocus();

		// Ensure input mode is set to Game and UI with mouse visible
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}

void AOutercorpCharacter::SetupItemInfoWidgetInWindow(UUserWidget *ModularWindow)
{
	if (!ModularWindow)
	{		return;
	}

	// Store the modular window reference
	ItemInfoWindow = ModularWindow;

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel *ChildCanvas = Cast<UCanvasPanel>(ModularWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	if (!ChildCanvas)
	{		return;
	}

	// Create the item info widget
	if (!ItemInfoWidgetClass)
	{		return;
	}

	ItemInfoWidget = CreateWidget<UUserWidget>(GetWorld(), ItemInfoWidgetClass);
	if (!ItemInfoWidget)
	{		return;
	}

	// Add the item info widget to the child canvas
	UCanvasPanelSlot *Slot = ChildCanvas->AddChildToCanvas(ItemInfoWidget);
	if (Slot)
	{
		// Make the widget fill the entire canvas
		Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		Slot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
	}

	// Try to bind to the modular window's close button
	UButton *WindowCloseButton = Cast<UButton>(ModularWindow->GetWidgetFromName(FName("CloseBtn")));
	if (WindowCloseButton)
	{
		WindowCloseButton->OnClicked.AddDynamic(this, &AOutercorpCharacter::CloseItemInfo);
	}

	// Set the window title on the modular window
	UTextBlock *WindowTitleText = Cast<UTextBlock>(ModularWindow->GetWidgetFromName(FName("TitleText")));
	if (WindowTitleText)
	{
		WindowTitleText->SetText(FText::FromString(TEXT("Info")));
	}
}

void AOutercorpCharacter::OpenItemInfo(const FInventoryItem &Item)
{
	// Store the item data
	CurrentDisplayedItem = Item;

	// If the window already exists (was pre-created in HUD)
	if (ItemInfoWindow)
	{
		// If the item info widget hasn't been created yet, create it now
		if (!ItemInfoWidget)
		{
			SetupItemInfoWidgetInWindow(ItemInfoWindow);
		}

		// Show the window first
		ItemInfoWindow->SetVisibility(ESlateVisibility::Visible);

		// Bring the window to front when opened
		BringWindowToFront(ItemInfoWindow);

		// Call SetItemInfo on the widget if it's a UItemInfoWidget
		if (ItemInfoWidget)
		{
			UItemInfoWidget* InfoWidget = Cast<UItemInfoWidget>(ItemInfoWidget);
			if (InfoWidget)
			{
				InfoWidget->SetItemInfo(Item);
			}
		}

		// Call Blueprint event to update the display with the item data
		UpdateItemInfoDisplay();
	}
	else
	{	}

	// Ensure UI input mode is enabled
	APlayerController *PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
	}
}

void AOutercorpCharacter::CloseItemInfo()
{
	// Hide the modular window instead of removing it
	if (ItemInfoWindow)
	{
		ItemInfoWindow->SetVisibility(ESlateVisibility::Hidden);
	}

	// Call the closed callback
	OnItemInfoWidgetClosed();
}

void AOutercorpCharacter::OnItemInfoWidgetClosed()
{
	// Only restore game input if no other UI widgets are open
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		// Check if other windows are still open
		bool bAnyWidgetOpen = (InventoryWindow != nullptr && InventoryWindow->GetVisibility() == ESlateVisibility::Visible) ||
							  (CharacterWindow != nullptr && CharacterWindow->GetVisibility() == ESlateVisibility::Visible);

		if (bAnyWidgetOpen)
		{
			// Keep UI mode
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}
		else
		{
			// No widgets open, restore game input
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
		}
	}
}

bool AOutercorpCharacter::IsWindowMaximized(UUserWidget *WindowWidget) const
{
	if (!IsValid(WindowWidget))
	{
		return false;
	}

	// Cast to UWindow to access modules
	UWindow *Window = Cast<UWindow>(WindowWidget);
	if (!Window)
	{
		return false;
	}

	// Check all modules on the window for fullscreen modules
	for (UWindow_Module *Module : Window->Modules)
	{
		if (UModule_Fullscreen_None *FullscreenModule = Cast<UModule_Fullscreen_None>(Module))
		{
			// Check if this window is currently fullscreened
			if (FullscreenModule->IsFullscreen(Window))
			{
				return true;
			}
		}
	}

	return false;
}

void AOutercorpCharacter::UnMaximizeWindow(UUserWidget *WindowWidget)
{
	if (!IsValid(WindowWidget))
	{
		return;
	}

	// Cast to UWindow to access modules
	UWindow *Window = Cast<UWindow>(WindowWidget);
	if (!Window)
	{
		return;
	}

	// Find and call Unscreen on any active fullscreen module
	for (UWindow_Module *Module : Window->Modules)
	{
		if (UModule_Fullscreen_None *FullscreenModule = Cast<UModule_Fullscreen_None>(Module))
		{
			if (FullscreenModule->IsFullscreen(Window))
			{
				FullscreenModule->Unscreen(Window);
				return;
			}
		}
	}
}

void AOutercorpCharacter::SaveUILayout()
{
	UOutercorpSaveGame *SaveGameInstance = Cast<UOutercorpSaveGame>(UGameplayStatics::CreateSaveGameObject(UOutercorpSaveGame::StaticClass()));
	if (!SaveGameInstance)
	{		return;
	}

	// Minimum valid window size to prevent saving collapsed/invalid states
	const float MinWindowSize = 100.0f;
	bool bHasValidData = false;

	// Save inventory window layout
	if (InventoryWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(InventoryWindow->Slot);
		if (Slot)
		{
			FVector2D WindowPosition = Slot->GetPosition();
			FVector2D WindowSize = Slot->GetSize();

			// Check if window appears to be maximized by checking if size is unusually large
			// We use a simple size check instead of relying on IsFullscreen() which may fail during EndPlay
			const float MaximizedThreshold = 1500.0f; // If either dimension > 1500, likely maximized
			bool bLooksMaximized = (WindowSize.X > MaximizedThreshold || WindowSize.Y > MaximizedThreshold);

			if (bLooksMaximized)
			{
				// Try to get the pre-maximized size from the fullscreen module
				UWindow *Window = Cast<UWindow>(InventoryWindow);
				if (Window)
				{
					for (UWindow_Module *Module : Window->Modules)
					{
						if (UModule_Fullscreen_None *FullscreenModule = Cast<UModule_Fullscreen_None>(Module))
						{
							// Check if the fullscreen module has valid saved values
							if (FullscreenModule->SizeSaved.X >= MinWindowSize && FullscreenModule->SizeSaved.Y >= MinWindowSize)
							{
								WindowPosition = FullscreenModule->PositionSaved;
								WindowSize = FullscreenModule->SizeSaved;
								break;
							}
						}
					}
				}
			}

			// Only save if the window has a valid size
			if (WindowSize.X >= MinWindowSize && WindowSize.Y >= MinWindowSize)
			{
				SaveGameInstance->InventoryWindowLayout = FWindowLayoutData(WindowPosition, WindowSize);
				bHasValidData = true;
			}
		}
	}

	// Save character window layout
	if (CharacterWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(CharacterWindow->Slot);
		if (Slot)
		{
			FVector2D WindowPosition = Slot->GetPosition();
			FVector2D WindowSize = Slot->GetSize();

			// Check if window is maximized - if so, use the saved pre-maximized position/size
			if (IsWindowMaximized(CharacterWindow))
			{
				UWindow *Window = Cast<UWindow>(CharacterWindow);
				if (Window)
				{
					for (UWindow_Module *Module : Window->Modules)
					{
						if (UModule_Fullscreen_None *FullscreenModule = Cast<UModule_Fullscreen_None>(Module))
						{
							if (FullscreenModule->IsFullscreen(Window))
							{
								WindowPosition = FullscreenModule->PositionSaved;
								WindowSize = FullscreenModule->SizeSaved;
								break;
							}
						}
					}
				}
			}

			// Only save if the window has a valid size
			if (WindowSize.X >= MinWindowSize && WindowSize.Y >= MinWindowSize)
			{
				SaveGameInstance->CharacterWindowLayout = FWindowLayoutData(WindowPosition, WindowSize);
				bHasValidData = true;
			}
		}
	}

	// Save item info window layout
	if (ItemInfoWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(ItemInfoWindow->Slot);
		if (Slot)
		{
			FVector2D WindowPosition = Slot->GetPosition();
			FVector2D WindowSize = Slot->GetSize();

			// Check if window is maximized - if so, use the saved pre-maximized position/size
			if (IsWindowMaximized(ItemInfoWindow))
			{
				UWindow *Window = Cast<UWindow>(ItemInfoWindow);
				if (Window)
				{
					for (UWindow_Module *Module : Window->Modules)
					{
						if (UModule_Fullscreen_None *FullscreenModule = Cast<UModule_Fullscreen_None>(Module))
						{
							if (FullscreenModule->IsFullscreen(Window))
							{
								WindowPosition = FullscreenModule->PositionSaved;
								WindowSize = FullscreenModule->SizeSaved;
								break;
							}
						}
					}
				}
			}

			// Only save if the window has a valid size
			if (WindowSize.X >= MinWindowSize && WindowSize.Y >= MinWindowSize)
			{
				SaveGameInstance->ItemInfoWindowLayout = FWindowLayoutData(WindowPosition, WindowSize);
				bHasValidData = true;
			}
		}
	}

	// Only save to disk if we have at least one valid window size
	if (!bHasValidData)
	{
		return;
	}

	// Save to disk
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, UOutercorpSaveGame::SaveSlotName, UOutercorpSaveGame::UserIndex);
}

void AOutercorpCharacter::LoadUILayout()
{
	if (!UGameplayStatics::DoesSaveGameExist(UOutercorpSaveGame::SaveSlotName, UOutercorpSaveGame::UserIndex))
	{
		return;
	}

	UOutercorpSaveGame *LoadedGame = Cast<UOutercorpSaveGame>(UGameplayStatics::LoadGameFromSlot(UOutercorpSaveGame::SaveSlotName, UOutercorpSaveGame::UserIndex));
	if (!LoadedGame)
	{		return;
	}

	// Minimum valid window size to prevent loading collapsed/invalid states
	const float MinWindowSize = 100.0f;

	// Restore inventory window layout
	if (InventoryWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(InventoryWindow->Slot);
		if (Slot)
		{
			FVector2D LoadedSize = LoadedGame->InventoryWindowLayout.Size;
			// Only restore if the saved size is valid
			if (LoadedSize.X >= MinWindowSize && LoadedSize.Y >= MinWindowSize)
			{
				Slot->SetPosition(LoadedGame->InventoryWindowLayout.Position);
				Slot->SetSize(LoadedSize);
			}
		}
	}

	// Restore character window layout
	if (CharacterWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(CharacterWindow->Slot);
		if (Slot)
		{
			FVector2D LoadedSize = LoadedGame->CharacterWindowLayout.Size;
			// Only restore if the saved size is valid
			if (LoadedSize.X >= MinWindowSize && LoadedSize.Y >= MinWindowSize)
			{
				Slot->SetPosition(LoadedGame->CharacterWindowLayout.Position);
				Slot->SetSize(LoadedSize);
			}
		}
	}

	// Restore item info window layout
	if (ItemInfoWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(ItemInfoWindow->Slot);
		if (Slot)
		{
			FVector2D LoadedSize = LoadedGame->ItemInfoWindowLayout.Size;
			// Only restore if the saved size is valid
			if (LoadedSize.X >= MinWindowSize && LoadedSize.Y >= MinWindowSize)
			{
				Slot->SetPosition(LoadedGame->ItemInfoWindowLayout.Position);
				Slot->SetSize(LoadedSize);
			}
		}
	}

	// CRITICAL FIX: If windows loaded at maximized size, we need to populate the fullscreen module's
	// saved position/size so that clicking the maximize button again will properly un-maximize
	// Otherwise the saved values are (0,0) and the window vanishes when you try to un-maximize

	// Check inventory window
	if (IsWindowMaximized(InventoryWindow))
	{
		UWindow *Window = Cast<UWindow>(InventoryWindow);
		if (Window)
		{
			// Find the fullscreen module and populate its saved position/size with the LOADED values
			for (UWindow_Module *Module : Window->Modules)
			{
				if (UModule_Fullscreen_None *FullscreenModule = Cast<UModule_Fullscreen_None>(Module))
				{
					// Use the loaded (non-maximized) position/size from the save file
					FullscreenModule->PositionSaved = LoadedGame->InventoryWindowLayout.Position;
					FullscreenModule->SizeSaved = LoadedGame->InventoryWindowLayout.Size;
					break;
				}
			}
		}
	}

	// Check character window
	if (IsWindowMaximized(CharacterWindow))
	{
		UWindow *Window = Cast<UWindow>(CharacterWindow);
		if (Window)
		{
			for (UWindow_Module *Module : Window->Modules)
			{
				if (UModule_Fullscreen_None *FullscreenModule = Cast<UModule_Fullscreen_None>(Module))
				{
					FullscreenModule->PositionSaved = LoadedGame->CharacterWindowLayout.Position;
					FullscreenModule->SizeSaved = LoadedGame->CharacterWindowLayout.Size;
					break;
				}
			}
		}
	}

	// Check item info window
	if (IsWindowMaximized(ItemInfoWindow))
	{
		UWindow *Window = Cast<UWindow>(ItemInfoWindow);
		if (Window)
		{
			for (UWindow_Module *Module : Window->Modules)
			{
				if (UModule_Fullscreen_None *FullscreenModule = Cast<UModule_Fullscreen_None>(Module))
				{
					FullscreenModule->PositionSaved = LoadedGame->ItemInfoWindowLayout.Position;
					FullscreenModule->SizeSaved = LoadedGame->ItemInfoWindowLayout.Size;
					break;
				}
			}
		}
	}
}

void AOutercorpCharacter::ResetUILayout()
{
	// Delete the save file
	if (UGameplayStatics::DoesSaveGameExist(UOutercorpSaveGame::SaveSlotName, UOutercorpSaveGame::UserIndex))
	{
		UGameplayStatics::DeleteGameInSlot(UOutercorpSaveGame::SaveSlotName, UOutercorpSaveGame::UserIndex);
	}

	// Reset all windows to default positions and sizes
	if (InventoryWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(InventoryWindow->Slot);
		if (Slot)
		{
			Slot->SetPosition(FVector2D(100, 100));
			Slot->SetSize(FVector2D(600, 400));
		}

		// Exit fullscreen if needed
		UWindow *Window = Cast<UWindow>(InventoryWindow);
		if (Window)
		{
			Window->UpdateUIForCapabilities();
		}
	}

	if (CharacterWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(CharacterWindow->Slot);
		if (Slot)
		{
			Slot->SetPosition(FVector2D(750, 100));
			Slot->SetSize(FVector2D(400, 500));
		}

		// Exit fullscreen if needed
		UWindow *Window = Cast<UWindow>(CharacterWindow);
		if (Window)
		{
			Window->UpdateUIForCapabilities();
		}
	}

	if (ItemInfoWindow)
	{
		UCanvasPanelSlot *Slot = Cast<UCanvasPanelSlot>(ItemInfoWindow->Slot);
		if (Slot)
		{
			Slot->SetPosition(FVector2D(400, 200));
			Slot->SetSize(FVector2D(350, 450));
		}

		// Exit fullscreen if needed
		UWindow *Window = Cast<UWindow>(ItemInfoWindow);
		if (Window)
		{
			Window->UpdateUIForCapabilities();
		}
	}
}

void AOutercorpCharacter::OnWindowLayoutChanged()
{
	// Don't auto-save if any window is currently maximized
	// This prevents saving the maximized state when the maximize action triggers the size change event
	if (IsWindowMaximized(InventoryWindow) || IsWindowMaximized(CharacterWindow) || IsWindowMaximized(ItemInfoWindow))
	{
		return;
	}

	// Auto-save whenever a window is moved or resized (when not maximized)
	SaveUILayout();
}

void AOutercorpCharacter::OnWindowClicked(UWindow* ClickedWindow)
{
	if (ClickedWindow)
	{
		BringWindowToFront(ClickedWindow);
	}
}

void AOutercorpCharacter::OnInventoryWindowInteractStart()
{
	BringWindowToFront(InventoryWindow);
}

void AOutercorpCharacter::OnCharacterWindowInteractStart()
{
	BringWindowToFront(CharacterWindow);
}

void AOutercorpCharacter::OnItemInfoWindowInteractStart()
{
	BringWindowToFront(ItemInfoWindow);
}

void AOutercorpCharacter::BringWindowToFront(UUserWidget* Window)
{
	if (!Window)
	{
		return;
	}

	UWindow* WindowObj = Cast<UWindow>(Window);
	if (!WindowObj)
	{
		return;
	}

	// Find the highest Z-order among all managed windows
	int32 HighestZOrder = 0;
	for (UUserWidget* ManagedWindow : ManagedWindows)
	{
		if (ManagedWindow && ManagedWindow != Window)
		{
			UWindow* ManagedWindowObj = Cast<UWindow>(ManagedWindow);
			if (ManagedWindowObj)
			{
				int32 ZOrder = ManagedWindowObj->GetZOrder();
				if (ZOrder > HighestZOrder)
				{
					HighestZOrder = ZOrder;
				}
			}
		}
	}

	// Set the clicked window to be one level higher than the highest
	WindowObj->SetZOrder(HighestZOrder + 1);
}

void AOutercorpCharacter::RegisterWindow(UUserWidget* Window)
{
	if (Window && !ManagedWindows.Contains(Window))
	{
		ManagedWindows.Add(Window);

		// Bind to the window clicked event
		UWindow* WindowObj = Cast<UWindow>(Window);
		if (WindowObj)
		{
			// Create a lambda to capture the window pointer
			WindowObj->ED_WindowClicked.AddDynamic(this, &AOutercorpCharacter::OnWindowClicked);

			// Set initial Z-order
			WindowObj->SetZOrder(CurrentWindowZOrder++);
		}
	}
}

void AOutercorpCharacter::SetupNotificationCanvas()
{
	if (!NotificationComponent)
	{		return;
	}

	// Get the HUD canvas to add notifications to
	UCanvasPanel* HUDCanvas = GetHUDCanvas();
	if (HUDCanvas)
	{
		NotificationComponent->NotificationCanvas = HUDCanvas;	}
	else
	{	}
}

void AOutercorpCharacter::ToggleConstructionMode()
{
	// Safety check: if we have a ghost but mode is false, clean up first
	if (!bIsInConstructionMode && ConstructionGhostPart)
	{
		if (ConstructionGhostPart)
		{
			ConstructionGhostPart->Destroy();
			ConstructionGhostPart = nullptr;
		}
	}

	if (bIsInConstructionMode)
	{
		ExitConstructionMode();
	}
	else
	{
		EnterConstructionMode();
	}
}

void AOutercorpCharacter::EnterConstructionMode()
{
	// Can't enter construction mode if UI is open
	if (IsAnyUIWidgetOpen())
	{
		return;
	}

	// Can't enter if holding an item
	if (HeldItemData)
	{		return;
	}

	// If in delete mode, exit it first
	if (bIsInDeleteMode)
	{
		ExitDeleteMode();
	}

	// Check if we have a test construction part class assigned
	if (!TestConstructionPartClass)
	{		if (NotificationComponent)
		{
			NotificationComponent->ShowSimpleNotification(FText::FromString("No construction part assigned!"), ENotificationType::Warning, 3.0f);
		}
		return;
	}

	bIsInConstructionMode = true;

	// Spawn the ghost construction part
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ConstructionGhostPart = GetWorld()->SpawnActor<AConstructionPart>(
		TestConstructionPartClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (ConstructionGhostPart)
	{
		// Set to ghost preview state
		ConstructionGhostPart->SetPartState(EConstructionPartState::GhostPreview);

		// Initialize placement distance to the part's max distance (or character default)
		if (ConstructionGhostPart->PartData)
		{
			CurrentPlacementDistance = ConstructionGhostPart->PartData->MaxPlacementDistance;
		}
		else
		{
			CurrentPlacementDistance = MaxPlacementDistance;
		}

		if (NotificationComponent)
		{
			NotificationComponent->ShowSimpleNotification(FText::FromString("Construction Mode: ON"), ENotificationType::Info, 2.0f);
		}
	}
	else
	{		bIsInConstructionMode = false;
	}
}

void AOutercorpCharacter::ExitConstructionMode()
{
	bIsInConstructionMode = false;

	// Destroy the ghost part
	if (ConstructionGhostPart)
	{
		ConstructionGhostPart->Destroy();
		ConstructionGhostPart = nullptr;
	}

	// Clear target references
	TargetConstructionPart = nullptr;
	TargetSocketName = NAME_None;
	GhostSocketName = NAME_None;
	if (NotificationComponent)
	{
		NotificationComponent->ShowSimpleNotification(FText::FromString("Construction Mode: OFF"), ENotificationType::Info, 2.0f);
	}
}

void AOutercorpCharacter::UpdateConstructionPreview()
{
	if (!ConstructionGhostPart || !FirstPersonCameraComponent)
	{
		return;
	}

	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector CameraForward = FirstPersonCameraComponent->GetForwardVector();

	// Calculate the mesh center offset so distance is measured from visual center, not pivot
	FVector MeshCenterOffset = FVector::ZeroVector;
	if (ConstructionGhostPart->MeshComponent && ConstructionGhostPart->MeshComponent->GetStaticMesh())
	{
		FBox MeshBounds = ConstructionGhostPart->MeshComponent->GetStaticMesh()->GetBoundingBox();
		FVector LocalCenter = MeshBounds.GetCenter();
		MeshCenterOffset = LocalCenter * ConstructionGhostPart->GetActorScale3D();
	}

	// Use CurrentPlacementDistance for all placement modes
	// Offset the target position so the visual center is at the desired distance, not the pivot
	FVector TargetCenterPos = CameraLocation + (CameraForward * CurrentPlacementDistance);
	FVector TraceEnd = TargetCenterPos - MeshCenterOffset;
	FVector FreePlacementPos = TargetCenterPos - MeshCenterOffset;

	// Get placement mode from data asset if available, otherwise use deprecated property
	EPlacementMode Mode = ConstructionGhostPart->PartData
		? ConstructionGhostPart->PartData->PlacementMode
		: ConstructionGhostPart->PlacementMode;

	// Override mode based on snap mode toggle
	EPlacementMode EffectiveMode = Mode;
	if (!bSnapModeEnabled)
	{
		// If snapping is disabled, force free placement
		EffectiveMode = EPlacementMode::FreePlace;
	}

	bool bFoundSnapPoint = false;
	bool bFoundGroundPoint = false;
	FVector PlacementPos;
	FRotator PlacementRot = FRotator::ZeroRotator;

	// Try socket snapping first (if mode allows)
	if (EffectiveMode == EPlacementMode::SocketSnap || EffectiveMode == EPlacementMode::Hybrid)
	{
		// Find all placed construction parts
		TArray<AActor*> AllConstructionParts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AConstructionPart::StaticClass(), AllConstructionParts);

		// Do a raycast to find where we're looking (ignore ghost parts)
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		if (ConstructionGhostPart)
		{
			QueryParams.AddIgnoredActor(ConstructionGhostPart);
		}

		// Position ghost at raycast hit or trace end
		FVector TempGhostPos = TraceEnd;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			// Hit something - position ghost at hit location
			TempGhostPos = HitResult.Location;
		}

		ConstructionGhostPart->SetActorLocation(TempGhostPos);
		ConstructionGhostPart->SetActorRotation(FRotator::ZeroRotator);

		// Now find the closest socket pair (ghost socket to target socket)
		float BestDistance = FLT_MAX;
		FName BestGhostSocket = NAME_None;
		FName BestTargetSocket = NAME_None;
		AConstructionPart* BestTargetPart = nullptr;
		int32 TotalSocketPairsChecked = 0;

		// Check all ghost sockets
		for (const FAttachmentPoint& GhostAttachPoint : ConstructionGhostPart->AttachmentPoints)
		{
			FTransform GhostSocketWorld = ConstructionGhostPart->GetSocketTransformByName(GhostAttachPoint.SocketName);

			// Check against all placed parts
			for (AActor* PartActor : AllConstructionParts)
			{
				AConstructionPart* TargetPart = Cast<AConstructionPart>(PartActor);
				if (!TargetPart || TargetPart == ConstructionGhostPart ||
					TargetPart->CurrentState == EConstructionPartState::InInventory ||
					TargetPart->CurrentState == EConstructionPartState::GhostPreview)
				{
					continue;
				}

				// Check all target sockets
				for (const FAttachmentPoint& TargetAttachPoint : TargetPart->AttachmentPoints)
				{
					FTransform TargetSocketWorld = TargetPart->GetSocketTransformByName(TargetAttachPoint.SocketName);

					// Calculate distance between sockets
					float Distance = FVector::Dist(GhostSocketWorld.GetLocation(), TargetSocketWorld.GetLocation());
					TotalSocketPairsChecked++;

					// Use a large search radius to find sockets even if ghost is positioned far away
					// This is especially important for top/bottom sockets where the ghost might be
					// positioned on top of the target part, making sockets 100+ units apart initially
					float SearchRadius = 300.0f; // Large search to find nearby sockets
					if (Distance < SearchRadius && Distance < BestDistance)
					{
						BestDistance = Distance;
						BestGhostSocket = GhostAttachPoint.SocketName;
						BestTargetSocket = TargetAttachPoint.SocketName;
						BestTargetPart = TargetPart;
						bFoundSnapPoint = true;
					}
				}
			}
		}

		// If we found a snap point, calculate the offset needed
		if (bFoundSnapPoint)
		{
			// Get the world transforms of the two sockets we want to align
			FTransform GhostSocketWorld = ConstructionGhostPart->GetSocketTransformByName(BestGhostSocket);
			FTransform TargetSocketWorld = BestTargetPart->GetSocketTransformByName(BestTargetSocket);

			// Calculate the offset from ghost origin to its socket
			FVector GhostToSocket = GhostSocketWorld.GetLocation() - ConstructionGhostPart->GetActorLocation();

			// Apply per-socket offset if configured
			FAttachmentPoint* GhostAttachPoint = ConstructionGhostPart->AttachmentPoints.FindByPredicate([BestGhostSocket](const FAttachmentPoint& Point) {
				return Point.SocketName == BestGhostSocket;
			});
			if (GhostAttachPoint && !GhostAttachPoint->SocketOffset.IsZero())
			{
				GhostToSocket += GhostAttachPoint->SocketOffset;
			}

			// New ghost position = target socket position - offset
			PlacementPos = TargetSocketWorld.GetLocation() - GhostToSocket;
			PlacementRot = FRotator::ZeroRotator;

			// Check if this placement would cause significant collision/overlap with the target part
			bool bWouldOverlap = false;
			if (ConstructionGhostPart->MeshComponent && BestTargetPart->MeshComponent)
			{
				// Get overlap settings from data asset
				bool bAllowOverlap = ConstructionGhostPart->PartData ? ConstructionGhostPart->PartData->bAllowOverlap : true;
				float MaxOverlapThreshold = ConstructionGhostPart->PartData ? ConstructionGhostPart->PartData->MaxOverlapThreshold : 0.25f;

				// Check if overlap is allowed for this part
				if (!bAllowOverlap)
				{
					// Overlap not allowed - check for ANY overlap
					FVector OldGhostPos = ConstructionGhostPart->GetActorLocation();
					ConstructionGhostPart->SetActorLocation(PlacementPos);

					FBox GhostBounds = ConstructionGhostPart->MeshComponent->Bounds.GetBox();
					FBox TargetBounds = BestTargetPart->MeshComponent->Bounds.GetBox();
					FBox Intersection = GhostBounds.Overlap(TargetBounds);

					if (Intersection.IsValid)
					{
						// Any overlap detected - reject placement
						bWouldOverlap = true;
					}

					ConstructionGhostPart->SetActorLocation(OldGhostPos);
				}
				else
				{
					// Overlap is allowed - check if it exceeds threshold
					// Temporarily move ghost to test position
					FVector OldGhostPos = ConstructionGhostPart->GetActorLocation();
					ConstructionGhostPart->SetActorLocation(PlacementPos);

					// Check for overlap between ghost bounds and target bounds
					FBox GhostBounds = ConstructionGhostPart->MeshComponent->Bounds.GetBox();
					FBox TargetBounds = BestTargetPart->MeshComponent->Bounds.GetBox();

					// Calculate the intersection volume to determine if it's a significant overlap
					// Parts connecting at sockets are EXPECTED to touch/overlap slightly at the connection point
					FBox Intersection = GhostBounds.Overlap(TargetBounds);

					if (Intersection.IsValid)
				{
					// Get the size of the intersection
					FVector IntersectionSize = Intersection.GetSize();
					FVector GhostSize = GhostBounds.GetSize();

					// Calculate what percentage of the ghost's volume is overlapping
					float OverlapPercentX = IntersectionSize.X / GhostSize.X;
					float OverlapPercentY = IntersectionSize.Y / GhostSize.Y;
					float OverlapPercentZ = IntersectionSize.Z / GhostSize.Z;

					// Determine which axis to check based on socket names
					// For top/bottom connections, only check Z-axis penetration (X/Y overlap is expected)
					// For side connections, check the perpendicular axes
					bool bIsTopBottomConnection =
						(BestGhostSocket.ToString().Contains("Top") || BestGhostSocket.ToString().Contains("Bottom")) &&
						(BestTargetSocket.ToString().Contains("Top") || BestTargetSocket.ToString().Contains("Bottom"));

					float RelevantOverlapPercent;
					if (bIsTopBottomConnection)
					{
						// Top/bottom stacking - only check Z-axis overlap (parts should align in X/Y)
						RelevantOverlapPercent = OverlapPercentZ;
					}
					else
					{
						// Side connection - determine which axis the connection is along
						bool bIsLeftRight = BestGhostSocket.ToString().Contains("Left") || BestGhostSocket.ToString().Contains("Right") ||
											BestTargetSocket.ToString().Contains("Left") || BestTargetSocket.ToString().Contains("Right");
						bool bIsFrontBack = BestGhostSocket.ToString().Contains("Front") || BestGhostSocket.ToString().Contains("Back") ||
											BestTargetSocket.ToString().Contains("Front") || BestTargetSocket.ToString().Contains("Back");

						if (bIsLeftRight)
						{
							// Left/Right connection - check Y/Z overlap (X overlap is expected)
							RelevantOverlapPercent = FMath::Max(OverlapPercentY, OverlapPercentZ);
						}
						else if (bIsFrontBack)
						{
							// Front/Back connection - check X/Z overlap (Y overlap is expected)
							RelevantOverlapPercent = FMath::Max(OverlapPercentX, OverlapPercentZ);
						}
						else
						{
							// Unknown connection type - check all axes
							RelevantOverlapPercent = FMath::Max3(OverlapPercentX, OverlapPercentY, OverlapPercentZ);
						}
					}

					// Only reject if the relevant axis overlap is significant
					if (RelevantOverlapPercent > MaxOverlapThreshold)
					{
						bWouldOverlap = true;
					}
				}

					// Restore ghost to original position
					ConstructionGhostPart->SetActorLocation(OldGhostPos);
				}
			}

			// Only accept this snap point if it doesn't cause significant overlap
			if (!bWouldOverlap)
			{
				TargetConstructionPart = BestTargetPart;
				TargetSocketName = BestTargetSocket;
				GhostSocketName = BestGhostSocket;
			}
			else
			{
				// Significant overlap detected - reject this snap point
				bFoundSnapPoint = false;
			}
		}
	}

	// Try ground snapping if no socket found (and mode allows)
	if (!bFoundSnapPoint && (EffectiveMode == EPlacementMode::GroundSnap || EffectiveMode == EPlacementMode::Hybrid))
	{
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(ConstructionGhostPart);

		// Use a sphere trace for more stable ground detection (reduces flickering at distance)
		float SphereRadius = 10.0f; // Small sphere to smooth out minor surface variations
		if (GetWorld()->SweepSingleByChannel(HitResult, CameraLocation, TraceEnd, FQuat::Identity,
			ECC_Visibility, FCollisionShape::MakeSphere(SphereRadius), QueryParams))
		{
			bFoundGroundPoint = true;

			// Offset up by mesh bounds so it sits on surface
			FVector BottomOffset = FVector::ZeroVector;
			if (ConstructionGhostPart->MeshComponent && ConstructionGhostPart->MeshComponent->GetStaticMesh())
			{
				FBox Bounds = ConstructionGhostPart->MeshComponent->GetStaticMesh()->GetBoundingBox();
				FVector ScaledMin = Bounds.Min * ConstructionGhostPart->GetActorScale3D();
				BottomOffset = FVector(0, 0, -ScaledMin.Z);
			}

			PlacementPos = HitResult.Location + BottomOffset;
			PlacementRot = FRotator::ZeroRotator;

			// Clear socket target
			TargetConstructionPart = nullptr;
			TargetSocketName = NAME_None;

			// Check for overlap even in ground snap mode
			bool bHasOverlap = false;
			if (ConstructionGhostPart->PartData && ConstructionGhostPart->MeshComponent)
			{
				// Even when overlap is "not allowed", use a small tolerance for surface contact
				// Surface contact is expected and normal - only reject significant penetration
				float OverlapThreshold = ConstructionGhostPart->PartData->bAllowOverlap
					? ConstructionGhostPart->PartData->MaxOverlapThreshold
					: 0.05f; // 5% tolerance for surface contact when overlap is disabled

				// Temporarily move ghost to test position
				FVector OldGhostPos = ConstructionGhostPart->GetActorLocation();
				ConstructionGhostPart->SetActorLocation(PlacementPos);

				FBox GhostBounds = ConstructionGhostPart->MeshComponent->Bounds.GetBox();
				FVector GhostSize = GhostBounds.GetSize();

				// Check against the ground/surface hit actor
				if (HitResult.GetActor())
				{
					TArray<UStaticMeshComponent*> MeshComponents;
					HitResult.GetActor()->GetComponents<UStaticMeshComponent>(MeshComponents);

					for (UStaticMeshComponent* MeshComp : MeshComponents)
					{
						if (MeshComp)
						{
							FBox OtherBounds = MeshComp->Bounds.GetBox();
							FBox Intersection = GhostBounds.Overlap(OtherBounds);

							if (Intersection.IsValid)
							{
								// Calculate overlap percentage to see if it's significant
								FVector IntersectionSize = Intersection.GetSize();
								float OverlapPercentX = IntersectionSize.X / GhostSize.X;
								float OverlapPercentY = IntersectionSize.Y / GhostSize.Y;
								float OverlapPercentZ = IntersectionSize.Z / GhostSize.Z;
								float MaxOverlapPercent = FMath::Max3(OverlapPercentX, OverlapPercentY, OverlapPercentZ);

								// Only reject if overlap exceeds threshold
								if (MaxOverlapPercent > OverlapThreshold)
								{
									bHasOverlap = true;
									break;
								}
							}
						}
					}
				}

				// Also check against all placed construction parts
				if (!bHasOverlap)
				{
					TArray<AActor*> AllConstructionParts;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), AConstructionPart::StaticClass(), AllConstructionParts);

					for (AActor* PartActor : AllConstructionParts)
					{
						AConstructionPart* OtherPart = Cast<AConstructionPart>(PartActor);
						if (!OtherPart || OtherPart == ConstructionGhostPart ||
							OtherPart->CurrentState == EConstructionPartState::InInventory ||
							OtherPart->CurrentState == EConstructionPartState::GhostPreview)
						{
							continue;
						}

						if (OtherPart->MeshComponent)
						{
							FBox OtherBounds = OtherPart->MeshComponent->Bounds.GetBox();
							FBox Intersection = GhostBounds.Overlap(OtherBounds);

							if (Intersection.IsValid)
							{
								// Calculate overlap percentage
								FVector IntersectionSize = Intersection.GetSize();
								float OverlapPercentX = IntersectionSize.X / GhostSize.X;
								float OverlapPercentY = IntersectionSize.Y / GhostSize.Y;
								float OverlapPercentZ = IntersectionSize.Z / GhostSize.Z;
								float MaxOverlapPercent = FMath::Max3(OverlapPercentX, OverlapPercentY, OverlapPercentZ);

								// Only reject if overlap exceeds threshold
								if (MaxOverlapPercent > OverlapThreshold)
								{
									bHasOverlap = true;
									break;
								}
							}
						}
					}
				}

				// Restore ghost to original position
				ConstructionGhostPart->SetActorLocation(OldGhostPos);
			}

			// If overlap detected and not allowed, invalidate ground placement
			if (bHasOverlap)
			{
				bFoundGroundPoint = false;
			}
		}
	}

	// Free placement (if mode allows and nothing else found)
	if (!bFoundSnapPoint && !bFoundGroundPoint && EffectiveMode == EPlacementMode::FreePlace)
	{
		PlacementPos = FreePlacementPos;
		PlacementRot = FRotator::ZeroRotator;
		TargetConstructionPart = nullptr;
		TargetSocketName = NAME_None;

		// Check for overlap with world geometry and construction parts
		bool bHasOverlap = false;
		if (ConstructionGhostPart->PartData && ConstructionGhostPart->MeshComponent)
		{
			// Even when overlap is "not allowed", use a small tolerance
			// This prevents false positives from minor contact
			float OverlapThreshold = ConstructionGhostPart->PartData->bAllowOverlap
				? ConstructionGhostPart->PartData->MaxOverlapThreshold
				: 0.05f; // 5% tolerance when overlap is disabled

			// Temporarily move ghost to test position
			FVector OldGhostPos = ConstructionGhostPart->GetActorLocation();
			ConstructionGhostPart->SetActorLocation(PlacementPos);

			FBox GhostBounds = ConstructionGhostPart->MeshComponent->Bounds.GetBox();
			FVector GhostSize = GhostBounds.GetSize();

			// Check for any world geometry that might overlap
			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams OverlapParams;
			OverlapParams.AddIgnoredActor(this);
			OverlapParams.AddIgnoredActor(ConstructionGhostPart);

			// Use box overlap to check for any colliding geometry
			if (GetWorld()->OverlapMultiByChannel(OverlapResults, PlacementPos, FQuat::Identity, ECC_Visibility,
				FCollisionShape::MakeBox(GhostBounds.GetExtent()), OverlapParams))
			{
				// Check each overlapping actor for overlap percentage
				for (const FOverlapResult& Result : OverlapResults)
				{
					if (Result.GetActor())
					{
						// Get all mesh components from the overlapping actor
						TArray<UStaticMeshComponent*> MeshComponents;
						Result.GetActor()->GetComponents<UStaticMeshComponent>(MeshComponents);

						for (UStaticMeshComponent* MeshComp : MeshComponents)
						{
							if (MeshComp)
							{
								FBox OtherBounds = MeshComp->Bounds.GetBox();
								FBox Intersection = GhostBounds.Overlap(OtherBounds);

								if (Intersection.IsValid)
								{
									// Calculate overlap percentage
									FVector IntersectionSize = Intersection.GetSize();
									float OverlapPercentX = IntersectionSize.X / GhostSize.X;
									float OverlapPercentY = IntersectionSize.Y / GhostSize.Y;
									float OverlapPercentZ = IntersectionSize.Z / GhostSize.Z;
									float MaxOverlapPercent = FMath::Max3(OverlapPercentX, OverlapPercentY, OverlapPercentZ);

									// Only reject if overlap exceeds threshold
									if (MaxOverlapPercent > OverlapThreshold)
									{
										bHasOverlap = true;
										break;
									}
								}
							}
						}

						if (bHasOverlap)
						{
							break;
						}
					}
				}
			}

			// Restore ghost to original position
			ConstructionGhostPart->SetActorLocation(OldGhostPos);
		}

		// If overlap detected and not allowed, mark placement as invalid but still show ghost
		if (bHasOverlap)
		{
			// Don't set bFoundSnapPoint or bFoundGroundPoint, so placement is considered free but invalid
			// The ghost will be shown in red (invalid) state
			ConstructionGhostPart->SetActorLocation(PlacementPos);
			ConstructionGhostPart->SetActorRotation(PlacementRot);
			ConstructionGhostPart->SetGhostPreview(false); // Red/invalid material
			bHasValidPlacement = false;
			return; // Exit early - don't proceed to normal placement logic
		}
	}

	// Apply placement
	bool bNewValidState = (bFoundSnapPoint || bFoundGroundPoint || EffectiveMode == EPlacementMode::FreePlace);

	// Stabilization: Only change state if it's been consistent or definitively invalid
	// This prevents rapid flickering between valid/invalid
	if (bNewValidState != bPreviousValidPlacement)
	{
		// State is changing - for ground snap at distance, be more lenient staying valid
		if (!bNewValidState && bPreviousValidPlacement && bFoundGroundPoint)
		{
			// Keep previous valid state for one more frame to prevent jitter
			bNewValidState = true;
		}
	}

	if (bNewValidState)
	{
		ConstructionGhostPart->SetActorLocation(PlacementPos);
		ConstructionGhostPart->SetActorRotation(PlacementRot);
		ConstructionGhostPart->SetGhostPreview(true);
		bHasValidPlacement = true;
	}
	else
	{
		// Invalid placement
		PlacementPos = TraceEnd;
		ConstructionGhostPart->SetActorLocation(PlacementPos);
		ConstructionGhostPart->SetActorRotation(FRotator::ZeroRotator);
		ConstructionGhostPart->SetGhostPreview(false);
		bHasValidPlacement = false;
	}

	// Store state for next frame
	bPreviousValidPlacement = bHasValidPlacement;
}

void AOutercorpCharacter::PlaceConstructionPart()
{
	if (!bIsInConstructionMode || !ConstructionGhostPart || !bHasValidPlacement)
	{		return;
	}

	// Spawn the actual construction part at the ghost location
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AConstructionPart* NewPart = GetWorld()->SpawnActor<AConstructionPart>(
		TestConstructionPartClass,
		ConstructionGhostPart->GetActorLocation(),
		ConstructionGhostPart->GetActorRotation(),
		SpawnParams
	);

	if (NewPart)
	{
		// If we have a target to snap to, attach
		if (TargetConstructionPart && TargetSocketName != NAME_None && GhostSocketName != NAME_None)
		{
			NewPart->AttachToPart(TargetConstructionPart, TargetSocketName, GhostSocketName);		}
		else
		{
			// Free placement - just set to Placed state
			NewPart->SetPartState(EConstructionPartState::Placed);		}

		if (NotificationComponent)
		{
			// Get the part name from the part's data
			FString PartName = "Part";
			if (NewPart->PartData && !NewPart->PartData->PartName.IsEmpty())
			{
				PartName = NewPart->PartData->PartName.ToString();
			}
			else if (!NewPart->PartName.IsEmpty())
			{
				PartName = NewPart->PartName;
			}

			FString NotificationText = FString::Printf(TEXT("%s Placed"), *PartName);
			NotificationComponent->ShowSimpleNotification(FText::FromString(NotificationText), ENotificationType::Success, 1.5f);
		}
	}
	else
	{	}

	// Keep construction mode active for placing more parts
	// Ghost stays visible for next placement
}

void AOutercorpCharacter::FastenConstructionPart()
{
	// TODO: Implement fastening logic (for Phase 2)
	// This will be used to "bolt" or "weld" parts after placement
}

void AOutercorpCharacter::ToggleSnapMode()
{
	// Only allow toggling when in construction mode
	if (!bIsInConstructionMode)
	{
		return;
	}

	bSnapModeEnabled = !bSnapModeEnabled;

	// Show notification to user
	if (NotificationComponent)
	{
		FString ModeText = bSnapModeEnabled ? "Snapping: ON" : "Free Placement: ON";
		NotificationComponent->ShowSimpleNotification(
			FText::FromString(ModeText),
			ENotificationType::Info,
			2.0f
		);
	}
}

void AOutercorpCharacter::AdjustPlacementDistance(const FInputActionValue& Value)
{
	// Only allow adjustment when in construction mode
	if (!bIsInConstructionMode || !ConstructionGhostPart)
	{
		return;
	}

	// Get the scroll wheel axis value (positive = scroll up, negative = scroll down)
	float ScrollValue = Value.Get<float>();

	// Adjust placement distance (50 units per scroll notch)
	float DistanceStep = 50.0f;
	CurrentPlacementDistance += ScrollValue * DistanceStep;

	// Get min/max from part data if available, otherwise use character defaults
	float MinDist = MinPlacementDistance;
	float MaxDist = MaxPlacementDistance;

	if (ConstructionGhostPart->PartData)
	{
		MinDist = ConstructionGhostPart->PartData->MinPlacementDistance;
		MaxDist = ConstructionGhostPart->PartData->MaxPlacementDistance;
	}

	// Clamp to reasonable values
	CurrentPlacementDistance = FMath::Clamp(CurrentPlacementDistance, MinDist, MaxDist);
}

void AOutercorpCharacter::ToggleDeleteMode()
{
	if (bIsInDeleteMode)
	{
		ExitDeleteMode();
	}
	else
	{
		EnterDeleteMode();
	}
}

void AOutercorpCharacter::EnterDeleteMode()
{
	// Can't enter delete mode if UI is open
	if (IsAnyUIWidgetOpen())
	{
		return;
	}

	// Can't enter if holding an item
	if (HeldItemData)
	{
		return;
	}

	// If in construction mode, exit it first
	if (bIsInConstructionMode)
	{
		ExitConstructionMode();
	}

	bIsInDeleteMode = true;

	if (NotificationComponent)
	{
		NotificationComponent->ShowSimpleNotification(
			FText::FromString("Delete Mode: ON"),
			ENotificationType::Warning,
			2.0f
		);
	}
}

void AOutercorpCharacter::ExitDeleteMode()
{
	bIsInDeleteMode = false;

	// Clear any highlighted part
	if (HighlightedPartForDeletion)
	{
		// Restore original materials
		if (HighlightedPartForDeletion->MeshComponent)
		{
			// The AConstructionPart class handles restoring materials when ghost preview ends
			HighlightedPartForDeletion->SetGhostPreview(true); // Temporarily set to valid
			HighlightedPartForDeletion->SetPartState(HighlightedPartForDeletion->CurrentState); // Restore to current state
		}
		HighlightedPartForDeletion = nullptr;
	}

	if (NotificationComponent)
	{
		NotificationComponent->ShowSimpleNotification(
			FText::FromString("Delete Mode: OFF"),
			ENotificationType::Info,
			2.0f
		);
	}
}

void AOutercorpCharacter::UpdateDeleteModeHighlight()
{
	if (!FirstPersonCameraComponent)
	{
		return;
	}

	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector CameraForward = FirstPersonCameraComponent->GetForwardVector();

	// Use a long trace distance to find all potential parts
	float MaxTraceDistance = 2000.0f; // Long enough to find any part
	FVector TraceEnd = CameraLocation + (CameraForward * MaxTraceDistance);

	// Raycast to find construction parts
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	AConstructionPart* HitPart = nullptr;
	if (bHit && HitResult.GetActor())
	{
		HitPart = Cast<AConstructionPart>(HitResult.GetActor());

		// Check if the hit part is within its delete distance
		if (HitPart && HitPart->PartData)
		{
			float DistanceToPart = FVector::Dist(CameraLocation, HitResult.Location);
			if (DistanceToPart > HitPart->PartData->MaxDeleteDistance)
			{
				// Too far to delete this part
				HitPart = nullptr;
			}
		}
	}

	// Update highlighting
	if (HitPart != HighlightedPartForDeletion)
	{
		// Clear previous highlight
		if (HighlightedPartForDeletion && HighlightedPartForDeletion->MeshComponent)
		{
			HighlightedPartForDeletion->SetPartState(HighlightedPartForDeletion->CurrentState);
		}

		// Set new highlight
		HighlightedPartForDeletion = HitPart;

		if (HighlightedPartForDeletion && HighlightedPartForDeletion->MeshComponent)
		{
			// Use the invalid placement material (red) to highlight for deletion
			if (DeleteHighlightMaterial)
			{
				// Apply delete highlight material to all mesh elements
				int32 NumMaterials = HighlightedPartForDeletion->MeshComponent->GetNumMaterials();
				for (int32 i = 0; i < NumMaterials; i++)
				{
					HighlightedPartForDeletion->MeshComponent->SetMaterial(i, DeleteHighlightMaterial);
				}
			}
			else
			{
				// Fallback to ghost preview with invalid state (red)
				HighlightedPartForDeletion->SetGhostPreview(false);
			}
		}
	}
}

void AOutercorpCharacter::DeleteHighlightedPart()
{
	// Only allow deletion when in delete mode
	if (!bIsInDeleteMode || !HighlightedPartForDeletion)
	{
		return;
	}

	// Get the part name for notification
	FString PartName = "Part";
	if (HighlightedPartForDeletion->PartData && !HighlightedPartForDeletion->PartData->PartName.IsEmpty())
	{
		PartName = HighlightedPartForDeletion->PartData->PartName.ToString();
	}
	else if (!HighlightedPartForDeletion->PartName.IsEmpty())
	{
		PartName = HighlightedPartForDeletion->PartName;
	}

	// Destroy the part
	HighlightedPartForDeletion->Destroy();
	HighlightedPartForDeletion = nullptr;

	// Show notification
	if (NotificationComponent)
	{
		FString NotificationText = FString::Printf(TEXT("%s Deleted"), *PartName);
		NotificationComponent->ShowSimpleNotification(
			FText::FromString(NotificationText),
			ENotificationType::Warning,
			1.5f
		);
	}
}
