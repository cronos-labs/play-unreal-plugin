
using UnrealBuildTool;
using System.Collections.Generic;

public class CronosPlaySdkEditorTarget : TargetRules
{
	public CronosPlaySdkEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("CronosPlaySdk");
	}
}
