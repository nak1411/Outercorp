// AnimNotify_ToolHit.cpp

#include "AnimNotify_ToolHit.h"
#include "OutercorpCharacter.h"
#include "EquippableTool.h"
#include "Components/SkeletalMeshComponent.h"

UAnimNotify_ToolHit::UAnimNotify_ToolHit()
{
}

void UAnimNotify_ToolHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	// Get the owning actor - should be the character
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	// Cast to our character type
	AOutercorpCharacter* Character = Cast<AOutercorpCharacter>(Owner);
	if (!Character)
	{
		return;
	}

	// Only trigger from first-person mesh to prevent double-hits
	// (montage plays on both FP and TP meshes)
	USkeletalMeshComponent* FirstPersonMesh = Character->GetFirstPersonMesh();
	if (MeshComp != FirstPersonMesh)
	{
		return;
	}

	// Get the equipped tool and call its hit notify
	AEquippableTool* EquippedTool = Character->GetEquippedTool();
	if (EquippedTool)
	{
		EquippedTool->OnToolHitNotify();
	}
}

FString UAnimNotify_ToolHit::GetNotifyName_Implementation() const
{
	return TEXT("ToolHit");
}
