// Fill out your copyright notice in the Description page of Project Settings.


#include "MSBoidHismHelper.h"

#include "MSBoidDevSettings.h"
#include "MSBoidSubsystem.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Materials/MaterialInstance.h"


// Sets default values
AMSBoidHismHelper::AMSBoidHismHelper()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMSBoidHismHelper::BeginPlay()
{
	Super::BeginPlay();

	Hism = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	Hism->RegisterComponent();
	Hism->SetEnableGravity(false);
	Hism->SetSimulatePhysics(false);
	Hism->bDisableCollision = true;

	const UMSBoidDevSettings* BoidSettings = GetDefault<UMSBoidDevSettings>();

	if (BoidSettings)
	{
		Hism->SetStaticMesh(BoidSettings->StaticMesh.LoadSynchronous());
	}

	UMSBoidSubsystem* BoidSubsystem = GetWorld()->GetSubsystem<UMSBoidSubsystem>();
	BoidSubsystem->Hism = Hism;
}

