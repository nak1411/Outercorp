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

	// If inventory is open, close it (check if the reference exists, not IsInViewport)
	if (InventoryDraggableWindow != nullptr)
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
	// This function should be overridden in Blueprint to create the draggable window
	// Blueprint should:
	// 1. Create Draggable Window with InventoryWidgetClass as child
	// 2. Set InventoryDraggableWindow variable
	// 3. Set InventoryWidget variable (from the content of draggable window)
	// 4. Call BindInventoryEvents()

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
	// Remove the draggable window from its parent (HUD canvas)
	if (InventoryDraggableWindow)
	{
		InventoryDraggableWindow->RemoveFromParent();
		UE_LOG(LogOutercorp, Log, TEXT("CloseInventory: Removed draggable window from parent"));
	}

	// Clear references
	InventoryDraggableWindow = nullptr;
	InventoryWidget = nullptr;

	// Call the closed callback
	OnInventoryWidgetClosed();
}

void AOutercorpCharacter::OnInventoryWidgetClosed()
{
	UE_LOG(LogOutercorp, Log, TEXT("OnInventoryWidgetClosed: Called"));

	// Only restore game input if no other UI widgets are open
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		// Check if character window is still open
		bool bAnyWidgetOpen = (CharacterDraggableWindow != nullptr);

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

	// If character window is open, close it (check if the reference exists, not IsInViewport)
	if (CharacterDraggableWindow != nullptr)
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
	// This function should be overridden in Blueprint to create the draggable window
	// Blueprint should:
	// 1. Create Draggable Window with CharacterWidgetClass as child
	// 2. Set CharacterDraggableWindow variable
	// 3. Set CharacterWidget variable (from the content of draggable window)
	// 4. Call BindCharacterEvents()

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
	// Remove the draggable window from its parent (HUD canvas)
	if (CharacterDraggableWindow)
	{
		CharacterDraggableWindow->RemoveFromParent();
		UE_LOG(LogOutercorp, Log, TEXT("CloseCharacter: Removed draggable window from parent"));
	}

	// Clear references
	CharacterDraggableWindow = nullptr;
	CharacterWidget = nullptr;

	// Call the closed callback
	OnCharacterWidgetClosed();
}

void AOutercorpCharacter::OnCharacterWidgetClosed()
{
	UE_LOG(LogOutercorp, Log, TEXT("OnCharacterWidgetClosed: Called"));

	// Only restore game input if no other UI widgets are open
	if (APlayerController *PC = Cast<APlayerController>(GetController()))
	{
		// Check if inventory window is still open
		bool bAnyWidgetOpen = (InventoryDraggableWindow != nullptr);

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
	if (InventoryDraggableWindow != nullptr)
	{
		CloseInventory();
	}

	// Close character window if open
	if (CharacterDraggableWindow != nullptr)
	{
		CloseCharacter();
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
	return (InventoryDraggableWindow != nullptr) || (CharacterDraggableWindow != nullptr);
}

UUserWidget* AOutercorpCharacter::GetDraggableWindowChild(UUserWidget* DraggableWindow) const
{
	if (!DraggableWindow)
	{
		UE_LOG(LogOutercorp, Warning, TEXT("GetDraggableWindowChild: DraggableWindow is null"));
		return nullptr;
	}

	UE_LOG(LogOutercorp, Log, TEXT("GetDraggableWindowChild: Searching in window type: %s"), *DraggableWindow->GetClass()->GetName());

	// First, try getting the root widget and traversing from there
	UPanelWidget* RootWidget = DraggableWindow->GetRootWidget() ? Cast<UPanelWidget>(DraggableWindow->GetRootWidget()) : nullptr;

	if (RootWidget)
	{
		UE_LOG(LogOutercorp, Log, TEXT("GetDraggableWindowChild: Root widget type: %s, child count: %d"),
			*RootWidget->GetClass()->GetName(), RootWidget->GetChildrenCount());

		// Iterate through all children of root widget
		for (int32 i = 0; i < RootWidget->GetChildrenCount(); ++i)
		{
			UWidget* Child = RootWidget->GetChildAt(i);
			if (Child)
			{
				UE_LOG(LogOutercorp, Log, TEXT("GetDraggableWindowChild: Root child %d: %s (name: %s)"),
					i, *Child->GetClass()->GetName(), *Child->GetName());

				// Check if this child is a UUserWidget
				if (UUserWidget* UserWidgetChild = Cast<UUserWidget>(Child))
				{
					// Check if it's our target widget type
					if (UserWidgetChild->IsA(UCharacterWidget::StaticClass()) || UserWidgetChild->IsA(UInventoryWidget::StaticClass()))
					{
						UE_LOG(LogOutercorp, Log, TEXT("GetDraggableWindowChild: Found target widget at root child %d"), i);
						return UserWidgetChild;
					}
				}

				// If it's a panel, check its children too
				if (UPanelWidget* Panel = Cast<UPanelWidget>(Child))
				{
					UE_LOG(LogOutercorp, Log, TEXT("GetDraggableWindowChild: Child %d is a panel with %d children"), i, Panel->GetChildrenCount());
					for (int32 j = 0; j < Panel->GetChildrenCount(); ++j)
					{
						UWidget* PanelChild = Panel->GetChildAt(j);
						if (PanelChild)
						{
							UE_LOG(LogOutercorp, Log, TEXT("GetDraggableWindowChild: Panel child %d: %s (name: %s)"),
								j, *PanelChild->GetClass()->GetName(), *PanelChild->GetName());

							if (UUserWidget* UserWidgetChild = Cast<UUserWidget>(PanelChild))
							{
								if (UserWidgetChild->IsA(UCharacterWidget::StaticClass()) || UserWidgetChild->IsA(UInventoryWidget::StaticClass()))
								{
									UE_LOG(LogOutercorp, Log, TEXT("GetDraggableWindowChild: Found target widget in panel at index %d"), j);
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
		if (UWidget* Widget = DraggableWindow->GetWidgetFromName(Name))
		{
			UE_LOG(LogOutercorp, Log, TEXT("GetDraggableWindowChild: Found widget by name '%s': %s"), *Name.ToString(), *Widget->GetClass()->GetName());
			if (UUserWidget* UserWidget = Cast<UUserWidget>(Widget))
			{
				return UserWidget;
			}
		}
	}

	UE_LOG(LogOutercorp, Warning, TEXT("GetDraggableWindowChild: Could not find child widget in draggable window after exhaustive search"));
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

void AOutercorpCharacter::SetupCharacterWidgetInWindow(UUserWidget* DraggableWindow)
{
	if (!DraggableWindow)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupCharacterWidgetInWindow: DraggableWindow is null"));
		return;
	}

	// Store the draggable window reference
	CharacterDraggableWindow = DraggableWindow;

	// Get the ChildWidgetCanvas from the draggable window
	UCanvasPanel* ChildCanvas = Cast<UCanvasPanel>(DraggableWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	if (!ChildCanvas)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupCharacterWidgetInWindow: Could not find ChildWidgetCanvas in draggable window"));
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

	UE_LOG(LogOutercorp, Log, TEXT("SetupCharacterWidgetInWindow: Successfully added character widget to draggable window"));

	// Bind events
	BindCharacterEvents();

	// Try to bind to the draggable window's close button
	UButton* WindowCloseButton = Cast<UButton>(DraggableWindow->GetWidgetFromName(FName("CloseBtn")));
	if (WindowCloseButton)
	{
		UE_LOG(LogOutercorp, Log, TEXT("SetupCharacterWidgetInWindow: Found CloseBtn in draggable window, binding to CloseCharacter"));
		WindowCloseButton->OnClicked.AddDynamic(this, &AOutercorpCharacter::CloseCharacter);
	}
	else
	{
		UE_LOG(LogOutercorp, Warning, TEXT("SetupCharacterWidgetInWindow: Could not find CloseBtn in draggable window - X button won't work!"));
	}

	// Add the draggable window to HUD canvas
	UCanvasPanel* HUDCanvas = GetHUDCanvas();
	if (HUDCanvas)
	{
		HUDCanvas->AddChildToCanvas(CharacterDraggableWindow);
		UE_LOG(LogOutercorp, Log, TEXT("SetupCharacterWidgetInWindow: Added draggable window to HUD canvas"));
	}

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

void AOutercorpCharacter::SetupInventoryWidgetInWindow(UUserWidget* DraggableWindow)
{
	if (!DraggableWindow)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupInventoryWidgetInWindow: DraggableWindow is null"));
		return;
	}

	// Store the draggable window reference
	InventoryDraggableWindow = DraggableWindow;

	// Get the ChildWidgetCanvas from the draggable window
	UCanvasPanel* ChildCanvas = Cast<UCanvasPanel>(DraggableWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	if (!ChildCanvas)
	{
		UE_LOG(LogOutercorp, Error, TEXT("SetupInventoryWidgetInWindow: Could not find ChildWidgetCanvas in draggable window"));
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

	UE_LOG(LogOutercorp, Log, TEXT("SetupInventoryWidgetInWindow: Successfully added inventory widget to draggable window"));

	// Bind events
	BindInventoryEvents();

	// Try to bind to the draggable window's close button
	UButton* WindowCloseButton = Cast<UButton>(DraggableWindow->GetWidgetFromName(FName("CloseBtn")));
	if (WindowCloseButton)
	{
		UE_LOG(LogOutercorp, Log, TEXT("SetupInventoryWidgetInWindow: Found CloseBtn in draggable window, binding to CloseInventory"));
		WindowCloseButton->OnClicked.AddDynamic(this, &AOutercorpCharacter::CloseInventory);
	}
	else
	{
		UE_LOG(LogOutercorp, Warning, TEXT("SetupInventoryWidgetInWindow: Could not find CloseBtn in draggable window - X button won't work!"));
	}

	// Add the draggable window to HUD canvas
	UCanvasPanel* HUDCanvas = GetHUDCanvas();
	if (HUDCanvas)
	{
		HUDCanvas->AddChildToCanvas(InventoryDraggableWindow);
		UE_LOG(LogOutercorp, Log, TEXT("SetupInventoryWidgetInWindow: Added draggable window to HUD canvas"));
	}

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
