// Copyright (C) RenZhai.2020.All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class SimpleDrawText : ModuleRules
{
	public string ProjectDirectory
	{
		get
		{
			return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../../"));//项目路径
		}
	}

	public string PluginsDirectory
	{
		get
		{
			return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));//插件路径
		}
	}

	private void CopyToProject(string FilePathName, ReadOnlyTargetRules Target)
	{
		string BineriesDirectory = ProjectDirectory;
		string Filename = Path.GetFileName(FilePathName);

		Directory.CreateDirectory(BineriesDirectory);

		string ProjectFileFullName = Path.Combine(BineriesDirectory, Filename);
		if(Directory.Exists(Filename))
        {
			File.Copy(FilePathName, ProjectFileFullName, true);
			RuntimeDependencies.Add(ProjectFileFullName);
		}
	}

	public SimpleDrawText(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		string PluginsDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));//路径
		string DrawTextPath = Path.Combine(PluginsDirectory, "DrawTextPath.txt"); //路径

		CopyToProject(DrawTextPath, Target);

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "UMG"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			});	
		}
}
