#include "PCH.h"
#include "StaticMeshComponent.h"

#include "Actor.h"

namespace Xin::Engine
{
	enum EMeshVertexBufferIndex
	{
		MVBI_Position = 0,
		MVBI_Normal,
		MVBI_Vertex,
		MVBI_Count
	};

	FStaticMeshPrimitive::FStaticMeshPrimitive(IRHIDevice & Device, FStaticMeshComponent & StaticMeshComponent)
		: FMeshPrimitive(Device), StaticMeshComponent(StaticMeshComponent)
	{
		RenderState.WorldTransform = StaticMeshComponent.Transform.ToMatrix();
	}

	FStringV FStaticMeshPrimitive::GetPrimitiveName() const
	{
		return StaticMeshComponent.StaticMesh->AssetPath;
	}

	void FStaticMeshPrimitive::InitializeRHI(FCommandExecutor & CommandExecutor)
	{
		FMeshPrimitive::InitializeRHI(CommandExecutor);

		if (!StaticMeshComponent.StaticMesh)
			return;

		if (!VertexBuffers[MVBI_Position])
		{
			const FStaticMeshData & StaticMeshData = StaticMeshComponent.StaticMesh->StaticMeshData;

			VertexBuffers[MVBI_Position] = CommandExecutor.CreateBuffer(FBufferDesc((uint32)(StaticMeshData.Positions.Size * sizeof(FVector3)), EResourceUsage::VertexBuffer | EResourceUsage::CopyWrite), ReinterpretView<byte>(StaticMeshData.Positions));
			VertexBuffers[MVBI_Normal] = CommandExecutor.CreateBuffer(FBufferDesc((uint32)(StaticMeshData.Normals.Size * sizeof(FVector3)), EResourceUsage::VertexBuffer | EResourceUsage::CopyWrite), ReinterpretView<byte>(StaticMeshData.Normals));
			VertexBuffers[MVBI_Vertex] = CommandExecutor.CreateBuffer(FBufferDesc((uint32)(StaticMeshData.Vertices.Size * sizeof(FStaticMeshVertex)), EResourceUsage::VertexBuffer | EResourceUsage::CopyWrite), ReinterpretView<byte>(StaticMeshData.Vertices));

			VertexStrides[MVBI_Position] = sizeof(FVector3);
			VertexStrides[MVBI_Normal] = sizeof(FVector3);
			VertexStrides[MVBI_Vertex] = sizeof(FStaticMeshVertex);

#if XIN_DEBUG
			VertexBuffers[MVBI_Position]->SetDebugName(Format(u8"{}//Position"V, StaticMeshComponent.StaticMesh->AssetPath));
			VertexBuffers[MVBI_Normal]->SetDebugName(Format(u8"{}//Normal"V, StaticMeshComponent.StaticMesh->AssetPath));
			VertexBuffers[MVBI_Vertex]->SetDebugName(Format(u8"{}//Vertex"V, StaticMeshComponent.StaticMesh->AssetPath));
#endif
			switch (StaticMeshData.IndexFormat)
			{
			case EIndexFormat::Index16:
				IndexBuffer = CommandExecutor.CreateBuffer(FBufferDesc((uint32)(StaticMeshData.Indices16.Size * sizeof(int16_t)), EResourceUsage::IndexBuffer | EResourceUsage::CopyWrite), ReinterpretView<byte>(StaticMeshData.Indices16));
				break;
			case EIndexFormat::Index32:
				IndexBuffer = CommandExecutor.CreateBuffer(FBufferDesc((uint32)(StaticMeshData.Indices16.Size * sizeof(int16_t)), EResourceUsage::IndexBuffer | EResourceUsage::CopyWrite) , ReinterpretView<byte>(StaticMeshData.Indices32));
				break;
			default:
				LogError(u8"Invalid index buffer format {0}", StaticMeshData.IndexFormat);
				break;
			}
#if XIN_DEBUG
			IndexBuffer->SetDebugName(Format(u8"{}//Indices"V, StaticMeshComponent.StaticMesh->AssetPath));
#endif
		}

		Materials.Resize(StaticMeshComponent.StaticMesh->Materials.Size);
		for (uintx ElementIndex = 0; ElementIndex < StaticMeshComponent.StaticMesh->StaticMeshData.Elements.Size; ++ElementIndex)
		{
			UMaterialRef & Material = StaticMeshComponent.Materials[ElementIndex] ? StaticMeshComponent.Materials[ElementIndex] : StaticMeshComponent.StaticMesh->Materials[ElementIndex];
			Materials[ElementIndex] = new FMaterialResource(StaticMeshComponent.StaticMesh->Engine, Material.Ref());

			if (Materials[ElementIndex])
				Materials[ElementIndex]->InitializeRHI(CommandExecutor);
		}

		UStaticMesh & StaticMesh = StaticMeshComponent.StaticMesh.Ref();
		FRenderMesh & RenderMesh = RenderMeshs.AddZeroed();

		RenderMesh.Topology = ETopology::TriangleList;
		for (uint32 VertexBufferIndex = 0; VertexBufferIndex < MVBI_Count; ++VertexBufferIndex)
		{
			if (VertexBuffers[VertexBufferIndex])
			{
				RenderMesh.VertexBuffers[VertexBufferIndex] = VertexBuffers[VertexBufferIndex];
				RenderMesh.VertexStrides[VertexBufferIndex] = VertexStrides[VertexBufferIndex];
			}
		}
		RenderMesh.VertexBufferCount = MVBI_Count;
		RenderMesh.VertexCount = uint32(StaticMesh.StaticMeshData.Vertices.Size);

		RenderMesh.IndexBuffer = IndexBuffer;
		RenderMesh.IndexFormat = StaticMesh.StaticMeshData.IndexFormat;
		RenderMesh.IndexCount = StaticMesh.StaticMeshData.IndexFormat == EIndexFormat::Index16 ? StaticMesh.StaticMeshData.Indices16.Size : StaticMesh.StaticMeshData.Indices32.Size;

		RenderMesh.InputElements = 
		{
			{ EInputSemantic::Position, 0, EFormat::Float3, 0, 0 },
			{ EInputSemantic::Normal, 0, EFormat::Float3, 1, 0 },
			{ EInputSemantic::Texcoord, 0, EFormat::Float2, 2, 0 },
			{ EInputSemantic::Tangent, 0, EFormat::Float3, 2, 8 },
			{ EInputSemantic::BiTangent, 0, EFormat::Float3, 2, 20 },
		};

		for (uint32_t ElementIndex = 0; ElementIndex < StaticMesh.StaticMeshData.Elements.Size; ++ElementIndex)
		{
			const FStaticMeshElement & StaticMeshElement = StaticMesh.StaticMeshData.Elements[ElementIndex];

			FRenderMeshElement & RenderMeshElement = RenderMesh.Elements.AddZeroed();
			RenderMeshElement.Material = Materials[ElementIndex].Get();
			RenderMeshElement.VertexOffset = StaticMeshElement.VertexOffset;
			RenderMeshElement.VertexCount = StaticMeshElement.VertexCount;
			RenderMeshElement.IndexOffset = StaticMeshElement.IndexOffset;
			RenderMeshElement.IndexCount = StaticMeshElement.IndexCount;
		}
	}

	void FStaticMeshPrimitive::UpdateRHI()
	{
		FMeshPrimitive::UpdateRHI();
	}

	FStaticMeshComponent::FStaticMeshComponent() : FMeshComponent()
	{
	}

	void FStaticMeshComponent::SetStaticMesh(TReferPtr<UStaticMesh> StaticMesh_)
	{
		StaticMesh = StaticMesh_;
		Materials.Reset(StaticMesh->StaticMeshData.Elements.Size, nullptr);
	}

	void FStaticMeshComponent::OnActorTransformChanged(const FTransform & Transform)
	{
		FMeshComponent::OnActorTransformChanged(Transform);
		UpdatePrimitiveTransform(Transform);
	}

	void FStaticMeshComponent::CollectPrimitives(FPrimitiveCollector & PrimitiveCollector)
	{
		MeshPrimitive = new FStaticMeshPrimitive(PrimitiveCollector.Engine.Device, *this);
		PrimitiveCollector.Primitives.Add(MeshPrimitive);
	}
}
