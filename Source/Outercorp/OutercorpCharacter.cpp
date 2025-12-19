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
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "CharacterWidget.h"
#include "Outercorp.h"
#include "Window.h"

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
				UCanvasPanel* WindowCanvas = GetHUDCanvas();
				if (WindowCanvas && ModularWindowClass)
				{
					// Create the inventory window and add it to the canvas
					InventoryWindow = CreateWidget<UUserWidget>(GetWorld(), ModularWindowClass);
					if (InventoryWindow)
					{
						UCanvasPanelSlot* Slot = WindowCanvas->AddChildToCanvas(InventoryWindow);
						if (Slot)
						{
							// Set position and size for the window
							Slot->SetPosition(FVector2D(100, 100));
							Slot->SetSize(FVector2D(600, 400));
							Slot->SetAnchors(FAnchors(0, 0, 0, 0));
							UE_LOG(LogOutercorp, Log, TEXT("BeginPlay: Set inventory window slot properties"));
						}

						// Call Init() on the window to initialize the modular window system
						UWindow* Window = Cast<UWindow>(InventoryWindow);
						if (Window)
						{
							bool bInitSuccess = Window->Init();
							UE_LOG(LogOutercorp, Log, TEXT("BeginPlay: Inventory window Init() = %s"), bInitSuccess ? TEXT("true") : TEXT("false"));
						}

						InventoryWindow->SetVisibility(ESlateVisibility::Hidden);
						UE_LOG(LogOutercorp, Log, TEXT("BeginPlay: Created and hid InventoryWindow"));
					}

					// Create the character window and add it to the canvas
					CharacterWindow = CreateWidget<UUserWidget>(GetWorld(), ModularWindowClass);
					if (CharacterWindow)
					{
						UCanvasPanelSlot* Slot = WindowCanvas->AddChildToCanvas(CharacterWindow);
						if (Slot)
						{
							// Set position and size for the window
							Slot->SetPosition(FVector2D(750, 100));
							Slot->SetSize(FVector2D(400, 500));
							Slot->SetAnchors(FAnchors(0, 0, 0, 0));
							UE_LOG(LogOutercorp, Log, TEXT("BeginPlay: Set character window slot properties"));
						}

						// Call Init() on the window to initialize the modular window system
						UWindow* Window = Cast<UWindow>(CharacterWindow);
						if (Window)
						{
							bool bInitSuccess = Window->Init();
							UE_LOG(LogOutercorp, Log, TEXT("BeginPlay: Character window Init() = %s"), bInitSuccess ? TEXT("true") : TEXT("false"));
						}

						CharacterWindow->SetVisibility(ESlateVisibility::Hidden);
						UE_LOG(LogOutercorp, Log, TEXT("BeginPlay: Created and hid CharacterWindow"));
					}

					// Create the item info window and add it to the canvas
					ItemInfoWindow = CreateWidget<UUserWidget>(GetWorld(), ModularWindowClass);
					if (ItemInfoWindow)
					{
						UCanvasPanelSlot* Slot = WindowCanvas->AddChildToCanvas(ItemInfoWindow);
						if (Slot)
						{
							// Set position and size for the window
							Slot->SetPosition(FVector2D(400, 200));
							Slot->SetSize(FVector2D(350, 450));
							Slot->SetAnchors(FAnchors(0, 0, 0, 0));
							UE_LOG(LogOutercorp, Log, TEXT("BeginPlay: Set item info window slot properties"));
						}

						// Call Init() on the window to initialize the modular window system
						UWindow* Window = Cast<UWindow>(ItemInfoWindow);
						if (Window)
						{
							bool bInitSuccess = Window->Init();
							UE_LOG(LogOutercorp, Log, TEXT("BeginPlay: Item info window Init() = %s"), bInitSuccess ? TEXT("true") : TEXT("false"));
						}

						ItemInfoWindow->SetVisibility(ESlateVisibility::Hidden);
						UE_LOG(LogOutercorp, Log, TEXT("BeginPlay: Created and hid ItemInfoWindow"));
					}
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
	}
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
		UE_LOG(LogOutercorp, Warning, TEXT("ToggleInventory: Not locally controlled"));
		return;
	}

	// Check if inventory is open by checking visibility
	if (InventoryWindow != nullptr && InventoryWindow->GetVisibility() == ESlateVisibility::Visible)
	{
		UE_LOG(LogOutercorp, Log, TEXT("ToggleInventory: Closing inventory"));
		CloseInventory();
	}
	else
	{
		UE_LOG(LogOutercorp, Log, TEXT("ToggleInventory: Opening inventory"));
		OpenInventory();
	}
}

void AOutercorpCharacter::OpenInventory_Implementation()
{
	UE_LOG(LogOutercorp, Log, TEXT("OpenInventory_Implementation: Called"));

	// If the window already exists (was pre-created in HUD)
	if (InventoryWindow)
	{
		UE_LOG(LogOutercorp, Log, TEXT("OpenInventory: InventoryWindow exists"));

		// If the inventory widget hasn't been created yet, create it now
		if (!InventoryWidget)
		{
			UE_LOG(LogOutercorp, Log, TEXT("OpenInventory: Setting up inventory widget in existing window"));
			SetupInventoryWidgetInWindow(InventoryWindow);
		}
		else
		{
			UE_LOG(LogOutercorp, Log, TEXT("OpenInventory: InventoryWidget already exists"));
		}

		ESlateVisibility CurrentVisibility = InventoryWindow->GetVisibility();
		UE_LOG(LogOutercorp, Log, TEXT("OpenInventory: Current visibility: %d"), (int32)CurrentVisibility);

		InventoryWindow->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogOutercorp, Log, TEXT("OpenInventory: Set window to Visible"));

		ESlateVisibility NewVisibility = InventoryWindow->GetVisibility();
		UE_LOG(LogOutercorp, Log, TEXT("OpenInventory: New visibility: %d"), (int32)NewVisibility);
	}
	else
	{
		UE_LOG(LogOutercorp, Error, TEXT("OpenInventory: InventoryWindow is null!"));
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		// Set input mode to Game and UI
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		UE_LOG(LogOutercorp, Log, TEXT("OpenInventory: Set input mode and mouse cursor"));
	}
}

void AOutercorpCharacter::BindInventoryEvents()
{
	if (!InventoryWidget)
	{
		UE_LOG(LogOutercorp, Error, TEXT("BindInventoryEvents: InventoryWidget is null! Make sure you set the InventoryWidget variable in Blueprint after creating the draggable window."));
		return;
	}

	if (!InventoryComponent)
	{
		UE_LOG(LogOutercorp, Error, TEXT("BindInventoryEvents: InventoryComponent is null!"));
		return;
	}

	UE_LOG(LogOutercorp, Log, TEXT("BindInventoryEvents: Binding events and initializing inventory"));

	// Bind to close event
	InventoryWidget->OnInventoryClosed.AddDynamic(this, &AOutercorpCharacter::CloseInventory);

	// Initialize the inventory widget
	InventoryWidget->InitializeInventory(InventoryComponent);
}

UCanvasPanel* AOutercorpCharacter::GetHUDCanvas() const
{
	if (!BaseHUDWidget)
	{
		UE_LOG(LogOutercorp, Error, TEXT("GetHUDCanvas: BaseHUDWidget is null! Make sure BaseHUDWidgetClass is set in BP_FirstPersonCharacter."));
		return nullptr;
	}

	// Try to get the canvas panel named "WindowCanvas" from the HUD widget
	UCanvasPanel* Canvas = Cast<UCanvasPanel>(BaseHUDWidget->GetWidgetFromName(FName("WindowCanvas")));

	if (!Canvas)
	{
		UE_LOG(LogOutercorp, Error, TEXT("GetHUDCanvas: Could not find Canvas Panel named 'WindowCanvas' in the HUD widget. Make sure your WBP_Base_HUD has a Canvas Panel named exactly 'WindowCanvas'."));
	}

	return Canvas;
}

void AOutercorpCharacter::CloseInventory()
{
	// Hide the modular window instead of removing it (to keep it in canvas hierarchy)
	if (InventoryWindow)
	{
		InventoryWindow->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogOutercorp, Log, TEXT("CloseInventory: Hidden inventory window"));
	}

	// Don't null out references - we'll reuse them
	// The window and widget stay intact for next time

	// Call the closed callback
	OnInventoryWidgetClosed();
}

void AOutercorpCharacter::OnInventoryWidgetClosed()
{
	UE_LOG(LogOutercorp, Log, TEXT("OnInventoryWidgetClosed: Called"));

	// Only restore game input if no other UI widgets are open
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		// Check if character window is still open (by visibility)
		bool bAnyWidgetOpen = (CharacterWindow != nullptr && CharacterWindow->GetVisibility() == ESlateVisibility::Visible);

		UE_LOG(LogOutercorp, Log, TEXT("OnInventoryWidgetClosed: bAnyWidgetOpen = %s"), bAnyWidgetOpen ? TEXT("true") : TEXT("false"));

		if (bAnyWidgetOpen)
		{
			// Keep UI mode - don't lock focus to specific widget
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
			UE_LOG(LogOutercorp, Log, TEXT("OnInventoryWidgetClosed: Keeping UI mode (other widgets open)"));
		}
		else
		{
			// No widgets open, restore game input
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
			UE_LOG(LogOutercorp, Log, TEXT("OnInventoryWidgetClosed: Restored game-only input mode"));
		}
	}
}

void AOutercorpCharacter::ToggleCharacter()
{
	if (!IsLocallyControlled())
	{
		UE_LOG(LogOutercorp, Warning, TEXT("ToggleCharacter: Not locally controlled"));
		return;
	}

	// Check if character window is open by checking visibility
	if (CharacterWindow != nullptr && CharacterWindow->GetVisibility() == ESlateVisibility::Visible)
	{
		UE_LOG(LogOutercorp, Log, TEXT("ToggleCharacter: Closing character window"));
		CloseCharacter();
	}
	else
	{
		UE_LOG(LogOutercorp, Log, TEXT("ToggleCharacter: Opening character window"));
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
			UE_LOG(LogOutercorp, Log, TEXT("OpenCharacter: Setting up character widget in existing window"));
			SetupCharacterWidgetInWindow(CharacterWindow);
		}

		CharacterWindow->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogOutercorp, Log, TEXT("OpenCharacter: Showing existing character window"));
	}
	else
	{
		// This function should be overridden in Blueprint to create the modular window
		// Blueprint should:
		// 1. Create Modular Window with CharacterWidgetClass as child
		// 2. Set CharacterWindow variable
		// 3. Set CharacterWidget variable (from the content of modular window)
		// 4. Call BindCharacterEvents()
		UE_LOG(LogOutercorp, Log, TEXT("OpenCharacter: CharacterWindow is null, needs to be created in Blueprint"));
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
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
		UE_LOG(LogOutercorp, Error, TEXT("BindCharacterEvents: CharacterWidget is null! Make sure you set the CharacterWidget variable in Blueprint after creating the draggable window."));
		return;
	}

	UE_LOG(LogOutercorp, Log, TEXT("BindCharacterEvents: Binding close event for character widget"));

	// Bind to close event
	CharacterWidget->OnCharacterClosed.AddDynamic(this, &AOutercorpCharacter::CloseCharacter);
}

void AOutercorpCharacter::CloseCharacter()
{
	// Hide the modular window instead of removing it (to keep it in canvas hierarchy)
	if (CharacterWindow)
	{
		CharacterWindow->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogOutercorp, Log, TEXT("CloseCharacter: Hidden character window"));
	}

	// Don't null out references - we'll reuse them
	// The window and widget stay intact for next time

	// Call the closed callback
	OnCharacterWidgetClosed();
}

void AOutercorpCharacter::OnCharacterWidgetClosed()
{
	UE_LOG(LogOutercorp, Log, TEXT("OnCharacterWidgetClosed: Called"));

	// Only restore game input if no other UI widgets are open
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		// Check if inventory window is still open (by visibility)
		bool bAnyWidgetOpen = (InventoryWindow != nullptr && InventoryWindow->GetVisibility() == ESlateVisibility::Visible);

		UE_LOG(LogOutercorp, Log, TEXT("OnCharacterWidgetClosed: bAnyWidgetOpen = %s"), bAnyWidgetOpen ? TEXT("true") : TEXT("false"));

		if (bAnyWidgetOpen)
		{
			// Keep UI mode - don't lock focus to specific widget
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
			UE_LOG(LogOutercorp, Log, TEXT("OnCharacterWidgetClosed: Keeping UI mode (other widgets open)"));
		}
		else
		{
			// No widgets open, restore game input
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
			UE_LOG(LogOutercorp, Log, TEXT("OnCharacterWidgetClosed: Restored game-only input mode"));
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

UUserWidget* AOutercorpCharacter::GetModularWindowChild(UUserWidget* ModularWindow) const
{
	if (!ModularWindow)
	{
		UE_LOG(LogOutercorp, Warning, TEXT("GetModularWindowChild: ModularWindow is null"));
		return nullptr;
	}

	UE_LOG(LogOutercorp, Log, TEXT("GetModularWindowChild: Searching in window type: %s"), *ModularWindow->GetClass()->GetName());

	// First, try getting the root widget and traversing from there
	UPanelWidget* RootWidget = ModularWindow->GetRootWidget() ? Cast<UPanelWidget>(ModularWindow->GetRootWidget()) : nullptr;

	if (RootWidget)
	{
		UE_LOG(LogOutercorp, Log, TEXT("GetModularWindowChild: Root widget type: %s, child count: %d"),
			*RootWidget->GetClass()->GetName(), RootWidget->GetChildrenCount());

		// Iterate through all children of root widget
		for (int32 i = 0; i < RootWidget->GetChildrenCount(); ++i)
		{
			UWidget* Child = RootWidget->GetChildAt(i);
			if (Child)
			{
				UE_LOG(LogOutercorp, Log, TEXT("GetModularWindowChild: Root child %d: %s (name: %s)"),
					i, *Child->GetClass()->GetName(), *Child->GetName());

				// Check if this child is a UUserWidget
				if (UUserWidget* UserWidgetChild = Cast<UUserWidget>(Child))
				{
					// Check if it's our target widget type
					if (UserWidgetChild->IsA(UCharacterWidget::StaticClass()) || UserWidgetChild->IsA(UInventoryWidget::StaticClass()))
					{
						UE_LOG(LogOutercorp, Log, TEXT("GetModularWindowChild: Found target widget at root child %d"), i);
						return UserWidgetChild;
					}
				}

				// If it's a panel, check its children too
				if (UPanelWidget* Panel = Cast<UPanelWidget>(Child))
				{
					UE_LOG(LogOutercorp, Log, TEXT("GetModularWindowChild: Child %d is a panel with %d children"), i, Panel->GetChildrenCount());
					for (int32 j = 0; j < Panel->GetChildrenCount(); ++j)
					{
						UWidget* PanelChild = Panel->GetChildAt(j);
						if (PanelChild)
						{
							UE_LOG(LogOutercorp, Log, TEXT("GetModularWindowChild: Panel child %d: %s (name: %s)"),
								j, *PanelChild->GetClass()->GetName(), *PanelChild->GetName());

							if (UUserWidget* UserWidgetChild = Cast<UUserWidget>(PanelChild))
							{
								if (UserWidgetChild->IsA(UCharacterWidget::StaticClass()) || UserWidgetChild->IsA(UInventoryWidget::StaticClass()))
								{
									UE_LOG(LogOutercorp, Log, TEXT("GetModularWindowChild: Found target widget in panel at index %d"), j);
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
		FName("WidgetContent")
	};

	for (const FName& Name : PossibleNames)
	{
		if (UWidget* Widget = ModularWindow->GetWidgetFromName(Name))
		{
			UE_LOG(LogOutercorp, Log, TEXT("GetModularWindowChild: Found widget by name '%s': %s"), *Name.ToString(), *Widget->GetClass()->GetName());
			if (UUserWidget* UserWidget = Cast<UUserWidget>(Widget))
			{
				return UserWidget;
			}
		}
	}

	UE_LOG(LogOutercorp, Warning, TEXT("GetModularWindowChild: Could not find child widget in modular window after exhaustive search"));
	return nullptr;
}

void AOutercorpCharacter::DebugPrintWidgetType(UUserWidget* Widget) const
{
	if (!Widget)
	{
		UE_LOG(LogOutercorp, Warning, TEXT("DebugPrintWidgetType: Widget is null"));
		return;
	}

	FString ClassName = Widget->GetClass()->GetName();
	FString FullPath = Widget->GetClass()->GetPathName();

	UE_LOG(LogOutercorp, Log, TEXT("DebugPrintWidgetType: Class Name = %s"), *ClassName);
	UE_LOG(LogOutercorp, Log, TEXT("DebugPrintWidgetType: Full Path = %s"), *FullPath);
	UE_LOG(LogOutercorp, Log, TEXT("DebugPrintWidgetType: Is CharacterWidget? %s"), Widget->IsA(UCharacterWidget::StaticClass()) ? TEXT("YES") : TEXT("NO"));
}

void AOutercorpCharacter::SetupCharacterWidgetInWindow(UUserWidget* ModularWindow)
{
	if (!ModularWindow)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupCharacterWidgetInWindow: ModularWindow is null"));
		return;
	}

	// Store the modular window reference
	CharacterWindow = ModularWindow;

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel* ChildCanvas = Cast<UCanvasPanel>(ModularWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
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
	UCanvasPanelSlot* Slot = ChildCanvas->AddChildToCanvas(CharacterWidget);
	if (Slot)
	{
		// Make the widget fill the entire canvas
		Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		Slot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
	}

	UE_LOG(LogOutercorp, Log, TEXT("SetupCharacterWidgetInWindow: Successfully added character widget to modular window"));

	// Bind events
	BindCharacterEvents();

	// Try to bind to the modular window's close button
	UButton* WindowCloseButton = Cast<UButton>(ModularWindow->GetWidgetFromName(FName("CloseBtn")));
	if (WindowCloseButton)
	{
		UE_LOG(LogOutercorp, Log, TEXT("SetupCharacterWidgetInWindow: Found CloseBtn in modular window, binding to CloseCharacter"));
		WindowCloseButton->OnClicked.AddDynamic(this, &AOutercorpCharacter::CloseCharacter);
	}
	else
	{
		UE_LOG(LogOutercorp, Warning, TEXT("SetupCharacterWidgetInWindow: Could not find CloseBtn in modular window - X button won't work!"));
	}

	// Note: The modular window should already be added to the HUD canvas in Blueprint
	// to ensure the canvas slot is set up before Init() is called.
	// We don't add it here to avoid duplicates.

	// Set keyboard focus to the character widget so it can receive ESC/I key events
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		CharacterWidget->SetKeyboardFocus();
		UE_LOG(LogOutercorp, Log, TEXT("SetupCharacterWidgetInWindow: Set keyboard focus to character widget"));

		// Ensure input mode is set to Game and UI with mouse visible
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		UE_LOG(LogOutercorp, Log, TEXT("SetupCharacterWidgetInWindow: Set input mode to Game and UI"));
	}
}

void AOutercorpCharacter::SetupInventoryWidgetInWindow(UUserWidget* ModularWindow)
{
	if (!ModularWindow)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupInventoryWidgetInWindow: ModularWindow is null"));
		return;
	}

	// Store the modular window reference
	InventoryWindow = ModularWindow;

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel* ChildCanvas = Cast<UCanvasPanel>(ModularWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
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
	UCanvasPanelSlot* Slot = ChildCanvas->AddChildToCanvas(InventoryWidget);
	if (Slot)
	{
		// Make the widget fill the entire canvas
		Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		Slot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
	}

	UE_LOG(LogOutercorp, Log, TEXT("SetupInventoryWidgetInWindow: Successfully added inventory widget to modular window"));

	// Bind events
	BindInventoryEvents();

	// Try to bind to the modular window's close button
	UButton* WindowCloseButton = Cast<UButton>(ModularWindow->GetWidgetFromName(FName("CloseBtn")));
	if (WindowCloseButton)
	{
		UE_LOG(LogOutercorp, Log, TEXT("SetupInventoryWidgetInWindow: Found CloseBtn in modular window, binding to CloseInventory"));
		WindowCloseButton->OnClicked.AddDynamic(this, &AOutercorpCharacter::CloseInventory);
	}
	else
	{
		UE_LOG(LogOutercorp, Warning, TEXT("SetupInventoryWidgetInWindow: Could not find CloseBtn in modular window - X button won't work!"));
	}

	// Note: The modular window should already be added to the HUD canvas in Blueprint
	// to ensure the canvas slot is set up before Init() is called.
	// We don't add it here to avoid duplicates.

	// Set keyboard focus to the inventory widget so it can receive ESC/C key events
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		InventoryWidget->SetKeyboardFocus();
		UE_LOG(LogOutercorp, Log, TEXT("SetupInventoryWidgetInWindow: Set keyboard focus to inventory widget"));

		// Ensure input mode is set to Game and UI with mouse visible
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		UE_LOG(LogOutercorp, Log, TEXT("SetupInventoryWidgetInWindow: Set input mode to Game and UI"));
	}
}

void AOutercorpCharacter::SetupItemInfoWidgetInWindow(UUserWidget* ModularWindow)
{
	if (!ModularWindow)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupItemInfoWidgetInWindow: ModularWindow is null"));
		return;
	}

	// Store the modular window reference
	ItemInfoWindow = ModularWindow;

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel* ChildCanvas = Cast<UCanvasPanel>(ModularWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
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
	UCanvasPanelSlot* Slot = ChildCanvas->AddChildToCanvas(ItemInfoWidget);
	if (Slot)
	{
		// Make the widget fill the entire canvas
		Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		Slot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
	}

	UE_LOG(LogOutercorp, Log, TEXT("SetupItemInfoWidgetInWindow: Successfully added item info widget to modular window"));

	// Try to bind to the modular window's close button
	UButton* WindowCloseButton = Cast<UButton>(ModularWindow->GetWidgetFromName(FName("CloseBtn")));
	if (WindowCloseButton)
	{
		UE_LOG(LogOutercorp, Log, TEXT("SetupItemInfoWidgetInWindow: Found CloseBtn in modular window, binding to CloseItemInfo"));
		WindowCloseButton->OnClicked.AddDynamic(this, &AOutercorpCharacter::CloseItemInfo);
	}
	else
	{
		UE_LOG(LogOutercorp, Warning, TEXT("SetupItemInfoWidgetInWindow: Could not find CloseBtn in modular window - X button won't work!"));
	}
}

void AOutercorpCharacter::OpenItemInfo(const FInventoryItem& Item)
{
	UE_LOG(LogOutercorp, Log, TEXT("OpenItemInfo: Called for item %s"), Item.IsValid() ? *Item.ItemData->ItemName.ToString() : TEXT("Invalid"));

	// Store the item data
	CurrentDisplayedItem = Item;

	// If the window already exists (was pre-created in HUD)
	if (ItemInfoWindow)
	{
		// If the item info widget hasn't been created yet, create it now
		if (!ItemInfoWidget)
		{
			UE_LOG(LogOutercorp, Log, TEXT("OpenItemInfo: Setting up item info widget in existing window"));
			SetupItemInfoWidgetInWindow(ItemInfoWindow);
		}

		// Show the window first
		ItemInfoWindow->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogOutercorp, Log, TEXT("OpenItemInfo: Showing item info window"));

		// Call Blueprint event to update the display with the item data
		UpdateItemInfoDisplay();
		UE_LOG(LogOutercorp, Log, TEXT("OpenItemInfo: Called UpdateItemInfoDisplay"));
	}
	else
	{
		UE_LOG(LogOutercorp, Error, TEXT("OpenItemInfo: ItemInfoWindow is null!"));
	}

	// Ensure UI input mode is enabled
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		UE_LOG(LogOutercorp, Log, TEXT("OpenItemInfo: Set input mode and mouse cursor"));
	}
}

void AOutercorpCharacter::CloseItemInfo()
{
	// Hide the modular window instead of removing it
	if (ItemInfoWindow)
	{
		ItemInfoWindow->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogOutercorp, Log, TEXT("CloseItemInfo: Hidden item info window"));
	}

	// Call the closed callback
	OnItemInfoWidgetClosed();
}

void AOutercorpCharacter::OnItemInfoWidgetClosed()
{
	UE_LOG(LogOutercorp, Log, TEXT("OnItemInfoWidgetClosed: Called"));

	// Only restore game input if no other UI widgets are open
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Check if other windows are still open
		bool bAnyWidgetOpen = (InventoryWindow != nullptr && InventoryWindow->GetVisibility() == ESlateVisibility::Visible) ||
							  (CharacterWindow != nullptr && CharacterWindow->GetVisibility() == ESlateVisibility::Visible);

		UE_LOG(LogOutercorp, Log, TEXT("OnItemInfoWidgetClosed: bAnyWidgetOpen = %s"), bAnyWidgetOpen ? TEXT("true") : TEXT("false"));

		if (bAnyWidgetOpen)
		{
			// Keep UI mode
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
			UE_LOG(LogOutercorp, Log, TEXT("OnItemInfoWidgetClosed: Keeping UI mode (other widgets open)"));
		}
		else
		{
			// No widgets open, restore game input
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
			UE_LOG(LogOutercorp, Log, TEXT("OnItemInfoWidgetClosed: Restored game-only input mode"));
		}
	}
}
