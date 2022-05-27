#pragma once

#include "CoreMinimal.h"
#include "Math/GenericOctree.h"

struct FMSBoidInOctree
{
	FVector Location;
	FVector Velocity;

	FMSBoidInOctree(FVector InLocation, FVector InVelocity) : Location(InLocation), Velocity(InVelocity)
	{}
};

struct FMSBoidOctreeSemantics
{
	// When a leaf gets more than this number of elements, it will split itself into a node with multiple child leaves
	enum { MaxElementsPerLeaf = 10 };

	// This is used for incremental updates.  When removing a polygon, larger values will cause leaves to be removed and collapsed into a parent node.
	enum { MinInclusiveElementsPerNode = 5 };

	// How deep the tree can go.
	enum { MaxNodeDepth = 20 };


	typedef TInlineAllocator<MaxElementsPerLeaf> ElementAllocator;

	FORCEINLINE static FBoxCenterAndExtent GetBoundingBox(const FMSBoidInOctree& Element)
	{
		// ignore size, just store a small 0x0x0 box at the actor location. Test if more performant
		return FBoxCenterAndExtent(Element.Location, FVector(0, 0, 0));
	}

	FORCEINLINE static bool AreElementsEqual(const FMSBoidInOctree& A, const FMSBoidInOctree& B)
	{
		return (A.Location == B.Location);
	}

	FORCEINLINE static void SetElementId(const FMSBoidInOctree& Element, FOctreeElementId2 OctreeElementID)
	{}
};

typedef TOctree2<FMSBoidInOctree, FMSBoidOctreeSemantics> FMSBoidOctree;
