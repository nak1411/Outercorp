// DestructibleMeshActor.cpp

#include "DestructibleMeshActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"

ADestructibleMeshActor::ADestructibleMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create geometry collection component
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	SetRootComponent(GeometryCollectionComponent);

	// Default settings
	DespawnTime = 30.0f;
}

void ADestructibleMeshActor::BeginPlay()
{
	Super::BeginPlay();

	SetupDespawnTimer();
}

void ADestructibleMeshActor::SetupDespawnTimer()
{
	if (DespawnTime <= 0.0f)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().SetTimer(
		DespawnTimerHandle,
		FTimerDelegate::CreateLambda([this]()
		{
			if (this && !IsActorBeingDestroyed())
			{
				Destroy();
			}
		}),
		DespawnTime,
		false
	);
}
