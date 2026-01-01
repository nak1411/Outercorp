// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterWidget.h"
#include "Components/Button.h"
#include "Input/Reply.h"

void UCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button events
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UCharacterWidget::OnCloseButtonClicked);
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
	// Just broadcast the close event - let the owning character handle cleanup
	OnCharacterClosed.Broadcast();
}

void UCharacterWidget::OnCloseButtonClicked()
{
	CloseCharacter();
}
