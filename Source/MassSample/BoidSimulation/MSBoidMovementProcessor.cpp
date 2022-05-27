// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidMovementProcessor.h"

#include "MassCommonTypes.h"
#include "MassRepresentationTypes.h"
#include "MSBoidDevSettings.h"
#include "MSBoidFragments.h"

UMSBoidMovementProcessor::UMSBoidMovementProcessor()
{
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UMSBoidMovementProcessor::Initialize(UObject& Owner)
{
	BoidSubsystem = GetWorld()->GetSubsystem<UMSBoidSubsystem>();
}

void UMSBoidMovementProcessor::ConfigureQueries()
{
	CalculateForcesQuery.AddRequirement<FMSBoidLocationFragment>(EMassFragmentAccess::ReadOnly);
	CalculateForcesQuery.AddRequirement<FMSBoidForcesFragment>(EMassFragmentAccess::ReadWrite);
	
	MoveBoidsQuery.AddRequirement<FMSBoidVelocityFragment>(EMassFragmentAccess::ReadOnly);
	MoveBoidsQuery.AddRequirement<FMSBoidLocationFragment>(EMassFragmentAccess::ReadWrite);
}

void UMSBoidMovementProcessor::Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context)
{
	if (BoidSubsystem->bIsStatic) return;

	CalculateForcesQuery.ForEachEntityChunk(EntitySubsystem, Context, [this](FMassExecutionContext& Context)
	{
		const int32 NumEntities = Context.GetNumEntities();
		const float SightRadius = BoidSubsystem->BoidSettings->BoidSightRadius;
		
		const auto Locations = Context.GetFragmentView<FMSBoidLocationFragment>();
		const auto Forces = Context.GetMutableFragmentView<FMSBoidForcesFragment>();

		for (int i = 0; i < NumEntities; ++i)
		{
			const FBoxCenterAndExtent OctreeQuery = FBoxCenterAndExtent(Locations[i].Location, FVector(SightRadius));
		}
	});

	MoveBoidsQuery.ForEachEntityChunk(EntitySubsystem, Context, [this](FMassExecutionContext& Context)
	{
		const int32 NumEntities = Context.GetNumEntities();
		const auto Locations = Context.GetMutableFragmentView<FMSBoidLocationFragment>();
		const auto Velocities = Context.GetFragmentView<FMSBoidVelocityFragment>();

		for (int i = 0; i < NumEntities; ++i)
		{
			Locations[i].Location = Locations[i].Location + Velocities[i].Velocity * Context.GetDeltaTimeSeconds();
		}
	});
}
