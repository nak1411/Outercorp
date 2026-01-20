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
