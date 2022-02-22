#include "PCH.h"
#include "Actor.h"

namespace Xin::Engine
{
	void FActor::SetWorldTransform(const FTransform & Transform_)
	{
		AssertExpr(World);
		Transform = Transform_;
		for (IActorComponentRef & Component : Components)
			Component->OnActorTransformChanged(Transform);
		OnTransformChanged.Excute(Transform);
	}

	void FActor::AddComponent(IActorComponentRef ActorComponent)
	{
		Components.Add(ActorComponent);
		ActorComponent->OnAttatch(this);
	}

	void FActor::SetWorld(FWorld * World_)
	{
		World = World_;
		for (IActorComponentRef & Component : Components)
			Component->OnActorWorld(World);
		OnWorldChanged.Excute(World);
	}
}