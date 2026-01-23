// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContextMenuWidget.generated.h"

/**
 * Base class for context menus that handles click-outside-to-close behavior
 * Inherit from this in Blueprint to get automatic closing when clicking outside the menu
 */
UCLASS()
class OUTERCORP_API UContextMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/** Timer handle for delayed click detection setup */
	FTimerHandle SetupTimerHandle;

	/** Setup click capture after a brief delay */
	void SetupClickCapture();

	/** Handle clicks anywhere in the viewport */
	void OnViewportClicked(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	/** Called when context menu should be closed */
	UFUNCTION(BlueprintImplementableEvent, Category = "Context Menu")
	void OnRequestClose();

public:
	/** Blueprint-callable function to manually close the menu */
	UFUNCTION(BlueprintCallable, Category = "Context Menu")
	void CloseMenu();

	/** Internal function called by input processor when clicking outside */
	void RequestClose();

	/** Reference to the widget that opened this context menu (can be InventorySlotWidget or InventoryListRowWidget) */
	UPROPERTY(BlueprintReadWrite, Category = "Context Menu")
	TObjectPtr<UUserWidget> OwningSlot;

	/** Whether the item in this context menu is equippable (set by slot widget on creation) */
	UPROPERTY(BlueprintReadOnly, Category = "Context Menu")
	bool bIsItemEquippable = false;

private:
	/** Whether click capture is currently active */
	bool bIsClickCaptureActive = false;

	/** Shared pointer to input preprocessor */
	TSharedPtr<class FContextMenuInputProcessor> InputProcessor;
};
