// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MSBoidHashGridProcessor.generated.h"

class UMSBoidSubsystem;
/**
 * 
 */
UCLASS()
class MASSSAMPLE_API UMSBoidHashGridProcessor : public UMassProcessor
{
	GENERATED_BODY()

	UMSBoidHashGridProcessor();

	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context) override;

	UPROPERTY()
	UMSBoidSubsystem* BoidSubsystem;

	FMassEntityQuery UpdateHashGrid;
};
