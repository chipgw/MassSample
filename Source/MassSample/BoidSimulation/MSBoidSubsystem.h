// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityConfigAsset.h"
#include "MassEntitySubsystem.h"
#include "MSBoidQuadtree.h"
#include "MSBoidSubsystem.generated.h"

class UMassEntitySubsystem;
/**
 * 
 */
UCLASS()
class MASSSAMPLE_API UMSBoidSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	UPROPERTY(Transient)
	UMassEntitySubsystem* MassEntitySubsystem;

	UPROPERTY()
	UMassEntityConfigAsset* BoidEntityConfig;
	FMassExecutionContext Context;
	
	TUniquePtr<FMSBoidOctree> BoidOctree;
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
