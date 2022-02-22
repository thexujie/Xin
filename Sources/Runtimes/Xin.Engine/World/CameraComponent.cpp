#include "PCH.h"
#include "CameraComponent.h"

#include "World/Actor.h"
#include "World/World.h"

namespace Xin::Engine
{
	FCameraComponent::FCameraComponent()
	{
	}

	void FCameraComponent::GetViewInfo(FCameraViewInfo & CameraViewInfo)
	{
		CameraViewInfo.Position = Actor->Transform.Translation;
		CameraViewInfo.Rotation = Actor->Transform.Rotation;
		CameraViewInfo.FOV = FOV;
		CameraViewInfo.AspectRatio = AspectRatio;
	}

	void FCameraComponent::Update(float32 DeltaTime)
	{
		IActorComponent::Update(DeltaTime);
	}

	FCameraActor::FCameraActor()
	{
		TReferPtr<FCameraComponent> Componnet = new FCameraComponent();
		AddComponent(Componnet);
		CameraComponent = Componnet.Get();
	}

	FCameraActor::~FCameraActor()
	{
		
	}
}
