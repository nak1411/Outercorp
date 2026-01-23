// AnimNotify_ToolHit.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ToolHit.generated.h"

/**
 * Notify triggered during tool swing animation to indicate moment of impact
 */
UCLASS()
class OUTERCORP_API UAnimNotify_ToolHit : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference) override;
};
