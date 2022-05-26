// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidMovementProcessor.h"

#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassRepresentationTypes.h"
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
	MoveBoidsQuery.AddRequirement<FMSBoidVelocityFragment>(EMassFragmentAccess::ReadOnly);
	MoveBoidsQuery.AddRequirement<FMSBoidLocationFragment>(EMassFragmentAccess::ReadWrite);
}

void UMSBoidMovementProcessor::Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context)
{
	if (!BoidSubsystem->bIsStatic)
	{
		MoveBoidsQuery.ForEachEntityChunk(EntitySubsystem, Context, [this](FMassExecutionContext& Context)
		{
			const int32 NumEntities = Context.GetNumEntities();
			const auto Locations = Context.GetMutableFragmentView<FMSBoidLocationFragment>();
			const auto Velocities = Context.GetFragmentView<FMSBoidVelocityFragment>();

			for (int i = 0; i < NumEntities; ++i)
			{
				Locations[i].Location = Locations[i].Location + Velocities[i].Velocity;
			}
		});
	}
}
