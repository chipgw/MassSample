// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidHealthBarProcessor.h"

#include "MassRepresentationTypes.h"
#include "HealthBarContainerHud.h"
#include "BoidSimulation/MSBoidFragments.h"
#include "BoidSimulation/MSBoidMovementProcessor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

DECLARE_CYCLE_STAT(TEXT("Boids Render ~ Health Bar update"), STAT_RenderUI, STATGROUP_BoidsUI);

UMSBoidHealthBarProcessor::UMSBoidHealthBarProcessor()
{
	ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::Movement);
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UMSBoidHealthBarProcessor::Initialize(UObject& Owner)
{
	BoidSubsystem = GetWorld()->GetSubsystem<UMSBoidSubsystem>();
}

void UMSBoidHealthBarProcessor::ConfigureQueries()
{
	RenderBoidsQuery.AddRequirement<FMSBoidLocationFragment>(EMassFragmentAccess::ReadOnly);
}

void UMSBoidHealthBarProcessor::Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context)
{
	SCOPE_CYCLE_COUNTER(STAT_RenderUI);
	if (BoidSubsystem && BoidSubsystem->HealthBarHud) {
		BoidSubsystem->HealthBarHud->StartHealthData(RenderBoidsQuery.GetNumMatchingEntities(EntitySubsystem));

		RenderBoidsQuery.ForEachEntityChunk(EntitySubsystem, Context, [this](FMassExecutionContext& Context)
		{
			const int32 NumEntities = Context.GetNumEntities();
			const auto Locations = Context.GetFragmentView<FMSBoidLocationFragment>();

			for (int32 i = 0; i < NumEntities; ++i)
			{
				const FVector& Location = Locations[i].Location;
				// TODO - Add Health Data
				BoidSubsystem->HealthBarHud->AddHealthBarData(Location, 0.9f);
			}
		});

		BoidSubsystem->HealthBarHud->FinalizeHealthData();
	}
}
