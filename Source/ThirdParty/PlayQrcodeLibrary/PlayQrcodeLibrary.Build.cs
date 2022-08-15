// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class PlayQrcodeLibrary : ModuleRules {
  public PlayQrcodeLibrary(ReadOnlyTargetRules Target) : base(Target) {
    Type = ModuleType.External;

    string IncludePath = Path.Combine(ModuleDirectory, "Include");
    string LibrariesPath = Path.Combine(ModuleDirectory, "Lib");
    PublicIncludePaths.Add(IncludePath);

    if (UnrealTargetPlatform.Win64 == Target.Platform) {
      PublicAdditionalLibraries.Add(
          Path.Combine(LibrariesPath, "Win64", "qrcodegen.lib"));

    } else if (UnrealTargetPlatform.Mac == Target.Platform) {
      PublicAdditionalLibraries.Add(
          Path.Combine(LibrariesPath, "Mac", "libqrcodegen.a"));
    }
  }
}
