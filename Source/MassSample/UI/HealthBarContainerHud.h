// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "HealthBarContainerHud.generated.h"

/**
 * 
 */
UCLASS()
class MASSSAMPLE_API UHealthBarContainerHud : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USlateVectorArtData* MeshAsset;

	UHealthBarContainerHud();

	virtual void SynchronizeProperties() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	/**
	* Call to clear previous frame health data before processing this frame
	* 
	* @param Size number of items to allocate space for
	*/
	void StartHealthData(int32 Size);

	/**
	* Add health bar data for a single item
	* 
	* @param Location		world space location of the health bar
	* @param HealthPercent	amount of the bar to fill, ranging from 0 to 1.
	* @param BarWidth		optional multiplier for displaying a bar that's wider or narrower than normal
	*/
	void AddHealthBarData(const FVector& Location, float HealthPercent, float BarWidth = 1.0f);

	/**
	* Call when done adding health data to send mesh instance data to the GPU.
	*/
	void FinalizeHealthData();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedPtr<class SMeshWidget> MeshWidget;

	TArray<FVector4f> InstanceData;

	int32 MeshId;
};
