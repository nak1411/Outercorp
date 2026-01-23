// AnimNotify_ToolHit.cpp

#include "AnimNotify_ToolHit.h"
#include "OutercorpCharacter.h"
#include "EquippableTool.h"

void UAnimNotify_ToolHit::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (!MeshComp)
    {
        return;
    }

    AActor *Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        return;
    }

    // Try to get character
    AOutercorpCharacter *Character = Cast<AOutercorpCharacter>(Owner);
    if (Character)
    {
        // Forward to equipped tool if one exists
        AEquippableTool *Tool = Character->GetEquippedTool();
        if (Tool)
        {
            // This will eventually call OnToolHitNotify_Implementation on the tool actor
            Tool->OnToolHitNotify();
            // UE_LOG(LogTemp, Verbose, TEXT("AnimNotify_ToolHit: Triggered hit on tool %s"), *Tool->GetName());
        }
    }
}
