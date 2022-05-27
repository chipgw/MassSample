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
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids")
	TSoftObjectPtr<UMassEntityConfigAsset> BoidEntityConfig;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids")
	TSoftObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids")
	int32 SimulationExtentFromCenter = 40000;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids")
	int32 NumOfBoids = 10;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids")
	int32 BoidMaxSpeed = 50;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids")
	bool DrawDebugBoxes = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Boids")
	bool Static = false;
};
