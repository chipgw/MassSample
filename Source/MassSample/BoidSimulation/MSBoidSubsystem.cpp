// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidSubsystem.h"

#include "MassCommonFragments.h"
#include "MassEntitySubsystem.h"
#include "MSBoidDevSettings.h"
#include "MSBoidFragments.h"
#include "Common/Misc/MSBPFunctionLibrary.h"
#include "GameFramework/GameStateBase.h"

void UMSBoidSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UMSBoidDevSettings* BoidSettings = GetDefault<UMSBoidDevSettings>();

	BoidEntityConfig = BoidSettings->BoidEntityConfig.LoadSynchronous();
	SimulationExtentFromCenter = BoidSettings->SimulationExtentFromCenter;
	NumOfBoids = BoidSettings->NumOfBoids;
	BoidMaxSpeed = BoidSettings->BoidMaxSpeed;
	bDrawDebugBoxes = BoidSettings->DrawDebugBoxes;
	bIsStatic = BoidSettings->Static;

	BoidOctree = MakeUnique<FMSBoidOctree>(FVector::ZeroVector, SimulationExtentFromCenter);

	MassEntitySubsystem = GetWorld()->GetSubsystem<UMassEntitySubsystem>();

	Context = MassEntitySubsystem->CreateExecutionContext(0);

	// Execute with delay so that actors are initialized to avoid crashes
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(
		UnusedHandle, this, &UMSBoidSubsystem::SpawnRandomBoids, 2, false);
}

void UMSBoidSubsystem::DrawDebugOctree()
{
	if (bDrawDebugBoxes)
	{
		// DrawDebugBox(
		// 	GetWorld(),
		// 	BoidOctree->GetRootBounds().GetBox().GetCenter(),
		// 	BoidOctree->GetRootBounds().GetBox().GetExtent(),
		// 	FColor::Green,
		// 	false,
		// 	-1,
		// 	0,
		// 	30
		// );

		BoidOctree->FindNodesWithPredicate(
			[](FMSBoidOctree::FNodeIndex ParentNodeIndex, FMSBoidOctree::FNodeIndex CurrentNodeIndex,
			   const FBoxCenterAndExtent& Bounds)
			{
				return true;
			},
			[&](FMSBoidOctree::FNodeIndex ParentNodeIndex, FMSBoidOctree::FNodeIndex CurrentNodeIndex,
			    const FBoxCenterAndExtent& Bounds)
			{
				// debug to generate rand color for each node
				// auto RandVector = FRandomStream(CurrentNodeIndex).VRand();
				// FColor RandColor = FColor(RandVector.X * 255, RandVector.Y * 255, RandVector.Z * 255, 1);

				DrawDebugBox(
					GetWorld(),
					Bounds.GetBox().GetCenter(),
					Bounds.GetBox().GetExtent(),
					FColor::Cyan,
					false,
					-1,
					0,
					8
				);

				TArrayView<const UE::Math::TVector<double>> BoidsInNode = BoidOctree->GetElementsForNode(
					CurrentNodeIndex);
				for (UE::Math::TVector<double> Boid : BoidsInNode)
				{
					DrawDebugSphere(
						GetWorld(),
						Boid,
						100,
						8,
						FColor::Red,
						false,
						-1,
						0,
						8
					);
				}
			});
	}
}

void UMSBoidSubsystem::SpawnRandomBoids()
{
	for (int i = 0; i < NumOfBoids; ++i)
	{
		SpawnBoid();
	}
}

void UMSBoidSubsystem::SpawnBoid()
{
	if (BoidEntityConfig)
	{
		FEntityHandleWrapper NewBoid = UMSBPFunctionLibrary::SpawnEntityFromEntityConfig(
			BoidEntityConfig,
			GetWorld(),
			false
		);

		MassEntitySubsystem->GetFragmentDataChecked<FMSBoidLocationFragment>(NewBoid.Entity).Location = FMath::VRand() *
			                     FMath::RandRange(10, SimulationExtentFromCenter / 2);

		MassEntitySubsystem->GetFragmentDataChecked<FMSBoidVelocityFragment>(NewBoid.Entity).Velocity = FMath::VRand() *
			FMath::RandRange(10, BoidMaxSpeed);

		MassEntitySubsystem->GetFragmentDataChecked<FMSBoidAccelerationFragment>(NewBoid.Entity).Acceleration =
			FMath::VRand();

		if (bDrawDebugBoxes) UE_LOG(LogTemp, Warning, TEXT("UMSBoidSubsystem::SpawnBoid() id: %d, location: %s"),
		                            NewBoid.Entity.Index,
		                            *MassEntitySubsystem->GetFragmentDataChecked<FMSBoidLocationFragment>(NewBoid.Entity
		                            ).
		                            Location.
		                            ToString())		;
	}
}
