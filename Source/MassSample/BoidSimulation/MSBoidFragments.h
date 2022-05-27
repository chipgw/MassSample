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
struct FMSBoidForcesFragment : public FMassFragment
{
	GENERATED_BODY()
	FVector ForceResult;
};

USTRUCT()
struct FMSBoidTargetFragment : public FMassFragment
{
	GENERATED_BODY()
	FVector Target;
};

USTRUCT()
struct FMSBoidRenderFragment : public FMassFragment
{
	GENERATED_BODY()
	uint32 HismId;
};
