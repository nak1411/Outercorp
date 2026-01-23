// HarvestableResourceData.cpp

#include "HarvestableResourceData.h"

const FHarvestStage* UHarvestableResourceData::GetStageForHealth(float HealthPercent) const
{
	const FHarvestStage* BestStage = nullptr;
	float BestThreshold = -1.0f;

	for (const FHarvestStage& Stage : DestructionStages)
	{
		// Find the stage with the highest threshold that's still below current health
		if (Stage.HealthThreshold <= HealthPercent && Stage.HealthThreshold > BestThreshold)
		{
			BestStage = &Stage;
			BestThreshold = Stage.HealthThreshold;
		}
	}

	return BestStage;
}

bool UHarvestableResourceData::GetStageForHealthBP(float HealthPercent, FHarvestStage& OutStage) const
{
	const FHarvestStage* Stage = GetStageForHealth(HealthPercent);
	if (Stage)
	{
		OutStage = *Stage;
		return true;
	}
	return false;
}

float UHarvestableResourceData::CalculateDamage(float IncomingDamage) const
{
	return FMath::Max(0.0f, IncomingDamage - Armor);
}

bool UHarvestableResourceData::IsToolTypeValid(EHarvestToolType ToolType, int32 ToolTier) const
{
	// If new tool effectiveness system is configured, use that
	if (ToolEffectiveness.Num() > 0)
	{
		float DamageMultiplier, YieldMultiplier;
		return GetToolEffectiveness(ToolType, ToolTier, DamageMultiplier, YieldMultiplier);
	}

	// Legacy behavior for backwards compatibility
	// Check if hand harvesting is allowed
	if (ToolType == EHarvestToolType::None)
	{
		return bCanHarvestByHand;
	}

	// Any tool type works
	if (RequiredToolType == EHarvestToolType::Any)
	{
		return ToolTier >= MinimumToolTier;
	}

	// Check specific tool type match
	if (ToolType == RequiredToolType)
	{
		return ToolTier >= MinimumToolTier;
	}

	// Wrong tool type - can still harvest by hand if allowed
	return bCanHarvestByHand;
}

bool UHarvestableResourceData::GetToolEffectiveness(EHarvestToolType ToolType, int32 ToolTier, float& OutDamageMultiplier, float& OutYieldMultiplier) const
{
	// If no tool effectiveness configured, fall back to legacy behavior
	if (ToolEffectiveness.Num() == 0)
	{
		// Legacy: If tool is valid, return full effectiveness
		if (IsToolTypeValid(ToolType, ToolTier))
		{
			OutDamageMultiplier = 1.0f;
			OutYieldMultiplier = 1.0f;
			return true;
		}
		OutDamageMultiplier = 0.0f;
		OutYieldMultiplier = 0.0f;
		return false;
	}

	// Find the best matching effectiveness entry for this tool
	const FToolEffectivenessEntry* BestMatch = nullptr;

	for (const FToolEffectivenessEntry& Entry : ToolEffectiveness)
	{
		// Skip if tool type doesn't match and isn't "Any"
		if (Entry.ToolType != ToolType && Entry.ToolType != EHarvestToolType::Any)
		{
			continue;
		}

		// Skip if tool tier is too low (but allow unarmed/None with tier 0)
		if (ToolTier < Entry.MinimumToolTier && !(ToolType == EHarvestToolType::None && ToolTier == 0))
		{
			continue;
		}

		// Prefer exact tool type matches over "Any"
		if (!BestMatch ||
			(Entry.ToolType == ToolType && BestMatch->ToolType == EHarvestToolType::Any) ||
			(Entry.ToolType == ToolType && BestMatch->ToolType == ToolType && Entry.MinimumToolTier > BestMatch->MinimumToolTier))
		{
			BestMatch = &Entry;
		}
	}

	// If no match found, tool is incompatible
	if (!BestMatch)
	{
		OutDamageMultiplier = 0.0f;
		OutYieldMultiplier = 0.0f;
		return false;
	}

	// Return the effectiveness multipliers
	OutDamageMultiplier = BestMatch->DamageMultiplier;
	OutYieldMultiplier = BestMatch->YieldMultiplier;

	// Tool can harvest if damage multiplier > 0
	return BestMatch->DamageMultiplier > 0.0f;
}

bool UHarvestableResourceData::CanToolHarvest(EHarvestToolType ToolType, int32 ToolTier) const
{
	float DamageMultiplier, YieldMultiplier;
	return GetToolEffectiveness(ToolType, ToolTier, DamageMultiplier, YieldMultiplier);
}

bool UHarvestableResourceData::ContainsMesh(UStaticMesh* Mesh) const
{
	if (!Mesh)
	{
		return false;
	}

	for (const FHarvestableSourceMesh& SourceMesh : SourceMeshes)
	{
		if (SourceMesh.Mesh == Mesh)
		{
			return true;
		}
	}

	return false;
}

const FHarvestableSourceMesh* UHarvestableResourceData::GetSourceMeshEntry(UStaticMesh* Mesh) const
{
	if (!Mesh)
	{
		return nullptr;
	}

	for (const FHarvestableSourceMesh& SourceMesh : SourceMeshes)
	{
		if (SourceMesh.Mesh == Mesh)
		{
			return &SourceMesh;
		}
	}

	return nullptr;
}
