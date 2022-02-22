#pragma once

#include "Xin.RHI/Xin.RHI.h"
#include "Xin.Rendering/Xin.Rendering.h"

#include "DrawCommand/DrawCommand.h"
#include "DrawCommand/DrawCommandCollector.h"

#include "Pipeline/DefferedSceneRenderer.h"

#include "Pipeline/ShadowDepthRenderer.h"
#include "Pipeline/PostProcessRenderer.h"
#include "Pipeline/PostProcesses/Bloom.h"
#include "Pipeline/PostProcesses/FXAA.h"

#include "Pipeline/RenderPass.h"
#include "Pipeline/RenderGraph.h"

#include "World/World.h"
#include "World/Actor.h"
#include "World/CameraComponent.h"
#include "World/StaticMeshComponent.h"
#include "World/ActorController.h"

#include "Engine.h"

namespace Xin::Engine
{
	using namespace Xin::RHI;
}
