#include "PCH.h"
#include "D3D12CommandList.h"

#include "D3D12Device.h"
#include "D3D12Resource.h"
#include "D3D12CommandQueue.h"
#include "D3D12Heap.h"
#include "D3D12ResourceView.h"

#define D3D12_COMMANDLIST_DEBUG_SINGLE_COMMAND 0

namespace Xin::RHI::D3D12
{
	static void WriteTextureDataWithFootprints(FCPUAddress CPUAddress, const FTextureDesc & TextureDesc, TView<byte> InitialData, TView<FCopyableFootprint> CopyableFootprints)
	{
		AssertExpr(CopyableFootprints.Size == TextureDesc.Depth * TextureDesc.MipLevels);
		uint64 SourceOffset = 0;
		for (uint32 DepthIndex = 0, SubresourceIndex = 0; DepthIndex < TextureDesc.Depth; ++DepthIndex)
		{
			for (uint32 MipmapIndex = 0; MipmapIndex < TextureDesc.MipLevels; ++MipmapIndex, ++SubresourceIndex)
			{
				FCopyableFootprint SubresourceFootPrint = CopyableFootprints[SubresourceIndex];

				SizeU MipmapSize = TextureDesc.Size >> MipmapIndex;
				//if (MipmapSize.X == 0) MipmapSize.X = 1;
				//if (MipmapSize.Y == 0) MipmapSize.Y = 1;
				FImageInfo FormatInfo = GetImageInfo(TextureDesc.Format, MipmapSize);
				AssertExpr(FormatInfo.RowPitch > 0);
				if (FormatInfo.RowPitch == 0)
					break;

				AssertExpr(FormatInfo.RowCount == SubresourceFootPrint.RowCount);
				AssertExpr(FormatInfo.RowPitch == SubresourceFootPrint.RowPitch);

				AssertExpr(SourceOffset + FormatInfo.TotalBytes == InitialData.Size);
				for (uint32 RowIndex = 0; RowIndex < FormatInfo.RowCount; ++RowIndex)
				{
					AssertExpr(SubresourceFootPrint.Format != EFormat::None);
					MemoryCopy(CPUAddress + SubresourceFootPrint.Offset + SubresourceFootPrint.RowPitch * RowIndex,
						InitialData.Data + SourceOffset + FormatInfo.RowPitch * RowIndex, FormatInfo.RowPitch);
				}
				SourceOffset += FormatInfo.TotalBytes;
			}
		}
	}

	static void WriteTextureDataWithFootprints(FCPUAddress CPUAddress, const FTextureDesc & TextureDesc, const FTextureSource & TextureSource, TView<FCopyableFootprint> CopyableFootprints)
	{
		AssertExpr(CopyableFootprints.Size == TextureDesc.Depth * TextureDesc.MipLevels);
		const byte * TexturePixels = TextureSource.Bytes.Data;
		for (uint32 DepthIndex = 0, SubresourceIndex = 0; DepthIndex < TextureDesc.Depth; ++DepthIndex)
		{
			for (uint32 MipmapIndex = 0; MipmapIndex < TextureDesc.MipLevels; ++MipmapIndex, ++SubresourceIndex)
			{
				FCopyableFootprint SubresourceFootPrint = CopyableFootprints[SubresourceIndex];
				const FTextureSlice & TextureSlice = TextureSource.Slices[TextureDesc.MipLevels * DepthIndex + SubresourceIndex];

				//SizeU MipmapSize = TextureDesc.Size >> MipmapIndex;
				SizeU MipmapSize = { TextureSlice.Width, TextureSlice.Height };
				//if (MipmapSize.X == 0) MipmapSize.X = 1;
				//if (MipmapSize.Y == 0) MipmapSize.Y = 1;
				FImageInfo FormatInfo = GetImageInfo(TextureDesc.Format, MipmapSize);
				AssertExpr(FormatInfo.RowPitch > 0);
				if (FormatInfo.RowPitch == 0)
					break;

				AssertExpr(FormatInfo.RowCount == SubresourceFootPrint.RowCount);
				AssertExpr(AlignWith(FormatInfo.RowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) == SubresourceFootPrint.RowPitch);
				AssertExpr(FormatInfo.TotalBytes == TextureSlice.SlicePitch);


				if (FormatInfo.RowPitch == SubresourceFootPrint.RowPitch)
				{
					MemoryCopy(CPUAddress + SubresourceFootPrint.Offset, TexturePixels, Min(FormatInfo.TotalBytes, TextureSlice.SlicePitch));
				}
				else
				{
					for (uint32 RowIndex = 0; RowIndex < FormatInfo.RowCount; ++RowIndex)
					{
						AssertExpr(SubresourceFootPrint.Format != EFormat::None);
						MemoryCopy(CPUAddress + SubresourceFootPrint.Offset + SubresourceFootPrint.RowPitch * RowIndex,
							TexturePixels + FormatInfo.RowPitch * RowIndex, FormatInfo.RowPitch);
					}
				}
				TexturePixels += TextureSlice.SlicePitch;
			}
		}
	}

	FD3D12CommandList::FD3D12CommandList(FD3D12Device & D3D12Devcie, ECommandType CommandType)
		: D3D12Devcie(D3D12Devcie), CommandType(CommandType)
	{
		DescriptorAddressStride = D3D12Devcie.d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	FD3D12CommandList::~FD3D12CommandList()
	{
		if (d3d12CommandList)
		{
			d3d12CommandList->Close();
			if (CommandAllocator)
			{
				D3D12Devcie.FreeCommandAllocator(CommandAllocator, CommandType);
				CommandAllocator = nullptr;
			}
		}
	}

	void FD3D12CommandList::SetDebugName(FStringV Name)
	{
		IRHICommandList::SetDebugName(Name);

		if (d3d12CommandList)
			SetD3D12ObjectName(d3d12CommandList.Get(), Name);
	}

	void FD3D12CommandList::BeginCommand(IRHICommandQueue * CommandQueue)
	{
#if XIN_DEBUG
		AssertExpr(Debug_CommandListOpening == 0);
		++Debug_CommandListOpening;
#endif
		AssertExpr(!d3d12CommandList);

		CommandAllocator = D3D12Devcie.AquireCommandAllocator(CommandType);
		d3d12CommandList = CommandAllocator->AcquireNextCommandList();
		FrameStatics = { };
	}

	void FD3D12CommandList::EndCommand(IRHICommandQueue * CommandQueue)
	{
#if XIN_DEBUG
		--Debug_CommandListOpening;
#endif

		FD3D12CommandQueue & D3D12CommandQueue = StaticCastRef<FD3D12CommandQueue>(CommandQueue);

		AssertExpr(d3d12CommandList);
		//FlushBarriers();
		d3d12CommandList->Close();

		AssertExpr(CommandAllocator);
		D3D12CommandQueue.FenceCommandList(d3d12CommandList, CommandAllocator);
		D3D12CommandQueue.FenceCommandAllocator(CommandAllocator, ECommandType::Direct);

		d3d12CommandList = nullptr;
		CommandAllocator = nullptr;
	}

	void FD3D12CommandList::ExecuteCommands(IRHICommandQueue * CommandQueue)
	{
		FlushBarriers();

		if (!NumCommands)
			return;

		NumCommands = 0;
		
		FD3D12CommandQueue & D3D12CommandQueue = StaticCastRef<FD3D12CommandQueue>(CommandQueue);

		// ExecuteCommandLists and Signal
		d3d12CommandList->Close();
		ID3D12CommandList * D3D12CommandLists[1] = { d3d12CommandList.Get() };
		D3D12CommandQueue.d3d12CommandQueue->ExecuteCommandLists(1, D3D12CommandLists);
		D3D12CommandQueue.d3d12CommandQueue->Signal(D3D12CommandQueue.QueueFence.d3d12Fence, ++D3D12CommandQueue.QueueFence.SignalValue);
		D3D12CommandQueue.FenceCommandList(d3d12CommandList, CommandAllocator);

		// - reset to init state
		d3d12CommandList = CommandAllocator->AcquireNextCommandList();

		ResetPipelineStates(D3D12CommandQueue);

		D3D12CommandQueue.UpdateContext();
	}

	void FD3D12CommandList::FlushCommands(IRHICommandQueue * CommandQueue)
	{
		FlushBarriers();

		if (!NumCommands)
			return;

		NumCommands = 0;

		FD3D12CommandQueue & D3D12CommandQueue = StaticCastRef<FD3D12CommandQueue>(CommandQueue);

		// ExecuteCommandLists and Signal
		d3d12CommandList->Close();
		ID3D12CommandList * D3D12CommandLists[1] = { d3d12CommandList.Get() };
		D3D12CommandQueue.d3d12CommandQueue->ExecuteCommandLists(1, D3D12CommandLists);
		D3D12CommandQueue.d3d12CommandQueue->Signal(D3D12CommandQueue.QueueFence.d3d12Fence, ++D3D12CommandQueue.QueueFence.SignalValue);
		D3D12CommandQueue.FenceCommandList(d3d12CommandList, CommandAllocator);
		D3D12CommandQueue.FenceCommandAllocator(CommandAllocator, ECommandType::Direct);

		// - reset to init state
		d3d12CommandList = nullptr;
		CommandAllocator = nullptr;
		CommandAllocator = D3D12Devcie.AquireCommandAllocator(CommandType);
		d3d12CommandList = CommandAllocator->AcquireNextCommandList();
		ResetPipelineStates(D3D12CommandQueue);

		D3D12CommandQueue.UpdateContext();
	}

	void FD3D12CommandList::SetMarker(FStringV Marker, const FColor & Color)
	{
#if XIN_D3D12_PIX
		::PIXSetMarker(d3d12CommandList.Get(), Color.ToARGB(), (const char *)Marker.Data);
#endif
	}

	void FD3D12CommandList::BeginLabel(FStringV Label, const FColor & Color)
	{
#if XIN_D3D12_PIX
		::PIXBeginEvent(d3d12CommandList.Get(), Color.ToARGB(), (const char *)Label.Data);
#endif
	}

	void FD3D12CommandList::EndLabel()
	{
#if XIN_D3D12_PIX
		::PIXEndEvent(d3d12CommandList.Get());
#endif
	}

	void FD3D12CommandList::CopyDescriptors(D3D12_DESCRIPTOR_RANGE_TYPE DescriptorRangeType, FCPUAddress DestinationDescriptor, FCPUAddress SourceDescriptor, uint32 DescriptorCount)
	{
		switch(DescriptorRangeType)
		{
		case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
		case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
		case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
			break;
		default:
			AssertFailed(u8"CopyDescriptors only surpport for D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV"V);
			return;
		}

		D3D12Devcie.d3d12Device->CopyDescriptorsSimple(DescriptorCount, { (SIZE_T)DestinationDescriptor }, { (SIZE_T)SourceDescriptor }, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	inline void FD3D12CommandList::FlushBarriers()
	{
		if (!ResourceBarriers.Size && !Barriers.Size)
			return;

		TList<D3D12_RESOURCE_BARRIER> d3d12ResourceBarriers { ResourceBarriers.Size + Barriers.Size };
		for (uintx ResourceIndex = 0; ResourceIndex < ResourceBarriers.Size; ++ResourceIndex)
		{
			FD3D12Resource * D3D12Resource = ResourceBarriers[ResourceIndex];
			D3D12_RESOURCE_BARRIER & d3d12ResourceBarrier = d3d12ResourceBarriers[ResourceIndex];

			d3d12ResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			d3d12ResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			d3d12ResourceBarrier.Transition.pResource = D3D12Resource->d3d12Resource;
			d3d12ResourceBarrier.Transition.StateBefore = D3D12Resource->StatesBefore;
			d3d12ResourceBarrier.Transition.StateAfter = D3D12Resource->StatesAfter;
			d3d12ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			D3D12Resource->StatesBefore = D3D12Resource->StatesAfter;
		}

		ArrayCopy(d3d12ResourceBarriers.Data + ResourceBarriers.Size, Barriers.Data, Barriers.Size);

		d3d12CommandList->ResourceBarrier((uint32)d3d12ResourceBarriers.Size, d3d12ResourceBarriers.Data);
		ResourceBarriers.Clear();
		Barriers.Clear();
		++NumCommands;
	}

	void FD3D12CommandList::BeginPass(const FRenderPassState & RenderPassState)
	{
		this->RenderPassState = RenderPassState;
		RenderPassStateChanged = true;
	}

	void FD3D12CommandList::EndPass()
	{
		this->RenderPassState = {};
		RenderPassStateChanged = true;
		FlushBarriers();
	}

	//void FD3D12CommandList::FlushCommands(IRHICommandExecutor * CommandQueue)
	//{
	//	FlushBarriers();
	//	if (!CommandCount)
	//		return;

	//	FD3D12CommandExecutor & D3D12CommandQueue = StaticCastRef<FD3D12CommandExecutor>(CommandQueue);
	//	D3D12CommandQueue.Execute(this);
	//}


	void FD3D12CommandList::DiscardResource(IRHIResource * Resource, TView<RectU> Rects)
	{
		FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(Resource);
		FlushBarriers();
		AssertExpr(Rects.Size == 0);
		d3d12CommandList->DiscardResource(D3D12Resource.d3d12Resource, nullptr);
	}

	void FD3D12CommandList::ClearDepthStencil(IRHIResourceView * DepthStencilView, float32 Depth, uint8 Stencial)
	{
		FD3D12ResourceView & D3D12ResourceView = StaticCastRef<FD3D12ResourceView>(DepthStencilView);
		ResourceAccess(&D3D12ResourceView.D3D12Resource, EResourceAccess::DepthWrite);

		if (D3D12ResourceView.DSV.Address.Offline())
		{
			D3D12_CLEAR_FLAGS ClearFlags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
			d3d12CommandList->ClearDepthStencilView(CPUDescriptorHandle(D3D12ResourceView.DSV.Address.CPUAddress), ClearFlags, Depth, Stencial, 0, nullptr);
		}
	}

	void FD3D12CommandList::ClearRenderTarget(IRHIResourceView * RenderTargetView, FColorF Color)
	{
		FD3D12ResourceView & D3D12ResourceView = StaticCastRef<FD3D12ResourceView>(RenderTargetView);

		AssertExpr(D3D12ResourceView.RTV.Address.Offline());
		FlushBarriers();

		d3d12CommandList->ClearRenderTargetView(CPUDescriptorHandle(D3D12ResourceView.RTV.Address.CPUAddress), Color.Values, 0, nullptr);
	}

	void FD3D12CommandList::ClearUnorderedAccess(IRHIResourceView * UnorderedAccessView, uint32 ClearValue)
	{
		FD3D12ResourceView & D3D12ResourceView = StaticCastRef<FD3D12ResourceView>(UnorderedAccessView);

		FRHIAddress DescriptorAddress = AllocateOnlineDescriptors(1);
		CopyDescriptors(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, DescriptorAddress.CPUAddress, D3D12ResourceView.UAV.Address.CPUAddress, 1);

		FlushBarriers();

		UINT Values[4] { ClearValue, ClearValue, ClearValue, ClearValue };
		AssertExpr(D3D12ResourceView.UAV.Address.Offline());
		d3d12CommandList->ClearUnorderedAccessViewUint(
			{ DescriptorAddress.GPUAddress }, CPUDescriptorHandle(D3D12ResourceView.UAV.Address.CPUAddress), 
			D3D12ResourceView.D3D12Resource.d3d12Resource, Values, 0, nullptr);
	}

	void FD3D12CommandList::SetViewPorts(TView<FViewport> ViewPorts)
	{
		NumViewports = (uint32)ViewPorts.Size;
		for (uintx ViewportIndex : IRange(ViewPorts.Size))
			D3D12ViewPorts[ViewportIndex] = FromViewport(ViewPorts[ViewportIndex]);
		ViewPortsChanged = true;
	}

	void FD3D12CommandList::SetScissorRects(TView<RectI> ScissorRects)
	{
		NumScissorRects = (uint32)ScissorRects.Size;
		for (uintx ScissorRectIndex : IRange(ScissorRects.Size))
			D3D12ScissorRects[ScissorRectIndex] = FromRect(ScissorRects[ScissorRectIndex]);
		ScissorRectsChanged = true;
	}

	void FD3D12CommandList::SetPipelineState(IRHIPipelineState * PipelineState)
	{
		if (this->PipelineState != StaticCast<FD3D12PipelineState>(PipelineState))
		{
			++FrameStatics.PipelineStates;
			this->PipelineState = StaticCast<FD3D12PipelineState>(PipelineState);
			PipelineStateChanged = true;
		}
	}

	static inline FRHIAddress GetPageableAddress(IRHIPageable * RHIPageable, D3D12_DESCRIPTOR_RANGE_TYPE d2d12DescriptorRangeType)
	{
		switch (RHIPageable->Pageable)
		{
		case EPageable::ResourceView:
			return StaticCastRef<FD3D12ResourceView>(RHIPageable).SRV.Address;
		default:
		{
			FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(RHIPageable);
			switch (d2d12DescriptorRangeType)
			{
			case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
				return D3D12Resource.BaseAddress + D3D12Resource.Location.Offset;
			case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
				return D3D12Resource.ResourceView->SRV.Address;
			case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
				return D3D12Resource.ResourceView->UAV.Address;
			default:
				return D3D12Resource.BaseAddress + D3D12Resource.Location.Offset;
			}
		}
		}
	}

	void FD3D12CommandList::SetParameter(FName ParameterName, const FRHIBlock & Block)
	{
		for (uintx ParameterIndex = 0; ParameterIndex < MaxD3D12ShaderParameters; ++ParameterIndex)
		{
			const FD3D12ShaderParameter & ShaderParameter = PipelineState->ShaderParameters.Parameters[ParameterIndex];
			if (ShaderParameter.ShaderStage == EShaderStage::None)
				break;

			if (ShaderParameter.BindingName == ParameterName)
			{
				BindingParameters[ParameterIndex] = Block.BaseAddress + Block.Location.Offset;
				ParametersChanged = true;
			}
		}

		ParametersChanged = true;
	}

	void FD3D12CommandList::SetParameter(FName ParameterName, IRHIPageable * Pageable)
	{
		AssertExpr(PipelineState);

		for (uintx ParameterIndex = 0; ParameterIndex < MaxD3D12ShaderParameters; ++ParameterIndex)
		{
			const FD3D12ShaderParameter & ShaderParameter = PipelineState->ShaderParameters.Parameters[ParameterIndex];
			if (ShaderParameter.ShaderStage == EShaderStage::None)
				break;

			if (ShaderParameter.BindingName == ParameterName)
			{
				BindingParameters[ParameterIndex] = GetPageableAddress(Pageable, ShaderParameter.ParameterType);
				ParametersChanged = true;
			}
		}

		ParametersChanged = true;
	}

	void FD3D12CommandList::IASetVertexBuffers(uint32 BaseSlotIndex, TView<IRHIResource *> VertexBuffers, TView<uint32> Sizes, TView<uint32> Strides, uint32 VertexCount)
	{
		AssertExpr(VertexBuffers.Size <= Sizes.Size);
		AssertExpr(VertexBuffers.Size <= Strides.Size);

		NumVertexBuffers = Min(VertexBuffers.Size, MaxVertexBuffers);
		for (uint32 VertexBufferIndex = 0; BaseSlotIndex + VertexBufferIndex < NumVertexBuffers; ++VertexBufferIndex)
		{
			FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(VertexBuffers[VertexBufferIndex]);
			this->VertexBuffers[VertexBufferIndex] = &D3D12Resource;
		}
		this->VertexBufferSizes = Sizes;
		this->VertexBufferStrides = Strides;
	}

	void FD3D12CommandList::IASetIndexBuffer(IRHIResource * IndexBuffer, EIndexFormat IndexFormat, uint32 IndexCount)
	{
		this->IndexBuffer = StaticCast<FD3D12Resource>(IndexBuffer);
		this->IndexFormat = IndexFormat;
	}

	void FD3D12CommandList::DrawInstanced(uint32 VertexOffset, uint32 VertexCount, uint32 InstanceOffset, uint32 InstanceCount)
	{
		AssertExpr(VertexCount >= 1);
		AssertExpr(InstanceCount >= 1);

		++FrameStatics.DrawCalls;
		switch (PipelineState->PipelineStateDesc.Topology)
		{
		case ETopology::PointList:
			FrameStatics.Primitives += VertexCount;
			break;
		case ETopology::LineList:
			FrameStatics.Primitives += VertexCount / 2;
			break;
		case ETopology::LineStrip:
			FrameStatics.Primitives += (VertexCount + 1) / 2;
			break;
		case ETopology::TriangleList:
			FrameStatics.Primitives += VertexCount / 3;
			break;
		case ETopology::TriangleStrip:
			FrameStatics.Primitives += (VertexCount + 2) / 3;
			break;
		default:
			break;
		}

		FlushBarriers();
		ApplyRenderStates();
		ApplyResources();

		d3d12CommandList->DrawInstanced(VertexCount, InstanceCount, VertexOffset, InstanceOffset);

		++NumCommands;
	}

	void FD3D12CommandList::DrawIndexedInstanced(uint32 IndexOffset, uint32 IndexCount, uint32 VertexOffset, uint32 InstanceOffset, uint32 InstanceCount)
	{
		AssertExpr(IndexCount >= 1);
		AssertExpr(InstanceCount >= 1);

		++FrameStatics.DrawCalls;
		switch(PipelineState->PipelineStateDesc.Topology)
		{
		case ETopology::PointList:
			FrameStatics.Primitives += IndexCount;
			break;
		case ETopology::LineList:
			FrameStatics.Primitives += IndexCount / 2;
			break;
		case ETopology::LineStrip:
			FrameStatics.Primitives += (IndexCount + 1) / 2;
			break;
		case ETopology::TriangleList:
			FrameStatics.Primitives += IndexCount / 3;
			break;
		case ETopology::TriangleStrip:
			FrameStatics.Primitives += (IndexCount + 2) / 3;
			break;
		default:
			break;
		}

		FlushBarriers();
		ApplyRenderStates();
		ApplyResources();

		d3d12CommandList->DrawIndexedInstanced(IndexCount, InstanceCount, IndexOffset, VertexOffset, InstanceOffset);
		++NumCommands;
	}

	void FD3D12CommandList::Dispatch(Vec3U GroupCount)
	{
		++FrameStatics.DispatchCalls;
		FlushBarriers();
		ApplyRenderStates();
		ApplyResources();
		d3d12CommandList->Dispatch(GroupCount.X, GroupCount.Y, GroupCount.Z);
	}

	void FD3D12CommandList::DispatchIndirect(IRHIIndirectSignature * IndirectSignature, IRHIResource * IndirectBuffer, uint32 IndirectBufferOffset, uint32 MaxCommandCount)
	{
		FD3D12IndirectSignature & D3D12IndirectSignature = StaticCastRef<FD3D12IndirectSignature>(IndirectSignature);
		FD3D12Resource & D3D12IndirectBuffer = StaticCastRef<FD3D12Resource>(IndirectBuffer);
		FlushBarriers();
		ApplyRenderStates();
		ApplyResources();
		d3d12CommandList->ExecuteIndirect(D3D12IndirectSignature.D3D12CommandSignature, MaxCommandCount, D3D12IndirectBuffer.d3d12Resource, IndirectBufferOffset, nullptr, 0);

		++FrameStatics.IndirectDispatchCalls;
	}

	void FD3D12CommandList::ResourceAccess(IRHIResource * Resource, EResourceAccess AccessAfter)
	{
		FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(Resource);
		if (!D3D12Resource.d3d12Resource)
			return;

		if (AccessAfter == EResourceAccess::None)
			AccessAfter = D3D12Resource.InitialAccess;

		D3D12_RESOURCE_STATES StatesAfter = FromResourceAccess(AccessAfter, D3D12Resource.InitialAccess);
		if (D3D12Resource.StatesBefore == StatesAfter)
			return;

		D3D12Resource.ResourceAccess = AccessAfter;
		D3D12Resource.StatesAfter = StatesAfter;
		ResourceBarriers.AddUnique(&D3D12Resource);

		//FlushBarriers();
	}

	void FD3D12CommandList::ResourceBarrierUAV(IRHIResource * Resource)
	{
		AssertExpr(Resource);
		if (!Resource)
			return;

		FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(Resource);
		AssertExpr(D3D12Resource.d3d12Resource);

		D3D12_RESOURCE_BARRIER ResourceBarrier;
		ResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		ResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		ResourceBarrier.UAV.pResource = D3D12Resource.d3d12Resource;
		//Barriers.Add(ResourceBarrier);
		d3d12CommandList->ResourceBarrier((uint32)1, &ResourceBarrier);
	}

	void FD3D12CommandList::ResourceBarrierAlias(IRHIResource * ResourceBefore, IRHIResource * ResourceAfter)
	{
		AssertExpr(ResourceAfter);
		if (!ResourceAfter)
			return;

		AssertExpr(!ResourceBefore || StaticCastRef<FD3D12Resource>(ResourceBefore).d3d12Resource);
		AssertExpr(StaticCastRef<FD3D12Resource>(ResourceAfter).d3d12Resource);
		D3D12_RESOURCE_BARRIER ResourceBarrier;
		ResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
		ResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		ResourceBarrier.Aliasing.pResourceBefore = ResourceBefore ? StaticCastRef<FD3D12Resource>(ResourceBefore).d3d12Resource.Get() : nullptr;
		ResourceBarrier.Aliasing.pResourceAfter = StaticCastRef<FD3D12Resource>(ResourceAfter).d3d12Resource;
		//Barriers.Add(ResourceBarrier);
		d3d12CommandList->ResourceBarrier(1, &ResourceBarrier);
	}

	void FD3D12CommandList::CopyResource(IRHIResource * TargetResource, IRHIResource * SourceResource)
	{
		AssertExpr(TargetResource->ResourceDesc.Dimension  == TargetResource->ResourceDesc.Dimension);
		if (TargetResource->ResourceDesc.Dimension != TargetResource->ResourceDesc.Dimension)
			return;

		FlushBarriers();
		d3d12CommandList->CopyResource(
			StaticCast<FD3D12Resource>(TargetResource)->d3d12Resource,
			StaticCast<FD3D12Resource>(SourceResource)->d3d12Resource
		);
		++NumCommands;
	}

	void FD3D12CommandList::CopyBuffer(IRHIResource * TargetResource, uint64 TargetOffset, IRHIResource * SourceResource, uint64 SourceOffset, uint64 NumBytes)
	{
		AssertExpr(TargetResource->ResourceDesc.Dimension == EResourceDimension::Buffer);
		AssertExpr(SourceResource->ResourceDesc.Dimension == EResourceDimension::Buffer);

		FD3D12Resource & D3D12TargetBuffer = StaticCastRef<FD3D12Resource>(TargetResource);
		FD3D12Resource & D3D12SourceBuffer = StaticCastRef<FD3D12Resource>(SourceResource);

		d3d12CommandList->CopyBufferRegion(D3D12TargetBuffer.d3d12Resource, TargetOffset, D3D12SourceBuffer.d3d12Resource, SourceOffset, NumBytes);
		++NumCommands;
	}

	void FD3D12CommandList::UpdateBuffer(IRHIResource * Resource, TView<byte> ResourceBytes, TFunction<FRHIMemory(uint64, uint32)> MemoryAllocator)
	{
		FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(Resource);
		AssertExpr(D3D12Resource.d3d12Resource);
		AssertExpr(D3D12Resource.ResourceDesc.Dimension == EResourceDimension::Buffer);

		if (D3D12Resource.ResourceDesc.Usage & EResourceUsage::HostWrite)
		{
			MemoryCopy(D3D12Resource.BaseAddress.CPUAddress + D3D12Resource.Location.Offset, ResourceBytes.Data, ResourceBytes.Size);
		}
		else
		{
			FRHIMemory StagingMemory = MemoryAllocator(ResourceBytes.Size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
			FD3D12Heap & StagingHeap = StaticCastRef<FD3D12Heap>(StagingMemory.PlaceHeap);

			AssertExpr(StagingHeap.d3d12Resource);
			FlushBarriers();

			MemoryCopy(StagingHeap.BaseAddress.CPUAddress + StagingMemory.Location.Offset, ResourceBytes.Data, ResourceBytes.Size);

			d3d12CommandList->CopyBufferRegion(
				D3D12Resource.d3d12Resource, D3D12Resource.Location.Offset,
				StagingHeap.d3d12Resource, StagingMemory.Location.Offset,
				ResourceBytes.Size);
			++NumCommands;
		}
	}

	void FD3D12CommandList::UpdateTexture(IRHIResource * Resource, const FTextureSource & TextureSource, TFunction<FRHIMemory(uint64, uint32)> MemoryAllocator)
	{
		FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(Resource);
		AssertExpr(D3D12Resource.d3d12Resource);

		TList<FCopyableFootprint> CopyableFootprints { D3D12Resource.ResourceDesc.Layers * D3D12Resource.ResourceDesc.Mipmaps };
		uint64 RequiedSize = D3D12Devcie.GetCopyableFootprints(0, D3D12Resource.ResourceDesc, CopyableFootprints);

		if (D3D12Resource.ResourceDesc.Usage & EResourceUsage::HostWrite)
			WriteTextureDataWithFootprints(D3D12Resource.BaseAddress.CPUAddress + D3D12Resource.Location.Offset, D3D12Resource.ResourceDesc, TextureSource, CopyableFootprints);
		else
		{
			FRHIMemory StagingMemory = MemoryAllocator(RequiedSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
			FD3D12Heap & StagingHeap = StaticCastRef<FD3D12Heap>(StagingMemory.PlaceHeap);

			AssertExpr(StagingHeap.d3d12Resource);
			FlushBarriers();

			WriteTextureDataWithFootprints(StagingHeap.BaseAddress.CPUAddress + StagingMemory.Location.Offset, D3D12Resource.ResourceDesc, TextureSource, CopyableFootprints);
			for (uint32 DepthIndex = 0, SubresourceIndex = 0; DepthIndex < D3D12Resource.ResourceDesc.Layers; ++DepthIndex)
			{
				for (uint32 MipmapIndex = 0; MipmapIndex < D3D12Resource.ResourceDesc.Mipmaps; ++MipmapIndex, ++SubresourceIndex)
				{
					FCopyableFootprint CopyableFootprint = CopyableFootprints[SubresourceIndex];
					CopyableFootprint.Offset += StagingMemory.Location.Offset;
					{
						D3D12_SUBRESOURCE_FOOTPRINT D3D12SourceSubresource;
						D3D12SourceSubresource.Format = FromFormat(CopyableFootprint.Format);
						D3D12SourceSubresource.Width = CopyableFootprint.Size.X;
						D3D12SourceSubresource.Height = CopyableFootprint.Size.Y;
						D3D12SourceSubresource.Depth = CopyableFootprint.Depth;
						D3D12SourceSubresource.RowPitch = CopyableFootprint.RowPitch;

						CD3DX12_TEXTURE_COPY_LOCATION DestinationLocation(D3D12Resource.d3d12Resource, SubresourceIndex);
						CD3DX12_TEXTURE_COPY_LOCATION SourceLocation(StagingHeap.d3d12Resource, { CopyableFootprint.Offset, D3D12SourceSubresource });
						d3d12CommandList->CopyTextureRegion(&DestinationLocation, 0, 0, 0, &SourceLocation, nullptr);

						++NumCommands;
					}
				}
			}
		}
	}

	FRHIAddress FD3D12CommandList::AllocateOnlineDescriptors(uint32 DescriptorsCount)
	{
		if (DescriptorsCount)
		{
			FD3D12DescriptorHeapRef CurrentDescriptorHeap;
			if (OnlineDescriptorHeaps.Size == 0 || !OnlineDescriptorHeaps.Back()->CanAllocate(DescriptorsCount))
			{
				CurrentDescriptorHeap = D3D12Devcie.AquireOnlineDescriptorHeap();
				OnlineDescriptorHeaps.Add(CurrentDescriptorHeap);

				DescriptorHeaps[ED3D12DescriptorHeapType::Resources] = CurrentDescriptorHeap->D3D12DescriptorHeap;
				DescriptorHeapsChanged = true;
			}
			else
				CurrentDescriptorHeap = OnlineDescriptorHeaps.Back();
			return CurrentDescriptorHeap->Allocate(DescriptorsCount).RHIAddress;
		}
		else
			return { };
	}

	void FD3D12CommandList::ApplyResources()
	{
		if (!ParametersChanged)
			return;

		ParametersChanged = false;

		EPipelineType PipelineType = PipelineState->PipelineStateDesc.Shaders[EShaderStage::Compute] ? EPipelineType::Compute : EPipelineType::Graphics;
		auto SetRootConstantBufferView = PipelineType == EPipelineType::Graphics ? &ID3D12GraphicsCommandList::SetGraphicsRootConstantBufferView : &ID3D12GraphicsCommandList::SetComputeRootConstantBufferView;
		auto SetRootShaderResourceView = PipelineType == EPipelineType::Graphics ? &ID3D12GraphicsCommandList::SetGraphicsRootShaderResourceView : &ID3D12GraphicsCommandList::SetComputeRootShaderResourceView;
		auto SetRootUnorderedAccessView = PipelineType == EPipelineType::Graphics ? &ID3D12GraphicsCommandList::SetGraphicsRootUnorderedAccessView : &ID3D12GraphicsCommandList::SetComputeRootUnorderedAccessView;
		auto SetRootDescriptorTable = PipelineType == EPipelineType::Graphics ? &ID3D12GraphicsCommandList::SetGraphicsRootDescriptorTable : &ID3D12GraphicsCommandList::SetComputeRootDescriptorTable;

		FRHIAddress DescriptorAddress = AllocateOnlineDescriptors(PipelineState->ShaderParameters.GetNumDescriptors());

		if (DescriptorHeapsChanged)
		{
			DescriptorHeapsChanged = false;
			if (DescriptorHeaps[ED3D12DescriptorHeapType::Resources] && DescriptorHeaps[ED3D12DescriptorHeapType::Samplers])
				d3d12CommandList->SetDescriptorHeaps(2, DescriptorHeaps.Data);
			else if (DescriptorHeaps[ED3D12DescriptorHeapType::Resources])
				d3d12CommandList->SetDescriptorHeaps(1, DescriptorHeaps.Data);
			else if (DescriptorHeaps[ED3D12DescriptorHeapType::Samplers])
				d3d12CommandList->SetDescriptorHeaps(1, DescriptorHeaps.Data + 1);
			else
			{
				//D3D12CommandList->SetDescriptorHeaps(0, nullptr);
			}
		}

		for (uint32 ParameterIndex = 0; ParameterIndex < MaxD3D12ShaderParameters; ++ParameterIndex)
		{
			const FD3D12ShaderParameter & ShaderParameter = PipelineState->ShaderParameters.Parameters[ParameterIndex];
			if (ShaderParameter.ShaderStage == EShaderStage::None)
				break;

			if (ShaderParameter.ParameterType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV || 
				ShaderParameter.ParameterType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV)
			{
				AssertExpr(BindingParameters[ParameterIndex].CPUAddress);

				FCPUAddress CPUAddress = DescriptorAddress.CPUAddress + ShaderParameter.DescriptorIndex * DescriptorAddressStride;
				CopyDescriptors(ShaderParameter.ParameterType, CPUAddress, BindingParameters[ParameterIndex].CPUAddress, 1);
			}
		}

		for (uint32 RootParameterIndex = 0; RootParameterIndex < PipelineState->ShaderParameters.RootParameters.Size; ++RootParameterIndex)
		{
			const FD3D12RootParameter & RootParameter = PipelineState->ShaderParameters.RootParameters[RootParameterIndex];
			if (RootParameter.ShaderStage == EShaderStage::None)
				break;

			switch (RootParameter.RootParameterType)
			{
			case D3D12_ROOT_PARAMETER_TYPE_CBV:
				(d3d12CommandList.Get()->*SetRootConstantBufferView)(RootParameterIndex, BindingParameters[RootParameter.ParameterIndex].GPUAddress);
				break;
			case D3D12_ROOT_PARAMETER_TYPE_SRV:
				(d3d12CommandList.Get()->*SetRootShaderResourceView)(RootParameterIndex, BindingParameters[RootParameter.ParameterIndex].GPUAddress);
				break;
			case D3D12_ROOT_PARAMETER_TYPE_UAV:
				(d3d12CommandList.Get()->*SetRootUnorderedAccessView)(RootParameterIndex, BindingParameters[RootParameter.ParameterIndex].GPUAddress);
				break;
			case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
			{
				AssertExpr(DescriptorHeaps[ED3D12DescriptorHeapType::Resources]);

				const FD3D12ParameterRange & ParameterRange = PipelineState->ShaderParameters.ParameterRanges[RootParameter.RangeIndex];
				FGPUAddress GPUAddress = DescriptorAddress.GPUAddress + ParameterRange.DescriptorIndex * DescriptorAddressStride;

				(d3d12CommandList.Get()->*SetRootDescriptorTable)(RootParameterIndex, { GPUAddress });
				break;
			}
			default:
				break;
			}
		}

		if (NumVertexBuffers > 0)
		{
			TArray<D3D12_VERTEX_BUFFER_VIEW, MaxVertexBuffers> VertexBufferViews;
			uint32 BaseSlotIndex = 0;
			for (uint32 VertexBufferIndex = 0; BaseSlotIndex + VertexBufferIndex < NumVertexBuffers; ++VertexBufferIndex)
			{
				FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(VertexBuffers[VertexBufferIndex]);
				AssertExpr(D3D12Resource.BaseAddress.Online());

				D3D12_VERTEX_BUFFER_VIEW D3D12VertexBufferView;
				{
					D3D12VertexBufferView.BufferLocation = D3D12Resource.BaseAddress.GPUAddress + D3D12Resource.Location.Offset;
					D3D12VertexBufferView.SizeInBytes = VertexBufferSizes[VertexBufferIndex];
					D3D12VertexBufferView.StrideInBytes = VertexBufferStrides[VertexBufferIndex];
				}
				VertexBufferViews[BaseSlotIndex + VertexBufferIndex] = D3D12VertexBufferView;
			}

			d3d12CommandList->IASetVertexBuffers(BaseSlotIndex, NumVertexBuffers, VertexBufferViews.Data);
		}

		if (IndexBuffer)
		{
			FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(IndexBuffer);
			AssertExpr(D3D12Resource.BaseAddress.Online());
			IndexBuffer = &D3D12Resource;

			D3D12_INDEX_BUFFER_VIEW IndexBufferView;
			IndexBufferView.BufferLocation = D3D12Resource.BaseAddress.GPUAddress + D3D12Resource.Location.Offset;
			IndexBufferView.SizeInBytes = D3D12Resource.ResourceDesc.Size.X;
			IndexBufferView.Format = FromIndexFormat(IndexFormat);
			d3d12CommandList->IASetIndexBuffer(&IndexBufferView);
		}

		d3d12CommandList->IASetPrimitiveTopology(FromTopology(PipelineState->PipelineStateDesc.Topology, 0));
	}

	void FD3D12CommandList::ApplyRenderStates()
	{
		EPipelineType PipelineType = PipelineState && PipelineState->PipelineStateDesc.Shaders[EShaderStage::Compute] ? EPipelineType::Compute : EPipelineType::Graphics;

		// Pipeline State
		if (PipelineStateChanged)
		{
			PipelineStateChanged = false;
			if (PipelineType == EPipelineType::Graphics)
				d3d12CommandList->SetGraphicsRootSignature(PipelineState->d3d12RootSignature);
			else
				d3d12CommandList->SetComputeRootSignature(PipelineState->d3d12RootSignature);
			d3d12CommandList->SetPipelineState(PipelineState->d3d12PipelineState);
		}

		// OMSetRenderTargets
		if (PipelineType == EPipelineType::Graphics && RenderPassStateChanged)
		{
			RenderPassStateChanged = false;

			uint32 NumRenderTargets = 0;
			D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetsCPUDescriptorHandles[MaxRenderTargets] = {};
			D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilCPUDescriptorHandle {};
			for (uint32 Index = 0; Index < MaxRenderTargets; ++Index)
			{
				if (const FRenderPassTarget & RenderPassTarget = RenderPassState.RenderTargets[Index])
				{
					FD3D12ResourceView & D3D12ResourceView = StaticCastRef<FD3D12ResourceView>(RenderPassState.RenderTargets[Index].ResourceView);
					switch (RenderPassTarget.LoadAction)
					{
					case EResourceLoad::Clear:
					{
						AssertExpr(D3D12ResourceView.RTV.Address.Offline());

						D3D12_CLEAR_VALUE ColorClearValue;
						if (RenderPassTarget.ClearValue)
							ColorClearValue = FromClearValue(RenderPassTarget.ClearValue, D3D12ResourceView.D3D12Resource.ResourceDesc.Format, D3D12ResourceView.D3D12Resource.ResourceDesc.Usage);
						else if (D3D12ResourceView.D3D12Resource.ResourceDesc.ClearValue)
							ColorClearValue = FromClearValue(D3D12ResourceView.D3D12Resource.ResourceDesc.ClearValue, D3D12ResourceView.D3D12Resource.ResourceDesc.Format, D3D12ResourceView.D3D12Resource.ResourceDesc.Usage);
						else
							ColorClearValue = {};

						d3d12CommandList->ClearRenderTargetView(CPUDescriptorHandle(D3D12ResourceView.RTV.Address.CPUAddress), ColorClearValue.Color, 0, nullptr);
						break;
					}
					case EResourceLoad::Discard:
						d3d12CommandList->DiscardResource(D3D12ResourceView.D3D12Resource.d3d12Resource, nullptr);
						break;
					default:
						break;
					}
					RenderTargetsCPUDescriptorHandles[Index] = CPUDescriptorHandle(D3D12ResourceView.RTV.Address.CPUAddress);
					NumRenderTargets = Index + 1;
				}
			}

			if (RenderPassState.DepthStencil)
			{
				FD3D12ResourceView & D3D12ResourceView = StaticCastRef<FD3D12ResourceView>(RenderPassState.DepthStencil.ResourceView);
				switch (RenderPassState.DepthStencil.LoadAction)
				{
				case EResourceLoad::Clear:
				{
					AssertExpr(D3D12ResourceView.DSV.Address.Offline());

					D3D12_CLEAR_VALUE ColorClearValue;
					if (RenderPassState.DepthStencil.ClearValue)
						ColorClearValue = FromClearValue(RenderPassState.DepthStencil.ClearValue, D3D12ResourceView.D3D12Resource.ResourceDesc.Format, D3D12ResourceView.D3D12Resource.ResourceDesc.Usage);
					else if (D3D12ResourceView.D3D12Resource.ResourceDesc.ClearValue)
						ColorClearValue = FromClearValue(D3D12ResourceView.D3D12Resource.ResourceDesc.ClearValue, D3D12ResourceView.D3D12Resource.ResourceDesc.Format, D3D12ResourceView.D3D12Resource.ResourceDesc.Usage);
					else
						ColorClearValue = {};

					D3D12_CLEAR_FLAGS ClearFlags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
					d3d12CommandList->ClearDepthStencilView(CPUDescriptorHandle(D3D12ResourceView.DSV.Address.CPUAddress), ClearFlags, ColorClearValue.DepthStencil.Depth, ColorClearValue.DepthStencil.Stencil, 0, nullptr);
					break;
				}
				case EResourceLoad::Discard:
					d3d12CommandList->DiscardResource(D3D12ResourceView.D3D12Resource.d3d12Resource, nullptr);
					break;
				default:
					break;
				}

				DepthStencilCPUDescriptorHandle = CPUDescriptorHandle(D3D12ResourceView.DSV.Address.CPUAddress);
			}

			d3d12CommandList->OMSetRenderTargets(NumRenderTargets, RenderTargetsCPUDescriptorHandles, FALSE, RenderPassState.DepthStencil ? &DepthStencilCPUDescriptorHandle : nullptr);

			if (!ViewPortsChanged)
			{
				NumViewports = 0;
				for (uintx ViewportIndex = 0; ViewportIndex < RenderPassState.Viewports.Size; ++ViewportIndex)
				{
					const FViewport & Viewport = RenderPassState.Viewports[ViewportIndex];
					if (Viewport.Width == 0 || Viewport.Height == 0)
						break;

					D3D12ViewPorts[ViewportIndex] = FromViewport(Viewport);
					++NumViewports;
				}

				if (!NumViewports)
				{
					AssertExpr(RenderPassState.RenderTargets[0] || RenderPassState.DepthStencil);
					NumViewports = 1;
					const FRenderPassTarget & FirstRenderPassTarget = RenderPassState.RenderTargets[0] ? RenderPassState.RenderTargets[0] : RenderPassState.DepthStencil;
					const FD3D12ResourceView & ResourceView = StaticCastRef<FD3D12ResourceView>(FirstRenderPassTarget.ResourceView);
					D3D12ViewPorts[0] = D3D12_VIEWPORT{ 0, 0, float32(ResourceView.Resource.ResourceDesc.Size.Width), float32(ResourceView.Resource.ResourceDesc.Size.Height), 0.0f, 1.0f };
				}

				ViewPortsChanged = true;
			}
		}
		else {}

		if (ViewPortsChanged)
		{
			ViewPortsChanged = false;
			d3d12CommandList->RSSetViewports(NumViewports, D3D12ViewPorts.Data);
		}

		if (ScissorRectsChanged)
		{
			ScissorRectsChanged = false;
			d3d12CommandList->RSSetScissorRects(NumScissorRects, D3D12ScissorRects.Data);
		}
		else
		{
			if (NumScissorRects == 0)
			{
				const D3D12_VIEWPORT & Viewport = D3D12ViewPorts[0];
				AssertExpr(Viewport.Width > 0 && Viewport.Height > 0);

				D3D12_RECT D3D12ScissorRect { int32(Viewport.TopLeftX), int32(Viewport.TopLeftY), int32(Viewport.TopLeftX + Viewport.Width), int32(Viewport.TopLeftY + Viewport.Height) };
				d3d12CommandList->RSSetScissorRects(1, &D3D12ScissorRect);
			}
		}
	}

	void FD3D12CommandList::ResetPipelineStates(FD3D12CommandQueue & D3D12CommandQueue)
	{
		for (FD3D12DescriptorHeapRef & OnlineDescriptorHeap : OnlineDescriptorHeaps)
			D3D12CommandQueue.FenceOnlineDescriptorHeap(OnlineDescriptorHeap);
		OnlineDescriptorHeaps.Clear();

		ParametersChanged = false;
		BindingParameters = { };

		PipelineStateChanged = false;
		PipelineState = nullptr;

		RenderPassStateChanged = false;
		RenderPassState = { };

		ViewPortsChanged = false;
		NumViewports = 0;

		ScissorRectsChanged = false;
		NumScissorRects = 0;

		DescriptorHeapsChanged = false;

		IndexBuffer = nullptr;
		NumVertexBuffers = 0;
		VertexBuffers = {};
	}
}
