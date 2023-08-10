/*
Copyright 2022, Cronos Labs.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

using System.IO;
using UnrealBuildTool;
using System.Diagnostics;
public class PlayCppSdkLibrary : ModuleRules {
    public PlayCppSdkLibrary(ReadOnlyTargetRules Target) : base(Target) {
        Type = ModuleType.External;
        CppStandard = CppStandardVersion.Cpp17;

        string IncludePath = Path.Combine(ModuleDirectory, "Include");
        string LibrariesPath = Path.Combine(ModuleDirectory, "Lib");

        PublicIncludePaths.Add(IncludePath);

        if (Target.Platform == UnrealTargetPlatform.Win64) {
            PublicAdditionalLibraries.Add(
                Path.Combine(LibrariesPath, "Win64", "play_cpp_sdk.lib"));

            PublicAdditionalLibraries.Add("Ncrypt.lib");
            PublicAdditionalLibraries.Add("userenv.lib");
            PublicAdditionalLibraries.Add("bcrypt.lib");
            PublicAdditionalLibraries.Add("ntdll.lib");
            PublicAdditionalLibraries.Add("Secur32.lib");
            PublicAdditionalLibraries.Add("crypt32.lib");
            PublicAdditionalLibraries.Add("ws2_32.lib");

        } else if (Target.Platform == UnrealTargetPlatform.Mac) {
            PublicAdditionalLibraries.Add(
                Path.Combine(LibrariesPath, "Mac", "libplay_cpp_sdk.a"));
        } else if (Target.Platform == UnrealTargetPlatform.Linux) {
            PublicAdditionalLibraries.Add(
                Path.Combine(LibrariesPath, "Linux", "libplay_cpp_sdk.a"));
        } else if (Target.Platform == UnrealTargetPlatform.Android) {
            PublicAdditionalLibraries.Add(Path.Combine(
                LibrariesPath, "Android", "arm64-v8a", "libplay_cpp_sdk.a"));
            PublicAdditionalLibraries.Add(Path.Combine(
                LibrariesPath, "Android", "armeabi-v7a", "libplay_cpp_sdk.a"));
            PublicAdditionalLibraries.Add(Path.Combine(
                LibrariesPath, "Android", "x86_64", "libplay_cpp_sdk.a"));
        } else if (Target.Platform == UnrealTargetPlatform.IOS) {
            PublicAdditionalLibraries.Add(
                Path.Combine(LibrariesPath, "iOS", "libplay_cpp_sdk.a"));
        }
    }
}
