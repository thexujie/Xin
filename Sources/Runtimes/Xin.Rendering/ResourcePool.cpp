#include "PCH.h"
#include "ResourcePool.h"

#include "CommandExecutor.h"
#include "BaseEngine.h"

namespace Xin::Rendering
{
	FResourcePool::FResourcePool(FCommandExecutor & Executor)
		: Executor(Executor)
	{
	}

	IRHIResourceRef FResourcePool::AcquireResource(const FResourceDesc & ResourceDesc)
	{
		AssertExpr(ResourceDesc.Layers != 0);
		AssertExpr(ResourceDesc.Mipmaps != 0);
		for (auto Iter = Resources.Begin(); Iter != Resources.End(); ++Iter)
		{
			if (Iter->Second->ResourceDesc == ResourceDesc)
			{
				IRHIResourceRef Resource = Iter->Second;
				Resources.Erase(Iter);
				return Resource;
			}
		}

		IRHIResourceRef Resource = Executor.CreateResource(ResourceDesc);
		if (ResourceDesc != Resource->ResourceDesc)
			AssertExpr(ResourceDesc == Resource->ResourceDesc);
		return Resource;
	}

	void FResourcePool::ForfeitResource(IRHIResource * Resource)
	{
		uint64 FenceSignalValue = Executor.PrimaryQueue->GetFence().GetSignalValue();
		FencedResources.Add({ FenceSignalValue, Resource });
	}

	void FResourcePool::Update(float32 DeltaTime)
	{
		UpdateContext();
	}

	void FResourcePool::UpdateContext()
	{
		uint64 FenceCompletedValue = Executor.PrimaryQueue->GetFence().GetCompletedValue();
		uint64 CurrentTimeMS = FDateTime::HighResolutionMilliseconds();

		{
			constexpr uint64 MaxEvictionGracePeriod = 1000;
			auto ResourceIter = Resources.Begin();
			while (ResourceIter != Resources.End())
			{
				if (CurrentTimeMS - ResourceIter->First >= MaxEvictionGracePeriod)
				{
					ResourceIter = Resources.Erase(ResourceIter);
				}
				else
					++ResourceIter;
			}
		}

		{
			auto FencedResourceIter = FencedResources.Begin();
			while (FencedResourceIter != FencedResources.End())
			{
				if (FencedResourceIter->First <= FenceCompletedValue)
				{
					Resources.Add({ CurrentTimeMS, FencedResourceIter->Second });
					FencedResourceIter = FencedResources.Erase(FencedResourceIter);
				}
				else
					++FencedResourceIter;
			}
		}
	}

	void FResourcePool::ClearContext()
	{
		UpdateContext();
		Resources.Clear();
	}
}
