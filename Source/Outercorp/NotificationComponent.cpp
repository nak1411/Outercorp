// Copyright Epic Games, Inc. All Rights Reserved.

#include "NotificationComponent.h"
#include "NotificationWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UNotificationComponent::UNotificationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNotificationComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache the player controller for widget creation
	CachedPlayerController = Cast<APlayerController>(GetOwner());
	if (!CachedPlayerController)
	{
		CachedPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	}
}

void UNotificationComponent::ShowNotification(const FNotificationData& NotificationData)
{
	if (!NotificationWidgetClass || !CachedPlayerController)
	{		return;
	}

	// Create the notification widget
	UNotificationWidget* NotificationWidget = CreateWidget<UNotificationWidget>(CachedPlayerController, NotificationWidgetClass);
	if (!NotificationWidget)
	{		return;
	}
	// Initialize the notification
	NotificationWidget->InitializeNotification(NotificationData);

	// Bind to expiration event
	NotificationWidget->OnNotificationExpired.AddDynamic(this, &UNotificationComponent::OnNotificationExpired);

	// Add to canvas or viewport
	if (NotificationCanvas)
	{		// Add to the specified canvas panel
		UCanvasPanelSlot* CanvasSlot = NotificationCanvas->AddChildToCanvas(NotificationWidget);
		if (CanvasSlot)
		{			// Initial position - will be updated by UpdateNotificationPositions
			CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 0.0f, 0.0f)); // Top-left
			CanvasSlot->SetAlignment(FVector2D(0.0f, 0.0f));
			CanvasSlot->SetPosition(FVector2D(10.0f, 20.0f));
			CanvasSlot->SetSize(FVector2D(350.0f, 70.0f)); // Default size
			CanvasSlot->SetZOrder(1000); // High Z-order
		}
		else
		{		}
	}
	else
	{		// Fallback: Add directly to viewport
		NotificationWidget->AddToViewport(1000); // High Z-order to appear above most UI
	}

	// Add to active notifications
	ActiveNotifications.Add(NotificationWidget);

	// Remove oldest notification if we exceed the maximum
	if (ActiveNotifications.Num() > MaxVisibleNotifications)
	{
		if (UNotificationWidget* OldestNotification = ActiveNotifications[0])
		{
			OldestNotification->ForceClose();
		}
	}

	// Update positions of all notifications
	UpdateNotificationPositions();
}

void UNotificationComponent::ShowSimpleNotification(const FText& Message, ENotificationType Type, float Duration)
{
	FNotificationData NotificationData(Message, Type, Duration);
	ShowNotification(NotificationData);
}

void UNotificationComponent::ShowItemPickupNotification(const FText& ItemName, int32 Quantity, UTexture2D* ItemIcon)
{
	FText Message;
	if (Quantity > 1)
	{
		Message = FText::Format(FText::FromString(TEXT("Picked up {0} x{1}")), ItemName, FText::AsNumber(Quantity));
	}
	else
	{
		Message = FText::Format(FText::FromString(TEXT("Picked up {0}")), ItemName);
	}

	FNotificationData NotificationData(Message, ENotificationType::Success, 2.5f, ItemIcon);
	ShowNotification(NotificationData);
}

void UNotificationComponent::ShowItemDestroyedNotification(const FText& ItemName, int32 Quantity)
{
	FText Message;
	if (Quantity > 1)
	{
		Message = FText::Format(FText::FromString(TEXT("Destroyed {0} x{1}")), ItemName, FText::AsNumber(Quantity));
	}
	else
	{
		Message = FText::Format(FText::FromString(TEXT("Destroyed {0}")), ItemName);
	}

	ShowSimpleNotification(Message, ENotificationType::Info, 2.0f);
}

void UNotificationComponent::ShowInventoryFullWarning()
{
	ShowSimpleNotification(
		FText::FromString(TEXT("Inventory is full")),
		ENotificationType::Warning,
		3.0f
	);
}

void UNotificationComponent::ShowNotEnoughRoomWarning(const FText& ItemName)
{
	FText Message = FText::Format(
		FText::FromString(TEXT("Not enough room for {0}")),
		ItemName
	);

	ShowSimpleNotification(Message, ENotificationType::Warning, 3.0f);
}

void UNotificationComponent::ShowErrorNotification(const FText& ErrorMessage)
{
	ShowSimpleNotification(ErrorMessage, ENotificationType::Error, 4.0f);
}

void UNotificationComponent::OnNotificationExpired(UNotificationWidget* Notification)
{
	if (Notification)
	{
		ActiveNotifications.Remove(Notification);
		UpdateNotificationPositions();
	}
}

void UNotificationComponent::UpdateNotificationPositions()
{
	// Position notifications from top-right, stacking downward
	float CurrentYOffset = 20.0f; // Start position from top of screen

	for (int32 i = ActiveNotifications.Num() - 1; i >= 0; --i)
	{
		if (UNotificationWidget* Notification = ActiveNotifications[i])
		{
			Notification->UpdatePosition(CurrentYOffset);
			CurrentYOffset += Notification->GetDesiredSize().Y + NotificationSpacing;
		}
	}
}
