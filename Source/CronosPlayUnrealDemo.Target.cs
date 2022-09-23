
using UnrealBuildTool;
using System.Collections.Generic;

public class CronosPlayUnrealDemoTarget : TargetRules {
  public CronosPlayUnrealDemoTarget(TargetInfo Target) : base(Target) {
    Type = TargetType.Game;
    DefaultBuildSettings = BuildSettingsVersion.V2;
    ExtraModuleNames.Add("CronosPlayUnrealDemo");
  }
}
