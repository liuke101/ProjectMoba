// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectMoba : ModuleRules
{
	public ProjectMoba(ReadOnlyTargetRules Target) : base(Target)
	{
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara","EnhancedInput", "GameplayTasks", "UMG", "Landscape", "SlateCore","AnimGraphRuntime"});
		
		PrivateDependencyModuleNames.AddRange(new string[] { "SimpleScreenMove","SimpleThread", "SimpleDrawText" });
		
		//服务端所用模块
		if (Target.bWithServerCode)
		{
        
		}
		else //客户端所用模块
		{
			PublicDependencyModuleNames.AddRange(new string[] { "ImageWrapper"});
		}
    }
}
