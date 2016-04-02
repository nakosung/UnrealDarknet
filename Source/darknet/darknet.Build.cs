// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;
using System;

public class darknet : ModuleRules
{
    protected string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "..", "ThirdParty")); }
    }

    public darknet(TargetInfo Target)
	{
        PrivateIncludePaths.AddRange(new string[]
        {
            Path.Combine(ThirdPartyPath),
        });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        LoadDarknet(Target);
    }

    private bool LoadDarknet(TargetInfo Target)
    {
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            string LibrariesPath = Path.Combine(ThirdPartyPath, "darknet", "darknet");

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                LibrariesPath = Path.Combine(LibrariesPath, "x64");
            }
            
            if (Target.Configuration == UnrealTargetConfiguration.Debug)
            {
                LibrariesPath = Path.Combine(LibrariesPath, "Debug");
            }
            else
            {
                LibrariesPath = Path.Combine(LibrariesPath, "Release");
            }

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "darknet.lib"));
            
            Definitions.Add(string.Format("WITH_DARKNET=1"));

            return true;
        }

        Definitions.Add(string.Format("WITH_DARKNET=0"));
        return false;
    }
}
