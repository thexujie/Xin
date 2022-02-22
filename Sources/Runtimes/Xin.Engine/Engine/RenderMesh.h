#pragma once

#include "../Xin.Engine.Types.h"

namespace Xin::Engine
{
	struct FRenderMeshElement
	{
		const class FMaterialResource * Material = nullptr;
		uint32 VertexOffset = 0;
		uint32 VertexCount= 0;
		uint32 IndexOffset = 0;
		uint32 IndexCount = 0;
	};

	struct ENGINE_API FRenderMesh
	{
	public:
		ETopology Topology = ETopology::None;
		TArray<FInputElement, MaxInputElements> InputElements {};

		TArray<IRHIResource *, MaxVertexBuffers> VertexBuffers {};
		TArray<uint32, MaxVertexBuffers> VertexStrides {};
		uint32 VertexBufferCount = 0;
		uint32 VertexCount = 0;

		IRHIResource * IndexBuffer = nullptr;
		EIndexFormat IndexFormat = EIndexFormat::None;
		uint32 IndexCount = 0;

		TList<FRenderMeshElement> Elements;
	};
}
