#pragma once

#include "DrawCommand.h"

namespace Xin::Engine
{
	struct alignas(sizeof(float4)) FGlobalParameter
	{
		float4x4 ShadowMatrix;
		float4 ViewPosition; // Eye to Point
		float4 LightDirection; // Point To Light
		float4 SunColor;
		float4 AmbientColor;
		float ShadowTexelSize;
		float ShadowDepthBias;
		int PCFBlurLoopBegin;
		int PCFBlurLoopEnd;
		float4 CSMScale[CSMCascadeMax];
		float4 CSMOffset[CSMCascadeMax];
		float CSMBorderPaddingMin;
		float CSMBorderPaddingMax;
		//float4x4 CSMMatrix[CascadeShadowMax];
	};

	class ENGINE_API FDrawCommandCollector
	{
	public:
		FDrawCommandCollector() = default;
		virtual ~FDrawCommandCollector() = default;

		void CollectDrawCommands(FRenderGraph & RenderGraph, EMeshPass MeshPass, FScene & Scene,
			IRHIResource * GlobalParameter, IRHIResource * ViewParameter,
			FRGResource * Occlusion, FRGResource * ShadowDepth,
			const FRasterizeState & RasterizeState,
			const FDepthStencilState & DepthStencilState,
			const IShaderSource & VertexShader, const IShaderVariance & VertexShaderVariance,
			const IShaderSource & PixelShader, const IShaderVariance & PixelShaderVariance);

	public:
		TList<FDrawCommand> DrawCommands;
	};
}
