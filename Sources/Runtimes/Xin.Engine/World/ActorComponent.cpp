#include "PCH.h"
#include "ActorComponent.h"

#include "Actor.h"

namespace Xin::Engine
{
	void IActorComponent::OnAttatch(FActor * Actor)
	{
		this->Actor = Actor;
	}

	void IActorComponent::OnActorTransformChanged(const FTransform & Transform)
	{
		TransformChanged.Excute(Actor, Transform);
	}

	void IActorComponent::OnActorWorld(FWorld * World)
	{
		
	}
}
