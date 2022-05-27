// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSBoidHismHelper.generated.h"

UCLASS()
class MASSSAMPLE_API AMSBoidHismHelper : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMSBoidHismHelper();

	virtual void BeginPlay() override;

	UPROPERTY()
	UHierarchicalInstancedStaticMeshComponent* Hism;
};
