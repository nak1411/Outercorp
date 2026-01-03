// Copyright Epic Games, Inc. All Rights Reserved.

#include "FabricationBase.h"
#include "FabricationData.h"
#include "InteractableComponent.h"
#include "InteractionManagerComponent.h"
#include "InteractionPromptWidget.h"
#include "OutercorpCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"

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

	// Interaction zones are created in Blueprint as needed

	// Default state
	CurrentState = EFabricationState::Idle;
	CurrentUser = nullptr;
	bIsInCraftingMode = false;
}

void AFabricationBase::BeginPlay()
{
	Super::BeginPlay();

	// Auto-assign zone components by name if not already set
	if (!ToolboxZone || !MaterialBinZone || !WorkSurfaceZone)
	{
		TArray<UActorComponent*> Components;
		GetComponents(UBoxComponent::StaticClass(), Components);

		for (UActorComponent* Comp : Components)
		{
			UBoxComponent* BoxComp = Cast<UBoxComponent>(Comp);
			if (BoxComp)
			{
				FString CompName = BoxComp->GetName();

				if (CompName.Contains(TEXT("Toolbox")) && !ToolboxZone)
				{
					ToolboxZone = BoxComp;
					UE_LOG(LogTemp, Log, TEXT("FabricationBase: Auto-assigned ToolboxZone to '%s'"), *CompName);
				}
				else if (CompName.Contains(TEXT("MaterialBin")) && !MaterialBinZone)
				{
					MaterialBinZone = BoxComp;
					UE_LOG(LogTemp, Log, TEXT("FabricationBase: Auto-assigned MaterialBinZone to '%s'"), *CompName);
				}
				else if (CompName.Contains(TEXT("WorkSurface")) && !WorkSurfaceZone)
				{
					WorkSurfaceZone = BoxComp;
					UE_LOG(LogTemp, Log, TEXT("FabricationBase: Auto-assigned WorkSurfaceZone to '%s'"), *CompName);
				}
			}
		}
	}

	// Configure and bind click events to zones
	if (ToolboxZone)
	{
		ToolboxZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ToolboxZone->SetCollisionResponseToAllChannels(ECR_Ignore);
		ToolboxZone->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		ToolboxZone->SetGenerateOverlapEvents(false);
		ToolboxZone->OnClicked.AddDynamic(this, &AFabricationBase::OnToolboxZoneClicked);
	}

	if (MaterialBinZone)
	{
		MaterialBinZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MaterialBinZone->SetCollisionResponseToAllChannels(ECR_Ignore);
		MaterialBinZone->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		MaterialBinZone->SetGenerateOverlapEvents(false);
		MaterialBinZone->OnClicked.AddDynamic(this, &AFabricationBase::OnMaterialBinZoneClicked);
	}

	if (WorkSurfaceZone)
	{
		WorkSurfaceZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WorkSurfaceZone->SetCollisionResponseToAllChannels(ECR_Ignore);
		WorkSurfaceZone->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		WorkSurfaceZone->SetGenerateOverlapEvents(false);
		WorkSurfaceZone->OnClicked.AddDynamic(this, &AFabricationBase::OnWorkSurfaceZoneClicked);
	}

	// Initialize from data asset if available
	if (FabricationData)
	{
		InitializeFromData(FabricationData);
	}
}

void AFabricationBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	// Re-enable the interactable component
	if (InteractableComponent)
	{
		InteractableComponent->SetActive(true);
	}
}

void AFabricationBase::OnToolboxClicked_Implementation()
{
	// Flash the zone for visual feedback
	if (ToolboxZone)
	{
		FlashZone(ToolboxZone);
	}

	// Blueprint can implement UI opening logic
}

void AFabricationBase::OnMaterialBinClicked_Implementation()
{
	// Flash the zone for visual feedback
	if (MaterialBinZone)
	{
		FlashZone(MaterialBinZone);
	}

	// Blueprint can implement UI opening logic
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
// Visual Feedback Methods
// ============================================================================

void AFabricationBase::HighlightZone(UPrimitiveComponent* Zone)
{
	if (!Zone)
	{
		return;
	}

	// Store original materials if not already stored
	TArray<UMaterialInterface*>* OriginalMaterials = nullptr;
	if (Zone == ToolboxZone)
	{
		OriginalMaterials = &ToolboxOriginalMaterials;
	}
	else if (Zone == MaterialBinZone)
	{
		OriginalMaterials = &MaterialBinOriginalMaterials;
	}
	else if (Zone == WorkSurfaceZone)
	{
		OriginalMaterials = &WorkSurfaceOriginalMaterials;
	}

	if (OriginalMaterials && OriginalMaterials->Num() == 0)
	{
		// Store original materials
		int32 NumMaterials = Zone->GetNumMaterials();
		for (int32 i = 0; i < NumMaterials; i++)
		{
			OriginalMaterials->Add(Zone->GetMaterial(i));
		}
	}

	// Apply highlight material if available
	if (FabricationData && FabricationData->ZoneHoverMaterial)
	{
		int32 NumMaterials = Zone->GetNumMaterials();
		for (int32 i = 0; i < NumMaterials; i++)
		{
			Zone->SetMaterial(i, FabricationData->ZoneHoverMaterial);
		}
	}
}

void AFabricationBase::UnhighlightZone(UPrimitiveComponent* Zone)
{
	if (!Zone)
	{
		return;
	}

	// Get the appropriate original materials array
	TArray<UMaterialInterface*>* OriginalMaterials = nullptr;
	if (Zone == ToolboxZone)
	{
		OriginalMaterials = &ToolboxOriginalMaterials;
	}
	else if (Zone == MaterialBinZone)
	{
		OriginalMaterials = &MaterialBinOriginalMaterials;
	}
	else if (Zone == WorkSurfaceZone)
	{
		OriginalMaterials = &WorkSurfaceOriginalMaterials;
	}

	// Restore original materials
	if (OriginalMaterials && OriginalMaterials->Num() > 0)
	{
		for (int32 i = 0; i < OriginalMaterials->Num(); i++)
		{
			Zone->SetMaterial(i, (*OriginalMaterials)[i]);
		}
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
