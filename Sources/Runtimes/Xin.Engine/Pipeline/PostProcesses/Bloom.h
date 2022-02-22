#pragma once

#include "../../Xin.Engine.Types.h"

namespace Xin::Engine
{
	class ENGINE_API FBloom
	{
	public:
		FBloom(FRenderGraph & RenderGraph);
		~FBloom();

		void Render(FRGTexture * SceneColor, FRGBuffer * Exposure);

		void RenderPS(FRGTexture * SceneColor, FRGBuffer * Exposure);
		void PreparePS(FRGTexture * SceneColor, FRGBuffer * Exposure);
		void DownsamplePS_4x16x();
		void DownsamplePS_2x4x8x16x();
		void BlendBlurPS(FRGTexture * Output, FRGTexture * Input, FRGTexture * InputBlend, float32 BlendFactor);

		void RenderCS(FRGTexture * SceneColor, FRGBuffer * Exposure);
		void PrepareCS(FRGTexture * SceneColor, FRGBuffer * Exposure);
		void DownsampleCS_4x16x();
		void DownsampleCS_2x4x8x16x();
		void BlendBlurCS(FRGTexture * Output, FRGTexture * Input, FRGTexture * InputBloomBlend, float32 BlendFactor);

	public:
		FRenderGraph & RenderGraph;

	private:
		SizeU BloomSize;

	public:
		FRGTexture * BloomLuminance;
		FRGTexture * Bloom1xA;
		FRGTexture * Bloom1xB;
		FRGTexture * Bloom2xA;
		FRGTexture * Bloom2xB;
		FRGTexture * Bloom4xA;
		FRGTexture * Bloom4xB;
		FRGTexture * Bloom8xA;
		FRGTexture * Bloom8xB;
		FRGTexture * Bloom16xA;
		FRGTexture * Bloom16xB;
	};
}
