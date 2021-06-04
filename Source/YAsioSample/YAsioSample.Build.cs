// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class YAsioSample : ModuleRules
{
	public YAsioSample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Adds ThirdParty to Public Include Paths, we can easy to use header-only c++ libraries
		// PublicIncludePaths.Add(Path.Combine(ThirdPartyPath));

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OpenSSL", "yasio" });

		PrivateDependencyModuleNames.AddRange(new string[] { "yasio" });
		PrivateIncludePathModuleNames.AddRange(new string[] { "yasio" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
