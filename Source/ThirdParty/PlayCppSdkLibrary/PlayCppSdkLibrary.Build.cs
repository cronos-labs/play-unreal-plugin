// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class PlayCppSdkLibrary : ModuleRules {
  public PlayCppSdkLibrary(ReadOnlyTargetRules Target) : base(Target) {
    Type = ModuleType.External;
    CppStandard = CppStandardVersion.Cpp17;

    string IncludePath = Path.Combine(ModuleDirectory, "Include");
    string LibrariesPath = Path.Combine(ModuleDirectory, "Lib");

    PublicIncludePaths.Add(IncludePath);

    if (UnrealTargetPlatform.Win64 == Target.Platform) {
      PublicAdditionalLibraries.Add(
          Path.Combine(LibrariesPath, "Win64", "play_cpp_sdk.lib"));

      PublicAdditionalLibraries.Add("Ncrypt.lib");
      PublicAdditionalLibraries.Add("userenv.lib");
      PublicAdditionalLibraries.Add("bcrypt.lib");
      PublicAdditionalLibraries.Add("ntdll.lib");
      PublicAdditionalLibraries.Add("Secur32.lib");
      PublicAdditionalLibraries.Add("crypt32.lib");
      PublicAdditionalLibraries.Add("ws2_32.lib");

    } else if (UnrealTargetPlatform.Mac == Target.Platform) {
      PublicAdditionalLibraries.Add(
          Path.Combine(LibrariesPath, "Mac", "libplay_cpp_sdk.a"));
    } else if (UnrealTargetPlatform.Linux == Target.Platform) {
      PublicAdditionalLibraries.Add(
          Path.Combine(LibrariesPath, "Linux", "libplay_cpp_sdk.a"));
    }
  }
}
