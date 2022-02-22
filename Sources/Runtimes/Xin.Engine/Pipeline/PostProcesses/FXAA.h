#pragma once

#include "../../Xin.Engine.Types.h"

namespace Xin::Engine
{
	enum class EFXAAFlag
	{
		None = 0,
		Debug,
	};

	class ENGINE_API FFXAARenderer
	{
	public:
		FFXAARenderer(FRenderGraph & RenderGraph, EFXAAFlag Flags = EFXAAFlag::None);
		~FFXAARenderer();

		void Render(FRGTexture * InputColor, FRGTexture * OutputColor);
		void RenderPS(FRGTexture * InputColor, FRGTexture * OutputColor);

		struct alignas(sizeof(float1)) FFXAACSShaderParameter
		{
			float2 FXAATexelSize;
			float1 FXAAEdgeThresholdMin;
			float1 FXAAEdgeThreshold;
			float1 FXAASubpix;

			uint1 LastQueueIndex;
			uint2 StartPixel;
		};

		void RenderCS(FRGTexture * InputColor, FRGTexture * OutputColor);
		void RenderCS_Prepare(FRGTexture * InputColor, FRGTexture * OutputColor, const FFXAACSShaderParameter & FXAACSShaderParameter);
		void RenderCS_Resolve(FRGTexture * InputColor, FRGTexture * OutputColor, const FFXAACSShaderParameter & FXAACSShaderParameter);
		void RenderCS_Final(FRGTexture * InputColor, FRGTexture * OutputColor, const FFXAACSShaderParameter & FXAACSShaderParameter);

	public:
		EFXAAFlag Flags;
		FRenderGraph & RenderGraph;

		SizeU BlockTile { 2, 2 };
		SizeU BlockSize {};
		uint32 DataQueuePacked = 0;
		// CS
		FRGBuffer * WorkCounter;
		FRGBuffer * WorkQueue;
		FRGBuffer * DataQueue;
		FRGBuffer * IndirectParameters;
	};
}
