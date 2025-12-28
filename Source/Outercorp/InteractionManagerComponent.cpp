// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractionManagerComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

UInteractionManagerComponent::UInteractionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Default values
	InteractionTraceDistance = 500.0f; // 5 meters
	InteractionCheckInterval = 0.1f; // Check 10 times per second
	InteractionTraceChannel = ECC_Visibility;
	bDrawDebugTrace = false;
	InteractionCheckTimer = 0.0f;
}

void UInteractionManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update timer
	InteractionCheckTimer += DeltaTime;

	// Only check at intervals (or every frame if interval is 0)
	if (InteractionCheckInterval <= 0.0f || InteractionCheckTimer >= InteractionCheckInterval)
	{
		CheckForInteractables();
		InteractionCheckTimer = 0.0f;
	}
}

void UInteractionManagerComponent::CheckForInteractables()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	// Get camera for line trace
	UCameraComponent* Camera = OwnerActor->FindComponentByClass<UCameraComponent>();
	if (!Camera)
	{
		return;
	}

	// Get trace start and end points
	FVector StartLocation = Camera->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * InteractionTraceDistance);

	// Perform line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		InteractionTraceChannel,
		QueryParams
	);

	// Draw debug line if enabled
	if (bDrawDebugTrace)
	{
		FColor DebugColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, DebugColor, false, InteractionCheckInterval, 0, 2.0f);

		if (bHit)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 8, FColor::Yellow, false, InteractionCheckInterval);
		}
	}

	TScriptInterface<IInteractableInterface> NewInteractable = nullptr;

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			// Check if the hit actor implements IInteractableInterface
			if (HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
			{
				IInteractableInterface* Interactable = Cast<IInteractableInterface>(HitActor);
				if (Interactable)
				{
					// Check if we're within interaction range
					float InteractionRange = IInteractableInterface::Execute_GetInteractionRange(HitActor);
					float Distance = FVector::Dist(StartLocation, HitResult.ImpactPoint);

					if (Distance <= InteractionRange)
					{
						// Check if we can interact
						if (IInteractableInterface::Execute_CanInteract(HitActor, OwnerActor))
						{
							NewInteractable = HitActor;
						}
					}
				}
			}
			else
			{
				// Check components for interactable interface
				TArray<UActorComponent*> Components = HitActor->GetComponentsByInterface(UInteractableInterface::StaticClass());
				for (UActorComponent* Component : Components)
				{
					IInteractableInterface* Interactable = Cast<IInteractableInterface>(Component);
					if (Interactable)
					{
						// Check if we're within interaction range
						float InteractionRange = IInteractableInterface::Execute_GetInteractionRange(Component);
						float Distance = FVector::Dist(StartLocation, HitResult.ImpactPoint);

						if (Distance <= InteractionRange)
						{
							// Check if we can interact
							if (IInteractableInterface::Execute_CanInteract(Component, OwnerActor))
							{
								NewInteractable = Component;
								break;
							}
						}
					}
				}
			}
		}
	}

	// Update current interactable
	SetCurrentInteractable(NewInteractable);
}

void UInteractionManagerComponent::SetCurrentInteractable(TScriptInterface<IInteractableInterface> NewInteractable)
{
	// If the interactable hasn't changed, do nothing
	if (CurrentInteractable == NewInteractable)
	{
		return;
	}

	AActor* OwnerActor = GetOwner();

	// Call OnLookAway on the previous interactable
	if (CurrentInteractable.GetObject())
	{
		IInteractableInterface::Execute_OnLookAway(CurrentInteractable.GetObject(), OwnerActor);
	}

	// Update current interactable
	PreviousInteractable = CurrentInteractable;
	CurrentInteractable = NewInteractable;

	// Call OnLookAt on the new interactable
	if (CurrentInteractable.GetObject())
	{
		IInteractableInterface::Execute_OnLookAt(CurrentInteractable.GetObject(), OwnerActor);
	}
}

void UInteractionManagerComponent::Interact()
{
	if (!CurrentInteractable.GetObject())
	{
		return;
	}

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	// Check if we can still interact (state may have changed)
	if (IInteractableInterface::Execute_CanInteract(CurrentInteractable.GetObject(), OwnerActor))
	{
		IInteractableInterface::Execute_OnInteract(CurrentInteractable.GetObject(), OwnerActor);
	}
}

AActor* UInteractionManagerComponent::GetCurrentInteractableActor() const
{
	if (!CurrentInteractable.GetObject())
	{
		return nullptr;
	}

	// If it's an actor, return it directly
	if (AActor* Actor = Cast<AActor>(CurrentInteractable.GetObject()))
	{
		return Actor;
	}

	// If it's a component, return its owner
	if (UActorComponent* Component = Cast<UActorComponent>(CurrentInteractable.GetObject()))
	{
		return Component->GetOwner();
	}

	return nullptr;
}

FText UInteractionManagerComponent::GetCurrentInteractionName() const
{
	if (!CurrentInteractable.GetObject())
	{
		return FText::GetEmpty();
	}

	return IInteractableInterface::Execute_GetInteractionName(CurrentInteractable.GetObject());
}

FText UInteractionManagerComponent::GetCurrentInteractionPrompt() const
{
	if (!CurrentInteractable.GetObject())
	{
		return FText::GetEmpty();
	}

	return IInteractableInterface::Execute_GetInteractionPrompt(CurrentInteractable.GetObject());
}

bool UInteractionManagerComponent::IsLookingAtInteractable() const
{
	return CurrentInteractable.GetObject() != nullptr;
}
