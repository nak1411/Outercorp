// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NotificationComponent.generated.h"

/**
 * Notification types for different events
 */
UENUM(BlueprintType)
enum class ENotificationType : uint8
{
	Info		UMETA(DisplayName = "Info"),
	Success		UMETA(DisplayName = "Success"),
	Warning		UMETA(DisplayName = "Warning"),
	Error		UMETA(DisplayName = "Error")
};

/**
 * Data structure for a notification
 */
USTRUCT(BlueprintType)
struct FNotificationData
{
	GENERATED_BODY()

	/** The message to display */
	UPROPERTY(BlueprintReadWrite, Category = "Notification")
	FText Message;

	/** Type of notification (affects appearance) */
	UPROPERTY(BlueprintReadWrite, Category = "Notification")
	ENotificationType Type;

	/** How long the notification should be displayed (seconds) */
	UPROPERTY(BlueprintReadWrite, Category = "Notification")
	float Duration;

	/** Optional icon to display */
	UPROPERTY(BlueprintReadWrite, Category = "Notification")
	class UTexture2D* Icon;

	FNotificationData()
		: Message(FText::GetEmpty())
		, Type(ENotificationType::Info)
		, Duration(3.0f)
		, Icon(nullptr)
	{
	}

	FNotificationData(const FText& InMessage, ENotificationType InType = ENotificationType::Info, float InDuration = 3.0f, UTexture2D* InIcon = nullptr)
		: Message(InMessage)
		, Type(InType)
		, Duration(InDuration)
		, Icon(InIcon)
	{
	}
};

/**
 * Component that manages notification display
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OUTERCORP_API UNotificationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNotificationComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** Show a notification with custom parameters */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ShowNotification(const FNotificationData& NotificationData);

	/** Show a simple text notification */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ShowSimpleNotification(const FText& Message, ENotificationType Type = ENotificationType::Info, float Duration = 3.0f);

	/** Show an item pickup notification */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ShowItemPickupNotification(const FText& ItemName, int32 Quantity, class UTexture2D* ItemIcon = nullptr);

	/** Show an item destroyed notification */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ShowItemDestroyedNotification(const FText& ItemName, int32 Quantity);

	/** Show an inventory full warning */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ShowInventoryFullWarning();

	/** Show a not enough room warning */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ShowNotEnoughRoomWarning(const FText& ItemName);

	/** Show a generic error notification */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ShowErrorNotification(const FText& ErrorMessage);

	/** Widget class for notification popup */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Notification")
	TSubclassOf<class UNotificationWidget> NotificationWidgetClass;

	/** Maximum number of notifications to display at once */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	int32 MaxVisibleNotifications = 5;

	/** Vertical spacing between notifications (pixels) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	float NotificationSpacing = 10.0f;

	/** Optional canvas panel to add notifications to (if null, adds directly to viewport) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification")
	class UCanvasPanel* NotificationCanvas;

	/** Get all active notification widgets */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Notification")
	TArray<class UNotificationWidget*> GetActiveNotifications() const { return ActiveNotifications; }

protected:
	/** Called when a notification expires */
	UFUNCTION()
	void OnNotificationExpired(class UNotificationWidget* Notification);

	/** Update positions of all active notifications */
	void UpdateNotificationPositions();

	/** Currently active notification widgets */
	UPROPERTY()
	TArray<class UNotificationWidget*> ActiveNotifications;

	/** Reference to the player controller for widget creation */
	class APlayerController* CachedPlayerController;
};
