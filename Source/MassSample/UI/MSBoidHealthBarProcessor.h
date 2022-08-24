// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MSBoidHealthBarProcessor.generated.h"

DECLARE_STATS_GROUP(TEXT("BoidsUI"), STATGROUP_BoidsUI, STATCAT_Advanced);

class UMSBoidSubsystem;
/**
 * 
 */
UCLASS()
class MASSSAMPLE_API UMSBoidHealthBarProcessor : public UMassProcessor
{
	GENERATED_BODY()

	UMSBoidHealthBarProcessor();

	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context) override;

	UPROPERTY()
	UMSBoidSubsystem* BoidSubsystem;

	FMassEntityQuery RenderBoidsQuery;
};
