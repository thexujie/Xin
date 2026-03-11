#pragma once

#include "CommandExecutor.h"
#include "Xin.Rendering.Types.h"

namespace Xin::Rendering
{
	class FCommandExecutor;

	class RENDERING_API FCommandList : public IRenderingObject
	{
	public:
		FCommandList(FCommandExecutor & CommandExecutor);
		~FCommandList();
		FCommandList(const FCommandList &) = delete;

	public:
		void Flush();

	public:
		class FScopedEvent
		{
		public:
			FScopedEvent(FCommandList & CommandList, FStringV EventName, FColor Color = Colors::None) : CommandList(CommandList)
			{
				if (EventName.Size)
				{
					CommandList.BeginLabel(EventName, Color);
					HasBegin = true;
				}
			}

			~FScopedEvent()
			{
				if (HasBegin)
					CommandList.EndLabel();
			}

		public:
			FCommandList & CommandList;
			bool HasBegin = false;
		};

	public:
		forceinline void SetMarker(FStringV Marker, const FColor & Color = {})
		{
			RHICommandList->SetMarker(Marker, Color);
		}

		forceinline void BeginLabel(FStringV Label, const FColor & Color = {})
		{
			RHICommandList->BeginLabel(Label, Color);
		}

		forceinline void EndLabel()
		{
			RHICommandList->EndLabel();
		}

		forceinline FFrameStatics GetFrameStatics() const
		{
			return RHICommandList->GetFrameStatics();
		}

		forceinline void FlushBarriers()
		{
			RHICommandList->FlushBarriers();
		}

		forceinline void ResourceAccess(IRHIResource * RHIResource, EResourceAccess ResourceAccess)
		{
			RHICommandList->ResourceAccess(RHIResource, ResourceAccess);
		}

		forceinline void DiscardResource(IRHIResource * RHIResource, TView<RectU> Rects = {})
		{
			RHICommandList->DiscardResource(RHIResource, Rects);
		}

		forceinline void ClearDepthStencil(IRHIResourceView * ResourceDSV, float32 Depth, uint8 Stencial)
		{
			RHICommandList->ClearDepthStencil(ResourceDSV, Depth, Stencial);
		}

		forceinline void ClearRenderTarget(IRHIResourceView * ResourceRTV, FColorF Color)
		{
			RHICommandList->ClearRenderTarget(ResourceRTV, Color);
		}

		forceinline void ClearRenderTarget(IRHIResource * RHIResource, FColorF Color)
		{
			RHICommandList->ClearRenderTarget(RHIResource->GetResourceView(), Color);
		}

		forceinline void ClearUnorderedAccess(IRHIResource * RHIResource, uint32 ClearValue)
		{
			RHICommandList->ClearUnorderedAccess(RHIResource->GetResourceView(), ClearValue);
		}

		forceinline void BeginPass(const FRenderPassState & RenderPassState)
		{
			for (uint32 RTIndex = 0; RTIndex < MaxRenderTargets; ++RTIndex)
			{
				const FRenderPassTarget & RenderPassTarget = RenderPassState.RenderTargets[RTIndex];
				if (RenderPassTarget && RenderPassTarget.ResourceView)
				{
					OutputFormat.RenderTargets[RTIndex] = RenderPassTarget.ResourceView->ViewDesc.Format;
				}
				else
				{
					OutputFormat.RenderTargets[RTIndex] = EFormat::None;
				}
			}
			OutputFormat.DepthStencil = RenderPassState.DepthStencil ? RenderPassState.DepthStencil.ResourceView->ViewDesc.Format : EFormat::None;
			RHICommandList->BeginPass(RenderPassState);
		}

		forceinline void EndPass()
		{
			RHICommandList->EndPass();
		}

		forceinline void SetViewPorts(TView<FViewport> ViewPorts)
		{
			RHICommandList->SetViewPorts(ViewPorts);
		}

		forceinline void SetScissorRects(TView<RectI> ScissorRects)
		{
			RHICommandList->SetScissorRects(ScissorRects);
		}

		forceinline void SetPipelineState(IRHIPipelineState * PipelineState)
		{
			RHIPipelineState = PipelineState;
			RHICommandList->SetPipelineState(PipelineState);
		}

		forceinline void IASetVertexBuffers(uint32 BaseSlotIndex, TView<IRHIResource *> VertexBuffers, TView<uint32> Strides, uint32 VertexCount)
		{
			TArray<uint32, MaxVertexBuffers> Sizes {};

			for (uint32 VertexBufferIndex = 0; VertexBufferIndex < Min(VertexBuffers.Size, MaxVertexBuffers); ++VertexBufferIndex)
			{
				Sizes[VertexBufferIndex] = VertexBuffers[VertexBufferIndex]->ResourceDesc.Size.X;
			}

			RHICommandList->IASetVertexBuffers(BaseSlotIndex, VertexBuffers, Sizes, Strides, VertexCount);
		}

		forceinline void IASetIndexBuffer(IRHIResource * IndexBuffer, EIndexFormat IndexFormat, uint32 IndexCount)
		{
			RHICommandList->IASetIndexBuffer(IndexBuffer, IndexFormat, IndexCount);
		}

		forceinline void SetParameter(FName ParameterName, const FRHIBlock & Block)
		{
			RHICommandList->SetParameter(ParameterName, Block);
		}

		forceinline void SetParameter(FName ParameterName, IRHIResource * Resource)
		{
			RHICommandList->SetParameter(ParameterName, Resource);
		}

		forceinline void SetParameter(FName ParameterName, IRHIResourceView * ResourceView)
		{
			RHICommandList->SetParameter(ParameterName, ResourceView);
		}

		forceinline void DrawInstanced(uint32 VertexOffset, uint32 VertexCount, uint32 InstanceOffset = 0, uint32 InstanceCount = 1)
		{
			RHICommandList->DrawInstanced(VertexOffset, VertexCount, InstanceOffset, InstanceCount);
		}

		forceinline void DrawIndexedInstanced(uint32 IndexOffset, uint32 IndexCount, uint32 VertexOffset, uint32 InstanceOffset = 0, uint32 InstanceCount = 1)
		{
			RHICommandList->DrawIndexedInstanced(IndexOffset, IndexCount, VertexOffset, InstanceOffset, InstanceCount);
		}

		forceinline void Dispatch(Vec3U GroupCount)
		{
			RHICommandList->Dispatch(GroupCount);
		}

		forceinline void DispatchIndirect(IRHIIndirectSignature * IndirectSignature, IRHIResource * IndirectBuffer, uint32 IndirectBufferOffset, uint32 MaxCommandCount)
		{
			RHICommandList->DispatchIndirect(IndirectSignature, IndirectBuffer, IndirectBufferOffset, MaxCommandCount);
		}

		forceinline void ResourceBarrierUAV(IRHIResource * Resource)
		{
			RHICommandList->ResourceBarrierUAV(Resource);
		}

		forceinline void ResourceBarrierAlias(IRHIResource * ResourceBefore, IRHIResource * ResourceAfter)
		{
			RHICommandList->ResourceBarrierAlias(ResourceBefore, ResourceAfter);
		}

		forceinline void CopyResource(IRHIResource * DestinationResource, IRHIResource * SourceResource)
		{
			RHICommandList->CopyResource(DestinationResource, SourceResource);
		}

	public:
		void SetMeshRenderState(const FMeshRenderState & RenderState);
		void SetMeshRenderState(FMeshRenderStateIndex RenderStateIndex);

	public:
		FRHIBlock AllocShaderParameter(size_t Size);

		template<typename T>
		FRHIBlock AllocShaderParameterT(const T & Value)
		{
			FRHIBlock Block = AllocShaderParameter(sizeof(T));
			MemoryCopy(Block.BaseAddress.CPUAddress + Block.Location.Offset, &Value, sizeof(T));
			return Block;
		}

		void UpdateBuffer(IRHIResource * Resource, TView<byte> Bytes);
		void UpdateTexture(IRHIResource * Resource, const FTextureSource & TextureSource);

		IRHIResourceRef CreateBuffer(const FBufferDesc & ResourceDesc, TView<byte> InitialBytes = {});

		IRHIResourceRef CreateShaderParameter(Size2U Size, EFormat Format, TView<byte> InitialBytes = {})
		{
			return CreateBuffer(FBufferDesc(Size, Format, EResourceUsage::UniformBuffer | EResourceUsage::HostWrite), InitialBytes);
		}

		IRHIResourceRef CreateShaderParameter(uint32 Size, TView<byte> InitialBytes = {})
		{
			return CreateBuffer(FBufferDesc(Size, EResourceUsage::UniformBuffer | EResourceUsage::HostWrite), InitialBytes);
		}
		IRHIResourceRef CreateShaderParameter(TView<byte> InitialBytes) { return CreateShaderParameter(uint32(InitialBytes.Size), InitialBytes); }

		template<typename T>
		IRHIResourceRef CreateShaderParameterT(const T & Value) { return CreateShaderParameter(AlignWith<uint32>(sizeof(T), sizeof(float4)), ViewBytes<T>(Value)); }

	public:
		FCommandExecutor & Executor;
		IRHICommandListRef RHICommandList;

	public:
		FOutputFormat OutputFormat;
		FMeshRenderState MeshRenderState;

		TList<FRHIMemory> InflightMemoryBlocks;
		TList<IRHIResourceRef> InflightResources;

	public:
		FMeshRenderStateIndex RenderStateIndex = NullIndex;
		IRHIPipelineState * RHIPipelineState = nullptr;
	};

	using FCommandListRef = TReferPtr<FCommandList>;
}
