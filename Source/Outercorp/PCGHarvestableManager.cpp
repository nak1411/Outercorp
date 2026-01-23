// PCGHarvestableManager.cpp

#include "PCGHarvestableManager.h"
#include "HarvestableResourceActor.h"
#include "HarvestableResourceData.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"

// Console variable for debug visualization
static TAutoConsoleVariable<int32> CVarPCGManagerDebug(
	TEXT("Harvesting.Debug.PCGManager"),
	0,
	TEXT("Show PCG Manager debug visualization (instance conversion tracking).\n")
		TEXT("0: Disabled (default)\n")
			TEXT("1: Enabled"),
	ECVF_Default);

APCGHarvestableManager::APCGHarvestableManager()
{
	PrimaryActorTick.bCanEverTick = true;

	UpdateInterval = 0.25f;
	MaxConversionsPerTick = 5;
	DespawnDistance = 1500.0f;
	bAllowReconversion = true;
	bDrawDebug = false;
	UpdateTimer = 0.0f;
}

void APCGHarvestableManager::BeginPlay()
{
	Super::BeginPlay();

	// Don't discover immediately - PCG components may not exist yet
	// Discovery will happen on first tick to allow PCG to spawn components
	bInitialDiscoveryDone = false;
	RediscoveryTimer = 0.0f;

	// Cache player pawn
	CachedPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
}

void APCGHarvestableManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle initial discovery (delayed to allow PCG to spawn components)
	if (!bInitialDiscoveryDone)
	{
		DiscoverISMComponents();
		bInitialDiscoveryDone = true;
	}

	// Handle periodic re-discovery for dynamically spawned components
	if (RediscoveryInterval > 0.0f)
	{
		RediscoveryTimer += DeltaTime;
		if (RediscoveryTimer >= RediscoveryInterval)
		{
			RediscoveryTimer = 0.0f;
			DiscoverISMComponents();
		}
	}

	UpdateTimer += DeltaTime;
	if (UpdateTimer >= UpdateInterval)
	{
		UpdateTimer = 0.0f;
		PerformUpdate();
	}

	if (bDrawDebug)
	{
		DrawDebugInfo();
	}
}

void APCGHarvestableManager::RegisterISMComponent(UInstancedStaticMeshComponent *ISMComponent)
{
	if (!ISMComponent)
	{
		return;
	}

	// Check if already tracked
	for (const auto &Tracked : TrackedISMComponents)
	{
		if (Tracked.Get() == ISMComponent)
		{
			return;
		}
	}

	UStaticMesh *Mesh = ISMComponent->GetStaticMesh();
	/*
	UE_LOG(LogTemp, Log, TEXT("PCGHarvestableManager: Registered tracking for ISM component %s (Mesh: %s, Instances: %d)"),
		   *ISMComponent->GetName(),
		   Mesh ? *Mesh->GetName() : TEXT("None"),
		   ISMComponent->GetInstanceCount());
	*/

	TrackedISMComponents.Add(ISMComponent);
}

void APCGHarvestableManager::UnregisterISMComponent(UInstancedStaticMeshComponent *ISMComponent)
{
	TrackedISMComponents.RemoveAll([ISMComponent](const TWeakObjectPtr<UInstancedStaticMeshComponent> &Weak)
								   { return Weak.Get() == ISMComponent; });
}

void APCGHarvestableManager::DiscoverISMComponents()
{
	// Note: We don't empty TrackedISMComponents here to support incremental discovery
	// RegisterISMComponent() already checks for duplicates

	UWorld *World = GetWorld();
	if (!World)
	{
		return;
	}

	// Find all ISM components in the level
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor *Actor = *It;
		if (!Actor)
		{
			continue;
		}

		TArray<UInstancedStaticMeshComponent *> Components;
		Actor->GetComponents<UInstancedStaticMeshComponent>(Components);

		for (UInstancedStaticMeshComponent *ISMComp : Components)
		{
			if (!ISMComp)
			{
				continue;
			}

			// Check if this ISM has a mesh
			UStaticMesh *Mesh = ISMComp->GetStaticMesh();
			if (!Mesh)
			{
				continue;
			}

			// Check if we have a mapping for this mesh
			const FPCGResourceMapping *Mapping = GetMappingForMesh(Mesh);
			if (!Mapping)
			{
				/*
				if (CVarPCGManagerDebug.GetValueOnGameThread())
				{
					UE_LOG(LogTemp, Warning, TEXT("PCGHarvestableManager: Discovery skipped component %s (Mesh: %s) - No mapping found"),
						   *ISMComp->GetName(), Mesh ? *Mesh->GetName() : TEXT("None"));
				}
				*/
				continue;
			}

			// Check tags if specified
			if (ISMComponentTags.Num() > 0)
			{
				bool bHasMatchingTag = false;
				for (const FName &Tag : ISMComponentTags)
				{
					if (ISMComp->ComponentHasTag(Tag))
					{
						bHasMatchingTag = true;
						break;
					}
				}
				if (!bHasMatchingTag)
				{
					continue;
				}
			}

			// Enable collision on this HISM/ISM component if it's disabled or relies on complex collision that might be missing
			// PCG often disables collision for performance - we need it for interaction
			bool bNeedsCollisionFix = ISMComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision;

			// Also fix if it doesn't block Visibility channel (required for line traces)
			if (!bNeedsCollisionFix && ISMComp->GetCollisionResponseToChannel(ECC_Visibility) != ECR_Block)
			{
				bNeedsCollisionFix = true;
			}

			if (bAutoEnableCollision && bNeedsCollisionFix)
			{
				/*
				UE_LOG(LogTemp, Log, TEXT("PCGHarvestableManager: Fixing collision for ISM %s (Mesh: %s)"),
					   *ISMComp->GetName(), Mesh ? *Mesh->GetName() : TEXT("None"));
				*/

				ISMComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

				// Note: We don't clear all channels to Ignore anymore, as we might overwrite desired settings.
				// We just ensure Visibility is blocked.
				ISMComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

				// Force recreation of physics state so per-instance bodies are created
				// This is required for HitResult.Item to return valid instance indices
				ISMComp->RecreatePhysicsState();
			}

			RegisterISMComponent(ISMComp);
		}
	}
}

const FPCGResourceMapping *APCGHarvestableManager::GetMappingForMesh(UStaticMesh *Mesh) const
{
	if (!Mesh)
	{
		return nullptr;
	}

	for (const FPCGResourceMapping &Mapping : ResourceMappings)
	{
		if (Mapping.ResourceData && Mapping.ResourceData->ContainsMesh(Mesh))
		{
			return &Mapping;
		}
	}

	return nullptr;
}

bool APCGHarvestableManager::GetMappingForMeshBP(UStaticMesh *Mesh, FPCGResourceMapping &OutMapping) const
{
	const FPCGResourceMapping *Mapping = GetMappingForMesh(Mesh);
	if (Mapping)
	{
		OutMapping = *Mapping;
		return true;
	}
	return false;
}

AHarvestableResourceActor *APCGHarvestableManager::ConvertInstance(UInstancedStaticMeshComponent *ISMComponent, int32 InstanceIndex)
{
	if (!ISMComponent || InstanceIndex < 0 || InstanceIndex >= ISMComponent->GetInstanceCount())
	{
		UE_LOG(LogTemp, Warning, TEXT("ConvertInstance: Invalid ISM component or instance index %d (count: %d)"),
			   InstanceIndex, ISMComponent ? ISMComponent->GetInstanceCount() : -1);
		return nullptr;
	}

	// Get mesh and find mapping
	UStaticMesh *Mesh = ISMComponent->GetStaticMesh();
	const FPCGResourceMapping *Mapping = GetMappingForMesh(Mesh);
	if (!Mapping || !Mapping->ResourceData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConvertInstance: No mapping found for mesh %s"), Mesh ? *Mesh->GetName() : TEXT("null"));
		return nullptr;
	}

	// Get instance transform
	FTransform InstanceTransform;
	if (!ISMComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true))
	{
		UE_LOG(LogTemp, Warning, TEXT("ConvertInstance: Failed to get transform for instance %d"), InstanceIndex);
		return nullptr;
	}

	// Check if already converted
	uint64 InstanceHash = GenerateInstanceHash(ISMComponent, InstanceIndex);
	if (ConvertedInstances.Contains(InstanceHash))
	{
		UE_LOG(LogTemp, Warning, TEXT("ConvertInstance: Instance already in ConvertedInstances set (hash: %llu) at location %.0f, %.0f, %.0f"),
			   InstanceHash, InstanceTransform.GetLocation().X, InstanceTransform.GetLocation().Y, InstanceTransform.GetLocation().Z);
		return nullptr;
	}

	// Debug: Get player position before conversion
	FVector PlayerPosBefore = FVector::ZeroVector;
	if (CachedPlayerPawn.IsValid())
	{
		PlayerPosBefore = CachedPlayerPawn->GetActorLocation();
	}

	// Mark as converted FIRST to prevent double-conversion race conditions
	ConvertedInstances.Add(InstanceHash);

	// Spawn the actor FIRST before hiding the instance
	// This way if spawn fails, we can remove from converted set and try again
	AHarvestableResourceActor *SpawnedActor = SpawnResourceActor(*Mapping, ISMComponent, InstanceIndex, InstanceTransform);
	if (SpawnedActor)
	{
		// Hide the original ISM instance AFTER successful spawn
		// This prevents any collision overlap between ISM and spawned actor
		HideInstance(ISMComponent, InstanceIndex);

		// Track the spawned resource
		FSpawnedResourceInfo Info;
		Info.SpawnedActor = SpawnedActor;
		Info.SourceComponent = ISMComponent;
		Info.InstanceIndex = InstanceIndex;
		Info.OriginalTransform = InstanceTransform;
		Info.bInstanceHidden = true;
		Info.InstanceHash = InstanceHash; // Store the hash for later removal
		SpawnedResources.Add(Info);

		// Bind events
		SpawnedActor->OnResourceDepleted.AddDynamic(this, &APCGHarvestableManager::OnResourceDepleted);
		SpawnedActor->OnResourceRespawned.AddDynamic(this, &APCGHarvestableManager::OnResourceRespawned);

		// Debug: Check if player moved during conversion (toggleable via console OR property)
		bool bShowDebug = CVarPCGManagerDebug.GetValueOnGameThread() || bDrawDebug;
		if (bShowDebug && CachedPlayerPawn.IsValid())
		{
			FVector PlayerPosAfter = CachedPlayerPawn->GetActorLocation();
			float ZDelta = PlayerPosAfter.Z - PlayerPosBefore.Z;

			// Draw conversion location
			DrawDebugSphere(GetWorld(), InstanceTransform.GetLocation(), 100.0f, 12, FColor::Orange, false, 5.0f, 0, 3.0f);
			DrawDebugString(GetWorld(), InstanceTransform.GetLocation() + FVector(0, 0, 200.0f),
							FString::Printf(TEXT("CONVERTED\nPlayer Z Delta: %.1f"), ZDelta),
							nullptr, FColor::Orange, 5.0f);

			// If significant Z change, highlight it in red
			if (FMath::Abs(ZDelta) > 10.0f)
			{
				DrawDebugLine(GetWorld(), PlayerPosBefore, PlayerPosAfter, FColor::Red, false, 5.0f, 0, 5.0f);
				UE_LOG(LogTemp, Warning, TEXT("PCGHarvestableManager: Player Z moved by %.1f during conversion!"), ZDelta);
			}
		}
	}
	else
	{
		// Spawn failed - remove from converted set so it can be tried again
		ConvertedInstances.Remove(InstanceHash);
	}

	return SpawnedActor;
}

void APCGHarvestableManager::PerformUpdate()
{
	// Update player pawn reference
	if (!CachedPlayerPawn.IsValid())
	{
		CachedPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	}

	// Check for actors that need despawning
	CheckForDespawn();

	// Clean up stale weak pointers
	TrackedISMComponents.RemoveAll([](const TWeakObjectPtr<UInstancedStaticMeshComponent> &Weak)
								   { return !Weak.IsValid(); });

	SpawnedResources.RemoveAll([](const FSpawnedResourceInfo &Info)
							   { return !Info.SpawnedActor.IsValid(); });
}

void APCGHarvestableManager::CheckForDespawn()
{
	if (!CachedPlayerPawn.IsValid())
	{
		return;
	}

	FVector PlayerLocation = CachedPlayerPawn->GetActorLocation();

	for (int32 i = SpawnedResources.Num() - 1; i >= 0; i--)
	{
		FSpawnedResourceInfo &Info = SpawnedResources[i];

		if (!Info.SpawnedActor.IsValid())
		{
			continue;
		}

		AHarvestableResourceActor *Actor = Info.SpawnedActor.Get();

		// Don't despawn depleted resources (they're waiting to respawn)
		if (Actor->bIsDepleted)
		{
			continue;
		}

		float Distance = FVector::Dist(PlayerLocation, Actor->GetActorLocation());
		if (Distance > DespawnDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("PCGHarvestableManager: Despawning resource at distance %.0f (> %.0f)"), Distance, DespawnDistance);

			// Restore the ISM instance
			if (Info.SourceComponent.IsValid() && Info.bInstanceHidden)
			{
				RestoreInstance(Info.SourceComponent.Get(), Info.InstanceIndex, Info.OriginalTransform);

				// Remove from converted set if we allow reconversion
				if (bAllowReconversion)
				{
					ConvertedInstances.Remove(Info.InstanceHash);
					UE_LOG(LogTemp, Warning, TEXT("PCGHarvestableManager: Removed hash %llu from ConvertedInstances"), Info.InstanceHash);
				}
			}

			// Destroy the actor
			Actor->Destroy();
			SpawnedResources.RemoveAt(i);
		}
	}
}

AHarvestableResourceActor *APCGHarvestableManager::SpawnResourceActor(const FPCGResourceMapping &Mapping, UInstancedStaticMeshComponent *ISMComponent, int32 InstanceIndex, const FTransform &InstanceTransform)
{
	UWorld *World = GetWorld();
	if (!World || !Mapping.ResourceData || !ISMComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnResourceActor: Invalid world/mapping/component"));
		return nullptr;
	}

	// Get the mesh entry for per-mesh settings
	UStaticMesh *SourceMesh = ISMComponent->GetStaticMesh();
	const FHarvestableSourceMesh *MeshEntry = Mapping.ResourceData->GetSourceMeshEntry(SourceMesh);

	// Determine actor class to spawn from mesh entry
	TSubclassOf<AHarvestableResourceActor> ActorClass = MeshEntry ? MeshEntry->ActorClass : nullptr;
	if (!ActorClass)
	{
		ActorClass = AHarvestableResourceActor::StaticClass();
	}

	// Spawn the actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AHarvestableResourceActor *SpawnedActor = World->SpawnActor<AHarvestableResourceActor>(
		ActorClass,
		InstanceTransform.GetLocation(),
		InstanceTransform.GetRotation().Rotator(),
		SpawnParams);

	if (SpawnedActor)
	{
		// Set scale
		SpawnedActor->SetActorScale3D(InstanceTransform.GetScale3D());

		// Initialize from PCG instance
		if (!Mapping.ResourceData)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnResourceActor: ResourceData is null after spawn"));
			SpawnedActor->Destroy();
			return nullptr;
		}

		SpawnedActor->InitializeFromPCGInstance(Mapping.ResourceData, ISMComponent, InstanceIndex, InstanceTransform);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnResourceActor: World->SpawnActor returned null for class %s at location %.0f, %.0f, %.0f"),
			   *ActorClass->GetName(), InstanceTransform.GetLocation().X, InstanceTransform.GetLocation().Y, InstanceTransform.GetLocation().Z);
	}

	return SpawnedActor;
}

void APCGHarvestableManager::HideInstance(UInstancedStaticMeshComponent *ISMComponent, int32 InstanceIndex)
{
	if (!ISMComponent || InstanceIndex < 0 || InstanceIndex >= ISMComponent->GetInstanceCount())
	{
		return;
	}

	// Check if this is a HISM component (PCG typically uses these)
	UHierarchicalInstancedStaticMeshComponent *HISMComp = Cast<UHierarchicalInstancedStaticMeshComponent>(ISMComponent);

	if (HISMComp)
	{
		// For HISM, use RemoveInstances which properly handles the tree structure
		TArray<int32> IndicesToRemove;
		IndicesToRemove.Add(InstanceIndex);
		HISMComp->RemoveInstances(IndicesToRemove);
	}
	else
	{
		// For regular ISM, use RemoveInstance
		ISMComponent->RemoveInstance(InstanceIndex);
	}
}

void APCGHarvestableManager::RestoreInstance(UInstancedStaticMeshComponent *ISMComponent, int32 InstanceIndex, const FTransform &OriginalTransform)
{
	if (!ISMComponent)
	{
		return;
	}

	// Add a new instance at the original transform
	// Since we removed the instance, we add it back fresh
	ISMComponent->AddInstance(OriginalTransform, true);
}

uint64 APCGHarvestableManager::GenerateInstanceHash(UInstancedStaticMeshComponent *ISMComponent, int32 InstanceIndex) const
{
	// Use transform location as unique identifier since instance indices shift when instances are removed
	// This ensures we track unique world positions rather than volatile indices
	FTransform InstanceTransform;
	if (ISMComponent && ISMComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true))
	{
		FVector Location = InstanceTransform.GetLocation();
		// Quantize to avoid floating point precision issues (1cm precision)
		int32 X = FMath::RoundToInt(Location.X);
		int32 Y = FMath::RoundToInt(Location.Y);
		int32 Z = FMath::RoundToInt(Location.Z);

		uint64 Hash = static_cast<uint64>(X);
		Hash ^= static_cast<uint64>(Y) << 21;
		Hash ^= static_cast<uint64>(Z) << 42;
		return Hash;
	}

	// Fallback to old method if transform unavailable
	uint64 ComponentHash = reinterpret_cast<uint64>(ISMComponent);
	uint64 IndexHash = static_cast<uint64>(InstanceIndex);
	return ComponentHash ^ (IndexHash << 32);
}

void APCGHarvestableManager::OnResourceDepleted(AHarvestableResourceActor *Resource)
{
	if (!Resource)
	{
		return;
	}

	// Find the spawned resource info
	for (FSpawnedResourceInfo &Info : SpawnedResources)
	{
		if (Info.SpawnedActor.Get() == Resource)
		{
			// Resource is depleted - it will handle its own respawn timer
			// The ISM instance stays hidden until the resource respawns or is destroyed
			break;
		}
	}
}

void APCGHarvestableManager::OnResourceRespawned(AHarvestableResourceActor *Resource)
{
	// Resource has respawned - nothing special needed
	// The actor is still valid and the ISM instance is still hidden
}

void APCGHarvestableManager::DrawDebugInfo()
{
	if (!CachedPlayerPawn.IsValid())
	{
		return;
	}

	UWorld *World = GetWorld();
	if (!World)
	{
		return;
	}

	FVector PlayerLocation = CachedPlayerPawn->GetActorLocation();

	// Draw despawn range
	DrawDebugSphere(World, PlayerLocation, DespawnDistance, 16, FColor::Red, false, UpdateInterval);

	// Draw lines to spawned resources
	for (const FSpawnedResourceInfo &Info : SpawnedResources)
	{
		if (Info.SpawnedActor.IsValid())
		{
			FColor LineColor = Info.SpawnedActor->bIsDepleted ? FColor::Orange : FColor::Cyan;
			DrawDebugLine(World, PlayerLocation, Info.SpawnedActor->GetActorLocation(), LineColor, false, UpdateInterval);
		}
	}
}
