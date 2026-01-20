// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractableComponent.h"
#include "Components/MeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Default values
	InteractionName = FText::FromString(TEXT("Interactable Object"));
	InteractionPrompt = FText::FromString(TEXT("Press E to Interact"));
	InteractionRange = 500.0f; // Default interaction range - can be overridden in Blueprint
	bShouldHighlight = true;
	bCanInteract = true;
	HighlightColor = FLinearColor::Yellow;
	HighlightIntensity = 3.0f;
	bIsHighlighted = false;
	HighlightOverlayMaterial = nullptr;
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractableComponent::OnLookAt_Implementation(AActor* LookingActor)
{
	if (bShouldHighlight)
	{
		SetHighlighted(true);
	}

	OnLookedAt.Broadcast(LookingActor);
}

void UInteractableComponent::OnLookAway_Implementation(AActor* LookingActor)
{
	if (bShouldHighlight)
	{
		SetHighlighted(false);
	}

	OnLookedAway.Broadcast(LookingActor);
}

void UInteractableComponent::OnInteract_Implementation(AActor* InteractingActor)
{
	OnInteracted.Broadcast(InteractingActor);
}

FText UInteractableComponent::GetInteractionName_Implementation() const
{
	return InteractionName;
}

FText UInteractableComponent::GetInteractionPrompt_Implementation() const
{
	return InteractionPrompt;
}

bool UInteractableComponent::CanInteract_Implementation(AActor* InteractingActor) const
{
	return bCanInteract;
}

float UInteractableComponent::GetInteractionRange_Implementation() const
{
	return InteractionRange;
}

bool UInteractableComponent::ShouldHighlight_Implementation() const
{
	return bShouldHighlight;
}

void UInteractableComponent::SetHighlighted(bool bHighlighted)
{
	if (bIsHighlighted == bHighlighted)
	{
		return;
	}

	bIsHighlighted = bHighlighted;

	UMeshComponent* Mesh = GetMeshComponent();
	if (!Mesh)
	{
		return;
	}

	if (bHighlighted)
	{
		Mesh->SetRenderCustomDepth(true);
		Mesh->SetCustomDepthStencilValue(1);
	}
	else
	{
		Mesh->SetRenderCustomDepth(false);
		Mesh->SetCustomDepthStencilValue(0);
	}
}

UMeshComponent* UInteractableComponent::GetMeshComponent() const
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	// Try to find a mesh component on the owner
	// First try static mesh
	if (UStaticMeshComponent* StaticMesh = Owner->FindComponentByClass<UStaticMeshComponent>())
	{
		return StaticMesh;
	}

	// Then try skeletal mesh
	if (USkeletalMeshComponent* SkeletalMesh = Owner->FindComponentByClass<USkeletalMeshComponent>())
	{
		return SkeletalMesh;
	}

	// Fallback to any mesh component
	return Owner->FindComponentByClass<UMeshComponent>();
}
