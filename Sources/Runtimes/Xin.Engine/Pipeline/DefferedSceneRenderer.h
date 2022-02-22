#pragma once

#include "Pipeline.Types.h"

namespace Xin::Engine
{
	class ENGINE_API FDefferedSceneRenderer
	{
	public:
		FDefferedSceneRenderer(FEngine & Engine);

		void Render(FRenderGraph & RenderGraph, IRHIResource * BackBuffer, IRHIResource * UIBackBuffer);

	public:
		FEngine & Engine;
		IRHIResourceRef SceneExposure;
	};
}
