// DestructibleMeshActor.h
// Actor for handling destructible/fracture effects with geometry collections

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleMeshActor.generated.h"

class UGeometryCollection;
class UGeometryCollectionComponent;

/**
 * Actor that spawns and manages a destructible geometry collection
 * Used for fracture effects when harvesting resources
 */
UCLASS()
class OUTERCORP_API ADestructibleMeshActor : public AActor
{
	GENERATED_BODY()

public:
	ADestructibleMeshActor();

	virtual void BeginPlay() override;

	// ============================================
	// PROPERTIES
	// ============================================

	/** Time in seconds before this actor despawns (0 = never despawn) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible", meta = (ClampMin = "0.0"))
	float DespawnTime = 30.0f;

	// ============================================
	// COMPONENTS
	// ============================================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UGeometryCollectionComponent* GeometryCollectionComponent;

protected:
	/** Timer handle for despawn */
	FTimerHandle DespawnTimerHandle;

	/**
	 * Setup the despawn timer
	 */
	void SetupDespawnTimer();
};
