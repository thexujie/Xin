#pragma once

#include "Actor.h"
#include "../Engine/Scene.h"

namespace Xin::Engine
{
	class ENGINE_API FWorld : public FActor
	{
	public:
		FWorld(FEngine & Engine);
		~FWorld();

		void AddActor(FActorRef Actor);
		void Update(float32 DeltaTime) override;

	public:
		FEngine & Engine;

		TList<FActorRef> Actors;

		FAxisSystem AxisSystem {};

	public:
		TEvent<void(FActor & Actor)> OnActorAdded;
	};

	class ENGINE_API FWorldScene : public FScene
	{
	public:
		FWorldScene(FWorld & World);

		void OnAddActor(FActor & Actor);

		void Update(FCommandExecutor & Executor) override;

	public:
		FWorld & World;

		TList<IPrimitiveRef> PenddingPrimitives;
	};
}
