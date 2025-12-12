// Copyright 2020 YetiTech Studios, Pvt Ltd. All Rights Reserved.

using System;
using UnrealBuildTool;

public class DraggableWindowUMG : ModuleRules
{
	public DraggableWindowUMG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "SlateCore", "InputCore", "UMG" });
		
		if (Environment.GetEnvironmentVariable("YETI_STRICT_BUILD") == "true")
		{
#if UE_5_5_OR_LATER
			CppStandard = CppStandardVersion.Cpp20;
#else
			CppStandard = CppStandardVersion.Cpp17;
#endif
			bUseUnity = false;
			PCHUsage = PCHUsageMode.NoPCHs;
		}
	}
}
