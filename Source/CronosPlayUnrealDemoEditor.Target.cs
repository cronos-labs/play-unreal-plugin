
using UnrealBuildTool;
using System.Collections.Generic;

public class CronosPlayUnrealDemoEditorTarget : TargetRules {
  public CronosPlayUnrealDemoEditorTarget(TargetInfo Target) : base(Target) {
    Type = TargetType.Editor;
    DefaultBuildSettings = BuildSettingsVersion.V2;
    ExtraModuleNames.Add("CronosPlayUnrealDemo");
  }
}
