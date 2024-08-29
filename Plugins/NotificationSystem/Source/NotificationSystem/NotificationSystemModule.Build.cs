// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NotificationSystemModule : ModuleRules
{
	public NotificationSystemModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
