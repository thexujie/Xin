#pragma once

#include "00.Test.RHI.Types.h"

struct FPlaneVertex
{
	float3 Positin;
	float2 Texcoord;
};

const TArray<FPlaneVertex, 6> PlanePureVertices =
{
	{{-1, 1, 1}, {0, 0}},
	{{1, 1, 1}, {1, 0}},
	{{1, -1, 1}, {1, 1}},

	{{1, -1, 1}, {1, 1}},
	{{-1, -1, 1}, {0, 1}},
	{{-1, 1, 1}, {0, 0}},
};
