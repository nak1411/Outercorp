// Copyright Epic Games, Inc. All Rights Reserved.

#include "PickupableItem.h"
#include "InteractableComponent.h"
#include "InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

APickupableItem::APickupableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Create mesh component
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(Root);
	// Default collision - allow Blueprint to override, but ensure visibility channel works
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ItemMesh->SetGenerateOverlapEvents(false);

	// Create interactable component
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	// Don't set InteractionRange here - let Blueprint defaults apply

	// Default values
	Quantity = 1;
}

void APickupableItem::BeginPlay()
{
	Super::BeginPlay();

	// Ensure ECC_Visibility is always enabled for interaction, but don't override other collision settings
	if (ItemMesh)
	{
		ItemMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	// Validate quantity against item data's max stack size
	if (ItemData)
	{
		// Clamp quantity to max stack size
		if (Quantity > ItemData->MaxStackSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("PickupableItem '%s' has Quantity %d which exceeds MaxStackSize %d. Clamping to MaxStackSize."),
				*ItemData->ItemName.ToString(), Quantity, ItemData->MaxStackSize);
			Quantity = ItemData->MaxStackSize;
		}

		// Update interactable component
		if (InteractableComponent)
		{
			InteractableComponent->InteractionName = ItemData->ItemName;
			InteractableComponent->InteractionPrompt = FText::Format(
				FText::FromString(TEXT("Pick Up(E)")),
				ItemData->ItemName);
			InteractableComponent->InteractionRange = ItemData->InteractionRange;
		}
	}
}


void APickupableItem::OnLookAt_Implementation(AActor *LookingActor)
{
	// Don't show highlight during drop cooldown
	if (bIsInDropCooldown)
	{
		return;
	}

	// Delegate to InteractableComponent if it exists
	if (InteractableComponent)
	{
		IInteractableInterface::Execute_OnLookAt(InteractableComponent, LookingActor);
	}
}

void APickupableItem::OnLookAway_Implementation(AActor *LookingActor)
{
	// Delegate to InteractableComponent if it exists
	if (InteractableComponent)
	{
		IInteractableInterface::Execute_OnLookAway(InteractableComponent, LookingActor);
	}
}

void APickupableItem::OnInteract_Implementation(AActor *InteractingActor)
{
	if (!ItemData || !InteractingActor)
	{
		return;
	}

	// Try to find inventory component on the interacting actor
	UInventoryComponent *InventoryComponent = InteractingActor->FindComponentByClass<UInventoryComponent>();
	if (!InventoryComponent)
	{
		return;
	}

	// Try to add item to inventory
	int32 OutSlotIndex = -1;
	bool bSuccess = InventoryComponent->AddItem(ItemData, Quantity, OutSlotIndex);

	if (bSuccess)
	{
		// Play pickup sound
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}

		// Spawn pickup particle
		if (PickupParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupParticle, GetActorLocation());
		}

		// Call OnPickedUp event
		OnPickedUp(InteractingActor);

		// Destroy this actor
		Destroy();
	}
	else
	{
		// Could add feedback here (e.g., "Inventory Full" message)
		UE_LOG(LogTemp, Warning, TEXT("Failed to add item to inventory - inventory may be full"));
	}
}

FText APickupableItem::GetInteractionName_Implementation() const
{
	if (InteractableComponent)
	{
		return IInteractableInterface::Execute_GetInteractionName(InteractableComponent);
	}

	if (ItemData)
	{
		return ItemData->ItemName;
	}

	return FText::FromString(TEXT("Item"));
}

FText APickupableItem::GetInteractionPrompt_Implementation() const
{
	if (InteractableComponent)
	{
		return IInteractableInterface::Execute_GetInteractionPrompt(InteractableComponent);
	}

	if (ItemData)
	{
		return FText::Format(
			FText::FromString(TEXT("Press E to Pick Up {0}")),
			ItemData->ItemName);
	}

	return FText::FromString(TEXT("Press E to Pick Up"));
}

bool APickupableItem::CanInteract_Implementation(AActor *InteractingActor) const
{
	// Cannot interact during drop cooldown
	if (bIsInDropCooldown)
	{
		return false;
	}

	if (!ItemData || !InteractingActor)
	{
		return false;
	}

	// Check if actor has inventory component
	UInventoryComponent *InventoryComponent = InteractingActor->FindComponentByClass<UInventoryComponent>();
	if (!InventoryComponent)
	{
		return false;
	}

	// Check if inventory can accept this item
	return InventoryComponent->CanAddItem(ItemData, Quantity);
}

float APickupableItem::GetInteractionRange_Implementation() const
{
	if (InteractableComponent)
	{
		return IInteractableInterface::Execute_GetInteractionRange(InteractableComponent);
	}

	// Shorter interaction range for pickup items
	return 150.0f;
}

bool APickupableItem::ShouldHighlight_Implementation() const
{
	// Don't highlight during drop cooldown
	if (bIsInDropCooldown)
	{
		return false;
	}

	if (InteractableComponent)
	{
		return IInteractableInterface::Execute_ShouldHighlight(InteractableComponent);
	}

	return true;
}

void APickupableItem::OnPickedUp_Implementation(AActor *PickedUpBy)
{
	// Blueprint implementable event
	// Can be overridden in Blueprint for custom behavior
}

void APickupableItem::InitializeItem(UInventoryItemData *InItemData, int32 InQuantity)
{
	ItemData = InItemData;

	if (ItemData)
	{
		// Clamp quantity to be at least 1 and at most MaxStackSize
		Quantity = FMath::Clamp(InQuantity, 1, ItemData->MaxStackSize);

		if (InQuantity > ItemData->MaxStackSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("InitializeItem: Requested quantity %d exceeds MaxStackSize %d for item '%s'. Clamping to MaxStackSize."),
				InQuantity, ItemData->MaxStackSize, *ItemData->ItemName.ToString());
		}

		// Update mesh if specified in item data
		if (ItemData->ItemMesh.Get() && ItemMesh)
		{
			ItemMesh->SetStaticMesh(ItemData->ItemMesh.Get());
		}

		// Update interactable component
		if (InteractableComponent)
		{
			InteractableComponent->InteractionName = ItemData->ItemName;
			InteractableComponent->InteractionPrompt = FText::Format(
				FText::FromString(TEXT("Press E to Pick Up {0}")),
				ItemData->ItemName);
			InteractableComponent->InteractionRange = ItemData->InteractionRange;
		}
	}
	else
	{
		Quantity = FMath::Max(1, InQuantity);
	}
}

void APickupableItem::StartDropCooldown()
{
	bIsInDropCooldown = true;

	// Clear any existing timer
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DropCooldownTimerHandle);

		// Start cooldown timer
		World->GetTimerManager().SetTimer(
			DropCooldownTimerHandle,
			this,
			&APickupableItem::OnDropCooldownComplete,
			DropCooldownTime,
			false
		);

		UE_LOG(LogTemp, Log, TEXT("Item '%s' dropped - cooldown for %.1f seconds"),
			ItemData ? *ItemData->ItemName.ToString() : TEXT("Unknown"), DropCooldownTime);
	}
}

void APickupableItem::OnDropCooldownComplete()
{
	bIsInDropCooldown = false;
	UE_LOG(LogTemp, Log, TEXT("Item '%s' is now interactable"), ItemData ? *ItemData->ItemName.ToString() : TEXT("Unknown"));
}
