// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class DefiWalletCore : ModuleRules
{
	public DefiWalletCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
        
        if (UnrealTargetPlatform.Win64 == Target.Platform) {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Private/walletcore/lib/Win64", "defi_wallet_core_cpp.dll.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Private/walletcore/lib/Win64", "cxxbridge1.lib"));
        
        }
        else if (UnrealTargetPlatform.Mac == Target.Platform) {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Private/walletcore/lib/Mac", "libdefi_wallet_core_cpp.dylib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Private/walletcore/lib/Mac", "libcxxbridge1.a"));
        }
	}
}
