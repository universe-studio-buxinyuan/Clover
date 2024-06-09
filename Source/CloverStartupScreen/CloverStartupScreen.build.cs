using UnrealBuildTool;

public class CloverStartupScreen : ModuleRules
{
	public CloverStartupScreen(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[] { "Core", "MoviePlayer" });
		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"MoviePlayer",
			"PreLoadScreen",
			"DeveloperSettings",
		});
		PublicIncludePaths.AddRange(new[] { "CloverStartupScreen/Public" });
		PrivateIncludePaths.AddRange(new[] { "CloverStartupScreen/Private" });
	}
}
