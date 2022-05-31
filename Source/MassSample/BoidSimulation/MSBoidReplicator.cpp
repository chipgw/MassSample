// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidReplicator.h"

#include "MSBoidFragments.h"
#include "MSBoidOctree.h"
#include "MSBoidSubsystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMSBoidReplicator::AMSBoidReplicator()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = false;
}

void AMSBoidReplicator::CheckLocations()
{
	// if (GetNetMode() == ENetMode::NM_Client) return;
	// StepNumber++;
	//
	// if (Boids.Num() == 0) return;
	//
	// // Calculate how many Boids are pending update from the array
	// int BoidsPendingUpdate = Boids.Num() - CurrentBoidIndex;
	//
	// if (BoidsPendingUpdate == 0) return;
	//
	// // Calculate how many batches are remaining
	// uint8 BatchesRemaining = BatchesPerUpdate - CurrentBatchIndex;
	//
	// // Distribute evenly the remaining Boids between the remaining batches
	// int BoidsToUpdateThisBatch = BoidsPendingUpdate / BatchesRemaining;
	//
	// // If there are so few Boids that it's less than one per batch (thus, 0), update them all
	// if (BoidsToUpdateThisBatch == 0)
	// {
	// 	BoidsToUpdateThisBatch = BoidsPendingUpdate;
	// }
	//
	// int LastBoidToUpdateIndex = BoidsToUpdateThisBatch + CurrentBoidIndex;
	//
	// // Loop from the current Boid until we finish this batch or arrive to the end of the array
	// while (
	// 	(CurrentBoidIndex < LastBoidToUpdateIndex) &&
	// 	(CurrentBoidIndex < Boids.Num())
	// 	)
	// {
	// 	FMSBoid CurrentBoid = Boids[CurrentBoidIndex];
	// 	FMSBoidCachedLocation* CachedLocation = CachedBoidLocations.Find(CurrentBoid);
	//
	// 	// UE_LOG(LogTemp, Error, TEXT("ARTSBoidLocationReplicator CurrentBoid is Valid"));
	// 	
	// 	if (!CachedLocation)
	// 	{
	// 		CachedBoidLocations.Emplace(CurrentBoid, FMSBoidCachedLocation(
	// 			CurrentBoid.Location.X,
	// 			CurrentBoid.Location.Y,
	// 			CurrentBoid.Location.Z
	// 			));
	//
	// 		CurrentBatchIndex = (CurrentBatchIndex + 1) % BatchesPerUpdate;
	// 		continue;
	// 	}
	// 	
	// 	if (CachedLocation->LocationX != CurrentBoid.Location.X || CachedLocation->LocationY != CurrentBoid.Location.Y)
	// 	{
	// 		// Cache new location
	// 		CachedLocation->LocationX = CurrentBoid.Location.X;
	// 		CachedLocation->LocationY = CurrentBoid.Location.Y;
	//
	// 		// Add new location to array for update
	// 		BoidsToUpdate.Push(FMSBoidLocationNet(
	// 			CurrentBoid.Location.X / NetUpdatePrecisionTolerance,
	// 			CurrentBoid.Location.Y / NetUpdatePrecisionTolerance,
	// 			CurrentBoid.Location.Z / NetUpdatePrecisionTolerance,
	// 			CurrentBoid.Id
	// 		));
	// 	}
	//
	// 	// Increment the CurrentBoidIndex but never surpass the array length
	// 	CurrentBoidIndex = (CurrentBoidIndex + 1) % Boids.Num();
	//
	// 	//to avoid infinite loops, break if after incrementing the Boid index is 0 again
	// 	if (CurrentBoidIndex == 0) break;
	// }
	//
	// CurrentBatchIndex = (CurrentBatchIndex + 1) % BatchesPerUpdate;
	//
	// //Ensure that every new update starts from the first Boid
	// if (CurrentBatchIndex == 0) CurrentBoidIndex = 0;
	//
	// if (BoidsToUpdate.Num() > 0) NetCastLocations(BoidsToUpdate, StepNumber);
	// UE_LOG(LogTemp, Error, TEXT("ARTSBoidLocationReplicator. Server Sent update no. %d at time %f"), StepNumber, UGameplayStatics::GetRealTimeSeconds(GetWorld()));
	//
	// BoidsToUpdate.Empty();
}

void AMSBoidReplicator::StartUpdates()
{
	if (GetNetMode() == ENetMode::NM_Client) return;

	if (GetWorld())
		GetWorldTimerManager().SetTimer(
			UpdateTimerHandle,
			this,
			&AMSBoidReplicator::CheckLocations,
			(LocationUpdateFrequency / BatchesPerUpdate),
			true
		);
}

void AMSBoidReplicator::StopUpdates()
{
	if (GetNetMode() == ENetMode::NM_Client) return;
	if (GetWorld()) GetWorldTimerManager().ClearTimer(UpdateTimerHandle);
}

void AMSBoidReplicator::AddBoid(const FMSBoid& Boid)
{
	if (GetNetMode() == ENetMode::NM_Client) return;
	//this->Boids.Push(Boid);
}

void AMSBoidReplicator::RemoveBoid(const FMSBoid& Boid)
{
	if (GetNetMode() == ENetMode::NM_Client) return;

	//this->Boids.Remove(Boid);
	//this->CachedBoidLocations.Remove(Boid);
}

bool AMSBoidReplicator::IsUpdateValid()
{
	float CurrentUpdateTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	if (LastUpdateTime == 0)
	{
		LastUpdateTime = CurrentUpdateTime;
		return true;
	}
	
	float TimeLapseBetweenUpdates = CurrentUpdateTime - LastUpdateTime;
	
	if (FMath::IsNearlyEqual(TimeLapseBetweenUpdates, LocationUpdateFrequency / BatchesPerUpdate, NetUpdateTimeThreshold))
	{
		UE_LOG(LogTemp, Error, TEXT("ARTSBoidLocationReplicator. Valid Update \n Current Time: %f\n Last Update Time: %f\n TimeLapse: %f"), CurrentUpdateTime, LastUpdateTime, TimeLapseBetweenUpdates);
		LastUpdateTime = CurrentUpdateTime;
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("ARTSBoidLocationReplicator. Invalid Update \n Current Time: %f\n Last Update Time: %f\n TimeLapse: %f"), CurrentUpdateTime, LastUpdateTime, TimeLapseBetweenUpdates);

	LastUpdateTime = CurrentUpdateTime;
	return false;
}

void AMSBoidReplicator::NetCastLocations_Implementation(const TArray<FMSBoidLocationNet>& BoidLocations,
                                                        int32 ServerStepNumber)
{
	if (GetNetMode() != ENetMode::NM_Client) return;

	UE_LOG(LogTemp, Error, TEXT("ARTSBoidLocationReplicator. Client recieve update no. %d at time %f"), ServerStepNumber, UGameplayStatics::GetRealTimeSeconds(GetWorld()));

	//if (!IsUpdateValid()) return;
	
	for (const auto& BoidLocation : BoidLocations)
	{
		FMassEntityHandle* CurrentBoidHandle = BoidSubsystem->NetIdMassHandleMap.Find(BoidLocation.BoidId);
		FVector CurrentLocation = BoidSubsystem->MassEntitySubsystem->GetFragmentDataChecked<FMSBoidLocationFragment>(*CurrentBoidHandle).Location;
		FVector ServerLocation = FVector(
			BoidLocation.LocationX * NetUpdatePrecisionTolerance,
			BoidLocation.LocationY * NetUpdatePrecisionTolerance,
			BoidLocation.LocationZ * NetUpdatePrecisionTolerance
			);

		if (!CurrentLocation.Equals(ServerLocation, NetUpdatePrecisionTolerance))
		{
			// BoidLocation.Boid->ApplyServerLocationUpdate(ServerLocation);
		}
	}
}

