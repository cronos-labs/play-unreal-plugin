using UnrealBuildTool;
using System.Collections.Generic;

public class CronosPlaySdkTarget : TargetRules {
  public CronosPlaySdkTarget(TargetInfo Target) : base(Target) {
    Type = TargetType.Game;
    DefaultBuildSettings = BuildSettingsVersion.V2;
    ExtraModuleNames.Add("CronosPlaySdk");
  }
}
