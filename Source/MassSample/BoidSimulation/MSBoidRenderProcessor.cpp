// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidRenderProcessor.h"

#include "MassRepresentationTypes.h"
#include "MSBoidFragments.h"
#include "MSBoidMovementProcessor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

UMSBoidRenderProcessor::UMSBoidRenderProcessor()
{
	ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::Movement);
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UMSBoidRenderProcessor::Initialize(UObject& Owner)
{
	BoidSubsystem = GetWorld()->GetSubsystem<UMSBoidSubsystem>();
}

void UMSBoidRenderProcessor::ConfigureQueries()
{
	RenderBoidsQuery.AddRequirement<FMSBoidLocationFragment>(EMassFragmentAccess::ReadOnly);
	RenderBoidsQuery.AddRequirement<FMSBoidRenderFragment>(EMassFragmentAccess::ReadOnly);
}

void UMSBoidRenderProcessor::Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context)
{
	RenderBoidsQuery.ForEachEntityChunk(EntitySubsystem, Context, [this](FMassExecutionContext& Context)
	{
		const int32 NumEntities = Context.GetNumEntities();
		const auto Locations = Context.GetFragmentView<FMSBoidLocationFragment>();
		const auto HismIndexes = Context.GetFragmentView<FMSBoidRenderFragment>();

		for (int i = 0; i < NumEntities; ++i)
		{
			const FVector& Location = Locations[i].Location;
			const uint32 HismIndex = HismIndexes[i].HismId;

			BoidSubsystem->Hism->UpdateInstanceTransform(
				HismIndex,
				FTransform(Location),
				true,
				false,
				true
			);
		}
	});

	if (BoidSubsystem && BoidSubsystem->Hism)
		BoidSubsystem->Hism->MarkRenderTransformDirty();
}
