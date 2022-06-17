using UnrealBuildTool;

public class DefiWalletCoreUnreal : ModuleRules
{
    public DefiWalletCoreUnreal(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
    }
}
