// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectMoba : ModuleRules
{
	public ProjectMoba(ReadOnlyTargetRules Target) : base(Target)
	{
		
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput","GameplayTasks"});
        
        PrivateDependencyModuleNames.AddRange(new string[] { "SimpleScreenMove" });
    }
}
