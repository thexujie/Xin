#pragma once

#include "RenderGraph.h"

namespace Xin::Engine
{
	static constexpr int SSAOSampleWeightCount = 12;

	class ENGINE_API FSSAO
	{
	public:
		FSSAO(FRenderGraph & RenderGraph);
		~FSSAO();

		void Render(FRGTexture * SceneDepth);
		void RenderPS(FRGTexture * SceneDepth);
		void RenderCS(FRGTexture * SceneDepth);

		void DownsamplePS(FRGTexture * Depth2x, FRGTexture * DepthTiled8x, FRGTexture * Depth4x, FRGTexture * DepthTiled16x, FRGTexture * Depth1x, FRGTexture * SceneDepth);
		void DownsampleTilePS(FRGTexture * DepthTiled4x, FRGTexture * Depth1x);
		void ComputePS(FRGTexture * Occlusion, FRGTexture * Depth1xOrTiled4x);
		void UpsamplePS(FRGTexture * OcclusionX2, FRGTexture * DepthX2, FRGTexture * Depth, FRGTexture * OcclusionHQ, FRGTexture * OcclusionMerged, FRGTexture * OcclusionMergedX2);

		void DownsampleCS(FRGTexture * Depth2x, FRGTexture * DepthTiled8x, FRGTexture * Depth4x, FRGTexture * DepthTiled16x, FRGTexture * Depth1x, FRGTexture * SceneDepth);
		void ComputeCS(FRGTexture * Occlusion, FRGTexture * DepthTile4x);
		void UpsampleCS(FRGTexture * OcclusionX2, FRGTexture * DepthX2, FRGTexture * Depth, FRGTexture * OcclusionHQ, FRGTexture * OcclusionMerged, FRGTexture * OcclusionMergedX2);

	public:
		FRenderGraph & RenderGraph;

		float ThicknessMultiplier = 1.0f;
		TArray<float, SSAOSampleWeightCount> ThicknessRcps {};
		TArray<float, SSAOSampleWeightCount> SampleWeights {};

	public:
		FRGTexture * SSAODepth1x;

		FRGTexture * SSAODepth2x;
		FRGTexture * SSAODepth4x;
		FRGTexture * SSAODepth8x;
		FRGTexture * SSAODepth16x;

		FRGTexture * SSAODepthTiled8x;
		FRGTexture * SSAODepthTiled16x;
		FRGTexture * SSAODepthTiled32x;
		FRGTexture * SSAODepthTiled64x;

		FRGTexture * SSAOOcclusionHQ2x;
		FRGTexture * SSAOOcclusionHQ4x;
		FRGTexture * SSAOOcclusionHQ8x;
		FRGTexture * SSAOOcclusionHQ16x;

		FRGTexture * SSAOOcclusionMerged2x;
		FRGTexture * SSAOOcclusionMerged4x;
		FRGTexture * SSAOOcclusionMerged8x;
		FRGTexture * SSAOOcclusionMerged16x;

		FRGTexture * SSAOOcclusionSmooth2x;
		FRGTexture * SSAOOcclusionSmooth4x;
		FRGTexture * SSAOOcclusionSmooth8x;

		FRGTexture * SSAOOcclusion;
	};
}
