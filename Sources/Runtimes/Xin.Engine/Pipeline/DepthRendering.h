#pragma once

#include "Pipeline.Types.h"

namespace Xin::Engine
{
	class FSceneView;

	class ENGINE_API FDepthRenderer
	{
	public:
		FDepthRenderer(FRenderGraph & RenderGraph);
		~FDepthRenderer() = default;

		void Render(IRHIResource * ViewParameter);

	public:
		FRenderGraph & RenderGraph;
		FDrawCommandCollector MeshCommandCollector;

	public:
		FRGTexture * SceneDepth;
	};
}
