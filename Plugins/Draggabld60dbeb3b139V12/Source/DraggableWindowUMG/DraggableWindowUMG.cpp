// Copyright 2020 YetiTech Studios, Pvt Ltd. All Rights Reserved.

#include "DraggableWindowUMG.h"

#define LOCTEXT_NAMESPACE "FDraggableWindowUMGModule"

DEFINE_LOG_CATEGORY_STATIC(LogOperatingSystemDraggableWindowModule, Display, Display);

void FDraggableWindowUMGModule::StartupModule()
{
	UE_LOG(LogOperatingSystemDraggableWindowModule, Display, TEXT("Starting Operating System Draggable Window UMG module..."));
}

void FDraggableWindowUMGModule::ShutdownModule()
{
	UE_LOG(LogOperatingSystemDraggableWindowModule, Display, TEXT("Shutting down Operating System Draggable Window UMG module..."));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDraggableWindowUMGModule, DraggableWindowUMG)
