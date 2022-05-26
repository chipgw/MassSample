// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MSBoidTrait.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName="BoidTrait"))
class MASSSAMPLE_API UMSBoidTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()
	
public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, UWorld& World) const override;
};
