// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

using System.IO;

public class jsoncons : ModuleRules
{
	public jsoncons(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bAddDefaultIncludePaths = false;
		Type = ModuleType.External;

        PublicDefinitions.AddRange(new string[]{});
		
		PublicDependencyModuleNames.AddRange(new string[]{});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "include")));
	}
}
