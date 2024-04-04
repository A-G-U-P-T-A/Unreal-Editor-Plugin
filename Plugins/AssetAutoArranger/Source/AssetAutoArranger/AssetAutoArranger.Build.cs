// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AssetAutoArranger : ModuleRules
{
	public AssetAutoArranger(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
		PublicIncludePaths.AddRange(
			new string[] {
				"Editor/ContentBrowser/Public",
				// ... add public include paths required by your module ...
			}
		);
        
		PrivateIncludePaths.AddRange(
			new string[] {
				"AssetAutoArranger/Private",
			}
		);
        
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);
        
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"AssetRegistry",
				"WorkspaceMenuStructure",
				"ContentBrowser"
				// ... add private dependencies that you statically link with here ...
			}
		);
        
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}