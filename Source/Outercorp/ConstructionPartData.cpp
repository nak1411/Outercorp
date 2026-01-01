// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConstructionPartData.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"

void UConstructionPartData::AutoPopulateSocketDefinitions()
{
	if (!PartMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot auto-populate sockets: No mesh assigned to PartMesh"));
		return;
	}

	// Get all socket names from the mesh
	TArray<FName> MeshSocketNames;
	if (PartMesh->Sockets.Num() > 0)
	{
		for (UStaticMeshSocket* Socket : PartMesh->Sockets)
		{
			if (Socket)
			{
				MeshSocketNames.Add(Socket->SocketName);
			}
		}
	}

	if (MeshSocketNames.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sockets found on mesh '%s'"), *PartMesh->GetName());
		return;
	}

	// Find sockets that are already defined
	TSet<FName> ExistingSocketNames;
	for (const FSocketTypeDefinition& SocketDef : SocketTypeDefinitions)
	{
		ExistingSocketNames.Add(SocketDef.SocketName);
	}

	// Add new socket definitions for sockets that aren't already defined
	int32 AddedCount = 0;
	for (const FName& SocketName : MeshSocketNames)
	{
		if (!ExistingSocketNames.Contains(SocketName))
		{
			FSocketTypeDefinition NewSocketDef;
			NewSocketDef.SocketName = SocketName;
			NewSocketDef.bUseAutomaticRotation = true;

			// Auto-detect socket type from socket name
			FString SocketNameStr = SocketName.ToString().ToLower();
			if (SocketNameStr.Contains(TEXT("top")))
			{
				NewSocketDef.SocketTypePreset = ESocketTypePreset::Top;
			}
			else if (SocketNameStr.Contains(TEXT("bottom")))
			{
				NewSocketDef.SocketTypePreset = ESocketTypePreset::Bottom;
			}
			else if (SocketNameStr.Contains(TEXT("front")))
			{
				NewSocketDef.SocketTypePreset = ESocketTypePreset::Front;
			}
			else if (SocketNameStr.Contains(TEXT("back")))
			{
				NewSocketDef.SocketTypePreset = ESocketTypePreset::Back;
			}
			else if (SocketNameStr.Contains(TEXT("left")))
			{
				NewSocketDef.SocketTypePreset = ESocketTypePreset::Left;
			}
			else if (SocketNameStr.Contains(TEXT("right")))
			{
				NewSocketDef.SocketTypePreset = ESocketTypePreset::Right;
			}
			else if (SocketNameStr.Contains(TEXT("mount")))
			{
				NewSocketDef.SocketTypePreset = ESocketTypePreset::MountPoint;
			}
			else if (SocketNameStr.Contains(TEXT("beam")))
			{
				NewSocketDef.SocketTypePreset = ESocketTypePreset::BeamEnd;
			}
			else if (SocketNameStr.Contains(TEXT("corner")))
			{
				NewSocketDef.SocketTypePreset = ESocketTypePreset::Corner;
			}
			else
			{
				// Default to Any if no pattern matches
				NewSocketDef.SocketTypePreset = ESocketTypePreset::Any;
			}

			SocketTypeDefinitions.Add(NewSocketDef);
			AddedCount++;
		}
	}

	if (AddedCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Auto-populated %d socket definition(s) from mesh '%s'"), AddedCount, *PartMesh->GetName());
	}
}
