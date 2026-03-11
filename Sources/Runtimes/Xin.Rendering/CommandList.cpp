#include "PCH.h"
#include "CommandList.h"

#include "CommandExecutor.h"

namespace Xin::Rendering
{
	FCommandList::FCommandList(FCommandExecutor & CommandExecutor)
		: Executor(CommandExecutor)
	{
		RHICommandList = CommandExecutor.AcquireCommandList(ECommandType::Direct);
		RHICommandList->BeginCommand(CommandExecutor.PrimaryQueue);
	}

	FCommandList::~FCommandList()
	{
		RHICommandList->EndCommand(Executor.PrimaryQueue);
		Executor.FenceCommandList(RHICommandList);
		for (FRHIMemory & InflightMemoryBlock : InflightMemoryBlocks)
			Executor.FenceMemoryBlock(InflightMemoryBlock);
		InflightMemoryBlocks.Reset();
		RHICommandList = nullptr;
	}

	void FCommandList::Flush()
	{
		Executor.Execute(*this);

		for (FRHIMemory & InflightMemoryBlock : InflightMemoryBlocks)
			Executor.FenceMemoryBlock(InflightMemoryBlock);
		InflightMemoryBlocks.Clear();

		for (IRHIResourceRef & InflightResource : InflightResources)
			Executor.FenceResource(InflightResource);
		InflightResources.Clear();
	}

	void FCommandList::SetMeshRenderState(const FMeshRenderState & RenderState)
	{
		MeshRenderState = RenderState;

		RHIPipelineState = Executor.PipelineStateCache.FetchPipelineState(MeshRenderState.Shaders, MeshRenderState.Topology,
			MeshRenderState.InputElements, OutputFormat,
			MeshRenderState.RasterizeState, MeshRenderState.BlendState, MeshRenderState.DepthStencilState);

		RHICommandList->SetPipelineState(RHIPipelineState);
	}

	void FCommandList::SetMeshRenderState(FMeshRenderStateIndex RenderStateIndex)
	{
		if (this->RenderStateIndex != RenderStateIndex)
		{
			this->RenderStateIndex = RenderStateIndex;
			const FMeshRenderState & MeshRenderState = Executor.PipelineStateCache.IndexMeshRenderState(RenderStateIndex);
			SetMeshRenderState(MeshRenderState);
		}
	}

	FRHIBlock FCommandList::AllocShaderParameter(size_t Size)
	{
		FRHIMemory MemoryBlock = Executor.SharedAllocator.Allocate(EResourceUsage::UniformBuffer | EResourceUsage::HostWrite, Size, 0);
		InflightMemoryBlocks.Add(MemoryBlock);
		return FRHIBlock{ MemoryBlock.PlaceHeap->BaseAddress, MemoryBlock.Location };
	}

	void FCommandList::UpdateBuffer(IRHIResource * Resource, TView<byte> Bytes)
	{
		if (Resource->ResourceDesc.Usage & EResourceUsage::HostMask)
		{
			MemoryCopy(Resource->BaseAddress.CPUAddress + Resource->Location.Offset, Bytes.Data, Bytes.Size);
		}
		else
		{
			RHICommandList->ResourceAccess(Resource, EResourceAccess::CopyWrite);
			RHICommandList->UpdateBuffer(Resource, Bytes, [this](uint64 Size, uint32 Alignment)
				{
					FRHIMemory StagingBlock = Executor.SharedAllocator.Allocate(EResourceUsage::HostWrite, Size, Alignment);
					InflightMemoryBlocks.Add(StagingBlock);
					return StagingBlock;
				});
			RHICommandList->ResourceAccess(Resource, EResourceAccess::None);
		}
	}

	void FCommandList::UpdateTexture(IRHIResource * Resource, const FTextureSource & TextureSource)
	{
		RHICommandList->ResourceAccess(Resource, EResourceAccess::CopyWrite);
		RHICommandList->UpdateTexture(Resource, TextureSource, [this](uint64 Size, uint32 Alignment)
			{
				FRHIMemory StagingBlock = Executor.SharedAllocator.Allocate(EResourceUsage::HostWrite, Size, Alignment);
				InflightMemoryBlocks.Add(StagingBlock);
				return StagingBlock;
			});
		RHICommandList->ResourceAccess(Resource, EResourceAccess::None);
		InflightResources.Add(Resource);
	}

	IRHIResourceRef FCommandList::CreateBuffer(const FBufferDesc & ResourceDesc, TView<byte> InitialBytes)
	{
		IRHIResourceRef Resource = Executor.Engine.Device.CreatePlacedResource(ResourceDesc, Executor.SharedAllocator);

		static uint32 CommandExecutorDynamicResourceIndex = 0;
		Resource->SetDebugName(Format(u8"CommandExecutor.Resource.{}"V, CommandExecutorDynamicResourceIndex++));
		if (InitialBytes.Size)
		{
			FCommandList CommandList(Executor);
			CommandList.UpdateBuffer(Resource, InitialBytes);
			CommandList.Flush();
		}

		InflightResources.Add(Resource);
		return Resource;
	}
}
