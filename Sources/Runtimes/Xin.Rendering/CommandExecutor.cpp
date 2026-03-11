#include "PCH.h"
#include "CommandExecutor.h"

namespace Xin::Rendering
{
	FCommandExecutor::FCommandExecutor(FEngine & Engine)
		: Engine(Engine), PipelineStateCache(Engine.Device, true)
	{
		PrimaryQueue = Engine.Device.CreateCommandQueue({ ECommandType::Direct });
		SharedAllocator.SetDebugName(u8"SharedAllocator"V);
	}

	FCommandExecutor::~FCommandExecutor()
	{
		PrimaryQueue->WaitFinish();
	}

	void FCommandExecutor::Update(float32 DeltaTime)
	{
		UpdateContext();
	}

	void FCommandExecutor::UpdateBuffer(IRHIResource * Resource, TView<byte> Bytes)
	{
		if (Resource->ResourceDesc.Usage & EResourceUsage::HostMask)
		{
			MemoryCopy(Resource->BaseAddress.CPUAddress + Resource->Location.Offset, Bytes.Data, Bytes.Size);
		}
		else
		{
			FCommandList CommandList(*this);
			CommandList.UpdateBuffer(Resource, Bytes);
			CommandList.Flush();
		}
	}

	void FCommandExecutor::UpdateTexture(IRHIResource * Resource, const FTextureSource & TextureSource)
	{
		{
			FCommandList CommandList(*this);
			CommandList.UpdateTexture(Resource, TextureSource);
			CommandList.Flush();
		}
	}

	IRHIResourceRef FCommandExecutor::CreateResource(const FResourceDesc & ResourceDesc)
	{
		IRHIResourceRef Resource = Engine.Device.CreatePlacedResource(ResourceDesc, SharedAllocator);
		static uint32 CommandExecutorDynamicResourceIndex = 0;
		Resource->SetDebugName(Format(u8"CommandExecutor.Resource.{}"V, CommandExecutorDynamicResourceIndex++));
		return Resource;
	}

	IRHIResourceRef FCommandExecutor::CreateTexture(const FTextureDesc & ResourceDesc, const FTextureSource & TextureSource)
	{
		AssertExpr(ResourceDesc.Dimension != EResourceDimension::Buffer);

		IRHIResourceRef Resource = Engine.Device.CreatePlacedResource(ResourceDesc, SharedAllocator);
		static uint32 DynamicTextureIndex = 0;
		Resource->SetDebugName(Format(u8"CommandExecutor.Texture.{}"V, DynamicTextureIndex++));

		if (TextureSource.Bytes.Size)
		{
			FCommandList CommandList(*this);
			CommandList.UpdateTexture(Resource, TextureSource);
			CommandList.Flush();
		}
		return Resource;
	}

	IRHIResourceRef FCommandExecutor::CreateBuffer(const FBufferDesc & ResourceDesc, TView<byte> InitialBytes)
	{
		IRHIResourceRef Resource = Engine.Device.CreatePlacedResource(ResourceDesc, SharedAllocator);

		static uint32 CommandExecutorDynamicResourceIndex = 0;
		Resource->SetDebugName(Format(u8"CommandExecutor.Resource.{}"V, CommandExecutorDynamicResourceIndex++));
		if (InitialBytes.Size)
		{
			if (Resource->BaseAddress.CPUAddress)
			{
				MemoryCopy(Resource->BaseAddress.CPUAddress + Resource->Location.Offset, InitialBytes.Data, Min(InitialBytes.Size, Resource->Location.Size));
			}
			else
			{
				FCommandList CommandList(*this);
				CommandList.UpdateBuffer(Resource, InitialBytes);
				CommandList.Flush();
			}
		}
		return Resource;
	}

	void FCommandExecutor::FenceResource(IRHIResource * Resource)
	{
		FencedResources.Add({ PrimaryQueue->GetFence().GetSignalValue(), Resource });
	}

	void FCommandExecutor::FenceResourceView(IRHIResourceView * ResourceView)
	{
		FencedResourceViews.Add({ PrimaryQueue->GetFence().GetSignalValue(), ResourceView });
	}

	void FCommandExecutor::Execute(FCommandList & CommandList)
	{
		CommandList.RHICommandList->ExecuteCommands(PrimaryQueue);
	}

	void FCommandExecutor::WaitFinish()
	{
		PrimaryQueue->WaitFinish();
		UpdateContext();

		AssertExpr(!FencedMemoryBlocks.Size);
		AssertExpr(!FencedCommandLists.Size);
		AssertExpr(!FencedResources.Size);
		AssertExpr(!FencedResourceViews.Size);
		AssertExpr(!FencedTransientAllocators.Size);
	}

	void FCommandExecutor::WaitRefresh()
	{
		PrimaryQueue->WaitFinish();
		ClearContext();
	}

	void FCommandExecutor::UpdateContext()
	{
		PrimaryQueue->UpdateContext();

		Engine.Device.UpdateContext();
		SharedAllocator.UpdateContext();
		ResourcePool.UpdateContext();

		uint64 FenceCompletedValue = PrimaryQueue->GetFence().GetCompletedValue();

		{
			auto Iter = FencedMemoryBlocks.Begin();
			while (Iter != FencedMemoryBlocks.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					FreeMemoryBlock(Iter->Second);
					Iter = FencedMemoryBlocks.Erase(Iter);
				}
				else
					++Iter;
			}
		}

		{
			auto Iter = FencedCommandLists.Begin();
			while (Iter != FencedCommandLists.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					FreeCommandLists.Add(Iter->Second);
					Iter = FencedCommandLists.Erase(Iter);
				}
				else
					++Iter;
			}
		}

		{
			auto Iter = FencedResourceViews.Begin();
			while (Iter != FencedResourceViews.End())
			{
				if (Iter->First <= FenceCompletedValue)
					Iter = FencedResourceViews.Erase(Iter);
				else
					++Iter;
			}
		}

		{
			auto Iter = FencedResources.Begin();
			while (Iter != FencedResources.End())
			{
				if (Iter->First <= FenceCompletedValue)
					Iter = FencedResources.Erase(Iter);
				else
					++Iter;
			}
		}

		uint64 CurrentTimeMS = FDateTime::HighResolutionMilliseconds();
		{
			//constexpr uint64 MaxEvictionGracePeriod = 1000;

			//auto Iter = FreeTransientAllocators.Begin();
			//while (Iter != FreeTransientAllocators.End())
			//{
			//	if (CurrentTimeMS - Iter->First >= MaxEvictionGracePeriod)
			//	{
			//		Iter = FreeTransientAllocators.Erase(Iter);
			//	}
			//	else
			//		++Iter;
			//}
		}

		{
			auto Iter = FencedTransientAllocators.Begin();
			while (Iter != FencedTransientAllocators.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					FreeTransientAllocators.Add({ CurrentTimeMS, Iter->Second });
					Iter = FencedTransientAllocators.Erase(Iter);
				}
				else
					++Iter;
			}
		}
	}

	void FCommandExecutor::ClearContext()
	{
		ResourcePool.ClearContext();
		UpdateContext();

		for (auto & Pair : FreeTransientAllocators)
			Pair.Second->ResetAllocator();
		FreeTransientAllocators.Reset();

		SharedAllocator.ClearContext();
		FreeCommandLists.Reset();
		Engine.Device.ClearContext();
	}

	IRHICommandListRef FCommandExecutor::AcquireCommandList(ECommandType CommandType)
	{
		{
			uint64 FenceCompletedValue = PrimaryQueue->GetFence().GetCompletedValue();

			auto Iter = FencedCommandLists.Begin();
			while (Iter != FencedCommandLists.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					IRHICommandListRef CommandList = Iter->Second;
					Iter = FencedCommandLists.Erase(Iter);
					return CommandList;
				}
				else
					++Iter;
			}
		}

		if (FreeCommandLists.Size)
		{
			IRHICommandListRef CommandList = FreeCommandLists.PopBack();
			return CommandList;
		}

		return Engine.Device.CreateCommandList(ECommandType::Direct);
	}

	void FCommandExecutor::FenceCommandList(IRHICommandList * CommandList)
	{
		FencedCommandLists.Add({ PrimaryQueue->GetFence().GetSignalValue(), CommandList });
	}

	void FCommandExecutor::ForfietCommandList(IRHICommandList * CommandList)
	{
		FreeCommandLists.Add(CommandList);
	}

	void FCommandExecutor::FenceMemoryBlock(const FRHIMemory & MemoryBlock)
	{
		FencedMemoryBlocks.Add({ PrimaryQueue->GetFence().GetSignalValue(), MemoryBlock });
	}

	void FCommandExecutor::FreeMemoryBlock(const FRHIMemory & MemoryBlock)
	{
		MemoryBlock.Instance->Deallocate(MemoryBlock);
	}

	FTransientAllocatorRef FCommandExecutor::AcquireTransientAllocator()
	{
		uint64 FenceCompletedValue = PrimaryQueue->GetFence().GetCompletedValue();
		{
			auto Iter = FencedTransientAllocators.Begin();
			while (Iter != FencedTransientAllocators.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					FTransientAllocatorRef TransientAllocator = Iter->Second;
					Iter = FencedTransientAllocators.Erase(Iter);
					return TransientAllocator;
				}
				else
					++Iter;
			}
		}

		if (FreeTransientAllocators.Size)
		{
			FTransientAllocatorRef TransientAllocator = FreeTransientAllocators.PopBack().Second;
			return TransientAllocator;
		}

		FTransientAllocatorRef TransientAllocator = new FTransientAllocator(Engine.Device);
		return TransientAllocator;
	}

	void FCommandExecutor::FenceTransientAllocator(FTransientAllocator * TransientAllocator)
	{
		FencedTransientAllocators.Add({ PrimaryQueue->GetFence().GetSignalValue(), TransientAllocator });
	}
}
