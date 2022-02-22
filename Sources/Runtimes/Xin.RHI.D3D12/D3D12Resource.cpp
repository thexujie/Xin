#include "PCH.h"
#include "D3D12Heap.h"
#include "D3D12Resource.h"
#include "D3D12Device.h"
#include "D3D12ResourceView.h"

namespace Xin::RHI::D3D12
{
	static FResourceDesc ResourceDescFromD3D12Resource(ID3D12Resource * D3D12Resource)
	{
		FResourceDesc ResourceDesc {};
		if (!D3D12Resource)
			return ResourceDesc;

		D3D12_RESOURCE_DESC D3D12ResourceDesc = D3D12Resource->GetDesc();
		ResourceDesc.Dimension = ToResourceDimension(D3D12ResourceDesc.Dimension);
		ResourceDesc.Usage = ToResourceUsage(D3D12ResourceDesc.Flags);
		ResourceDesc.Format = ToFormat(D3D12ResourceDesc.Format);
		ResourceDesc.Size = { (uint32)D3D12ResourceDesc.Width, (uint32)D3D12ResourceDesc.Height };
		ResourceDesc.Layers = D3D12ResourceDesc.DepthOrArraySize;
		ResourceDesc.Mipmaps = D3D12ResourceDesc.MipLevels;
		ResourceDesc.ClearValue = {};
		return ResourceDesc;
	}

	FD3D12Resource::FD3D12Resource(FD3D12Device & D3D12Devcie, const FResourceDesc & ResourceDesc)
		: IRHIResource(ResourceDesc), D3D12Devcie(D3D12Devcie), InitialAccess(GetInitialAccess(ResourceDesc))
	{
		ResourceAccess = InitialAccess;
		StatesAfter = StatesBefore = FromResourceAccess(InitialAccess);
	}

	FD3D12Resource::FD3D12Resource(FD3D12Device & D3D12Devcie, ID3D12Resource * d3d12Resource, EResourceAccess InitialAccess)
		: IRHIResource(ResourceDescFromD3D12Resource(d3d12Resource)), D3D12Devcie(D3D12Devcie), InitialAccess(InitialAccess), d3d12Resource(d3d12Resource)
	{
		AssertExpr(d3d12Resource);

		ResourceAccess = InitialAccess;
		StatesAfter = StatesBefore = FromResourceAccess(InitialAccess);

		D3D12_HEAP_PROPERTIES D3D12HeapProperties;
		D3D12_HEAP_FLAGS D3D12HeapFlags;
		d3d12Resource->GetHeapProperties(&D3D12HeapProperties, &D3D12HeapFlags);

		if (D3D12HeapProperties.Type == D3D12_HEAP_TYPE_UPLOAD || D3D12HeapProperties.Type == D3D12_HEAP_TYPE_READBACK)
		{
			bytep MappedAddress = nullptr;
			HRESULT Result = d3d12Resource->Map(0, nullptr, (void **)(&MappedAddress));
			if (FAILED(Result))
			{
				LogError(u8" ID3D12Resource::Map failed, {}"V, ErrorDescription(Result));
			}
			else
				BaseAddress.CPUAddress = MappedAddress;
		}

		if (ResourceDesc.Usage & EResourceUsage::BufferMask)
			BaseAddress.GPUAddress = d3d12Resource->GetGPUVirtualAddress();

		FAllocateInfo AllocateInfo = D3D12Devcie.GetAllocateInfo(ResourceDesc);
		Location = { 0, AllocateInfo.Size, AllocateInfo.Size, AllocateInfo.Alignment };
		CreateResourceViews();
	}

	FD3D12Resource::~FD3D12Resource()
	{
		if (Memory.Instance)
		{
			if (!(ResourceDesc.Usage & EResourceUsage::Transisent))
				Memory.Instance->Deallocate(Memory);
			Memory = { };
		}
	}

	void FD3D12Resource::SetDebugName(FStringV Name)
	{
		IRHIResource::SetDebugName(Name);
		if (d3d12Resource)
			SetD3D12ObjectName(d3d12Resource, Name);
	}

	bool FD3D12Resource::IsImplicitStateTransition(EResourceAccess AccessAfter) const
	{
		if (InitialAccess != EResourceAccess::Common)
			return false;

		bool BuffersOrSimultaneous = (ResourceDesc.Dimension == EResourceDimension::Buffer) || (ResourceDesc.Usage & EResourceUsage::Simultaneous);
		if (BuffersOrSimultaneous)
		{
			//return !(ResourceDesc.Usage & EResourceUsage::DepthStencil);
			return !(AccessAfter & EResourceAccess::DepthStencil);
		}
		else
		{
			return AnyFlags(AccessAfter, EResourceUsage::SampledTexture | EResourceUsage::CopyMask);
		}
	}

	bool FD3D12Resource::IsImplicitStateTransition(D3D12_RESOURCE_STATES StateAfter) const
	{
		if (InitialAccess != EResourceAccess::Common)
			return false;

		if (ResourceDesc.Dimension == EResourceDimension::Buffer)
			return true;

		if (ResourceDesc.Usage & EResourceUsage::Simultaneous)
		{
			if (AnyFlags(ResourceDesc.Usage,
				EResourceUsage::SampledTexture
				| EResourceUsage::RenderTarget
				| EResourceUsage::TexelMask
				| EResourceUsage::CopyMask
				| EResourceUsage::ResolveMask))
				return true;
		}
		else
		{
			if (AnyFlags(ResourceDesc.Usage,
				EResourceUsage::SampledTexture
				| EResourceUsage::CopyMask))
				return AnyBits(StateAfter, D3D12_RESOURCE_STATE_COPY_DEST);
		}

		return false;
	}

	void FD3D12Resource::CreateResource(IHeapAllocator & HeapAllocator)
	{
		FAllocateInfo AllocateInfo = D3D12Devcie.GetAllocateInfo(ResourceDesc);
		Memory = HeapAllocator.Allocate(ResourceDesc.Usage, AllocateInfo.Size, AllocateInfo.Alignment);
		CreateResource();
		CreateResourceViews();
	}

	void FD3D12Resource::CreateResource(const FRHIMemory & Memory)
	{
		this->Memory = Memory;
		CreateResource();
		CreateResourceViews();
	}

	void FD3D12Resource::CreateCommitedResource()
	{
		D3D12_HEAP_PROPERTIES HeapProperties { FromHeapType(ResourceDesc.Usage) };
		HeapProperties.CreationNodeMask = 1;
		HeapProperties.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC D3D12ResourceDesc = FromResourceDesc(ResourceDesc, 0);
		D3D12_CLEAR_VALUE d3d12ClearValue = FromClearValue(ResourceDesc.ClearValue, ResourceDesc.Format, ResourceDesc.Usage);
		HRESULT Result = D3D12Devcie.d3d12Device->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE, 
			&D3D12ResourceDesc, StatesBefore, 
			ResourceDesc.Usage & EResourceUsage::TargetMask ? &d3d12ClearValue : nullptr, 
			__uuidof(ID3D12Resource), d3d12Resource.GetVV());
		if (FAILED(Result))
		{
			LogError(u8"ID3D12Device::CreateCommittedResource failed, {}"V, ErrorDescription(Result));
		}

		FAllocateInfo AllocateInfo = D3D12Devcie.GetAllocateInfo(ResourceDesc);
		Location = { 0, AllocateInfo.Size, AllocateInfo.Size, AllocateInfo.Alignment };

		if (ResourceDesc.Usage & EResourceUsage::HostMask)
		{
			bytep MappedAddress = nullptr;
			Result = d3d12Resource->Map(0, nullptr, (void **)(&MappedAddress));
			if (FAILED(Result))
			{
				LogError(u8" ID3D12Resource::Map failed, {}"V, ErrorDescription(Result));
			}
			else
				BaseAddress.CPUAddress = MappedAddress;
		}
		if (ResourceDesc.Usage & EResourceUsage::BufferMask)
			BaseAddress.GPUAddress = d3d12Resource->GetGPUVirtualAddress();

		CreateResourceViews();
	}

	void FD3D12Resource::CreateReservedResource()
	{
		D3D12_RESOURCE_DESC D3D12ResourceDesc = FromResourceDesc(ResourceDesc, 0);
		D3D12ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE;
		D3D12_CLEAR_VALUE d3d12ClearValue = FromClearValue(ResourceDesc.ClearValue, ResourceDesc.Format, ResourceDesc.Usage);
		HRESULT Result = D3D12Devcie.d3d12Device->CreateReservedResource(&D3D12ResourceDesc, StatesBefore, 
			ResourceDesc.Usage & EResourceUsage::TargetMask ? &d3d12ClearValue : nullptr, 
			__uuidof(ID3D12Resource), d3d12Resource.GetVV());
		CreateResourceViews();
	}

	void FD3D12Resource::CreateResource()
	{
		Memory.Instance->SetBlockResource(Memory.Location.Offset, this);

		FD3D12Heap & D3D12Heap = StaticCastRef<FD3D12Heap>(Memory.PlaceHeap);

		if (D3D12Heap.d3d12Resource)
		{
			d3d12Resource = D3D12Heap.d3d12Resource;
			BaseAddress = D3D12Heap.BaseAddress;
			Location = Memory.Location;
		}
		else
		{
			D3D12_RESOURCE_DESC D3D12ResourceDesc = FromResourceDesc(ResourceDesc, Memory.Location.Alignment);
			D3D12_CLEAR_VALUE d3d12ClearValue = FromClearValue(ResourceDesc.ClearValue, ResourceDesc.Format, ResourceDesc.Usage);

			HRESULT Result = D3D12Devcie.d3d12Device->CreatePlacedResource(
				StaticCastRef<FD3D12Heap>(Memory.PlaceHeap).d3d12Heap, Memory.Location.Offset, &D3D12ResourceDesc, StatesBefore,
				ResourceDesc.Usage & EResourceUsage::TargetMask ? &d3d12ClearValue : nullptr,
				__uuidof(ID3D12Resource), d3d12Resource.GetVV());
			if (FAILED(Result))
			{
				LogError(u8" ID3D12Device::CreatePlacedResource failed, {}"V, ErrorDescription(Result));
				return;
			}

			Location = { 0, Memory.Location.Size, Memory.Location.BlockSize };
			if (ResourceDesc.Usage & EResourceUsage::HostMask)
			{
				bytep MappedAddress = nullptr;
				Result = d3d12Resource->Map(0, nullptr, (void **)(&MappedAddress));
				if (FAILED(Result))
				{
					LogError(u8" ID3D12Resource::Map failed, {}"V, ErrorDescription(Result));
				}
				else
					BaseAddress.CPUAddress = MappedAddress;
				BaseAddress.GPUAddress = d3d12Resource->GetGPUVirtualAddress();
			}
			else
			{
				if (ResourceDesc.Usage & EResourceUsage::BufferMask)
					BaseAddress.GPUAddress = d3d12Resource->GetGPUVirtualAddress();
			}
		}
	}

	void FD3D12Resource::CreateResourceViews()
	{
		if (!d3d12Resource)
			return;

		ResourceAccess = InitialAccess;
		ResourceView = new FD3D12ResourceView(*this);
		if (ResourceDesc.Layers > 1)
		{
			SubresourceViews.Resize(ResourceDesc.Layers);
			for (uint32 LayerIndex = 0; LayerIndex < ResourceDesc.Layers; ++LayerIndex)
			{
				SubresourceViews[LayerIndex] = new FD3D12ResourceView(*this, LayerIndex);
			}
		}
	}

	D3D12_RESOURCE_STATES FD3D12Resource::GetResourceDefaultState(const FResourceDesc & ResourceDesc)
	{
		if (ResourceDesc.Usage & EResourceUsage::HostMask)
			return D3D12_RESOURCE_STATE_GENERIC_READ;

		if (ResourceDesc.Dimension == EResourceDimension::Buffer)
			return D3D12_RESOURCE_STATE_COMMON;
		else if (ResourceDesc.Usage & EResourceUsage::Interoperable)
			return D3D12_RESOURCE_STATE_COMMON;
		else
		{
			if (ResourceDesc.Usage & EResourceUsage::RenderTarget)
				return D3D12_RESOURCE_STATE_RENDER_TARGET;
			if (ResourceDesc.Usage & EResourceUsage::DepthStencil)
				return D3D12_RESOURCE_STATE_DEPTH_WRITE;
			return D3D12_RESOURCE_STATE_COMMON;
		}
	}
}
