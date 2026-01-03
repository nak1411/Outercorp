// Copyright Epic Games, Inc. All Rights Reserved.

#include "WrenchTool.h"
#include "OutercorpCharacter.h"
#include "NotificationComponent.h"
#include "ConstructionPart.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"

AWrenchTool::AWrenchTool()
{
	// Wrench-specific defaults
	WrenchRange = 300.0f;
	ConnectionTime = 2.0f;
	bRequiresContinuousHold = true;
	UsageCooldown = 0.5f;
	bSeparateConnectDisconnect = false;

	CurrentTarget = nullptr;
	ConnectionProgress = 0.0f;
	WrenchingSoundComponent = nullptr;

	ConnectSound = nullptr;
	DisconnectSound = nullptr;
	WrenchingSound = nullptr;
	ConnectEffect = nullptr;

	// Set up attachment
	AttachSocketName = FName("GripPoint");
}

void AWrenchTool::BeginPlay()
{
	Super::BeginPlay();
}

void AWrenchTool::StartPrimaryUse_Implementation()
{
	if (!CanUseTool())
	{
		return;
	}

	// Find target
	AActor* Target = PerformWrenchRaycast();
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wrench: No connectable target found"));
		return;
	}

	CurrentTarget = Target;
	bIsInUse = true;
	CurrentUsageMode = EToolUsageMode::Primary;

	// Start connection process
	StartConnection();
}

void AWrenchTool::StopPrimaryUse_Implementation()
{
	// Cancel if we were in progress
	if (bIsInUse && ConnectionProgress < 1.0f)
	{
		CancelConnection();
	}

	bIsInUse = false;
	CurrentUsageMode = EToolUsageMode::None;
	CurrentTarget = nullptr;
}

void AWrenchTool::PerformPrimaryAction_Implementation()
{
	// Find connectable target
	AActor* Target = GetTargetConnectable();
	if (!Target)
	{
		return;
	}

	// Check if it implements the connectable interface
	if (!Target->Implements<UConnectableInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Wrench: Target does not implement IConnectableInterface"));
		return;
	}

	// Check if it can be connected
	if (!IConnectableInterface::Execute_CanBeConnected(Target))
	{
		if (OwningCharacter)
		{
			UNotificationComponent* NotificationComp = OwningCharacter->GetNotificationComponent();
			if (NotificationComp)
			{
				NotificationComp->ShowNotification(FText::FromString(TEXT("Cannot connect this object")));
			}
		}
		return;
	}

	// Check current state
	bool bIsCurrentlyConnected = IConnectableInterface::Execute_IsConnected(Target);

	if (bSeparateConnectDisconnect)
	{
		// Primary action = connect only
		if (!bIsCurrentlyConnected)
		{
			// Perform connection
			IConnectableInterface::Execute_OnConnected(Target);

			// Play feedback
			if (ConnectSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ConnectSound, Target->GetActorLocation());
			}

			if (ConnectEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ConnectEffect, Target->GetActorLocation());
			}

			if (OwningCharacter)
			{
				UNotificationComponent* NotificationComp = OwningCharacter->GetNotificationComponent();
				if (NotificationComp)
				{
					NotificationComp->ShowNotification(FText::FromString(TEXT("Connected")));
				}
			}

			UE_LOG(LogTemp, Log, TEXT("Wrench: Connected %s"), *Target->GetName());
		}
		else
		{
			if (OwningCharacter)
			{
				UNotificationComponent* NotificationComp = OwningCharacter->GetNotificationComponent();
				if (NotificationComp)
				{
					NotificationComp->ShowNotification(FText::FromString(TEXT("Already connected")));
				}
			}
		}
	}
	else
	{
		// Toggle mode - connect or disconnect
		if (bIsCurrentlyConnected)
		{
			// Disconnect
			IConnectableInterface::Execute_OnDisconnected(Target);

			if (DisconnectSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DisconnectSound, Target->GetActorLocation());
			}

			if (OwningCharacter)
			{
				UNotificationComponent* NotificationComp = OwningCharacter->GetNotificationComponent();
				if (NotificationComp)
				{
					NotificationComp->ShowNotification(FText::FromString(TEXT("Disconnected")));
				}
			}

			UE_LOG(LogTemp, Log, TEXT("Wrench: Disconnected %s"), *Target->GetName());
		}
		else
		{
			// Connect
			IConnectableInterface::Execute_OnConnected(Target);

			if (ConnectSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ConnectSound, Target->GetActorLocation());
			}

			if (ConnectEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ConnectEffect, Target->GetActorLocation());
			}

			if (OwningCharacter)
			{
				UNotificationComponent* NotificationComp = OwningCharacter->GetNotificationComponent();
				if (NotificationComp)
				{
					NotificationComp->ShowNotification(FText::FromString(TEXT("Connected")));
				}
			}

			UE_LOG(LogTemp, Log, TEXT("Wrench: Connected %s"), *Target->GetName());
		}
	}

	// Consume durability
	Super::PerformPrimaryAction_Implementation();
}

void AWrenchTool::PerformSecondaryAction_Implementation()
{
	if (!bSeparateConnectDisconnect)
	{
		// In toggle mode, secondary does nothing
		return;
	}

	// Find connectable target
	AActor* Target = GetTargetConnectable();
	if (!Target || !Target->Implements<UConnectableInterface>())
	{
		return;
	}

	// Secondary action = disconnect only
	bool bIsCurrentlyConnected = IConnectableInterface::Execute_IsConnected(Target);
	if (bIsCurrentlyConnected)
	{
		IConnectableInterface::Execute_OnDisconnected(Target);

		if (DisconnectSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DisconnectSound, Target->GetActorLocation());
		}

		if (OwningCharacter)
		{
			UNotificationComponent* NotificationComp = OwningCharacter->GetNotificationComponent();
			if (NotificationComp)
			{
				NotificationComp->ShowNotification(FText::FromString(TEXT("Disconnected")));
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Wrench: Disconnected %s"), *Target->GetName());
	}
	else
	{
		if (OwningCharacter)
		{
			UNotificationComponent* NotificationComp = OwningCharacter->GetNotificationComponent();
			if (NotificationComp)
			{
				NotificationComp->ShowNotification(FText::FromString(TEXT("Not connected")));
			}
		}
	}
}

AActor* AWrenchTool::GetTargetConnectable()
{
	return PerformWrenchRaycast();
}

AActor* AWrenchTool::PerformWrenchRaycast()
{
	if (!OwningCharacter)
	{
		return nullptr;
	}

	UCameraComponent* Camera = OwningCharacter->GetFirstPersonCameraComponent();
	if (!Camera)
	{
		return nullptr;
	}

	// Get camera location and forward vector
	FVector CameraLocation = Camera->GetComponentLocation();
	FVector CameraForward = Camera->GetForwardVector();
	FVector TraceEnd = CameraLocation + (CameraForward * WrenchRange);

	// Perform line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwningCharacter);
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	// Debug visualization
	#if WITH_EDITOR
	DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, bHit ? FColor::Green : FColor::Red, false, 0.1f);
	#endif

	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();

		// Check if it implements the connectable interface
		if (HitActor->Implements<UConnectableInterface>())
		{
			return HitActor;
		}
	}

	return nullptr;
}

void AWrenchTool::StartConnection()
{
	ConnectionProgress = 0.0f;

	// Play wrenching sound (looping)
	if (WrenchingSound && CurrentTarget)
	{
		WrenchingSoundComponent = UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			WrenchingSound,
			CurrentTarget->GetActorLocation()
		);
	}

	// Start timer to update progress
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			ConnectionTimerHandle,
			this,
			&AWrenchTool::UpdateConnectionProgress,
			0.1f,
			true
		);
	}

	UE_LOG(LogTemp, Log, TEXT("Wrench: Starting connection process"));
}

void AWrenchTool::UpdateConnectionProgress()
{
	if (!CurrentTarget)
	{
		CancelConnection();
		return;
	}

	// Increment progress
	ConnectionProgress += 0.1f / ConnectionTime;

	// Check if complete
	if (ConnectionProgress >= 1.0f)
	{
		CompleteConnection();
	}
}

void AWrenchTool::CompleteConnection()
{
	// Clear timer
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ConnectionTimerHandle);
	}

	// Stop wrenching sound
	if (WrenchingSoundComponent)
	{
		WrenchingSoundComponent->Stop();
		WrenchingSoundComponent = nullptr;
	}

	// Perform the actual connection
	PerformPrimaryAction();

	ConnectionProgress = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("Wrench: Connection complete"));
}

void AWrenchTool::CancelConnection()
{
	// Clear timer
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ConnectionTimerHandle);
	}

	// Stop wrenching sound
	if (WrenchingSoundComponent)
	{
		WrenchingSoundComponent->Stop();
		WrenchingSoundComponent = nullptr;
	}

	ConnectionProgress = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("Wrench: Connection cancelled"));
}
