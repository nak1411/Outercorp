// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuantityInputDialog.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;

// Delegate for C++ confirmation callback
DECLARE_DELEGATE_OneParam(FOnQuantityConfirmed, int32);

/**
 * Dialog widget for inputting a quantity value (e.g., for dropping/splitting items)
 */
UCLASS()
class OUTERCORP_API UQuantityInputDialog : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	/** Text box for quantity input */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> QuantityInputBox;

	/** Confirm button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	/** Cancel button */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> CancelButton;

	/** Confirm button text block (optional, child of ConfirmButton to change button text) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ConfirmButtonText;

	/** Title text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TitleText;

	/** Description text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DescriptionText;

	/** Maximum allowed quantity */
	UPROPERTY(BlueprintReadOnly, Category = "Quantity Dialog")
	int32 MaxQuantity = 1;

	/** Current input value */
	UPROPERTY(BlueprintReadOnly, Category = "Quantity Dialog")
	int32 CurrentValue = 1;

	/** Button click handlers */
	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnCancelClicked();

	/** Text committed handler */
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	/** Text changed handler */
	UFUNCTION()
	void OnTextChanged(const FText& Text);

public:
	/** Initialize the dialog with max quantity and optional title/description/button text */
	UFUNCTION(BlueprintCallable, Category = "Quantity Dialog")
	void SetupDialog(int32 InMaxQuantity, const FText& Title = FText::GetEmpty(), const FText& Description = FText::GetEmpty(), const FText& ConfirmText = FText::GetEmpty());

	/** Get the input value */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quantity Dialog")
	int32 GetQuantity() const { return CurrentValue; }

	/** Close the dialog */
	UFUNCTION(BlueprintCallable, Category = "Quantity Dialog")
	void CloseDialog();

	/** Blueprint event called when confirmed */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quantity Dialog")
	void OnConfirmed(int32 Quantity);

	/** Blueprint event called when cancelled */
	UFUNCTION(BlueprintImplementableEvent, Category = "Quantity Dialog")
	void OnCancelled();

	/** C++ delegate called when confirmed (binds in C++ instead of Blueprint) */
	FOnQuantityConfirmed OnQuantityConfirmedDelegate;
};
