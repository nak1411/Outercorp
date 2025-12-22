// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OutercorpSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FWindowLayoutData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	FVector2D Position;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	FVector2D Size;

	FWindowLayoutData()
		: Position(FVector2D::ZeroVector)
		, Size(FVector2D::ZeroVector)
	{}

	FWindowLayoutData(FVector2D InPosition, FVector2D InSize)
		: Position(InPosition)
		, Size(InSize)
	{}
};

/**
 * Save game class for Outercorp
 * Stores UI layout data and other game state
 */
UCLASS()
class OUTERCORP_API UOutercorpSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UOutercorpSaveGame();

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	FWindowLayoutData InventoryWindowLayout;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	FWindowLayoutData CharacterWindowLayout;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	FWindowLayoutData ItemInfoWindowLayout;

	// Save slot name
	static const FString SaveSlotName;
	static const int32 UserIndex;
};
