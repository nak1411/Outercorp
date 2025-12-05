// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryItemData.h"

FLinearColor UInventoryItemData::GetRarityColor() const
{
	switch (Rarity)
	{
		case EItemRarity::Common:
			return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f); // Gray
		case EItemRarity::Uncommon:
			return FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green
		case EItemRarity::Rare:
			return FLinearColor(0.0f, 0.5f, 1.0f, 1.0f); // Blue
		case EItemRarity::Epic:
			return FLinearColor(0.7f, 0.0f, 1.0f, 1.0f); // Purple
		case EItemRarity::Legendary:
			return FLinearColor(1.0f, 0.5f, 0.0f, 1.0f); // Orange
		default:
			return FLinearColor::White;
	}
}
