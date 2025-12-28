// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuantityInputDialog.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UQuantityInputDialog::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button click events
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UQuantityInputDialog::OnConfirmClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UQuantityInputDialog::OnCancelClicked);
	}

	// Bind text box events
	if (QuantityInputBox)
	{
		QuantityInputBox->OnTextCommitted.AddDynamic(this, &UQuantityInputDialog::OnTextCommitted);
		QuantityInputBox->OnTextChanged.AddDynamic(this, &UQuantityInputDialog::OnTextChanged);
	}
}

void UQuantityInputDialog::SetupDialog(int32 InMaxQuantity, const FText& Title, const FText& Description, const FText& ConfirmText)
{
	MaxQuantity = FMath::Max(1, InMaxQuantity);
	CurrentValue = 1; // Default to 1 (will be updated when user types)

	// Set title if provided
	if (TitleText && !Title.IsEmpty())
	{
		TitleText->SetText(Title);
	}

	// Set description if provided
	if (DescriptionText && !Description.IsEmpty())
	{
		DescriptionText->SetText(Description);
	}

	// Set confirm button text if provided
	if (ConfirmButtonText && !ConfirmText.IsEmpty())
	{
		ConfirmButtonText->SetText(ConfirmText);
		UE_LOG(LogTemp, Log, TEXT("Set confirm button text to: %s"), *ConfirmText.ToString());
	}
	else if (!ConfirmButtonText)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConfirmButtonText widget not bound in Blueprint!"));
	}
	else if (ConfirmText.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ConfirmText parameter is empty!"));
	}

	// Set initial text box value
	if (QuantityInputBox)
	{
		// Clear hint text
		QuantityInputBox->SetHintText(FText::GetEmpty());

		// Leave text box empty for user to type
		QuantityInputBox->SetText(FText::GetEmpty());

		// Set focus to text box for immediate typing
		QuantityInputBox->SetKeyboardFocus();
	}
}

void UQuantityInputDialog::OnConfirmClicked()
{
	// Validate and clamp the value
	CurrentValue = FMath::Clamp(CurrentValue, 1, MaxQuantity);

	// Call C++ delegate if bound
	if (OnQuantityConfirmedDelegate.IsBound())
	{
		OnQuantityConfirmedDelegate.Execute(CurrentValue);
	}

	// Call Blueprint event
	OnConfirmed(CurrentValue);

	// Close the dialog
	CloseDialog();
}

void UQuantityInputDialog::OnCancelClicked()
{
	// Call Blueprint event
	OnCancelled();

	// Close the dialog
	CloseDialog();
}

void UQuantityInputDialog::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// If user pressed Enter, confirm
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnConfirmClicked();
	}
}

void UQuantityInputDialog::OnTextChanged(const FText& Text)
{
	// Try to parse the text as a number
	FString TextString = Text.ToString();

	if (TextString.IsEmpty())
	{
		CurrentValue = 1; // Default to 1 if empty
		return;
	}

	// Remove any non-numeric characters
	TextString = TextString.TrimStartAndEnd();

	if (TextString.IsNumeric())
	{
		int32 ParsedValue = FCString::Atoi(*TextString);

		// Clamp to valid range
		CurrentValue = FMath::Clamp(ParsedValue, 1, MaxQuantity);

		// Update text box if the value was clamped
		if (ParsedValue != CurrentValue && QuantityInputBox)
		{
			QuantityInputBox->SetText(FText::AsNumber(CurrentValue));
		}
	}
	else
	{
		// Invalid input - reset to current valid value
		if (QuantityInputBox)
		{
			QuantityInputBox->SetText(FText::AsNumber(CurrentValue));
		}
	}
}

void UQuantityInputDialog::CloseDialog()
{
	RemoveFromParent();
}
