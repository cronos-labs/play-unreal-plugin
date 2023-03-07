// Copyright 2022, Cronos Labs. All Rights Reserved

using System;
using System.IO;
using UnrealBuildTool;

public class CronosPlayUnreal : ModuleRules {

    protected string ThirdPartyPath {
        get {
            return Path.GetFullPath(
                Path.Combine(ModuleDirectory, "..", "ThirdParty"));
        }
    }

    public CronosPlayUnreal(ReadOnlyTargetRules Target) : base(Target) {
        bEnableExceptions = true;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp17;

        PublicIncludePaths.AddRange(new string[] {});
        PrivateIncludePaths.AddRange(
            new string[] { Path.Combine(ThirdPartyPath) });

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "Json", "JsonUtilities", "HTTP" });

        PrivateDependencyModuleNames.AddRange(
            new string[] { "CoreUObject", "Engine", "Slate", "SlateCore",
                           "Projects", "PlayCppSdkLibrary" });

        DynamicallyLoadedModuleNames.AddRange(new string[] {});

        string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory,
                                                     Target.RelativeEnginePath);
        if (Target.Platform == UnrealTargetPlatform.Android) {
            PrivateDependencyModuleNames.Add("Launch");
            AdditionalPropertiesForReceipt.Add(
                "AndroidPlugin",
                System.IO.Path.Combine(PluginPath, "Android.xml"));
        }
    }
}
