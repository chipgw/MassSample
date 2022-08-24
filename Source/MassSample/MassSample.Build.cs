// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MassSample : ModuleRules
{
	public MassSample(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
		PublicIncludePaths.AddRange(
            new string[] {
                "MassSample"
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
            }
        );

        PublicDependencyModuleNames.AddRange(
			new string[] { 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"SlateCore"
			}
		);

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"MassEntity",
				"StructUtils",
				"MassCommon",
				"MassMovement",
				"MassActors",
				"MassSpawner",
				"MassGameplayDebug",
				"MassSignals",
				"MassCrowd",
				"MassActors",
				"MassSpawner",
				"MassRepresentation",
				"MassReplication",
				"MassNavigation",
				//needed for replication setup
				"NetCore",
				"AIModule",

				"ZoneGraph",
				"MassGameplayDebug",
				"MassZoneGraphNavigation", 
				"Niagara",
				"DeveloperSettings",
				"GeometryCore",
				"MassAIBehavior",
				"StateTreeModule",
				"MassLOD",
				"NavigationSystem",
				//todo: maybe do thee editor only stuff on another module?
				
			}
		);
        
        
        
        if (Target.bBuildEditor)
        {
	        PrivateDependencyModuleNames.Add("CodeView");
	        PrivateDependencyModuleNames.Add("UnrealEd");
        }
	}
}
