// Copyright 2022, Cronos Labs. All Rights Reserved

using System;
using System.IO;
using System.Reflection;
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
        
        // Set C++ standard version based on UE version and availability
        SetCppStandardVersion(Target);

        PublicIncludePaths.AddRange(new string[] {});
        PrivateIncludePaths.AddRange(
            new string[] { Path.Combine(ThirdPartyPath) });

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "Json", "JsonUtilities", "HTTP" });

        PrivateDependencyModuleNames.AddRange(
            new string[] { "CoreUObject", "Engine", "Slate", "SlateCore",
                           "Projects", "PlayCppSdkLibrary", "RenderCore", "RHI" });

        DynamicallyLoadedModuleNames.AddRange(new string[] {});

        string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory,
                                                     Target.RelativeEnginePath);
        if (Target.Platform == UnrealTargetPlatform.Android) {
            PrivateDependencyModuleNames.Add("Launch");
            AdditionalPropertiesForReceipt.Add(
                "AndroidPlugin",
                System.IO.Path.Combine(PluginPath, "Android.xml"));
        }
        if (Target.Platform == UnrealTargetPlatform.Mac) {
            // Add frameworks for macOS
            PublicFrameworks.AddRange(new string[] { "Security",
                                                     "CoreFoundation",
                                                     "SystemConfiguration" });
        }
    }

    private void SetCppStandardVersion(ReadOnlyTargetRules Target) {
        // Use C++17 for versions before UE5.5, attempt C++20 for UE5.5 and later
        if (Target.Version.MajorVersion == 5 && Target.Version.MinorVersion >= 5) {
            // Check if Cpp20 enum value exists (for UE5.5+)
            if (Enum.IsDefined(typeof(CppStandardVersion), "Cpp20")) {
                CppStandard = (CppStandardVersion)Enum.Parse(typeof(CppStandardVersion), "Cpp20");
            } else {
                // Fallback to Cpp17 if Cpp20 is not available
                CppStandard = CppStandardVersion.Cpp17;
            }
        } else {
            CppStandard = CppStandardVersion.Cpp17;
        }
    }
}
