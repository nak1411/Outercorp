// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryContextMenuHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInventoryContextMenuHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for widgets that can handle inventory context menu actions
 * Implemented by InventorySlotWidget, InventoryListRowWidget, and InventoryWidget
 */
class OUTERCORP_API IInventoryContextMenuHandler
{
	GENERATED_BODY()

public:
	/** Handle a context menu action by ID */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	void HandleContextMenuAction(FName ActionID);
};
