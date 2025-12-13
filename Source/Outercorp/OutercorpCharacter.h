// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "OutercorpCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UUserWidget;
class UInventoryComponent;
class UInventoryWidget;
class UCharacterWidget;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AOutercorpCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent *FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FirstPersonCameraComponent;

	/** Inventory component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent *InventoryComponent;

	/** Crosshair widget */
	UPROPERTY()
	UUserWidget *CrosshairWidget;

	/** Base HUD widget (contains canvas for modular windows) */
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UUserWidget *BaseHUDWidget;

protected:
	/** Inventory widget (the actual content widget) */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UInventoryWidget *InventoryWidget;

	/** Modular window that wraps the inventory widget */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget *InventoryWindow;

	/** Character widget (the actual content widget) */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UCharacterWidget *CharacterWidget;

	/** Modular window that wraps the character widget */
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget *CharacterWindow;

	/** Crosshair widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	/** Base HUD widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BaseHUDWidgetClass;

	/** Inventory widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	/** Character widget class */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCharacterWidget> CharacterWidgetClass;

	/** Modular window class for wrapping widgets */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ModularWindowClass;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction *JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction *MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *MouseLookAction;

	/** Inventory Toggle Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *InventoryAction;

	/** Character Window Toggle Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction *CharacterAction;

public:
	AOutercorpCharacter();

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

protected:
	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue &Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue &Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	/** Toggle inventory display */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void ToggleInventory();

	/** Open inventory (BlueprintNativeEvent allows Blueprint override) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void OpenInventory();

	/** Called after inventory widget is created to bind events */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BindInventoryEvents();

	/** Get the HUD canvas panel for adding draggable windows */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	class UCanvasPanel *GetHUDCanvas() const;

	/** Close inventory (to be called from Blueprint or C++) */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventory();

	/** Called when inventory widget is closed */
	UFUNCTION()
	void OnInventoryWidgetClosed();

	/** Toggle character window display */
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void ToggleCharacter();

	/** Open character window (BlueprintNativeEvent allows Blueprint override) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OpenCharacter();

	/** Called after character widget is created to bind events */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void BindCharacterEvents();

	/** Setup character widget in modular window - call this after Create Modular Window */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetupCharacterWidgetInWindow(UUserWidget *ModularWindow);

	/** Setup inventory widget in modular window - call this after Create Modular Window */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetupInventoryWidgetInWindow(UUserWidget *ModularWindow);

	/** Helper to get child widget from modular window */
	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget *GetModularWindowChild(UUserWidget *ModularWindow) const;

	/** Debug helper to print widget type */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void DebugPrintWidgetType(UUserWidget *Widget) const;

	/** Close character window (to be called from Blueprint or C++) */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void CloseCharacter();

	/** Called when character widget is closed */
	UFUNCTION()
	void OnCharacterWidgetClosed();

	/** Close all open widgets */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseAllWidgets();

	/** Check if any UI widget is currently open */
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsAnyUIWidgetOpen() const;

protected:
	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent *InputComponent) override;

public:
	/** Returns the first person mesh **/
	USkeletalMeshComponent *GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent *GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Returns inventory component **/
	UInventoryComponent *GetInventoryComponent() const { return InventoryComponent; }
};
