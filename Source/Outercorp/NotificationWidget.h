// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationComponent.h"
#include "NotificationWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotificationExpired, UNotificationWidget*, Notification);

/**
 * Widget that displays a single notification popup
 */
UCLASS()
class OUTERCORP_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Initialize the notification with data */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void InitializeNotification(const FNotificationData& InNotificationData);

	/** Start the fade-out animation and close the notification */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void StartClosing();

	/** Force close immediately without animation */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ForceClose();

	/** Update the position of the notification */
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void UpdatePosition(float YOffset);

	/** Called when notification should be removed */
	UPROPERTY(BlueprintAssignable, Category = "Notification")
	FOnNotificationExpired OnNotificationExpired;

	/** Get the notification data */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Notification")
	FNotificationData GetNotificationData() const { return NotificationData; }

	/** Blueprint implementable event to update the UI when notification data changes */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Notification")
	void OnNotificationDataUpdated();

	/** Blueprint implementable event called when starting fade out animation */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Notification")
	void PlayFadeOutAnimation();

	/** Blueprint implementable event called when starting fade in animation */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Notification")
	void PlayFadeInAnimation();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativePreConstruct() override;

	/** Native implementation of updating the UI */
	void NativeUpdateUI();

	/** Native implementation of fade in */
	void NativeFadeIn();

	/** Native implementation of fade out */
	void NativeFadeOut();

	/** UI Elements - Created in NativeConstruct */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Notification")
	class UBorder* BackgroundBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Notification")
	class UTextBlock* MessageText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Notification")
	class UImage* IconImage;

	/** The notification data */
	UPROPERTY(BlueprintReadOnly, Category = "Notification")
	FNotificationData NotificationData;

	/** Timer handle for auto-close */
	FTimerHandle CloseTimerHandle;

	/** Timer handle for fade-out delay */
	FTimerHandle FadeOutTimerHandle;

	/** Timer handle for fade animations */
	FTimerHandle FadeAnimationTimerHandle;

	/** Called when it's time to start fading out */
	UFUNCTION()
	void OnFadeOutTime();

	/** Called after fade out completes to actually remove the widget */
	UFUNCTION()
	void OnFadeOutComplete();

	/** Update fade animation */
	UFUNCTION()
	void UpdateFadeAnimation();

	/** Duration of fade out animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Notification|Animation")
	float FadeOutDuration = 0.3f;

	/** Duration of fade in animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Notification|Animation")
	float FadeInDuration = 0.3f;

	/** Distance to slide in from (negative = from left) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Notification|Animation")
	float SlideDistance = -300.0f;

	/** Enable slide animations */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Notification|Animation")
	bool bEnableSlideAnimation = true;

	/** Current fade state */
	bool bIsFadingIn = false;
	bool bIsFadingOut = false;
	float FadeStartTime = 0.0f;
	float InitialXPosition = 0.0f;
};
