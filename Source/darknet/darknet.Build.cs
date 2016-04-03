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

    protected string CUDAPath
    {
        get { return System.Environment.GetEnvironmentVariable("CUDA_PATH"); }
    }

    public darknet(TargetInfo Target)
	{
        PrivateIncludePaths.AddRange(new string[]
        {
            Path.Combine(ThirdPartyPath),
        });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "V8" });

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

            PublicAdditionalLibraries.Add(Path.Combine(CUDAPath, "lib", "x64", "cudart.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(CUDAPath, "lib", "x64", "cuda.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(CUDAPath, "lib", "x64", "cublas.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(CUDAPath, "lib", "x64", "curand.lib"));
            
            return true;
        }

        Definitions.Add(string.Format("WITH_DARKNET=0"));
        return false;
    }
}
