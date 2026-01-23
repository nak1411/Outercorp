// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractionManagerComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "PCGHarvestableManager.h"
#include "HarvestableResourceData.h"
#include "HarvestableResourceActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

UInteractionManagerComponent::UInteractionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Default values
	InteractionTraceDistance = 500.0f;
	InteractionCheckInterval = 0.1f;
	InteractionTraceChannel = ECC_Visibility;
	InteractionTraceRadius = 0.0f;
	bDrawDebugTrace = false;
	InteractionCheckTimer = 0.0f;

	// ISM/Harvestable defaults
	PendingInstanceIndex = INDEX_NONE;
	bLookingAtHarvestableISM = false;
	ISMHighlightMesh = nullptr;
	HighlightedInstanceIndex = INDEX_NONE;
	HighlightOverlayMaterial = nullptr;
}

void UInteractionManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ISMHighlightMesh)
	{
		ISMHighlightMesh->DestroyComponent();
		ISMHighlightMesh = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void UInteractionManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractionCheckTimer += DeltaTime;

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

	UCameraComponent* Camera = OwnerActor->FindComponentByClass<UCameraComponent>();
	if (!Camera)
	{
		return;
	}

	FVector StartLocation = Camera->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * InteractionTraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);

	bool bHit = false;
	if (InteractionTraceRadius > 0.0f)
	{
		// Sphere sweep for more precise control
		FCollisionShape SphereShape = FCollisionShape::MakeSphere(InteractionTraceRadius);
		bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			FQuat::Identity,
			InteractionTraceChannel,
			SphereShape,
			QueryParams
		);
	}
	else
	{
		// Standard line trace
		bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			InteractionTraceChannel,
			QueryParams
		);
	}

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
	bool bFoundHarvestableISM = false;

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		if (HitActor)
		{
			// Skip PCG partition actors for regular interactable checks
			bool bIsPCGActor = HitActor->GetClass()->GetName().Contains(TEXT("PCG"));

			// Check if the hit actor implements IInteractableInterface
			if (!bIsPCGActor && HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
			{
				IInteractableInterface* Interactable = Cast<IInteractableInterface>(HitActor);
				if (Interactable)
				{
					float InteractionRange = IInteractableInterface::Execute_GetInteractionRange(HitActor);
					float Distance = FVector::Dist(StartLocation, HitResult.ImpactPoint);

					if (Distance <= InteractionRange)
					{
						if (IInteractableInterface::Execute_CanInteract(HitActor, OwnerActor))
						{
							NewInteractable = HitActor;
						}
					}
				}
			}
			else if (!bIsPCGActor)
			{
				// Check components for interactable interface
				TArray<UActorComponent*> Components = HitActor->GetComponentsByInterface(UInteractableInterface::StaticClass());
				for (UActorComponent* Component : Components)
				{
					IInteractableInterface* Interactable = Cast<IInteractableInterface>(Component);
					if (Interactable)
					{
						float InteractionRange = IInteractableInterface::Execute_GetInteractionRange(Component);
						float Distance = FVector::Dist(StartLocation, HitResult.ImpactPoint);

						if (Distance <= InteractionRange)
						{
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

		// If no regular interactable found, check for ISM harvestable
		if (!NewInteractable.GetObject() && HitComponent)
		{
			UInstancedStaticMeshComponent* ISMComp = Cast<UInstancedStaticMeshComponent>(HitComponent);
			if (ISMComp)
			{
				APCGHarvestableManager* Manager = GetHarvestableManager();
				if (Manager)
				{
					UStaticMesh* Mesh = ISMComp->GetStaticMesh();
					const FPCGResourceMapping* Mapping = Manager->GetMappingForMesh(Mesh);

					if (Mapping && Mapping->ResourceData)
					{
						float Distance = FVector::Dist(StartLocation, HitResult.ImpactPoint);

						if (Distance <= Mapping->ResourceData->InteractionRange)
						{
							int32 FoundInstanceIndex = HitResult.Item;

							if (FoundInstanceIndex != INDEX_NONE)
							{
								bFoundHarvestableISM = true;
								PendingISMComponent = ISMComp;
								PendingInstanceIndex = FoundInstanceIndex;
								PendingISMHitLocation = HitResult.ImpactPoint;
							}
						}
					}
				}
			}
		}
	}

	// Update ISM state
	if (bFoundHarvestableISM)
	{
		UpdateISMHighlight(PendingISMComponent.Get(), PendingInstanceIndex);
		bLookingAtHarvestableISM = true;
	}
	else
	{
		// Clear ISM state whenever we're not looking at a harvestable ISM
		PendingISMComponent = nullptr;
		PendingInstanceIndex = INDEX_NONE;
		ClearISMHighlight();
		bLookingAtHarvestableISM = false;
	}

	// Also clear ISM highlight if we found a regular interactable (converted actor)
	// This handles the case where we just converted an ISM to an actor
	if (NewInteractable.GetObject())
	{
		ClearISMHighlight();
	}

	SetCurrentInteractable(NewInteractable);
}

void UInteractionManagerComponent::SetCurrentInteractable(TScriptInterface<IInteractableInterface> NewInteractable)
{
	if (CurrentInteractable == NewInteractable)
	{
		return;
	}

	AActor* OwnerActor = GetOwner();

	if (CurrentInteractable.GetObject())
	{
		IInteractableInterface::Execute_OnLookAway(CurrentInteractable.GetObject(), OwnerActor);
	}

	PreviousInteractable = CurrentInteractable;
	CurrentInteractable = NewInteractable;

	if (CurrentInteractable.GetObject())
	{
		IInteractableInterface::Execute_OnLookAt(CurrentInteractable.GetObject(), OwnerActor);
	}
}

void UInteractionManagerComponent::Interact()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	// First check regular interactable (non-harvestable)
	if (CurrentInteractable.GetObject())
	{
		// Skip direct pickup harvestables - they should only be pickable via E key
		if (AHarvestableResourceActor* Harvestable = Cast<AHarvestableResourceActor>(CurrentInteractable.GetObject()))
		{
			if (Harvestable->bIsDirectPickup)
			{
				return; // Don't allow left-click harvesting for direct pickup items
			}
		}

		if (IInteractableInterface::Execute_CanInteract(CurrentInteractable.GetObject(), OwnerActor))
		{
			IInteractableInterface::Execute_OnInteract(CurrentInteractable.GetObject(), OwnerActor);
		}
		return;
	}

	// Check for harvestable ISM instance
	if (bLookingAtHarvestableISM && PendingISMComponent.IsValid() && PendingInstanceIndex != INDEX_NONE)
	{
		APCGHarvestableManager* Manager = GetHarvestableManager();
		if (Manager)
		{
			// Check if this is a direct pickup type before converting
			UStaticMesh* Mesh = PendingISMComponent->GetStaticMesh();
			const FPCGResourceMapping* Mapping = Manager->GetMappingForMesh(Mesh);

			if (Mapping && Mapping->ResourceData)
			{
				const FHarvestableSourceMesh* MeshEntry = Mapping->ResourceData->GetSourceMeshEntry(Mesh);
				if (MeshEntry && MeshEntry->bDirectPickup)
				{
					return; // Don't allow left-click harvesting for direct pickup items
				}
			}

			AHarvestableResourceActor* SpawnedActor = Manager->ConvertInstance(PendingISMComponent.Get(), PendingInstanceIndex);
			if (SpawnedActor)
			{
				IInteractableInterface::Execute_OnInteract(SpawnedActor, OwnerActor);

				PendingISMComponent = nullptr;
				PendingInstanceIndex = INDEX_NONE;
				bLookingAtHarvestableISM = false;
				ClearISMHighlight();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Interact: ConvertInstance failed for ISM %s, Index %d"),
					*PendingISMComponent->GetName(), PendingInstanceIndex);
				// Clear pending state so we don't keep trying to convert this instance
				PendingISMComponent = nullptr;
				PendingInstanceIndex = INDEX_NONE;
				bLookingAtHarvestableISM = false;
				ClearISMHighlight();
			}
		}
	}
	else if (bLookingAtHarvestableISM)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact: ISM interaction skipped - Component Valid: %d, InstanceIndex: %d"),
			PendingISMComponent.IsValid() ? 1 : 0, PendingInstanceIndex);
	}
}

void UInteractionManagerComponent::InteractNonHarvestable()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	// Check if we're looking at a direct pickup PCG harvestable ISM instance
	if (bLookingAtHarvestableISM && PendingISMComponent.IsValid() && PendingInstanceIndex != INDEX_NONE)
	{
		APCGHarvestableManager* Manager = GetHarvestableManager();
		if (Manager)
		{
			// Get the resource data for this ISM
			UStaticMesh* Mesh = PendingISMComponent->GetStaticMesh();
			const FPCGResourceMapping* Mapping = Manager->GetMappingForMesh(Mesh);

			if (Mapping && Mapping->ResourceData)
			{
				// Check if this is a direct pickup type by looking at source mesh settings
				const FHarvestableSourceMesh* MeshEntry = Mapping->ResourceData->GetSourceMeshEntry(Mesh);
				if (MeshEntry && MeshEntry->bDirectPickup)
				{
					// Convert and interact with the direct pickup harvestable
					AHarvestableResourceActor* SpawnedActor = Manager->ConvertInstance(PendingISMComponent.Get(), PendingInstanceIndex);
					if (SpawnedActor)
					{
						IInteractableInterface::Execute_OnInteract(SpawnedActor, OwnerActor);
						PendingISMComponent = nullptr;
						PendingInstanceIndex = INDEX_NONE;
						bLookingAtHarvestableISM = false;
						ClearISMHighlight();
					}
				}
			}
		}
		return;
	}

	// Check if we're looking at a direct pickup harvestable actor
	if (CurrentInteractable.GetObject())
	{
		// Try to cast to harvestable resource actor
		if (AHarvestableResourceActor* Harvestable = Cast<AHarvestableResourceActor>(CurrentInteractable.GetObject()))
		{
			// If this is a direct pickup item, allow interaction via E key
			if (Harvestable->bIsDirectPickup && Harvestable->DirectPickupItem)
			{
				if (IInteractableInterface::Execute_CanInteract(CurrentInteractable.GetObject(), OwnerActor))
				{
					IInteractableInterface::Execute_OnInteract(CurrentInteractable.GetObject(), OwnerActor);
				}
			}
			// Don't interact with non-direct-pickup harvestables via E key
			return;
		}

		// Interact with regular non-harvestable interactables only
		if (IInteractableInterface::Execute_CanInteract(CurrentInteractable.GetObject(), OwnerActor))
		{
			IInteractableInterface::Execute_OnInteract(CurrentInteractable.GetObject(), OwnerActor);
		}
	}
}

AActor* UInteractionManagerComponent::GetCurrentInteractableActor() const
{
	if (!CurrentInteractable.GetObject())
	{
		return nullptr;
	}

	if (AActor* Actor = Cast<AActor>(CurrentInteractable.GetObject()))
	{
		return Actor;
	}

	if (UActorComponent* Component = Cast<UActorComponent>(CurrentInteractable.GetObject()))
	{
		return Component->GetOwner();
	}

	return nullptr;
}

FText UInteractionManagerComponent::GetCurrentInteractionName() const
{
	if (CurrentInteractable.GetObject())
	{
		return IInteractableInterface::Execute_GetInteractionName(CurrentInteractable.GetObject());
	}

	if (bLookingAtHarvestableISM)
	{
		return GetPendingHarvestableName();
	}

	return FText::GetEmpty();
}

FText UInteractionManagerComponent::GetCurrentInteractionPrompt() const
{
	if (CurrentInteractable.GetObject())
	{
		return IInteractableInterface::Execute_GetInteractionPrompt(CurrentInteractable.GetObject());
	}

	if (bLookingAtHarvestableISM && PendingISMComponent.IsValid())
	{
		APCGHarvestableManager* Manager = GetHarvestableManager();
		if (Manager)
		{
			UStaticMesh* Mesh = PendingISMComponent->GetStaticMesh();
			const FPCGResourceMapping* Mapping = Manager->GetMappingForMesh(Mesh);
			if (Mapping && Mapping->ResourceData)
			{
				if (!Mapping->ResourceData->InteractionPrompt.IsEmpty())
				{
					return Mapping->ResourceData->InteractionPrompt;
				}
				return FText::FromString(TEXT("Harvest"));
			}
		}
	}

	return FText::GetEmpty();
}

bool UInteractionManagerComponent::IsLookingAtInteractable() const
{
	return CurrentInteractable.GetObject() != nullptr || bLookingAtHarvestableISM;
}

APCGHarvestableManager* UInteractionManagerComponent::GetHarvestableManager() const
{
	if (CachedHarvestableManager.IsValid())
	{
		return CachedHarvestableManager.Get();
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(World, APCGHarvestableManager::StaticClass());
	if (FoundActor)
	{
		CachedHarvestableManager = Cast<APCGHarvestableManager>(FoundActor);
		return CachedHarvestableManager.Get();
	}

	return nullptr;
}

FText UInteractionManagerComponent::GetPendingHarvestableName() const
{
	if (!bLookingAtHarvestableISM || !PendingISMComponent.IsValid())
	{
		return FText::GetEmpty();
	}

	APCGHarvestableManager* Manager = GetHarvestableManager();
	if (Manager)
	{
		UStaticMesh* Mesh = PendingISMComponent->GetStaticMesh();
		const FPCGResourceMapping* Mapping = Manager->GetMappingForMesh(Mesh);
		if (Mapping && Mapping->ResourceData)
		{
			const FHarvestableSourceMesh* MeshEntry = Mapping->ResourceData->GetSourceMeshEntry(Mesh);
			if (MeshEntry && !MeshEntry->DisplayName.IsEmpty())
			{
				return MeshEntry->DisplayName;
			}
		}
	}

	return FText::FromString(TEXT("Harvestable"));
}

void UInteractionManagerComponent::UpdateISMHighlight(UInstancedStaticMeshComponent* ISMComp, int32 InstanceIndex)
{
	if (!ISMComp || InstanceIndex == INDEX_NONE)
	{
		ClearISMHighlight();
		return;
	}

	if (InstanceIndex < 0 || InstanceIndex >= ISMComp->GetInstanceCount())
	{
		ClearISMHighlight();
		return;
	}

	FTransform InstanceTransform;
	if (!ISMComp->GetInstanceTransform(InstanceIndex, InstanceTransform, true))
	{
		ClearISMHighlight();
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!ISMHighlightMesh)
	{
		AActor* OwnerActor = GetOwner();
		if (!OwnerActor)
		{
			return;
		}

		ISMHighlightMesh = NewObject<UStaticMeshComponent>(OwnerActor, TEXT("ISMHighlightMesh"));
		if (ISMHighlightMesh)
		{
			ISMHighlightMesh->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			ISMHighlightMesh->RegisterComponent();
			ISMHighlightMesh->SetAbsolute(true, true, true);
			ISMHighlightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ISMHighlightMesh->SetCastShadow(false);
			ISMHighlightMesh->bReceivesDecals = false;
			ISMHighlightMesh->SetVisibility(false);
			ISMHighlightMesh->SetHiddenInGame(false);
			ISMHighlightMesh->SetRenderCustomDepth(true);
			ISMHighlightMesh->SetCustomDepthStencilValue(1);
		}
	}

	if (!ISMHighlightMesh)
	{
		return;
	}

	if (HighlightedISMComponent.Get() == ISMComp && HighlightedInstanceIndex == InstanceIndex && ISMHighlightMesh->IsVisible())
	{
		return;
	}

	UStaticMesh* Mesh = ISMComp->GetStaticMesh();
	if (Mesh != ISMHighlightMesh->GetStaticMesh())
	{
		ISMHighlightMesh->SetStaticMesh(Mesh);

		// Copy materials from ISM to preserve WPO animations
		for (int32 i = 0; i < ISMComp->GetNumMaterials(); i++)
		{
			ISMHighlightMesh->SetMaterial(i, ISMComp->GetMaterial(i));
		}
	}

	ISMHighlightMesh->SetWorldTransform(InstanceTransform);
	ISMHighlightMesh->SetVisibility(true);

	HighlightedISMComponent = ISMComp;
	HighlightedInstanceIndex = InstanceIndex;
}

void UInteractionManagerComponent::ClearISMHighlight()
{
	if (ISMHighlightMesh)
	{
		ISMHighlightMesh->SetVisibility(false);
	}

	HighlightedISMComponent = nullptr;
	HighlightedInstanceIndex = INDEX_NONE;
}

void UInteractionManagerComponent::ClearPendingISMState()
{
	PendingISMComponent = nullptr;
	PendingInstanceIndex = INDEX_NONE;
	bLookingAtHarvestableISM = false;
	ClearISMHighlight();
}
