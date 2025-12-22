// Copyright Epic Games, Inc. All Rights Reserved.

#include "OutercorpSaveGame.h"

const FString UOutercorpSaveGame::SaveSlotName = TEXT("OutercorpUILayout");
const int32 UOutercorpSaveGame::UserIndex = 0;

UOutercorpSaveGame::UOutercorpSaveGame()
{
	// Set default window layouts
	InventoryWindowLayout = FWindowLayoutData(FVector2D(100.0f, 100.0f), FVector2D(600.0f, 400.0f));
	CharacterWindowLayout = FWindowLayoutData(FVector2D(750.0f, 100.0f), FVector2D(400.0f, 500.0f));
	ItemInfoWindowLayout = FWindowLayoutData(FVector2D(400.0f, 200.0f), FVector2D(350.0f, 450.0f));
}
