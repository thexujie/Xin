#include "PCH.h"
#include "DrawCommandCollector.h"

#include "Asset/Material.h"
#include "Engine/Primitive.h"
#include "Engine/Scene.h"

#include "Pipeline/RenderGraph.h"

namespace Xin::Engine
{
	void FDrawCommandCollector::CollectDrawCommands(FRenderGraph & RenderGraph, EMeshPass MeshPass, FScene & Scene,
		IRHIResource * GlobalParameter, IRHIResource * ViewParameter,
		FRGResource * Occlusion, FRGResource * ShadowDepth,
		const FRasterizeState & RasterizeState,
		const FDepthStencilState & DepthStencilState,
		const IShaderSource & VertexShader, const IShaderVariance & VertexShaderVariance,
		const IShaderSource & PixelShader, const IShaderVariance & PixelShaderVariance)
	{
		for (uintx PrimitiveIndex = 0; PrimitiveIndex < Scene.Primitives.Size; ++PrimitiveIndex)
		{
			IPrimitive & RenderPrimitive = Scene.Primitives[PrimitiveIndex].Ref();
			FPrimitiveParameter PrimitiveParameterData;
			PrimitiveParameterData.LocalToWorld = RenderPrimitive.GetWorldMatrix();
			IRHIResource * PrimitiveParameter = RenderGraph.CreateShaderParameterT(PrimitiveParameterData);

			for (const FRenderMesh & RenderMesh : RenderPrimitive.RenderMeshs)
			{
				DrawCommands.Reserve(DrawCommands.Size + RenderMesh.Elements.Size);
				for (uintx ElementIndex : IRange(RenderMesh.Elements.Size))
				{
					const FRenderMeshElement & RenderMeshElement = RenderMesh.Elements[ElementIndex];
					FMeshRenderState MeshRenderState;
					{
						MeshRenderState.Shaders = RenderMeshElement.Material->GetShaders(VertexShader, VertexShaderVariance, PixelShader, PixelShaderVariance);
						MeshRenderState.InputElements = RenderMesh.InputElements;
						MeshRenderState.RasterizeState = RasterizeState;
						MeshRenderState.BlendState = FBlendState::Default;
						MeshRenderState.DepthStencilState = DepthStencilState;
					}

					FDrawCommand & DrawCommand = DrawCommands.AddDefault();

					DrawCommand.MeshRenderStateIndex = RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);


					if (GlobalParameter)
						DrawCommand.Parameters.Emplace(u8"GlobalParameter"N, GlobalParameter);

					if (Occlusion)
						DrawCommand.Parameters.Emplace(u8"Occlusion"N, Occlusion);

					if (ShadowDepth)
						DrawCommand.Parameters.Emplace(u8"ShadowDepth"N, ShadowDepth);

					if (ViewParameter)
						DrawCommand.Parameters.Emplace(u8"ViewParameter"N, ViewParameter);

					DrawCommand.Parameters.Emplace(u8"PrimitiveParameter"N, PrimitiveParameter);

					//MeshAssembler.GetParameters(MeshCommand.Resources);
					for (const auto & TexturePair : RenderMeshElement.Material->Textures)
						DrawCommand.Parameters.Emplace(TexturePair.First, TexturePair.Second->RHIResource);

					//for (const FRenderResources::FResource & Resource : MeshCommand.Resources.Resources)
					//	AssertExpresion(Resource.RHIResource);

					DrawCommand.VertexBuffers = RenderMesh.VertexBuffers;
					DrawCommand.VertexStrides = RenderMesh.VertexStrides;
					DrawCommand.VertexBufferCount = RenderMesh.VertexBufferCount;
					DrawCommand.VertexCount = RenderMesh.VertexCount;
					DrawCommand.IndexBuffer = RenderMesh.IndexBuffer;
					DrawCommand.IndexFormat = RenderMesh.IndexFormat;
					DrawCommand.IndexCount = RenderMesh.IndexCount;

					if (RenderMeshElement.IndexCount > 0)
					{
						DrawCommand.IndexedInstanced.VertexOffset = RenderMeshElement.VertexOffset;
						DrawCommand.IndexedInstanced.IndexOffset = RenderMeshElement.IndexOffset;
						DrawCommand.IndexedInstanced.IndexCount = RenderMeshElement.IndexCount;
					}
					else
					{
						DrawCommand.Instanced.VertexOffset = RenderMeshElement.VertexOffset;
						DrawCommand.Instanced.VertexCount = RenderMeshElement.VertexCount;
					}
#ifdef XIN_DEBUG
					DrawCommand.RenderPrimitive = &RenderPrimitive;
					DrawCommand.Material = RenderMeshElement.Material;
#endif
				}
			}
		}

		//std::sort(PassMeshCommands.Begin(), PassMeshCommands.End(), [](const FVisibleMeshCommand & A, const FVisibleMeshCommand & B) { return A.MeshRenderStateIndex < B.MeshRenderStateIndex; });
	}
}
