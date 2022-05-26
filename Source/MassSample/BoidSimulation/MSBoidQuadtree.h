#pragma once

#include "CoreMinimal.h"
#include "Math/GenericOctree.h"

struct FMSBoidQuadtreeSemantics
{
	// When a leaf gets more than this number of elements, it will split itself into a node with multiple child leaves
	enum { MaxElementsPerLeaf = 10 };

	// This is used for incremental updates.  When removing a polygon, larger values will cause leaves to be removed and collapsed into a parent node.
	enum { MinInclusiveElementsPerNode = 5 };

	// How deep the tree can go.
	enum { MaxNodeDepth = 20 };


	typedef TInlineAllocator<MaxElementsPerLeaf> ElementAllocator;

	FORCEINLINE static FBoxCenterAndExtent GetBoundingBox(const FVector& Element)
	{
		// ignore size, just store a small 0x0x0 box at the actor location. Test if more performant
		return FBoxCenterAndExtent(Element, FVector(0, 0, 0));
	}

	FORCEINLINE static bool AreElementsEqual(const FVector& A, const FVector& B)
	{
		return (A == B);
	}

	FORCEINLINE static void SetElementId(const FVector& Element, FOctreeElementId2 OctreeElementID)
	{}
};

typedef TOctree2<FVector, FMSBoidQuadtreeSemantics> FMSBoidOctree;
