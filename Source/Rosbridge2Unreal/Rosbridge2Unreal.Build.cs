// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

using System.IO;

public class Rosbridge2Unreal : ModuleRules
{
	public Rosbridge2Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDefinitions.AddRange(
			new string[]{}
			);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Sockets",
				"Networking",
				"Engine",
				"WebSockets",
				"ProceduralMeshComponent",
				"jsoncons"
			}
			);
		
		#if UE_4_26_OR_LATER
		PublicDependencyModuleNames.Add("DeveloperSettings");
		#endif
		
		PrivateDependencyModuleNames.AddRange(
			new string[]{}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]{}
		);

		AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");
	}
}
