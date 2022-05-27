// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityConfigAsset.h"
#include "MassEntitySubsystem.h"
#include "MSBoidOctree.h"
#include "MSBoidSubsystem.generated.h"

class UMSBoidDevSettings;
class UMassEntitySubsystem;
/**
 * 
 */
UCLASS()
class MASSSAMPLE_API UMSBoidSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

public:

	void GetBoidsInRadius(const FBoxCenterAndExtent& QueryBox);

	UPROPERTY(Transient)
	UMassEntitySubsystem* MassEntitySubsystem;

	UPROPERTY()
	UMassEntityConfigAsset* BoidEntityConfig;
	FMassExecutionContext Context;
	
	TUniquePtr<FMSBoidOctree> BoidOctree;

	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* Hism;

	UPROPERTY()
	UMSBoidDevSettings* BoidSettings;
	
	void DrawDebugOctree();

private:

	void SpawnRandomBoids();

	void SpawnBoid();

	int32 SimulationExtentFromCenter;
	int32 NumOfBoids;
	int32 BoidMaxSpeed;

public:
	bool bDrawDebugBoxes;
	bool bIsStatic;
};
