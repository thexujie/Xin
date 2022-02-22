#include "PCH.h"
#include "World.h"

namespace Xin::Engine
{
	FWorld::FWorld(FEngine & Engine) : Engine(Engine)
	{
	}

	FWorld::~FWorld()
	{
		
	}

	void FWorld::AddActor(FActorRef Actor)
	{
		Actors.Add(Actor);
		Actor->SetWorld(this);
		OnActorAdded.Excute(Actor.Ref());
	}

	void FWorld::Update(float32 DeltaTime)
	{
		for (FActorRef & Actor : Actors)
			Actor->Update(DeltaTime);
	}

	FWorldScene::FWorldScene(FWorld & World) : World(World)
	{
		World.OnActorAdded += { this, &FWorldScene::OnAddActor };
		for (FActorRef & Actor : World.Actors)
		{
			for (IActorComponentRef & ActorComponent : Actor->Components)
			{
				FPrimitiveCollector PrimitiveCollector(World.Engine);
				ActorComponent->CollectPrimitives(PrimitiveCollector);
				PenddingPrimitives.AddRange(PrimitiveCollector.Primitives);
			}
		}
	}

	void FWorldScene::OnAddActor(FActor & Actor)
	{
		for (IActorComponentRef & ActorComponent : Actor.Components)
		{
			FPrimitiveCollector PrimitiveCollector(World.Engine);
			ActorComponent->CollectPrimitives(PrimitiveCollector);
			PenddingPrimitives.AddRange(PrimitiveCollector.Primitives);
		}
	}

	void FWorldScene::Update(FCommandExecutor & Executor)
	{
		for (IPrimitiveRef & RenderPrimitive : PenddingPrimitives)
			RenderPrimitive->InitializeRHI(Executor);
		Primitives.AddRange(PenddingPrimitives);
		for (IPrimitiveRef & RenderPrimitive : Primitives)
			RenderPrimitive->UpdateRHI();
		PenddingPrimitives.Clear();
	}
}
