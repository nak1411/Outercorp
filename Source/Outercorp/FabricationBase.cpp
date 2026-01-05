// Copyright Epic Games, Inc. All Rights Reserved.

#include "FabricationBase.h"
#include "FabricationData.h"
#include "InteractableComponent.h"
#include "InteractionManagerComponent.h"
#include "InteractionPromptWidget.h"
#include "OutercorpCharacter.h"
#include "ContainerWidget.h"
#include "InventoryComponent.h"
#include "InventoryItemData.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Window.h"
#include "UObject/ConstructorHelpers.h"

AFabricationBase::AFabricationBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Create mesh component (mesh set in Blueprint)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(Root);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

	// Create interactable component
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->InteractionName = FText::FromString(TEXT("Workbench"));
	InteractableComponent->InteractionPrompt = FText::FromString(TEXT("Use (E)"));

	// Create crafting camera (position set in Blueprint)
	CraftingCameraPosition = CreateDefaultSubobject<UCameraComponent>(TEXT("CraftingCameraPosition"));
	CraftingCameraPosition->SetupAttachment(Root);
	CraftingCameraPosition->SetActive(false); // Inactive by default, activated during crafting mode

	// DEPRECATED: Legacy inventory components (now created dynamically in DiscoverZones)
	// Keeping for backward compatibility, but will be populated from ZoneInventories at runtime
	ToolboxInventory = nullptr;
	MaterialBinInventory = nullptr;

	// Interaction zones are created in Blueprint as needed

	// Default state
	CurrentState = EFabricationState::Idle;
	CurrentUser = nullptr;
	bIsInCraftingMode = false;
	CurrentToolboxWindow = nullptr;
	CurrentMaterialBinWindow = nullptr;
	CurrentlyHoveredMesh = nullptr;
}

void AFabricationBase::BeginPlay()
{
	Super::BeginPlay();

	// Initialize from data asset if available
	if (FabricationData)
	{
		UE_LOG(LogTemp, Warning, TEXT("FabricationBase::BeginPlay - FabricationData = %s"), *FabricationData->GetName());
		UE_LOG(LogTemp, Warning, TEXT("  Full path: %s"), *FabricationData->GetPathName());
		InitializeFromData(FabricationData);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FabricationBase::BeginPlay - FabricationData is NULL! Set it in Blueprint!"));
	}

	// Discover zones from Blueprint components
	DiscoverZones();

	// LEGACY: Populate legacy zone references for backward compatibility
	// This will be removed once all code is updated to use DiscoveredZones
	if (UBoxComponent** FoundToolbox = DiscoveredZones.Find(FName("Toolbox")))
	{
		ToolboxZone = *FoundToolbox;
	}
	if (UBoxComponent** FoundMaterialBin = DiscoveredZones.Find(FName("MaterialBin")))
	{
		MaterialBinZone = *FoundMaterialBin;
	}
	if (UStaticMeshComponent** FoundToolboxMesh = ZoneMeshes.Find(FName("Toolbox")))
	{
		ToolboxMesh = *FoundToolboxMesh;
	}
	if (UStaticMeshComponent** FoundMaterialBinMesh = ZoneMeshes.Find(FName("MaterialBin")))
	{
		MaterialBinMesh = *FoundMaterialBinMesh;
	}
	if (UInventoryComponent** FoundToolboxInventory = ZoneInventories.Find(FName("Toolbox")))
	{
		ToolboxInventory = *FoundToolboxInventory;
	}
	if (UInventoryComponent** FoundMaterialBinInventory = ZoneInventories.Find(FName("MaterialBin")))
	{
		MaterialBinInventory = *FoundMaterialBinInventory;
	}

	// Add test items to toolbox for debugging
	AddTestItemsToToolbox();
}

void AFabricationBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Mesh hover is now handled by event-driven OnBeginCursorOver/OnEndCursorOver
	// No need for tick-based ray tracing
}

void AFabricationBase::InitializeFromData(UFabricationData* Data)
{
	if (!Data)
	{
		return;
	}

	FabricationData = Data;

	// Update interactable component
	if (InteractableComponent)
	{
		InteractableComponent->InteractionName = Data->StationName;
		InteractableComponent->InteractionPrompt = FText::Format(
			FText::FromString(TEXT("Use {0} (E)")),
			Data->StationName
		);
		InteractableComponent->InteractionRange = Data->InteractionRange;
	}
}

void AFabricationBase::SetFabricationState(EFabricationState NewState)
{
	CurrentState = NewState;

	// Blueprint can respond to state changes via events
}

bool AFabricationBase::CanBeUsed() const
{
	// Can be used if idle or already in use by same actor
	return CurrentState == EFabricationState::Idle ||
	       CurrentState == EFabricationState::InUse;
}

void AFabricationBase::StartUsing(AActor* User)
{
	UE_LOG(LogTemp, Log, TEXT("FabricationBase::StartUsing called with User: %s"), User ? *User->GetName() : TEXT("NULL"));

	if (!User || !CanBeUsed())
	{
		UE_LOG(LogTemp, Warning, TEXT("FabricationBase::StartUsing - Cannot use: User=%s, CanBeUsed=%d"),
			User ? TEXT("Valid") : TEXT("NULL"), CanBeUsed());
		return;
	}

	CurrentUser = User;
	SetFabricationState(EFabricationState::InUse);

	// Get player controller
	APlayerController* PlayerController = Cast<APlayerController>(User);
	if (!PlayerController)
	{
		// If User is a Pawn, get its controller
		APawn* UserPawn = Cast<APawn>(User);
		if (UserPawn)
		{
			PlayerController = Cast<APlayerController>(UserPawn->GetController());
		}
	}

	// Set this station as the active fabrication station on the character
	AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(User);
	if (!Character && PlayerController)
	{
		Character = Cast<AOutercorpCharacter>(PlayerController->GetPawn());
	}
	if (Character)
	{
		Character->ActiveFabricationStation = this;
	}

	// Enter interactive crafting mode
	if (PlayerController && FabricationData && FabricationData->bEnableInteractiveMode)
	{
		EnterCraftingMode(PlayerController);
	}

	// Open crafting UI (Blueprint implementable)
	OpenCraftingUI(User);

	UE_LOG(LogTemp, Log, TEXT("FabricationBase '%s' started being used by '%s'"),
		*GetName(), *User->GetName());
}

void AFabricationBase::StopUsing()
{
	if (CurrentState != EFabricationState::InUse)
	{
		return;
	}

	AActor* PreviousUser = CurrentUser;
	CurrentUser = nullptr;
	SetFabricationState(EFabricationState::Idle);

	// Exit interactive crafting mode
	if (bIsInCraftingMode)
	{
		ExitCraftingMode();
	}

	// Close crafting UI
	CloseCraftingUI();

	UE_LOG(LogTemp, Log, TEXT("FabricationBase '%s' stopped being used"), *GetName());
}

// ============================================================================
// IInteractableInterface Implementation
// ============================================================================

void AFabricationBase::OnLookAt_Implementation(AActor* LookingActor)
{
	// Delegate to InteractableComponent if it exists
	if (InteractableComponent)
	{
		IInteractableInterface::Execute_OnLookAt(InteractableComponent, LookingActor);
	}
}

void AFabricationBase::OnLookAway_Implementation(AActor* LookingActor)
{
	// Delegate to InteractableComponent if it exists
	if (InteractableComponent)
	{
		IInteractableInterface::Execute_OnLookAway(InteractableComponent, LookingActor);
	}
}

void AFabricationBase::OnInteract_Implementation(AActor* InteractingActor)
{
	UE_LOG(LogTemp, Log, TEXT("FabricationBase::OnInteract_Implementation called with Actor: %s"),
		InteractingActor ? *InteractingActor->GetName() : TEXT("NULL"));

	if (!InteractingActor || !CanBeUsed())
	{
		UE_LOG(LogTemp, Warning, TEXT("FabricationBase::OnInteract_Implementation - Cannot interact: Actor=%s, CanBeUsed=%d"),
			InteractingActor ? TEXT("Valid") : TEXT("NULL"), CanBeUsed());
		return;
	}

	// Start using the fabrication station
	StartUsing(InteractingActor);
}

FText AFabricationBase::GetInteractionName_Implementation() const
{
	if (InteractableComponent)
	{
		return IInteractableInterface::Execute_GetInteractionName(InteractableComponent);
	}

	if (FabricationData)
	{
		return FabricationData->StationName;
	}

	return FText::FromString(TEXT("Fabrication Station"));
}

FText AFabricationBase::GetInteractionPrompt_Implementation() const
{
	if (InteractableComponent)
	{
		return IInteractableInterface::Execute_GetInteractionPrompt(InteractableComponent);
	}

	if (FabricationData)
	{
		return FText::Format(
			FText::FromString(TEXT("Use {0} (E)")),
			FabricationData->StationName
		);
	}

	return FText::FromString(TEXT("Use (E)"));
}

bool AFabricationBase::CanInteract_Implementation(AActor* InteractingActor) const
{
	if (!InteractingActor)
	{
		return false;
	}

	// Can interact if the station can be used
	return CanBeUsed();
}

float AFabricationBase::GetInteractionRange_Implementation() const
{
	if (InteractableComponent)
	{
		return IInteractableInterface::Execute_GetInteractionRange(InteractableComponent);
	}

	if (FabricationData)
	{
		return FabricationData->InteractionRange;
	}

	return 200.0f;
}

bool AFabricationBase::ShouldHighlight_Implementation() const
{
	if (InteractableComponent)
	{
		return IInteractableInterface::Execute_ShouldHighlight(InteractableComponent);
	}

	return true;
}

// ============================================================================
// Interactive Crafting Mode
// ============================================================================

void AFabricationBase::EnterCraftingMode(APlayerController* PlayerController)
{
	if (!PlayerController || !CraftingCameraPosition)
	{
		return;
	}

	bIsInCraftingMode = true;

	// Store original camera position
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (PlayerPawn)
	{
		StoredCameraLocation = PlayerPawn->GetActorLocation();
		StoredCameraRotation = PlayerController->GetControlRotation();

		// Hide the player character mesh to prevent it from appearing in crafting view
		// Also disable interaction manager to prevent highlighting during crafting
		if (AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(PlayerPawn))
		{
			Character->GetMesh()->SetVisibility(false, true);

			// Disable interaction manager to prevent interaction highlighting
			if (UInteractionManagerComponent* InteractionManager = Character->GetInteractionManagerComponent())
			{
				InteractionManager->SetComponentTickEnabled(false);
			}

			// Hide interaction prompt widget
			if (UInteractionPromptWidget* PromptWidget = Character->GetInteractionPromptWidget())
			{
				PromptWidget->SetVisibility(ESlateVisibility::Hidden);
			}

			// Hide crosshair
			Character->SetCrosshairVisibility(false);
		}
	}

	// Activate crafting camera and blend to it
	CraftingCameraPosition->SetActive(true);
	PlayerController->SetViewTargetWithBlend(this, 0.5f, VTBlend_Cubic);

	// Set input mode to Game and UI to allow both clicking and key presses
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	PlayerController->SetInputMode(InputMode);

	// Enable mouse cursor and click events
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	UE_LOG(LogTemp, Warning, TEXT("EnterCraftingMode - Mouse cursor enabled, click events enabled, mouse over events enabled"));
	UE_LOG(LogTemp, Warning, TEXT("EnterCraftingMode - Discovered zones count: %d"), DiscoveredZones.Num());

	// Log each zone's configuration
	for (const TPair<FName, UBoxComponent*>& ZonePair : DiscoveredZones)
	{
		UBoxComponent* Zone = ZonePair.Value;
		if (Zone)
		{
			UE_LOG(LogTemp, Warning, TEXT("  Zone '%s': Collision=%d, VisibilityResponse=%d, Location=%s"),
				*ZonePair.Key.ToString(),
				(int32)Zone->GetCollisionEnabled(),
				(int32)Zone->GetCollisionResponseToChannel(ECC_Visibility),
				*Zone->GetComponentLocation().ToString());
		}
	}

	// Disable the interactable component to prevent highlighting the bench itself
	if (InteractableComponent)
	{
		// Force unhighlight before disabling
		InteractableComponent->SetHighlighted(false);
		InteractableComponent->SetActive(false);
	}
}

void AFabricationBase::ExitCraftingMode()
{
	if (!bIsInCraftingMode)
	{
		return;
	}

	bIsInCraftingMode = false;

	// Get player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		return;
	}

	// Restore camera to player pawn - blend first, THEN deactivate crafting camera
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (PlayerPawn)
	{
		// Start the blend while crafting camera is still active
		PlayerController->SetViewTargetWithBlend(PlayerPawn, 0.5f, VTBlend_Cubic);

		// Deactivate crafting camera and restore control rotation after blend completes
		FTimerHandle BlendCompleteTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			BlendCompleteTimerHandle,
			[this, PlayerController]()
			{
				if (CraftingCameraPosition)
				{
					CraftingCameraPosition->SetActive(false);
				}
				PlayerController->SetControlRotation(StoredCameraRotation);
			},
			0.6f, // Slightly longer than blend duration
			false
		);

		// Show the player character mesh again and re-enable interaction manager
		if (AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(PlayerPawn))
		{
			Character->GetMesh()->SetVisibility(true, true);

			// Re-enable interaction manager
			if (UInteractionManagerComponent* InteractionManager = Character->GetInteractionManagerComponent())
			{
				InteractionManager->SetComponentTickEnabled(true);
			}

			// Restore interaction prompt widget visibility
			if (UInteractionPromptWidget* PromptWidget = Character->GetInteractionPromptWidget())
			{
				PromptWidget->SetVisibility(ESlateVisibility::Visible);
			}

			// Show crosshair
			Character->SetCrosshairVisibility(true);
		}
	}

	// Restore input mode to Game Only
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);

	// Disable mouse cursor
	PlayerController->bShowMouseCursor = false;
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;

	// Close any open container windows
	if (CurrentToolboxWindow)
	{
		CurrentToolboxWindow->RemoveFromParent();
		CurrentToolboxWindow = nullptr;
	}

	if (CurrentMaterialBinWindow)
	{
		CurrentMaterialBinWindow->RemoveFromParent();
		CurrentMaterialBinWindow = nullptr;
	}

	// Clear hovered mesh and unhighlight
	if (CurrentlyHoveredMesh)
	{
		UnhighlightMesh(CurrentlyHoveredMesh);
		CurrentlyHoveredMesh = nullptr;
	}

	// Re-enable the interactable component
	if (InteractableComponent)
	{
		InteractableComponent->SetActive(true);
	}
}

UContainerWidget* AFabricationBase::CreateToolboxContent()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateToolboxContent - Starting..."));

	// Get the Toolbox zone module configuration
	const FFabricationZoneTypeConfig* ZoneConfig = GetZoneTypeConfig(FName("Toolbox"));
	if (!ZoneConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateToolboxContent - No Toolbox zone configuration found in data asset!"));
		return nullptr;
	}
	UE_LOG(LogTemp, Warning, TEXT("CreateToolboxContent - Found ZoneConfig"));
	UE_LOG(LogTemp, Warning, TEXT("  DefaultModule.ModuleID = %s"), *ZoneConfig->DefaultModule.ModuleID.ToString());
	UE_LOG(LogTemp, Warning, TEXT("  DefaultModule.ModuleName = %s"), *ZoneConfig->DefaultModule.ModuleName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("  DefaultModule.ContainerWidgetClass = %s"),
		ZoneConfig->DefaultModule.ContainerWidgetClass ? *ZoneConfig->DefaultModule.ContainerWidgetClass->GetName() : TEXT("NULL"));

	// Get the installed module for the Toolbox zone
	FName* InstalledModuleID = InstalledModules.Find(FName("Toolbox"));
	if (!InstalledModuleID)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateToolboxContent - No module installed in Toolbox zone!"));
		return nullptr;
	}
	UE_LOG(LogTemp, Warning, TEXT("CreateToolboxContent - InstalledModuleID = %s"), *InstalledModuleID->ToString());

	// Find the module data
	const FFabricationZoneModule* Module = nullptr;

	// If DefaultModule.ModuleID is None, it auto-generates to match the zone type
	FName DefaultModuleEffectiveID = ZoneConfig->DefaultModule.ModuleID;
	if (DefaultModuleEffectiveID == NAME_None || DefaultModuleEffectiveID.IsNone())
	{
		DefaultModuleEffectiveID = FName("Toolbox"); // Auto-generated ID matches zone type
	}

	if (DefaultModuleEffectiveID == *InstalledModuleID)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateToolboxContent - Using DefaultModule"));
		Module = &ZoneConfig->DefaultModule;
	}
	else
	{
		for (const FFabricationZoneModule& CompatibleModule : ZoneConfig->CompatibleModules)
		{
			if (CompatibleModule.ModuleID == *InstalledModuleID)
			{
				Module = &CompatibleModule;
				break;
			}
		}
	}

	if (!Module)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateToolboxContent - Module not found for ID '%s'"), *InstalledModuleID->ToString());
		return nullptr;
	}

	if (!Module->ContainerWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateToolboxContent - Module has no ContainerWidgetClass set!"));
		return nullptr;
	}
	UE_LOG(LogTemp, Warning, TEXT("CreateToolboxContent - Found Module with ContainerWidgetClass = %s"),
		*Module->ContainerWidgetClass->GetName());

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateToolboxContent - Could not get PlayerController"));
		return nullptr;
	}

	// Create the toolbox content widget
	UContainerWidget* ToolboxContent = CreateWidget<UContainerWidget>(PC, Module->ContainerWidgetClass);
	if (!ToolboxContent)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateToolboxContent - Failed to create widget from class!"));
		return nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("CreateToolboxContent - Widget created successfully"));

	// Get player's inventory component
	AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(CurrentUser);
	UInventoryComponent* PlayerInventory = nullptr;

	if (Character)
	{
		PlayerInventory = Character->FindComponentByClass<UInventoryComponent>();
	}

	UE_LOG(LogTemp, Warning, TEXT("CreateToolboxContent - PlayerInventory=%s, ToolboxInventory=%s"),
		PlayerInventory ? TEXT("Valid") : TEXT("NULL"),
		ToolboxInventory ? TEXT("Valid") : TEXT("NULL"));

	// Setup content widget with player and toolbox inventories
	ToolboxContent->SetupContainer(PlayerInventory, ToolboxInventory);

	UE_LOG(LogTemp, Warning, TEXT("CreateToolboxContent - Setup complete, returning widget"));

	return ToolboxContent;
}

UContainerWidget* AFabricationBase::CreateMaterialBinContent()
{
	// Get the MaterialBin zone module configuration
	const FFabricationZoneTypeConfig* ZoneConfig = GetZoneTypeConfig(FName("MaterialBin"));
	if (!ZoneConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("No MaterialBin zone configuration found in data asset!"));
		return nullptr;
	}

	// Get the installed module for the MaterialBin zone
	FName* InstalledModuleID = InstalledModules.Find(FName("MaterialBin"));
	if (!InstalledModuleID)
	{
		UE_LOG(LogTemp, Warning, TEXT("No module installed in MaterialBin zone!"));
		return nullptr;
	}

	// Find the module data
	const FFabricationZoneModule* Module = nullptr;

	// If DefaultModule.ModuleID is None, it auto-generates to match the zone type
	FName DefaultModuleEffectiveID = ZoneConfig->DefaultModule.ModuleID;
	if (DefaultModuleEffectiveID == NAME_None || DefaultModuleEffectiveID.IsNone())
	{
		DefaultModuleEffectiveID = FName("MaterialBin"); // Auto-generated ID matches zone type
	}

	if (DefaultModuleEffectiveID == *InstalledModuleID)
	{
		Module = &ZoneConfig->DefaultModule;
	}
	else
	{
		for (const FFabricationZoneModule& CompatibleModule : ZoneConfig->CompatibleModules)
		{
			if (CompatibleModule.ModuleID == *InstalledModuleID)
			{
				Module = &CompatibleModule;
				break;
			}
		}
	}

	if (!Module || !Module->ContainerWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("MaterialBin module has no container widget class!"));
		return nullptr;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return nullptr;
	}

	// Create the material bin content widget
	UContainerWidget* MaterialBinContent = CreateWidget<UContainerWidget>(PC, Module->ContainerWidgetClass);
	if (MaterialBinContent)
	{
		// Get player's inventory component
		AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(CurrentUser);
		UInventoryComponent* PlayerInventory = nullptr;

		if (Character)
		{
			PlayerInventory = Character->FindComponentByClass<UInventoryComponent>();
		}

		// Setup content widget with player and material bin inventories
		MaterialBinContent->SetupContainer(PlayerInventory, MaterialBinInventory);

		UE_LOG(LogTemp, Log, TEXT("Created material bin container content widget"));
	}

	return MaterialBinContent;
}

void AFabricationBase::OnToolboxClicked_Implementation()
{
	// Don't open if already open
	if (CurrentToolboxWindow)
	{
		return;
	}

	if (!BaseWindowClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWindowClass not set in Blueprint!"));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return;
	}

	// Get the character to access the HUD WindowCanvas
	AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(CurrentUser);
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentUser is not an OutercorpCharacter!"));
		return;
	}

	UCanvasPanel* WindowCanvas = Character->GetHUDCanvas();
	if (!WindowCanvas)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get WindowCanvas from character!"));
		return;
	}

	// Create the modular window
	CurrentToolboxWindow = CreateWidget<UUserWidget>(GetWorld(), BaseWindowClass);
	if (!CurrentToolboxWindow)
	{
		return;
	}

	// Add window to the WindowCanvas
	UCanvasPanelSlot* WindowSlot = WindowCanvas->AddChildToCanvas(CurrentToolboxWindow);
	if (WindowSlot)
	{
		// Set initial position and size
		WindowSlot->SetPosition(FVector2D(200, 150));
		WindowSlot->SetSize(FVector2D(500, 400));
		WindowSlot->SetAnchors(FAnchors(0, 0, 0, 0));
	}

	// Initialize the window
	UWindow* Window = Cast<UWindow>(CurrentToolboxWindow);
	if (Window)
	{
		Window->Init();

		// Apply capabilities from content widget
		UContainerWidget* TempContent = CreateToolboxContent();
		if (TempContent)
		{
			Window->ApplyCapabilitiesFromContent(TempContent);
			TempContent->RemoveFromParent(); // Remove temp widget, we'll create the real one below
		}
	}

	// Register window for Z-order management
	Character->RegisterWindow(CurrentToolboxWindow);

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel* ChildCanvas = Cast<UCanvasPanel>(CurrentToolboxWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	UE_LOG(LogTemp, Warning, TEXT("OnToolboxClicked - ChildWidgetCanvas = %s"), ChildCanvas ? TEXT("Valid") : TEXT("NULL"));

	if (ChildCanvas)
	{
		// Create the toolbox content widget
		UContainerWidget* ToolboxContent = CreateToolboxContent();
		UE_LOG(LogTemp, Warning, TEXT("OnToolboxClicked - ToolboxContent widget created = %s"), ToolboxContent ? TEXT("Valid") : TEXT("NULL"));

		if (ToolboxContent)
		{
			// Add content to the child canvas
			UCanvasPanelSlot* ContentSlot = ChildCanvas->AddChildToCanvas(ToolboxContent);
			UE_LOG(LogTemp, Warning, TEXT("OnToolboxClicked - ContentSlot added = %s"), ContentSlot ? TEXT("Valid") : TEXT("NULL"));

			if (ContentSlot)
			{
				// Make the widget fill the entire canvas
				ContentSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
				ContentSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
			}

			// Set the window title from module name
			UTextBlock* WindowTitleText = Cast<UTextBlock>(CurrentToolboxWindow->GetWidgetFromName(FName("TitleText")));
			if (WindowTitleText)
			{
				// Get the module name from the zone config
				const FFabricationZoneTypeConfig* ZoneConfig = GetZoneTypeConfig(FName("Toolbox"));
				FName* InstalledModuleID = InstalledModules.Find(FName("Toolbox"));

				FText WindowTitle = FText::FromString(TEXT("Toolbox")); // Fallback

				if (ZoneConfig && InstalledModuleID)
				{
					const FFabricationZoneModule* Module = nullptr;

					// If DefaultModule.ModuleID is None, it auto-generates to match the zone type
					FName DefaultModuleEffectiveID = ZoneConfig->DefaultModule.ModuleID;
					if (DefaultModuleEffectiveID == NAME_None || DefaultModuleEffectiveID.IsNone())
					{
						DefaultModuleEffectiveID = FName("Toolbox");
					}

					if (DefaultModuleEffectiveID == *InstalledModuleID)
					{
						Module = &ZoneConfig->DefaultModule;
					}
					else
					{
						for (const FFabricationZoneModule& CompatibleModule : ZoneConfig->CompatibleModules)
						{
							if (CompatibleModule.ModuleID == *InstalledModuleID)
							{
								Module = &CompatibleModule;
								break;
							}
						}
					}

					if (Module && !Module->ModuleName.IsEmpty())
					{
						WindowTitle = Module->ModuleName;
					}
				}

				WindowTitleText->SetText(WindowTitle);
			}

			// Bind to close button
			UButton* WindowCloseButton = Cast<UButton>(CurrentToolboxWindow->GetWidgetFromName(FName("CloseBtn")));
			if (WindowCloseButton)
			{
				WindowCloseButton->OnClicked.AddDynamic(this, &AFabricationBase::CloseToolboxWindow);
			}

			UE_LOG(LogTemp, Log, TEXT("Opened toolbox container window with content"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create toolbox content widget!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find ChildWidgetCanvas in Base_Window!"));
	}
}

void AFabricationBase::OnMaterialBinClicked_Implementation()
{
	// Don't open if already open
	if (CurrentMaterialBinWindow)
	{
		return;
	}

	if (!BaseWindowClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseWindowClass not set in Blueprint!"));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return;
	}

	// Get the character to access the HUD WindowCanvas
	AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(CurrentUser);
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentUser is not an OutercorpCharacter!"));
		return;
	}

	UCanvasPanel* WindowCanvas = Character->GetHUDCanvas();
	if (!WindowCanvas)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get WindowCanvas from character!"));
		return;
	}

	// Create the modular window
	CurrentMaterialBinWindow = CreateWidget<UUserWidget>(GetWorld(), BaseWindowClass);
	if (!CurrentMaterialBinWindow)
	{
		return;
	}

	// Add window to the WindowCanvas
	UCanvasPanelSlot* WindowSlot = WindowCanvas->AddChildToCanvas(CurrentMaterialBinWindow);
	if (WindowSlot)
	{
		// Set initial position and size
		WindowSlot->SetPosition(FVector2D(250, 200));
		WindowSlot->SetSize(FVector2D(500, 400));
		WindowSlot->SetAnchors(FAnchors(0, 0, 0, 0));
	}

	// Initialize the window
	UWindow* Window = Cast<UWindow>(CurrentMaterialBinWindow);
	if (Window)
	{
		Window->Init();

		// Apply capabilities from content widget
		UContainerWidget* TempContent = CreateMaterialBinContent();
		if (TempContent)
		{
			Window->ApplyCapabilitiesFromContent(TempContent);
			TempContent->RemoveFromParent(); // Remove temp widget, we'll create the real one below
		}
	}

	// Register window for Z-order management
	Character->RegisterWindow(CurrentMaterialBinWindow);

	// Get the ChildWidgetCanvas from the modular window
	UCanvasPanel* ChildCanvas = Cast<UCanvasPanel>(CurrentMaterialBinWindow->GetWidgetFromName(FName("ChildWidgetCanvas")));
	if (ChildCanvas)
	{
		// Create the material bin content widget
		UContainerWidget* MaterialBinContent = CreateMaterialBinContent();
		if (MaterialBinContent)
		{
			// Add content to the child canvas
			UCanvasPanelSlot* ContentSlot = ChildCanvas->AddChildToCanvas(MaterialBinContent);
			if (ContentSlot)
			{
				// Make the widget fill the entire canvas
				ContentSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
				ContentSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
			}

			// Set the window title from module name
			UTextBlock* WindowTitleText = Cast<UTextBlock>(CurrentMaterialBinWindow->GetWidgetFromName(FName("TitleText")));
			if (WindowTitleText)
			{
				// Get the module name from the zone config
				const FFabricationZoneTypeConfig* ZoneConfig = GetZoneTypeConfig(FName("MaterialBin"));
				FName* InstalledModuleID = InstalledModules.Find(FName("MaterialBin"));

				FText WindowTitle = FText::FromString(TEXT("Material Bin")); // Fallback

				if (ZoneConfig && InstalledModuleID)
				{
					const FFabricationZoneModule* Module = nullptr;

					// If DefaultModule.ModuleID is None, it auto-generates to match the zone type
					FName DefaultModuleEffectiveID = ZoneConfig->DefaultModule.ModuleID;
					if (DefaultModuleEffectiveID == NAME_None || DefaultModuleEffectiveID.IsNone())
					{
						DefaultModuleEffectiveID = FName("MaterialBin");
					}

					if (DefaultModuleEffectiveID == *InstalledModuleID)
					{
						Module = &ZoneConfig->DefaultModule;
					}
					else
					{
						for (const FFabricationZoneModule& CompatibleModule : ZoneConfig->CompatibleModules)
						{
							if (CompatibleModule.ModuleID == *InstalledModuleID)
							{
								Module = &CompatibleModule;
								break;
							}
						}
					}

					if (Module && !Module->ModuleName.IsEmpty())
					{
						WindowTitle = Module->ModuleName;
					}
				}

				WindowTitleText->SetText(WindowTitle);
			}

			// Bind to close button
			UButton* WindowCloseButton = Cast<UButton>(CurrentMaterialBinWindow->GetWidgetFromName(FName("CloseBtn")));
			if (WindowCloseButton)
			{
				WindowCloseButton->OnClicked.AddDynamic(this, &AFabricationBase::CloseMaterialBinWindow);
			}

			UE_LOG(LogTemp, Log, TEXT("Opened material bin container window"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find ChildWidgetCanvas in Base_Window!"));
	}
}

void AFabricationBase::OnWorkSurfaceClicked_Implementation()
{
	// Flash the zone for visual feedback
	if (WorkSurfaceZone)
	{
		FlashZone(WorkSurfaceZone);
	}

	// Blueprint can implement UI opening logic
}

void AFabricationBase::CloseToolboxWindow()
{
	if (CurrentToolboxWindow)
	{
		CurrentToolboxWindow->RemoveFromParent();
		CurrentToolboxWindow = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Closed toolbox window"));
	}
}

void AFabricationBase::CloseMaterialBinWindow()
{
	if (CurrentMaterialBinWindow)
	{
		CurrentMaterialBinWindow->RemoveFromParent();
		CurrentMaterialBinWindow = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Closed material bin window"));
	}
}

void AFabricationBase::AddTestItemsToToolbox()
{
	if (!ToolboxInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("ToolboxInventory is null, cannot add test items"));
		return;
	}

	// Load the wrench data asset
	UInventoryItemData* WrenchData = LoadObject<UInventoryItemData>(nullptr, TEXT("/Game/Items/Equippable/Tools/DataAssets/DA_Wrench.DA_Wrench"));

	if (WrenchData)
	{
		int32 OutSlot;
		bool bAdded = ToolboxInventory->AddItem(WrenchData, 1, OutSlot);

		if (bAdded)
		{
			UE_LOG(LogTemp, Log, TEXT("Added wrench to toolbox at slot %d"), OutSlot);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to add wrench to toolbox"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load wrench data asset at /Game/Items/Equippable/Tools/DataAssets/DA_Wrench"));
	}
}

// ============================================================================
// Auto-Discovery System
// ============================================================================

void AFabricationBase::DiscoverZones()
{
	if (!FabricationData)
	{
		UE_LOG(LogTemp, Warning, TEXT("FabricationBase::DiscoverZones - FabricationData is null, cannot discover zones"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("FabricationBase::DiscoverZones - Starting zone discovery..."));

	// Get all box components (zone components)
	TArray<UActorComponent*> Components;
	GetComponents(UBoxComponent::StaticClass(), Components);

	int32 ZonesDiscovered = 0;

	for (UActorComponent* Comp : Components)
	{
		UBoxComponent* BoxComp = Cast<UBoxComponent>(Comp);
		if (!BoxComp)
		{
			continue;
		}

		FString CompName = BoxComp->GetName();

		// Check if this component matches the zone naming pattern
		if (!CompName.EndsWith(FabricationData->ZoneComponentSuffix))
		{
			continue;
		}

		// Extract zone type from component name
		FName ZoneType = ExtractZoneType(CompName);
		if (ZoneType == NAME_None)
		{
			UE_LOG(LogTemp, Warning, TEXT("FabricationBase::DiscoverZones - Failed to extract zone type from '%s'"), *CompName);
			continue;
		}

		// Check if we have a config for this zone type
		const FFabricationZoneTypeConfig* ZoneConfig = GetZoneTypeConfig(ZoneType);
		if (!ZoneConfig)
		{
			UE_LOG(LogTemp, Warning, TEXT("FabricationBase::DiscoverZones - No config found for zone type '%s' (from component '%s')"), *ZoneType.ToString(), *CompName);
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("FabricationBase::DiscoverZones - Found zone '%s' of type '%s'"), *CompName, *ZoneType.ToString());

		// Register the zone
		DiscoveredZones.Add(ZoneType, BoxComp);

		// Find associated mesh
		UStaticMeshComponent* AssociatedMesh = FindAssociatedMesh(BoxComp, ZoneType);
		if (AssociatedMesh)
		{
			ZoneMeshes.Add(ZoneType, AssociatedMesh);

			// Disable collision on mesh so zone can be detected
			AssociatedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			UE_LOG(LogTemp, Log, TEXT("FabricationBase::DiscoverZones - Found mesh '%s' for zone '%s'"), *AssociatedMesh->GetName(), *ZoneType.ToString());
		}

		// Install default module for this zone type
		// If ModuleID is None, auto-generate it from zone type (e.g., "Toolbox" zone → "Toolbox" module ID)
		FName ModuleIDToInstall = ZoneConfig->DefaultModule.ModuleID;
		if (ModuleIDToInstall == NAME_None || ModuleIDToInstall.IsNone())
		{
			ModuleIDToInstall = ZoneType; // Auto-generate: use zone type as module ID
			UE_LOG(LogTemp, Log, TEXT("FabricationBase::DiscoverZones - ModuleID was None, auto-generated '%s' for zone '%s'"),
				*ModuleIDToInstall.ToString(), *ZoneType.ToString());
		}
		InstallModule(ZoneType, ModuleIDToInstall);

		// Setup zone events (hover, click)
		SetupZoneEvents(BoxComp, ZoneType);

		ZonesDiscovered++;
	}

	UE_LOG(LogTemp, Log, TEXT("FabricationBase::DiscoverZones - Discovery complete. Found %d zones."), ZonesDiscovered);
}

FName AFabricationBase::ExtractZoneType(const FString& ComponentName) const
{
	if (!FabricationData)
	{
		return NAME_None;
	}

	// Remove suffix to get zone type
	// Example: "ToolboxZone" -> "Toolbox"
	FString ZoneTypeName = ComponentName;

	if (ZoneTypeName.EndsWith(FabricationData->ZoneComponentSuffix))
	{
		// Remove the suffix
		ZoneTypeName.RemoveFromEnd(FabricationData->ZoneComponentSuffix);
		return FName(*ZoneTypeName);
	}

	return NAME_None;
}

UStaticMeshComponent* AFabricationBase::FindAssociatedMesh(UBoxComponent* ZoneComponent, FName ZoneType) const
{
	if (!ZoneComponent || !FabricationData)
	{
		return nullptr;
	}

	// Strategy 1: Find mesh by naming convention (e.g., "ToolboxMesh" for "Toolbox" zone)
	FString MeshName = ZoneType.ToString() + FabricationData->ZoneMeshSuffix;

	TArray<UActorComponent*> MeshComponents;
	GetComponents(UStaticMeshComponent::StaticClass(), MeshComponents);

	for (UActorComponent* Comp : MeshComponents)
	{
		UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp);
		if (!MeshComp || MeshComp == MeshComponent) // Skip main workbench mesh
		{
			continue;
		}

		FString CompName = MeshComp->GetName();

		// Check for exact name match
		if (CompName == MeshName || CompName.Contains(ZoneType.ToString()))
		{
			return MeshComp;
		}
	}

	// Strategy 2: Find mesh by proximity (closest mesh to zone center)
	FVector ZoneLocation = ZoneComponent->GetComponentLocation();
	UStaticMeshComponent* ClosestMesh = nullptr;
	float ClosestDistance = 500.0f; // Max search radius in cm (5m)

	for (UActorComponent* Comp : MeshComponents)
	{
		UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp);
		if (!MeshComp || MeshComp == MeshComponent)
		{
			continue;
		}

		float Distance = FVector::Dist(ZoneLocation, MeshComp->GetComponentLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestMesh = MeshComp;
		}
	}

	return ClosestMesh;
}

void AFabricationBase::InstallModule(FName ZoneType, FName ModuleID)
{
	if (!FabricationData)
	{
		return;
	}

	// Get zone type config
	const FFabricationZoneTypeConfig* ZoneConfig = GetZoneTypeConfig(ZoneType);
	if (!ZoneConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("FabricationBase::InstallModule - No config for zone type '%s'"), *ZoneType.ToString());
		return;
	}

	// Find the module in the compatible modules list
	const FFabricationZoneModule* Module = nullptr;

	// Check default module
	// If DefaultModule.ModuleID is None, it auto-generates to match the zone type
	FName DefaultModuleEffectiveID = ZoneConfig->DefaultModule.ModuleID;
	if (DefaultModuleEffectiveID == NAME_None || DefaultModuleEffectiveID.IsNone())
	{
		DefaultModuleEffectiveID = ZoneType; // Auto-generated ID matches zone type
	}

	if (DefaultModuleEffectiveID == ModuleID)
	{
		Module = &ZoneConfig->DefaultModule;
	}
	else
	{
		// Check compatible modules
		for (const FFabricationZoneModule& CompatibleModule : ZoneConfig->CompatibleModules)
		{
			if (CompatibleModule.ModuleID == ModuleID)
			{
				Module = &CompatibleModule;
				break;
			}
		}
	}

	if (!Module)
	{
		UE_LOG(LogTemp, Warning, TEXT("FabricationBase::InstallModule - Module '%s' not found in zone type '%s'"), *ModuleID.ToString(), *ZoneType.ToString());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("FabricationBase::InstallModule - Installing module '%s' in zone '%s'"), *ModuleID.ToString(), *ZoneType.ToString());

	// Install the module
	InstalledModules.Add(ZoneType, ModuleID);

	// Create inventory component if module has storage
	if (Module->InventorySlots > 0)
	{
		// Check if inventory already exists
		if (UInventoryComponent** ExistingInventory = ZoneInventories.Find(ZoneType))
		{
			// Update inventory size
			(*ExistingInventory)->MaxSlots = Module->InventorySlots;
			UE_LOG(LogTemp, Log, TEXT("FabricationBase::InstallModule - Updated inventory for zone '%s' to %d slots"), *ZoneType.ToString(), Module->InventorySlots);
		}
		else
		{
			// Create new inventory component
			FString InventoryName = ZoneType.ToString() + TEXT("Inventory");
			UInventoryComponent* NewInventory = NewObject<UInventoryComponent>(this, FName(*InventoryName));

			if (NewInventory)
			{
				NewInventory->RegisterComponent();
				NewInventory->MaxSlots = Module->InventorySlots;
				ZoneInventories.Add(ZoneType, NewInventory);

				UE_LOG(LogTemp, Log, TEXT("FabricationBase::InstallModule - Created inventory for zone '%s' with %d slots"), *ZoneType.ToString(), Module->InventorySlots);
			}
		}
	}
}

const FFabricationZoneTypeConfig* AFabricationBase::GetZoneTypeConfig(FName ZoneType) const
{
	if (!FabricationData)
	{
		return nullptr;
	}

	for (const FFabricationZoneTypeConfig& Config : FabricationData->ZoneTypeConfigs)
	{
		if (Config.GetZoneTypeName() == ZoneType)
		{
			return &Config;
		}
	}

	return nullptr;
}

void AFabricationBase::SetupZoneEvents(UBoxComponent* ZoneComponent, FName ZoneType)
{
	if (!ZoneComponent)
	{
		return;
	}

	// Configure collision for cursor interaction
	ZoneComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZoneComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ZoneComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ZoneComponent->SetGenerateOverlapEvents(false);

	// Bind hover events
	ZoneComponent->OnBeginCursorOver.AddDynamic(this, &AFabricationBase::OnZoneBeginHover);
	ZoneComponent->OnEndCursorOver.AddDynamic(this, &AFabricationBase::OnZoneEndHover);
	ZoneComponent->OnClicked.AddDynamic(this, &AFabricationBase::OnZoneClicked);

	UE_LOG(LogTemp, Log, TEXT("FabricationBase::SetupZoneEvents - Configured zone '%s': Collision=%s, Delegates bound"),
		*ZoneType.ToString(),
		ZoneComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly ? TEXT("QueryOnly") : TEXT("Other"));
}

void AFabricationBase::HandleZoneClick(UPrimitiveComponent* ClickedComponent)
{
	if (!ClickedComponent)
	{
		return;
	}

	// Check which zone was clicked
	if (ClickedComponent == ToolboxZone)
	{
		OnToolboxClicked();
	}
	else if (ClickedComponent == MaterialBinZone)
	{
		OnMaterialBinClicked();
	}
	else if (ClickedComponent == WorkSurfaceZone)
	{
		OnWorkSurfaceClicked();
	}
}

// ============================================================================
// Internal Click Handlers
// ============================================================================

void AFabricationBase::OnToolboxZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (!bIsInCraftingMode)
	{
		return;
	}

	OnToolboxClicked();
}

void AFabricationBase::OnMaterialBinZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (!bIsInCraftingMode)
	{
		return;
	}

	OnMaterialBinClicked();
}

void AFabricationBase::OnWorkSurfaceZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (!bIsInCraftingMode)
	{
		return;
	}

	OnWorkSurfaceClicked();
}

// ============================================================================
// Zone Hover Event Handlers (Highlight Meshes)
// ============================================================================

// ============================================================================
// Generic Zone Event Handlers (Auto-Discovery System)
// ============================================================================

void AFabricationBase::OnZoneClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("OnZoneClicked called! Component=%s, Button=%s, bIsInCraftingMode=%d"),
		TouchedComponent ? *TouchedComponent->GetName() : TEXT("NULL"),
		*ButtonPressed.ToString(), bIsInCraftingMode);

	if (!TouchedComponent || !bIsInCraftingMode)
	{
		return;
	}

	// Find which zone was clicked by comparing component pointers
	for (const TPair<FName, UBoxComponent*>& ZonePair : DiscoveredZones)
	{
		if (ZonePair.Value == TouchedComponent)
		{
			FName ZoneType = ZonePair.Key;
			UE_LOG(LogTemp, Log, TEXT("Generic zone clicked: %s"), *ZoneType.ToString());

			// Route to specific zone handlers based on zone type
			if (ZoneType == FName("Toolbox"))
			{
				OnToolboxClicked();
			}
			else if (ZoneType == FName("MaterialBin"))
			{
				OnMaterialBinClicked();
			}
			else if (ZoneType == FName("WorkSurface"))
			{
				OnWorkSurfaceClicked();
			}
			else
			{
				// Generic handler for other zone types
				UE_LOG(LogTemp, Warning, TEXT("No specific handler for zone type '%s' - implement custom logic"), *ZoneType.ToString());
			}

			break;
		}
	}
}

void AFabricationBase::OnZoneBeginHover(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("OnZoneBeginHover called! Component=%s, bIsInCraftingMode=%d"),
		TouchedComponent ? *TouchedComponent->GetName() : TEXT("NULL"), bIsInCraftingMode);

	if (!TouchedComponent || !bIsInCraftingMode)
	{
		return;
	}

	// Find which zone was hovered
	for (const TPair<FName, UBoxComponent*>& ZonePair : DiscoveredZones)
	{
		if (ZonePair.Value == TouchedComponent)
		{
			FName ZoneType = ZonePair.Key;

			// Find associated mesh and highlight it
			if (UStaticMeshComponent** ZoneMesh = ZoneMeshes.Find(ZoneType))
			{
				HighlightMesh(*ZoneMesh);
				CurrentlyHoveredMesh = *ZoneMesh;
				UE_LOG(LogTemp, Log, TEXT("Zone %s hovered - highlighting mesh"), *ZoneType.ToString());
			}

			break;
		}
	}
}

void AFabricationBase::OnZoneEndHover(UPrimitiveComponent* TouchedComponent)
{
	if (!TouchedComponent || !bIsInCraftingMode)
	{
		return;
	}

	// Find which zone was unhovered
	for (const TPair<FName, UBoxComponent*>& ZonePair : DiscoveredZones)
	{
		if (ZonePair.Value == TouchedComponent)
		{
			FName ZoneType = ZonePair.Key;

			// Find associated mesh and unhighlight it
			if (UStaticMeshComponent** ZoneMesh = ZoneMeshes.Find(ZoneType))
			{
				UnhighlightMesh(*ZoneMesh);
				if (CurrentlyHoveredMesh == *ZoneMesh)
				{
					CurrentlyHoveredMesh = nullptr;
				}
				UE_LOG(LogTemp, Log, TEXT("Zone %s unhovered - removing highlight"), *ZoneType.ToString());
			}

			break;
		}
	}
}

// ============================================================================
// DEPRECATED: Legacy Zone-Specific Event Handlers
// ============================================================================

void AFabricationBase::OnToolboxZoneBeginHover(UPrimitiveComponent* TouchedComponent)
{
	if (!bIsInCraftingMode || !ToolboxMesh)
	{
		return;
	}

	HighlightMesh(ToolboxMesh);
	CurrentlyHoveredMesh = ToolboxMesh;
	UE_LOG(LogTemp, Log, TEXT("Toolbox zone hovered - highlighting mesh"));
}

void AFabricationBase::OnToolboxZoneEndHover(UPrimitiveComponent* TouchedComponent)
{
	if (!bIsInCraftingMode || !ToolboxMesh)
	{
		return;
	}

	UnhighlightMesh(ToolboxMesh);
	if (CurrentlyHoveredMesh == ToolboxMesh)
	{
		CurrentlyHoveredMesh = nullptr;
	}
	UE_LOG(LogTemp, Log, TEXT("Toolbox zone unhovered - removing highlight"));
}

void AFabricationBase::OnMaterialBinZoneBeginHover(UPrimitiveComponent* TouchedComponent)
{
	if (!bIsInCraftingMode || !MaterialBinMesh)
	{
		return;
	}

	HighlightMesh(MaterialBinMesh);
	CurrentlyHoveredMesh = MaterialBinMesh;
	UE_LOG(LogTemp, Log, TEXT("Material bin zone hovered - highlighting mesh"));
}

void AFabricationBase::OnMaterialBinZoneEndHover(UPrimitiveComponent* TouchedComponent)
{
	if (!bIsInCraftingMode || !MaterialBinMesh)
	{
		return;
	}

	UnhighlightMesh(MaterialBinMesh);
	if (CurrentlyHoveredMesh == MaterialBinMesh)
	{
		CurrentlyHoveredMesh = nullptr;
	}
	UE_LOG(LogTemp, Log, TEXT("Material bin zone unhovered - removing highlight"));
}

// ============================================================================
// Visual Feedback Methods
// ============================================================================

void AFabricationBase::HighlightZone(UPrimitiveComponent* Zone)
{
	// DEPRECATED: Legacy function for backward compatibility
	// New code should use HighlightMesh() with the zone's associated mesh
	if (!Zone)
	{
		return;
	}

	UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Zone);
	if (MeshComp)
	{
		HighlightMesh(MeshComp);
	}
}

void AFabricationBase::UnhighlightZone(UPrimitiveComponent* Zone)
{
	// DEPRECATED: Legacy function for backward compatibility
	// New code should use UnhighlightMesh() with the zone's associated mesh
	if (!Zone)
	{
		return;
	}

	UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Zone);
	if (MeshComp)
	{
		UnhighlightMesh(MeshComp);
	}
}

void AFabricationBase::FlashZone(UPrimitiveComponent* Zone)
{
	if (!Zone)
	{
		return;
	}

	// Box components don't have materials, so we'll use debug visualization instead
	// Draw a pulsing debug box for visual feedback
	UBoxComponent* BoxZone = Cast<UBoxComponent>(Zone);
	if (BoxZone && GetWorld())
	{
		FVector Location = BoxZone->GetComponentLocation();
		FVector Extent = BoxZone->GetScaledBoxExtent();

		// Draw a bright colored box that pulses
		FColor FlashColor = FColor::Yellow;
		if (Zone == ToolboxZone)
		{
			FlashColor = FColor::Red;
		}
		else if (Zone == MaterialBinZone)
		{
			FlashColor = FColor::Green;
		}
		else if (Zone == WorkSurfaceZone)
		{
			FlashColor = FColor::Blue;
		}

		// Draw a thick box for 0.5 seconds
		DrawDebugBox(GetWorld(), Location, Extent * 1.1f, FlashColor, false, 0.5f, 0, 10.0f);
	}
}

void AFabricationBase::DebugDrawZones()
{
	if (!GetWorld())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("=== DEBUG: Drawing Zone Bounds ==="));

	if (ToolboxZone)
	{
		FVector Location = ToolboxZone->GetComponentLocation();
		FVector Extent = Cast<UBoxComponent>(ToolboxZone)->GetScaledBoxExtent();
		DrawDebugBox(GetWorld(), Location, Extent, FColor::Red, false, 10.0f, 0, 5.0f);
		UE_LOG(LogTemp, Warning, TEXT("Toolbox Zone - Location: %s, Extent: %s"), *Location.ToString(), *Extent.ToString());
	}

	if (MaterialBinZone)
	{
		FVector Location = MaterialBinZone->GetComponentLocation();
		FVector Extent = Cast<UBoxComponent>(MaterialBinZone)->GetScaledBoxExtent();
		DrawDebugBox(GetWorld(), Location, Extent, FColor::Green, false, 10.0f, 0, 5.0f);
		UE_LOG(LogTemp, Warning, TEXT("MaterialBin Zone - Location: %s, Extent: %s"), *Location.ToString(), *Extent.ToString());
	}

	if (WorkSurfaceZone)
	{
		FVector Location = WorkSurfaceZone->GetComponentLocation();
		FVector Extent = Cast<UBoxComponent>(WorkSurfaceZone)->GetScaledBoxExtent();
		DrawDebugBox(GetWorld(), Location, Extent, FColor::Blue, false, 10.0f, 0, 5.0f);
		UE_LOG(LogTemp, Warning, TEXT("WorkSurface Zone - Location: %s, Extent: %s"), *Location.ToString(), *Extent.ToString());
	}
}

// ============================================================================
// Mesh Highlighting System
// ============================================================================

void AFabricationBase::HighlightMesh(UStaticMeshComponent* Mesh)
{
	if (!Mesh || !FabricationData || !FabricationData->ZoneHoverMaterial)
	{
		return;
	}

	int32 NumMaterials = Mesh->GetNumMaterials();

	// If mesh only has 1 material slot, we can't use overlay - fallback to replacement
	if (NumMaterials < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("HighlightMesh: Mesh '%s' only has %d material slot(s). Need at least 2 for overlay material."),
			*Mesh->GetName(), NumMaterials);
		UE_LOG(LogTemp, Warning, TEXT("  FIX: In Blueprint, select '%s' mesh and add a second Element to the Materials array (set it to None)."),
			*Mesh->GetName());

		// Store original material in cache
		TArray<UMaterialInterface*>& OriginalMaterials = OriginalMaterialsCache.FindOrAdd(Mesh);
		OriginalMaterials.Empty();
		OriginalMaterials.Add(Mesh->GetMaterial(0));

		Mesh->SetMaterial(0, FabricationData->ZoneHoverMaterial);
		return;
	}

	// Use overlay material slot (non-destructive)
	Mesh->SetOverlayMaterial(FabricationData->ZoneHoverMaterial);

	UE_LOG(LogTemp, Log, TEXT("Highlighted mesh: %s (using overlay material)"), *Mesh->GetName());
}

void AFabricationBase::UnhighlightMesh(UStaticMeshComponent* Mesh)
{
	if (!Mesh)
	{
		return;
	}

	int32 NumMaterials = Mesh->GetNumMaterials();

	// If mesh has overlay material support (2+ slots), clear the overlay
	if (NumMaterials >= 2)
	{
		Mesh->SetOverlayMaterial(nullptr);
		UE_LOG(LogTemp, Log, TEXT("Unhighlighted mesh: %s (cleared overlay material)"), *Mesh->GetName());
	}
	else
	{
		// Fallback: restore original material for single-slot meshes from cache
		TArray<UMaterialInterface*>* OriginalMaterials = OriginalMaterialsCache.Find(Mesh);

		if (OriginalMaterials && OriginalMaterials->Num() > 0)
		{
			if ((*OriginalMaterials)[0])
			{
				Mesh->SetMaterial(0, (*OriginalMaterials)[0]);
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Unhighlighted mesh: %s (restored original material)"), *Mesh->GetName());
	}
}

void AFabricationBase::CheckMeshHover()
{
	if (!bIsInCraftingMode)
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return;
	}

	// Get mouse cursor position and perform ray trace
	FVector WorldLocation, WorldDirection;
	if (!PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		return;
	}

	// Perform line trace
	FVector TraceStart = WorldLocation;
	FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f); // 10 meter trace

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	UStaticMeshComponent* HoveredMesh = nullptr;

	if (bHit)
	{
		// Check if we hit one of our interactive meshes
		UStaticMeshComponent* HitMesh = Cast<UStaticMeshComponent>(HitResult.GetComponent());

		if (HitMesh == ToolboxMesh || HitMesh == MaterialBinMesh)
		{
			HoveredMesh = HitMesh;
		}
	}

	// Handle hover state changes
	if (HoveredMesh != CurrentlyHoveredMesh)
	{
		// Unhighlight previous mesh
		if (CurrentlyHoveredMesh)
		{
			UnhighlightMesh(CurrentlyHoveredMesh);
		}

		// Highlight new mesh
		if (HoveredMesh)
		{
			HighlightMesh(HoveredMesh);
		}

		CurrentlyHoveredMesh = HoveredMesh;
	}
}

// Old mesh hover handlers removed - zones now handle hover detection
