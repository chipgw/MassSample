// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MSBoidSubsystem.h"
#include "MSBoidOctreeProcessor.generated.h"

/**
 * 
 */
UCLASS()
class MASSSAMPLE_API UMSBoidOctreeProcessor : public UMassProcessor
{
	GENERATED_BODY()

	UMSBoidOctreeProcessor();

	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context) override;

	UPROPERTY()
	UMSBoidSubsystem* BoidSubsystem;

	FMassEntityQuery RebuildOctreeQuery;
};
