#pragma once

#include "Pipeline.Types.h"

namespace Xin::Engine
{
	class FRGResourceView;
	class FSceneView;

	class ENGINE_API FShadowDepthRenderer
	{
	public:
		FShadowDepthRenderer(FRenderGraph & RenderGraph);
		~FShadowDepthRenderer() = default;

		void Render(const FDirectionalLight & DirectionalLight);
		void RenderSM(const FMatrix & ProjectMatrix, FRGResourceView * ShadowDepthRTV, FDrawCommandCollector & CommandCollector);

	public:
		FRenderGraph & RenderGraph;
		TArray<FDrawCommandCollector, CSMCascadeMax> CommandCollectors {};

	public:
		uint32 ShadowDepthTextureSize = 1024;
		float4x4 LightViewMatrix {};
		TArray<float4x4, CSMCascadeMax> ProjectMatrix {};

	public:
		FRGTexture * ShadowDepth;
	};
}
