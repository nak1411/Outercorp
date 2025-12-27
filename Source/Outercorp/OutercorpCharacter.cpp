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
#include "Outercorp.h"
#include "Window.h"
#include "OutercorpSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Module_Fullscreen_None.h"
#include "Module_Fullscreen_Point.h"
#include "Module_Fullscreen_Line.h"
#include "Window_Module.h"

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
				}
				else
				{
					if (!WindowCanvas)
					{
						UE_LOG(LogOutercorp, Error, TEXT("BeginPlay: WindowCanvas not found"));
					}
					if (!ModularWindowClass)
					{
						UE_LOG(LogOutercorp, Error, TEXT("BeginPlay: ModularWindowClass not set"));
					}
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

		// Set initial input mode to game-only (no UI, no cursor)
		APlayerController *PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
		}
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
	}
	else
	{
		UE_LOG(LogOutercorp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
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
	{
		UE_LOG(LogOutercorp, Error, TEXT("OpenInventory: InventoryWindow is null!"));
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

void AOutercorpCharacter::BindInventoryEvents()
{
	if (!InventoryWidget)
	{
		UE_LOG(LogOutercorp, Error, TEXT("BindInventoryEvents: InventoryWidget is null!"));
		return;
	}

	if (!InventoryComponent)
	{
		UE_LOG(LogOutercorp, Error, TEXT("BindInventoryEvents: InventoryComponent is null!"));
		return;
	}

	// Bind to close event
	InventoryWidget->OnInventoryClosed.AddDynamic(this, &AOutercorpCharacter::CloseInventory);

	// Initialize the inventory widget
	InventoryWidget->InitializeInventory(InventoryComponent);
}

UCanvasPanel *AOutercorpCharacter::GetHUDCanvas() const
{
	if (!BaseHUDWidget)
	{
		UE_LOG(LogOutercorp, Error, TEXT("GetHUDCanvas: BaseHUDWidget is null!"));
		return nullptr;
	}

	// Try to get the canvas panel named "WindowCanvas" from the HUD widget
	UCanvasPanel *Canvas = Cast<UCanvasPanel>(BaseHUDWidget->GetWidgetFromName(FName("WindowCanvas")));

	if (!Canvas)
	{
		UE_LOG(LogOutercorp, Error, TEXT("GetHUDCanvas: Could not find WindowCanvas in HUD widget"));
	}

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
	{
		UE_LOG(LogOutercorp, Error, TEXT("BindCharacterEvents: CharacterWidget is null!"));
		return;
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
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupCharacterWidgetInWindow: ModularWindow is null"));
		return;
	}

	// Store the modular window reference
	CharacterWindow = ModularWindow;

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel *ChildCanvas = Cast<UCanvasPanel>(ModularWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	if (!ChildCanvas)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupCharacterWidgetInWindow: Could not find ChildWidgetCanvas in modular window"));
		return;
	}

	// Create the character widget
	if (!CharacterWidgetClass)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupCharacterWidgetInWindow: CharacterWidgetClass is not set"));
		return;
	}

	CharacterWidget = CreateWidget<UCharacterWidget>(GetWorld(), CharacterWidgetClass);
	if (!CharacterWidget)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupCharacterWidgetInWindow: Failed to create CharacterWidget"));
		return;
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
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupInventoryWidgetInWindow: ModularWindow is null"));
		return;
	}

	// Store the modular window reference
	InventoryWindow = ModularWindow;

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel *ChildCanvas = Cast<UCanvasPanel>(ModularWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	if (!ChildCanvas)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupInventoryWidgetInWindow: Could not find ChildWidgetCanvas in modular window"));
		return;
	}

	// Create the inventory widget
	if (!InventoryWidgetClass)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupInventoryWidgetInWindow: InventoryWidgetClass is not set"));
		return;
	}

	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	if (!InventoryWidget)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupInventoryWidgetInWindow: Failed to create InventoryWidget"));
		return;
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
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupItemInfoWidgetInWindow: ModularWindow is null"));
		return;
	}

	// Store the modular window reference
	ItemInfoWindow = ModularWindow;

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel *ChildCanvas = Cast<UCanvasPanel>(ModularWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	if (!ChildCanvas)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupItemInfoWidgetInWindow: Could not find ChildWidgetCanvas in modular window"));
		return;
	}

	// Create the item info widget
	if (!ItemInfoWidgetClass)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupItemInfoWidgetInWindow: ItemInfoWidgetClass is not set"));
		return;
	}

	ItemInfoWidget = CreateWidget<UUserWidget>(GetWorld(), ItemInfoWidgetClass);
	if (!ItemInfoWidget)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupItemInfoWidgetInWindow: Failed to create ItemInfoWidget"));
		return;
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
	{
		UE_LOG(LogOutercorp, Error, TEXT("OpenItemInfo: ItemInfoWindow is null!"));
	}

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
	{
		UE_LOG(LogOutercorp, Error, TEXT("SaveUILayout: Failed to create save game object"));
		return;
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
	{
		UE_LOG(LogOutercorp, Error, TEXT("LoadUILayout: Failed to load save game"));
		return;
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
