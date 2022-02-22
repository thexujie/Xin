#pragma once

#include "ActorComponent.h"

namespace Xin::Engine
{
	class FWorld;

	class ENGINE_API FActor : public IEngineObject
	{
	public:
		void SetWorldTransform(const FTransform & Transform_);
		void AddComponent(IActorComponentRef ActorComponent);

	public:
		void SetWorld(FWorld * World);

		virtual void Update(float32 DeltaTime)
		{
			for (IActorComponentRef & Component : Components)
				Component->Update(DeltaTime);
		}

	public:
		FWorld * World = nullptr;

		FTransform Transform;
		TList<IActorComponentRef> Components;

	public:
		TEvent<void(FWorld *)> OnWorldChanged;
		TEvent<void(const FTransform &)> OnTransformChanged;
	};

	using FActorRef = TReferPtr<FActor>;
}
