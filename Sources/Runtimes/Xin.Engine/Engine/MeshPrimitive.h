#pragma once

#include "Primitive.h"
#include "RenderMesh.h"

namespace Xin::Engine
{
	struct FMeshPrimitiveState
	{
		FMatrix WorldTransform;
	};

	class ENGINE_API FMeshPrimitive : public IPrimitive
	{
	public:
		FMeshPrimitive(IRHIDevice & Device);

		void InitializeRHI(FCommandExecutor & CommandExecutor) override;
		void UpdateRHI() override;
		FMatrix GetWorldMatrix() const override { return RenderState.WorldTransform; }

	public:
		void SetPrimitveState(const FMeshPrimitiveState & PrimitiveState)
		{
			RenderState = PrimitiveState;
			RenderStateChanged = true;
		}

	public:
		FMeshPrimitiveState RenderState;
		bool RenderStateChanged = true;
	};
	using FMeshPrimitiveRef = TReferPtr<FMeshPrimitive>;
}
