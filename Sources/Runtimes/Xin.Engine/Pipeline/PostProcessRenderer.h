#pragma once

#include "Pipeline.Types.h"
#include "PostProcesses/Bloom.h"

namespace Xin::Engine
{
	class ENGINE_API FPostProcessRenderer
	{
	public:
		FPostProcessRenderer(FRenderGraph & RenderGraph);
		~FPostProcessRenderer();

		void Render(FRGTexture * SceneColor);

		void GenerateLuminance(FRGTexture * Luminance, FRGTexture * Color);
		void GenerateHistogram(FRGBuffer * Histogram, FRGTexture * Luminance);
		void AdaptExposure(FRGBuffer * Histogram, FRGTexture * Luminance);

		void GenerateLuminanceCS(FRGTexture * Color, FRGTexture * Luminance);
		void GenerateHistogramCS(FRGTexture * Luminance, FRGBuffer * Histogram);
		void AdaptExposureCS(FRGBuffer * Histogram, FRGTexture * Luminance);
		void ToneMappingCS(FRGTexture * Color);


	public:
		FRenderGraph & RenderGraph;

	public:
		FBloom Bloom;
		FRGBuffer * SceneExposure;
		FRGTexture * SceneLuminance;
		FRGBuffer * SceneHistogram;

		SizeU BloomSize;
	};
}
