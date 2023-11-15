// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OWN : ModuleRules
{
	public OWN(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"GameplayTags",
			"GameplayAbilities",
            "GameplayTasks",
            "UMG",
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
            "NetCore",
            "GameplayMessageRuntime",
			"CommonUI",
        });
    }
}
