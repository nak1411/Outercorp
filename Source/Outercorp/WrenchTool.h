// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquippableTool.h"
#include "WrenchTool.generated.h"

class AConstructionPart;
class UNotificationComponent;

/**
 * Interface for objects that can be connected/fastened with a wrench
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UConnectableInterface : public UInterface
{
	GENERATED_BODY()
};

class IConnectableInterface
{
	GENERATED_BODY()

public:
	/** Check if this object can be connected/fastened */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Connectable")
	bool CanBeConnected() const;
	virtual bool CanBeConnected_Implementation() const { return true; }

	/** Get the visual feedback text when looking at this connectable */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Connectable")
	FText GetConnectionPrompt() const;
	virtual FText GetConnectionPrompt_Implementation() const { return FText::FromString(TEXT("Connect")); }

	/** Called when wrench successfully connects/fastens this object */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Connectable")
	void OnConnected();
	virtual void OnConnected_Implementation() { /* Default implementation */ }

	/** Check if this object is already connected/fastened */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Connectable")
	bool IsConnected() const;
	virtual bool IsConnected_Implementation() const { return false; }

	/** Called when wrench disconnects/unfastens this object */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Connectable")
	void OnDisconnected();
	virtual void OnDisconnected_Implementation() { /* Default implementation */ }
};

/**
 * Wrench tool for connecting/fastening construction parts and other connectable objects
 */
UCLASS()
class OUTERCORP_API AWrenchTool : public AEquippableTool
{
	GENERATED_BODY()

public:
	AWrenchTool();

protected:
	virtual void BeginPlay() override;

public:
	/** Maximum range for wrench usage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrench")
	float WrenchRange;

	/** Time required to complete a connection (hold duration) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrench")
	float ConnectionTime;

	/** Sound played when connecting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrench|Feedback")
	class USoundBase* ConnectSound;

	/** Sound played when disconnecting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrench|Feedback")
	class USoundBase* DisconnectSound;

	/** Sound played while wrenching (looping) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrench|Feedback")
	class USoundBase* WrenchingSound;

	/** Particle effect when connecting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrench|Feedback")
	class UParticleSystem* ConnectEffect;

	/** Whether primary action connects and secondary disconnects (if false, both toggle) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrench")
	bool bSeparateConnectDisconnect;

	// Override tool actions
	virtual void PerformPrimaryAction_Implementation() override;
	virtual void PerformSecondaryAction_Implementation() override;
	virtual void StartPrimaryUse_Implementation() override;
	virtual void StopPrimaryUse_Implementation() override;

	/** Get the connectable object currently being looked at */
	UFUNCTION(BlueprintCallable, Category = "Wrench")
	AActor* GetTargetConnectable();

protected:
	/** Current target being looked at */
	UPROPERTY()
	AActor* CurrentTarget;

	/** Timer for connection progress */
	FTimerHandle ConnectionTimerHandle;

	/** Progress of current connection (0-1) */
	float ConnectionProgress;

	/** Audio component for looping wrench sound */
	UPROPERTY()
	class UAudioComponent* WrenchingSoundComponent;

	/** Perform raycast to find connectable objects */
	AActor* PerformWrenchRaycast();

	/** Start the connection process */
	void StartConnection();

	/** Complete the connection */
	void CompleteConnection();

	/** Cancel ongoing connection */
	void CancelConnection();

	/** Update connection progress (called each tick during connection) */
	void UpdateConnectionProgress();
};
