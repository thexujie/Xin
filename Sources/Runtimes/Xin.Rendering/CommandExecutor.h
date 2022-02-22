#pragma once

#include "Xin.Rendering.Types.h"
#include "CommandList.h"
#include "HeapAllocator.h"
#include "PipelineStateCache.h"
#include "BaseEngine.h"
#include "ResourcePool.h"
#include "TransientAllocator.h"

namespace Xin::Rendering
{
	class RENDERING_API FCommandExecutor : public IRenderingObject
	{
	public:
		FCommandExecutor(FEngine & Engine);
		~FCommandExecutor();

	public:
		void Update(float32 DeltaTime);

	public:
		void UpdateBuffer(IRHIResource * Resource, TView<byte> Bytes);
		void UpdateTexture(IRHIResource * Resource, const FTextureSource & TextureSource);

		IRHIResourceRef CreateResource(const FResourceDesc & ResourceDesc);
		IRHIResourceRef CreateTexture(const FTextureDesc & ResourceDesc, const FTextureSource & TextureSource = {});
		IRHIResourceRef CreateBuffer(const FBufferDesc & ResourceDesc, TView<byte> InitialBytes = { });
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

		void FenceResource(IRHIResource * Resource);
		void FenceResourceView(IRHIResourceView * ResourceView);

	public:
		void Execute(FCommandList & CommandList);
		void Flush(FCommandList & CommandList);
		void WaitFinish();
		void WaitRefresh();

		void UpdateContext();
		void ClearContext();

	public:
		FEngine & Engine;
		IRHICommandQueueRef PrimaryQueue;

	public:
		FPipelineStateCache PipelineStateCache;

		FResourcePool ResourcePool { *this };

	public:
		FHeapAllocator SharedAllocator { Engine.Device };

	public:
		TList<TPair<uint64, IRHIResourceRef>> FencedResources;
		TList<TPair<uint64, IRHIResourceViewRef>> FencedResourceViews;

	public:
		IRHICommandListRef AcquireCommandList(ECommandType CommandType);
		void FenceCommandList(IRHICommandList * CommandList);
		void ForfietCommandList(IRHICommandList * CommandList);

	private:
		TList<IRHICommandListRef> FreeCommandLists;
		TList<TPair<uint64, IRHICommandListRef>> FencedCommandLists;

	public:
		void FenceMemoryBlock(const FRHIMemory & MemoryBlock);
		void FreeMemoryBlock(const FRHIMemory & MemoryBlock);

	private:
		TList<TPair<uint64, FRHIMemory>> FencedMemoryBlocks;

	public:
		FTransientAllocatorRef AcquireTransientAllocator();
		void FenceTransientAllocator(FTransientAllocator * TransientAllocator);

	private:
		TList<TPair<uint64, FTransientAllocatorRef>> FreeTransientAllocators;
		TList<TPair<uint64, FTransientAllocatorRef>> FencedTransientAllocators;
		uint32 TransientAllocatorIndentifyIndex = 0;
	};
}
