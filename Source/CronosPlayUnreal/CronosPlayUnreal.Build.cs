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
    PrivateIncludePaths.AddRange(new string[] { Path.Combine(ThirdPartyPath) });

    PublicDependencyModuleNames.AddRange(
        new string[] { "Core", "Json", "JsonUtilities", "HTTP" });

    PrivateDependencyModuleNames.AddRange(
        new string[] { "CoreUObject", "Engine", "Slate", "SlateCore",
                       "PlayCppSdkLibrary", "PlayQrcodeLibrary" });

    DynamicallyLoadedModuleNames.AddRange(new string[] {});
  }
}
