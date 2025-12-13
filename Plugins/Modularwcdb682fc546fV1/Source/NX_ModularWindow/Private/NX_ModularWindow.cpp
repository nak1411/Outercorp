// Copyright Epic Games, Inc. All Rights Reserved.

#include "NX_ModularWindow.h"

#define LOCTEXT_NAMESPACE "FNX_ModularWindowModule"

void FNX_ModularWindowModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FNX_ModularWindowModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNX_ModularWindowModule, NX_ModularWindow)