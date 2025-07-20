// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WebToCPPTest : ModuleRules
{
	public WebToCPPTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "WebBrowserWidget" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
