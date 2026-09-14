// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectKillerTime : ModuleRules
{
	public ProjectKillerTime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ProjectKillerTime",
			"ProjectKillerTime/Variant_Platforming",
			"ProjectKillerTime/Variant_Platforming/Animation",
			"ProjectKillerTime/Variant_Combat",
			"ProjectKillerTime/Variant_Combat/AI",
			"ProjectKillerTime/Variant_Combat/Animation",
			"ProjectKillerTime/Variant_Combat/Gameplay",
			"ProjectKillerTime/Variant_Combat/Interfaces",
			"ProjectKillerTime/Variant_Combat/UI",
			"ProjectKillerTime/Variant_SideScrolling",
			"ProjectKillerTime/Variant_SideScrolling/AI",
			"ProjectKillerTime/Variant_SideScrolling/Gameplay",
			"ProjectKillerTime/Variant_SideScrolling/Interfaces",
			"ProjectKillerTime/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
