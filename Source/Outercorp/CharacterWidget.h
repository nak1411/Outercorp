// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterWidget.generated.h"

class UButton;

/**
 * Base class for character window widget
 * Contains core logic for character display and management
 */
UCLASS()
class OUTERCORP_API UCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent) override;
	virtual bool NativeSupportsKeyboardFocus() const override { return true; }

	/** Close button (X button in widget) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton;

public:
	/** Close character window */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void CloseCharacter();

	/** Delegate called when character window is closed */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterClosed);

	UPROPERTY(BlueprintAssignable, Category = "Character")
	FOnCharacterClosed OnCharacterClosed;

protected:
	/** Close button clicked */
	UFUNCTION()
	void OnCloseButtonClicked();
};
