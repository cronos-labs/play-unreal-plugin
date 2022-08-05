// Copyright 2022, Cronos Labs. All Rights Reserved

using System;
using System.IO;
using UnrealBuildTool;

public class CronosPlayUnreal : ModuleRules {

  protected string ThirdPartyPath {
    get {
      return Path.GetFullPath(
          Path.Combine(ModuleDirectory, "..", "..", "ThirdParty"));
    }
  }

  public CronosPlayUnreal(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
    CppStandard = CppStandardVersion.Cpp17;

    string LibrariesPath = Path.Combine(ThirdPartyPath);

    PublicIncludePaths.AddRange(new string[] {});

    PrivateIncludePaths.AddRange(new string[] { Path.Combine(ThirdPartyPath) });

    PublicDependencyModuleNames.AddRange(
        new string[] { "Core", "Json", "JsonUtilities", "HTTP"

        });

    PrivateDependencyModuleNames.AddRange(new string[] {
      "CoreUObject",
      "Engine",
      "Slate",
      "SlateCore",
    });

    DynamicallyLoadedModuleNames.AddRange(new string[] {});

    if (UnrealTargetPlatform.Win64 == Target.Platform) {
      PublicAdditionalLibraries.Add(Path.Combine(
          LibrariesPath, "cronosplay", "lib", "Win64", "play_cpp_sdk.lib"));

      PublicAdditionalLibraries.Add("Ncrypt.lib");
      PublicAdditionalLibraries.Add("userenv.lib");
      PublicAdditionalLibraries.Add("bcrypt.lib");
      PublicAdditionalLibraries.Add("ntdll.lib");
      PublicAdditionalLibraries.Add("Secur32.lib");
      PublicAdditionalLibraries.Add("crypt32.lib");
      PublicAdditionalLibraries.Add("ws2_32.lib");

    } else if (UnrealTargetPlatform.Mac == Target.Platform) {
      PublicAdditionalLibraries.Add(Path.Combine(
          LibrariesPath, "cronosplay", "lib", "Mac", "libplay_cpp_sdk.a"));
    }
  }
}
