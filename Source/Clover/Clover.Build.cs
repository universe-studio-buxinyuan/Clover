// Copyright Clover Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Clover : ModuleRules
{
	public Clover(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "GameplayTags",
			"InputCore", "EnhancedInput",
			"CommonUI", "CommonInput", "Slate", "SlateCore", "UMG",
			"ModularGameplay",
		});
	}
}
