#include "PCH.h"
#include "MeshComponent.h"

namespace Xin::Engine
{

	FMeshComponent::FMeshComponent()
	{
	}

	void FMeshComponent::UpdatePrimitiveTransform(const FTransform & Transform)
	{
		if (MeshPrimitive)
		{
			FMeshPrimitiveState PrimitiveState;
			PrimitiveState.WorldTransform = Transform.ToMatrix();
			MeshPrimitive->SetPrimitveState(PrimitiveState);
		}
	}
}
