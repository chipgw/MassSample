// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidHashGridProcessor.h"

#include "MSBoidFragments.h"
#include "MSBoidMovementProcessor.h"

UMSBoidHashGridProcessor::UMSBoidHashGridProcessor()
{
	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Movement);
	ExecutionFlags = (int32)EProcessorExecutionFlags::None;
}

void UMSBoidHashGridProcessor::Initialize(UObject& Owner)
{
	BoidSubsystem = GetWorld()->GetSubsystem<UMSBoidSubsystem>();
}

void UMSBoidHashGridProcessor::ConfigureQueries()
{
	UpdateHashGrid.AddRequirement<FMSBoidCachedLocationFragment>(EMassFragmentAccess::ReadWrite);
	UpdateHashGrid.AddRequirement<FMSBoidLocationFragment>(EMassFragmentAccess::ReadOnly);
}

void UMSBoidHashGridProcessor::Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context)
{
	UpdateHashGrid.ForEachEntityChunk(EntitySubsystem, Context, [this](FMassExecutionContext& Context)
	{
		const int32 NumEntities = Context.GetNumEntities();
		const auto Locations = Context.GetFragmentView<FMSBoidLocationFragment>();
		const auto CachedLocations = Context.GetMutableFragmentView<FMSBoidCachedLocationFragment>();

		for (int i = 0; i < NumEntities; ++i)
		{
			const FVector& Location = Locations[i].Location;
			const FVector& CachedLocation = CachedLocations[i].CachedLocation;

			BoidSubsystem->HashGrid.UpdatePoint(Context.GetEntity(i), CachedLocation, Location);

			// update the cached location for next processor execution
			CachedLocations[i].CachedLocation = Location;
		}
	});
}
