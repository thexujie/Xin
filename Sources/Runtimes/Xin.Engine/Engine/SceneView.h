#pragma once

#include "../Xin.Engine.Types.h"

namespace Xin::Engine
{
	class alignas(sizeof(float4)) FViewParameter
	{
	public:
		float4x4 ViewMatrix;
		float4x4 ProjectMatrix;
		float4x4 ViewProjectMatrix;
	};

	struct FSceneViewState
	{
		RectI ViewRect;
		FVector3 ViewPosition;
		FVector3 ViewDirection;

		FMatrix ViewMatrix;
		FMatrix ProjectMatrix;
	};

	class ENGINE_API FSceneView
	{
	public:
		FSceneView() = default;
		explicit FSceneView(const FSceneViewState & RenderViewState)
			: ViewRect(RenderViewState.ViewRect)
			, Position(RenderViewState.ViewPosition)
			, Direction(RenderViewState.ViewDirection)
			, ViewMatrix(RenderViewState.ViewMatrix)
			, ProjectMatrix(RenderViewState.ProjectMatrix)
		{
			
		}
	public:
		void Update() {}

	public:
		uint64 FrameIndex = 0;

		float32 NearZ = 0;
		float32 FarZ = 1;

		RectI ViewRect { };
		FViewport Viewport;
		FVector3 Position { };
		FVector3 Direction { };

		FMatrix ViewMatrix { };
		FMatrix ProjectMatrix { };
	};
}
