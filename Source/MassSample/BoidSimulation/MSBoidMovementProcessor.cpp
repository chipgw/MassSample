// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidMovementProcessor.h"

#include "MassCommonTypes.h"
#include "MassRepresentationTypes.h"
#include "MSBoidDevSettings.h"
#include "MSBoidFragments.h"
#include "Kismet/KismetMathLibrary.h"

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

	CalculateVelocityQuery.AddRequirement<FMSBoidForcesFragment>(EMassFragmentAccess::ReadOnly);
	CalculateVelocityQuery.AddRequirement<FMSBoidVelocityFragment>(EMassFragmentAccess::ReadWrite);

	MoveBoidsQuery.AddRequirement<FMSBoidVelocityFragment>(EMassFragmentAccess::ReadOnly);
	MoveBoidsQuery.AddRequirement<FMSBoidLocationFragment>(EMassFragmentAccess::ReadWrite);
}

void UMSBoidMovementProcessor::Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context)
{
	if (BoidSubsystem->bIsStatic) return;

	CalculateForcesQuery.ForEachEntityChunk(EntitySubsystem, Context, [this](FMassExecutionContext& Context)
	{
		const int32 NumEntities = Context.GetNumEntities();
		const float SightRadius = BoidSubsystem->BoidSightRadius;
		const float TargetWeight = BoidSubsystem->TargetWeight;
		const float AlignWeight = BoidSubsystem->AlignWeight;
		const float CohesionWeight = BoidSubsystem->CohesionWeight;
		const float SeparationWeight = BoidSubsystem->SeparationWeight;

		const auto Locations = Context.GetFragmentView<FMSBoidLocationFragment>();
		const auto Forces = Context.GetMutableFragmentView<FMSBoidForcesFragment>();

		for (int i = 0; i < NumEntities; ++i)
		{
			const FBoxCenterAndExtent OctreeQuery = FBoxCenterAndExtent(Locations[i].Location, FVector(SightRadius));
			const TArray<FMSBoidInOctree> BoidsInRadius = BoidSubsystem->GetBoidsInRadius(OctreeQuery);
			TArray<FVector> BoidVelocities;
			TArray<FVector> BoidLocations;
			TArray<FVector> BoidRepulsionForces;

			for (const FMSBoidInOctree& Boid : BoidsInRadius)
			{
				BoidVelocities.Push(Boid.Velocity);
				BoidLocations.Push(Boid.Location);
				FVector RepulsionDirection = Locations[i].Location - Boid.Location;
				//RepulsionDirection = RepulsionDirection * (1 / RepulsionDirection.Length());

				BoidRepulsionForces.Push(RepulsionDirection);
			}

			const auto TargetForce = FVector::ZeroVector - Locations[i].Location;
			const auto AlignForce = UKismetMathLibrary::GetVectorArrayAverage(BoidVelocities);
			const auto AverageLocations = UKismetMathLibrary::GetVectorArrayAverage(BoidLocations);
			const auto AverageRepulsion = UKismetMathLibrary::GetVectorArrayAverage(BoidRepulsionForces);

			const auto CohesionForce = AverageLocations - Locations[i].Location;

			Forces[i].ForceResult = (AlignForce * AlignWeight) + (CohesionForce * CohesionWeight) + (AverageRepulsion *
				SeparationWeight) + (TargetForce * TargetWeight);
		}
	});

	CalculateVelocityQuery.ForEachEntityChunk(EntitySubsystem, Context, [this](FMassExecutionContext& Context)
	{
		const int32 NumEntities = Context.GetNumEntities();
		const auto Velocities = Context.GetMutableFragmentView<FMSBoidVelocityFragment>();
		const auto Forces = Context.GetFragmentView<FMSBoidForcesFragment>();

		for (int i = 0; i < NumEntities; ++i)
		{
			Velocities[i].Velocity = UKismetMathLibrary::ClampVectorSize(
				(Velocities[i].Velocity + Forces[i].ForceResult), 0, BoidSubsystem->BoidMaxSpeed);
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
