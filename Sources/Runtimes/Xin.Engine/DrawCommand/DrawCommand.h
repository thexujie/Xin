#pragma once

#include "../Xin.Engine.Types.h"

namespace Xin::Engine
{
	class IPrimitive;

	struct FCommandParameter
	{
		FCommandParameter() = default;
		FCommandParameter(const FCommandParameter &) = default;

		FCommandParameter(FName Name, IRHIResource * Resource) :Name(Name), Resource(Resource){}
		FCommandParameter(FName Name, IRHIResourceView * ResourceView) :Name(Name), ResourceView(ResourceView){}
		FCommandParameter(FName Name, FRGResource * RGResource) :Name(Name), RGResource(RGResource){}
		FCommandParameter(FName Name, FRGResourceView * RGResourceView) :Name(Name), RGResourceView(RGResourceView){}

		FName Name;

		IRHIResource * Resource = nullptr;
		IRHIResourceView * ResourceView = nullptr;

		FRGResource * RGResource = nullptr;
		FRGResourceView * RGResourceView = nullptr;
	};

	struct ENGINE_API FDrawCommand
	{
		FMeshRenderStateIndex MeshRenderStateIndex = NullIndex;

		TArray<IRHIResource *, MaxVertexBuffers> VertexBuffers {};
		TArray<uint32, MaxVertexBuffers> VertexStrides {};
		uint32 VertexCount = 0;
		uint32 VertexBufferCount = 0;

		IRHIResource * IndexBuffer = nullptr;
		EIndexFormat IndexFormat = EIndexFormat::Index16;
		uint32 IndexCount = 0;

		struct
		{
			uint32 VertexOffset = 0;
			uint32 VertexCount = 0;
		}Instanced;

		struct
		{
			uint32 VertexOffset = 0;
			uint32 IndexOffset = 0;
			uint32 IndexCount = 0;
		}IndexedInstanced;

		struct
		{
			uint32 GorupSizeX = 0;
			uint32 GorupSizeY = 0;
			uint32 GorupSizeZ = 0;
		}Dispatch;

		struct
		{
			IRHIIndirectSignature * IndirectSignature = nullptr;
			IRHIResource * IndirectBuffer = nullptr;
			uint32 IndirectBufferOffset = 0;
			uint32 MaxCommandCount = 1;
		}DispatchIndirect;

		TList<FCommandParameter> Parameters;

#ifdef XIN_DEBUG
		IPrimitive * RenderPrimitive = nullptr;
		const class FMaterialResource * Material = nullptr;
#endif
	};
}
