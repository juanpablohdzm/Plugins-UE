// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PopupSystemModule : ModuleRules
{
	public PopupSystemModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"NotificationSystemModule",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}