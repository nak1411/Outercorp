// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemPreviewImage.generated.h"

class AItemPreviewController;
class UImage;

/**
 * Custom widget wrapper for item preview that captures mouse input for rotation/zoom
 */
UCLASS()
class OUTERCORP_API UItemPreviewImage : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemPreviewImage(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/** The actual image widget */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> PreviewImage;

	/** Is the user currently dragging to rotate? */
	bool bIsDragging = false;

	/** Last mouse position for calculating delta */
	FVector2D LastMousePosition;

	/** Find the preview controller in the world */
	void FindPreviewController();

public:
	/** Reference to the item preview controller */
	UPROPERTY(BlueprintReadWrite, Category = "Preview")
	TObjectPtr<AItemPreviewController> PreviewController;
};
