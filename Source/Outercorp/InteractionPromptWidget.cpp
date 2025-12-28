// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractionPromptWidget.h"
#include "InteractionManagerComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/Pawn.h"

UInteractionPromptWidget::UInteractionPromptWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Enable ticking for this widget
	bIsFocusable = false;
}

void UInteractionPromptWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Get the interaction manager from the owning player's pawn
	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		InteractionManager = OwningPawn->FindComponentByClass<UInteractionManagerComponent>();
	}

	// Start hidden
	HidePrompt();
}

void UInteractionPromptWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!InteractionManager)
	{
		return;
	}

	// Update prompt based on current interactable
	if (InteractionManager->IsLookingAtInteractable())
	{
		FText Name = InteractionManager->GetCurrentInteractionName();
		FText Prompt = InteractionManager->GetCurrentInteractionPrompt();

		UpdateInteractionInfo(Name, Prompt);
		ShowPrompt();
	}
	else
	{
		HidePrompt();
	}
}

void UInteractionPromptWidget::UpdateInteractionInfo(const FText& Name, const FText& Prompt)
{
	if (InteractionNameText)
	{
		InteractionNameText->SetText(Name);
	}

	if (InteractionPromptText)
	{
		InteractionPromptText->SetText(Prompt);
	}
}

void UInteractionPromptWidget::ShowPrompt()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInteractionPromptWidget::HidePrompt()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

bool UInteractionPromptWidget::IsPromptVisible() const
{
	return GetVisibility() != ESlateVisibility::Collapsed;
}

void UInteractionPromptWidget::InitializeInteraction()
{
	// Get the interaction manager from the owning player's pawn
	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		InteractionManager = OwningPawn->FindComponentByClass<UInteractionManagerComponent>();
	}

	// Start hidden
	HidePrompt();

	// Start a timer to update the prompt (since NativeTick might not be called)
	GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UInteractionPromptWidget::UpdatePrompt, 0.1f, true);
}

void UInteractionPromptWidget::UpdatePrompt()
{
	if (!InteractionManager)
	{
		return;
	}

	// Check if we're looking at something
	bool bLookingAt = InteractionManager->IsLookingAtInteractable();
	bool bCurrentlyVisible = IsPromptVisible();

	// Only update if state changed (optimization: avoid setting text/visibility every frame)
	if (bLookingAt)
	{
		FText Name = InteractionManager->GetCurrentInteractionName();
		FText Prompt = InteractionManager->GetCurrentInteractionPrompt();

		// Always update text (in case we're looking at a different object)
		UpdateInteractionInfo(Name, Prompt);

		// Only show if not already visible
		if (!bCurrentlyVisible)
		{
			ShowPrompt();
		}
	}
	else
	{
		// Only hide if currently visible
		if (bCurrentlyVisible)
		{
			HidePrompt();
		}
	}
}
