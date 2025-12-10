// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterWidget.h"
#include "Components/Button.h"
#include "Input/Reply.h"

void UCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Log, TEXT("UCharacterWidget::NativeConstruct: Starting button binding"));

	// Bind button events
	if (CloseButton)
	{
		UE_LOG(LogTemp, Log, TEXT("UCharacterWidget::NativeConstruct: CloseButton found, binding OnClicked"));
		CloseButton->OnClicked.AddDynamic(this, &UCharacterWidget::OnCloseButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterWidget::NativeConstruct: CloseButton is NULL! Make sure you have a Button named 'CloseButton' in your widget Blueprint with BindWidget or BindWidgetOptional"));
	}
}

void UCharacterWidget::NativeDestruct()
{
	// Delegates are automatically cleaned up when the widget is destroyed
	Super::NativeDestruct();
}

FReply UCharacterWidget::NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent)
{
	// Handle ESC or C key to close character window
	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::C)
	{
		CloseCharacter();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UCharacterWidget::CloseCharacter()
{
	UE_LOG(LogTemp, Log, TEXT("UCharacterWidget::CloseCharacter: Broadcasting close event"));
	// Just broadcast the close event - let the owning character handle cleanup
	OnCharacterClosed.Broadcast();
	UE_LOG(LogTemp, Log, TEXT("UCharacterWidget::CloseCharacter: Broadcast complete"));
}

void UCharacterWidget::OnCloseButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("UCharacterWidget::OnCloseButtonClicked: X button clicked"));
	CloseCharacter();
}
