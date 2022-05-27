// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityConfigAsset.h"
#include "MSBoidDevSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Boid Settings"))
class MASSSAMPLE_API UMSBoidDevSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Assets")
	TSoftObjectPtr<UMassEntityConfigAsset> BoidEntityConfig;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Assets")
	TSoftObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Simulation")
	int32 SimulationExtentFromCenter = 40000;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Simulation")
	int32 NumOfBoids = 10;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Simulation")
	int32 BoidMaxSpeed = 50;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Simulation")
	float BoidSightRadius = 200;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Forces")
	float TargetWeight = 1;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Forces")
	float AlignWeight = 0.5;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Forces")
	float SeparationWeight = 0.5;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Forces")
	float CohesionWeight = 0.5;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Debug")
	bool DrawDebugBoxes = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids|Debug")
	bool Static = false;
};
