// AnimNotify_ToolHit.h
// Animation notify that triggers when a tool hit should occur (e.g., axe impact)

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ToolHit.generated.h"

/**
 * Animation notify that triggers tool hit events
 * Add this to attack/swing montages at the moment of impact
 */
UCLASS()
class OUTERCORP_API UAnimNotify_ToolHit : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_ToolHit();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
