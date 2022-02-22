#pragma once

#include "ActorComponent.h"
#include "MeshComponent.h"

#include "../Asset/StaticMesh.h"

namespace Xin::Engine
{
	class FStaticMeshComponent;

	class ENGINE_API FStaticMeshPrimitive : public FMeshPrimitive
	{
	public:
		FStaticMeshPrimitive(IRHIDevice & Device, FStaticMeshComponent & StaticMeshComponent);
		FStringV GetPrimitiveName() const override;

		void InitializeRHI(FCommandExecutor & CommandExecutor) override;
		void UpdateRHI() override;

	public:
		FStaticMeshComponent & StaticMeshComponent;

		TArray<IRHIResourceRef, MaxVertexBuffers> VertexBuffers { };
		TArray<uint32_t, MaxVertexBuffers> VertexStrides { };

		IRHIResourceRef IndexBuffer;

		TList<FMaterialResourceRef> Materials;
	};

	class ENGINE_API FStaticMeshComponent : public FMeshComponent
	{
	public:
		FStaticMeshComponent();

		void SetStaticMesh(TReferPtr<UStaticMesh> StaticMesh_);

	public:
		void OnActorTransformChanged(const FTransform & Transform) override;
		void CollectPrimitives(FPrimitiveCollector & PrimitiveCollector) override;

	public:
		TReferPtr<UStaticMesh> StaticMesh;
	};
}
