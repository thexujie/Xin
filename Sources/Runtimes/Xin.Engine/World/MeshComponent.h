#pragma once

#include "ActorComponent.h"

#include "../Asset/Material.h"
#include "../Engine/MeshPrimitive.h"

namespace Xin::Engine
{
	class ENGINE_API FMeshComponent : public IActorComponent
	{
	public:
		FMeshComponent();

		void UpdatePrimitiveTransform(const FTransform & Transform);

	public:
		TReferPtr<FMeshPrimitive> MeshPrimitive;
		TList<UMaterialRef> Materials;
		FTransform Transform;
	};
}
