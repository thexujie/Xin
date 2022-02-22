#pragma once

#include "World.Types.h"

namespace Xin::Engine
{
	class FPrimitiveCollector;
	class FActor;

	class ENGINE_API IActorComponent : public IEngineObject
	{
	public:
		IActorComponent() {}

	public:
		virtual void OnAttatch(FActor * Actor);
		virtual void OnActorTransformChanged(const FTransform & Transform);
		virtual void OnActorWorld(FWorld * World);

		virtual void CollectPrimitives(FPrimitiveCollector & PrimitiveCollector) {}

		virtual void Update(float32 DeltaTime) {}
	public:
		FActor * Actor = nullptr;

	public:
		TEvent<void(FActor *, const FTransform &)> TransformChanged;
	};
	using IActorComponentRef = TReferPtr<IActorComponent>;
}
