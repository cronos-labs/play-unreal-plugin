
using UnrealBuildTool;

public class CronosPlayUnrealDemo : ModuleRules {
  public CronosPlayUnrealDemo(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject",
                                                        "Engine", "InputCore",
                                                        "HeadMountedDisplay" });
  }
}
