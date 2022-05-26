﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidOctreeProcessor.h"

#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassRepresentationTypes.h"
#include "MSBoidFragments.h"

UMSBoidOctreeProcessor::UMSBoidOctreeProcessor()
{
	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Movement);
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UMSBoidOctreeProcessor::Initialize(UObject& Owner)
{
	BoidSubsystem = GetWorld()->GetSubsystem<UMSBoidSubsystem>();
}

void UMSBoidOctreeProcessor::ConfigureQueries()
{
	RebuildOctreeQuery.AddRequirement<FMSBoidLocationFragment>(EMassFragmentAccess::ReadOnly);
}

void UMSBoidOctreeProcessor::Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context)
{
	// Reset octree each frame
	BoidSubsystem->BoidOctree->Destroy();

	RebuildOctreeQuery.ForEachEntityChunk(EntitySubsystem, Context, [this](FMassExecutionContext& Context)
	{
		const int32 NumEntities = Context.GetNumEntities();
		const auto Locations = Context.GetFragmentView<FMSBoidLocationFragment>();

		for (int i = 0; i < NumEntities; ++i)
		{
			const FVector& Location = Locations[i].Location;
			BoidSubsystem->BoidOctree->AddElement(Location);
		}
	});

	BoidSubsystem->DrawDebugOctree();
}