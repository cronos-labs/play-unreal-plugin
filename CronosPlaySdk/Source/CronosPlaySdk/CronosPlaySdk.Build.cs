
using UnrealBuildTool;

public class CronosPlaySdk : ModuleRules {
  public CronosPlaySdk(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject",
                                                        "Engine", "InputCore",
                                                        "HeadMountedDisplay" });
  }
}
