#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MSBoidFragments.generated.h"

USTRUCT()
struct FMSBoidLocationFragment : public FMassFragment
{
	GENERATED_BODY()
	FVector Location;
};

USTRUCT()
struct FMSBoidVelocityFragment : public FMassFragment
{
	GENERATED_BODY()
	FVector Velocity;
};

USTRUCT()
struct FMSBoidAccelerationFragment : public FMassFragment
{
	GENERATED_BODY()
	FVector Acceleration;
};
