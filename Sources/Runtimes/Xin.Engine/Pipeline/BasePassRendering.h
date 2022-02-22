#pragma once

#include "Pipeline.Types.h"

namespace Xin::Engine
{
	class FSceneView;

	class ENGINE_API FBasePassRenderer
	{
	public:
		FBasePassRenderer(FRenderGraph & RenderGraph);
		~FBasePassRenderer() = default;

		void Render(IRHIResource * GlobalParameter, IRHIResource * ViewParameter, FRGTexture * SceneDepth, FRGTexture * SSAOOcclusion, FRGTexture * ShadowDepth);

	public:
		FRenderGraph & RenderGraph;
		FDrawCommandCollector MeshCommandCollector;

	public:
		FRGTexture * BaseNormal;
		FRGTexture * BaseColor;
	};
}
