#pragma once

#include "Actor.h"
#include "ActorComponent.h"

namespace Xin::Engine
{
	enum class ECameraProjectionMode
	{
		Perspective = 0,
		Orthographic
	};

	struct FCameraViewInfo
	{
		FVector3 Position;
		FQuat Rotation;

		float32 FOV;
		float32 AspectRatio;
	};

	class ENGINE_API FCameraComponent : public IActorComponent
	{
	public:
		FCameraComponent();

		void GetViewInfo(FCameraViewInfo & CameraViewInfo);
		void Update(float32 DeltaTime) override;

	public:
		ECameraProjectionMode ProjectionMode = ECameraProjectionMode::Perspective;

		float32 FOV = Float32PI * 0.5f;
		float32 AspectRatio = 16.0f / 9.0f;
	};

	class ENGINE_API FCameraActor : public FActor
	{
	public:
		FCameraActor();
		~FCameraActor();

	public:
		FCameraComponent * CameraComponent = nullptr;
	};
}
