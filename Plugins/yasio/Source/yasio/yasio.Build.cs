// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
public class yasio : ModuleRules
{
	public yasio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseRTTI = true;
		PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private")});
		PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public")});
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "OpenSSL", "Core"
				// ... add private dependencies that you statically link with here ...	
			}
			);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		PublicDefinitions.Add("YASIO_SSL_BACKEND=1");
		PublicDefinitions.Add("YASIO_BUILD_SHARED_LIBS=1");
		PrivateDefinitions.Add("YASIO_LIB=1");
	}
}
