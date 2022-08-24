// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarContainerHud.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Slate/SMeshWidget.h"
#include "Slate/SlateVectorArtInstanceData.h"
#include "Slate/SlateVectorArtData.h"

UHealthBarContainerHud::UHealthBarContainerHud()
	: MeshId(-1)
{
}

void UHealthBarContainerHud::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MeshAsset)
	{
		MeshId = MeshWidget->AddMesh(*MeshAsset);
	}
}

void UHealthBarContainerHud::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MeshWidget.Reset();
}

void UHealthBarContainerHud::StartHealthData(int32 Size)
{
	InstanceData.Empty(Size);
}

void UHealthBarContainerHud::AddHealthBarData(const FVector& Location, float HealthPercent, float BarWidth)
{
	// this struct is outdated because it contains the now double-based FVector4, but we need the float-based FVector4f.
	//FSlateVectorArtInstanceData BarData;
	FVector4f BarData;

	if (FVector2D ScreenPosition; UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), Location, ScreenPosition, false)) {
		// This assumes that this widget fills the viewport
		ScreenPosition = GetPaintSpaceGeometry().LocalToAbsolute(ScreenPosition);
		BarData.X = ScreenPosition.X;
		BarData.Y = ScreenPosition.Y;
		BarData.Z = BarWidth;
		// TODO - might be nice to track previous health and animate the changes in some way
		BarData.W = HealthPercent;

		InstanceData.Add(BarData);
	}
}

void UHealthBarContainerHud::FinalizeHealthData()
{
	if (MeshId != -1)
	{
		MeshWidget->UpdatePerInstanceBuffer(MeshId, InstanceData);
	}
}

TSharedRef<SWidget> UHealthBarContainerHud::RebuildWidget()
{
	MeshWidget = SNew(SMeshWidget);

	return MeshWidget.ToSharedRef();
}
