#include "PCH.h"
#include "D3D12Heap.h"
#include "D3D12Device.h"

namespace Xin::RHI::D3D12
{
	forceinline D3D12_HEAP_TYPE FromHeapType2(EHeapUsage HeapUsage)
	{
		switch (HeapUsage & EHeapUsage::HostMask)
		{
		case EHeapUsage::HostWrite:
		case EHeapUsage::HostRead:
			return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
		default:
			return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		}
	}

	forceinline D3D12_CPU_PAGE_PROPERTY FromCPUPageProperty2(EHeapUsage HeapUsage)
	{
		if (HeapUsage & EHeapUsage::HostMask)
		{
			switch (HeapUsage & EHeapUsage::HostMask)
			{
			case EHeapUsage::HostRead:
				return D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
			case EHeapUsage::HostWrite:
				return D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE;
			default:
				return D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
			}
		}
		return D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	}

	forceinline D3D12_MEMORY_POOL FromMemoryPool2(EHeapUsage HeapUsage)
	{
		if (HeapUsage & EHeapUsage::HostMask)
		{
			switch (HeapUsage & EHeapUsage::MemoryMask)
			{
			case EHeapUsage::MemorySysRAM:
				return D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
			case EHeapUsage::MemoryVRAM:
				return D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L1;
			default:
				return D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
			}
		}
		return D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	}

	FD3D12Heap::FD3D12Heap(FD3D12Device & Device, const FHeapDesc & HeapDesc) : IRHIHeap(HeapDesc), Device(Device)
	{
		//AssertExpresion(HeapDesc.Usage & EHeapUsage::AllowMask);

		if (HeapDesc.Usage & EHeapUsage::HostMask)
		{
			D3D12_RESOURCE_STATES InitialStates = (HeapDesc.Usage & EHeapUsage::HostWrite) ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COPY_DEST;
			AssertExpr(HeapDesc.Usage & EHeapUsage::AllowBuffers);

			D3D12_HEAP_PROPERTIES HeapProperties { FromHeapType(HeapDesc.Usage) };
			if (HeapProperties.Type == D3D12_HEAP_TYPE_CUSTOM)
			{
				HeapProperties.CPUPageProperty = FromCPUPageProperty(HeapDesc.Usage);
				HeapProperties.MemoryPoolPreference = FromMemoryPool(HeapDesc.Usage);
			}
			else
			{
				HeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
				HeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			}
			HeapProperties.CreationNodeMask = 1;
			HeapProperties.VisibleNodeMask = 1;

			D3D12_RESOURCE_DESC D3D12ResourceDesc;
			D3D12ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			D3D12ResourceDesc.Alignment = 0;
			D3D12ResourceDesc.Width = HeapDesc.Size;
			D3D12ResourceDesc.Height = 1;
			D3D12ResourceDesc.DepthOrArraySize = 1;
			D3D12ResourceDesc.MipLevels = 1;
			D3D12ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			D3D12ResourceDesc.SampleDesc = { 1, 0 };
			D3D12ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			D3D12ResourceDesc.Flags = D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
			HRESULT Result = Device.d3d12Device->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE, &D3D12ResourceDesc, InitialStates, nullptr, __uuidof(ID3D12Resource), d3d12Resource.GetVV());
			if (FAILED(Result))
			{
				LogError(u8"ID3D12Device::CreateCommittedResource failed, {}"V, ErrorDescription(Result));
			}

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
			D3D12_HEAP_DESC D3D12HeapDesc { HeapDesc.Size };
			//D3D12HeapDesc.Properties.Type = D3D12_HEAP_TYPE_CUSTOM;
			D3D12HeapDesc.Properties.Type = FromHeapType(HeapDesc.Usage);
			// D3D12_CPU_PAGE_PROPERTY_UNKNOWN must be used when creating heaps that are not D3D12_HEAP_TYPE_CUSTOM
			if (D3D12HeapDesc.Properties.Type == D3D12_HEAP_TYPE_CUSTOM)
			{
				D3D12HeapDesc.Properties.CPUPageProperty = FromCPUPageProperty(HeapDesc.Usage);
				D3D12HeapDesc.Properties.MemoryPoolPreference = FromMemoryPool(HeapDesc.Usage);
			}
			else
			{
				D3D12HeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
				D3D12HeapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			}
			D3D12HeapDesc.Properties.CreationNodeMask = 1;
			D3D12HeapDesc.Properties.VisibleNodeMask = 1;
			D3D12HeapDesc.Alignment = (HeapDesc.Usage & EHeapUsage::SupportMSAA) ? D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT : D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			D3D12HeapDesc.Flags = FromHeapFlags(HeapDesc.Usage, Device.D3D12Options.ResourceHeapTier, Device.FeatureOptions7);

			HRESULT Result = Device.d3d12Device->CreateHeap(&D3D12HeapDesc, __uuidof(ID3D12Heap), d3d12Heap.GetVV());
			if (FAILED(Result))
			{
				LogError(u8"ID3D12Device::CreateHeap failed, {}"V, ErrorDescription(Result));
			}
			EnsureExpr(d3d12Heap);
		}
	}

	void FD3D12Heap::SetDebugName(FStringV Name)
	{
		IRHIHeap::SetDebugName(Name);
		if (d3d12Heap)
			SetD3D12ObjectName(d3d12Heap, u8"{}.Heap"V.Format(Name));
		if (d3d12Resource)
			SetD3D12ObjectName(d3d12Resource, u8"{}.Resource"V.Format(Name));
	}

	FD3D12DescriptorLocation::~FD3D12DescriptorLocation()
	{
		Reset();
	}

	FD3D12DescriptorLocation::FD3D12DescriptorLocation(FD3D12DescriptorLocation && That) noexcept
		: Allocator(That.Allocator), DescriptorHeap(That.DescriptorHeap)
		, BlockLocation(That.BlockLocation)
		, Address(That.Address)
	{
		That.Detatch();
	}

	FD3D12DescriptorLocation & FD3D12DescriptorLocation::operator =(FD3D12DescriptorLocation && That) noexcept
	{
		Allocator = That.Allocator;
		DescriptorHeap = That.DescriptorHeap;
		BlockLocation = That.BlockLocation;
		Address = That.Address;
		That.Detatch();
		return *this;
	}

	void FD3D12DescriptorLocation::Detatch()
	{
		Allocator = nullptr;
		Reset();
	}

	void FD3D12DescriptorLocation::Reset()
	{
		if (Allocator)
			Allocator->Deallocate(BlockLocation);

		Allocator = nullptr;
		DescriptorHeap = nullptr;
		BlockLocation = { };
		Address = { };
	}

	FD3D12DescriptorHeap::FD3D12DescriptorHeap(FD3D12Device & Device, const FDescriptorHeapDesc & DescriptorHeapDesc)
		: Device(Device), Desc(DescriptorHeapDesc)
	{
		D3D12_DESCRIPTOR_HEAP_DESC D3D12DescriptorHeapDesc;
		D3D12DescriptorHeapDesc.Type = FromDescriptorHeapType(DescriptorHeapDesc.Type);
		D3D12DescriptorHeapDesc.NumDescriptors = DescriptorHeapDesc.Size;
		D3D12DescriptorHeapDesc.Flags = FromDescriptorHeapFlags(DescriptorHeapDesc.Flags);
		D3D12DescriptorHeapDesc.NodeMask = 0;
		HRESULT Result = Device.d3d12Device->CreateDescriptorHeap(&D3D12DescriptorHeapDesc, IID_PPV_ARGS(D3D12DescriptorHeap.GetPP()));
		if (FAILED(Result))
		{
			LogError(u8"ID3D12Device::CreateDescriptorHeap failed, {}"V, ErrorDescription(Result));
		}

		Address.CPUAddress = reinterpret_cast<byte *>(D3D12DescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr);
		if (DescriptorHeapDesc.Flags & EDescriptorHeapFlag::ShaderVisible)
			Address.GPUAddress = D3D12DescriptorHeap->GetGPUDescriptorHandleForHeapStart().ptr;
		else
			Address.GPUAddress = NullAddress;
		HandleSize = Device.d3d12Device->GetDescriptorHandleIncrementSize(D3D12DescriptorHeapDesc.Type);
	}

	void FD3D12DescriptorHeap::SetDebugName(FStringV Name)
	{
		IRHIObject::SetDebugName(Name);
		SetD3D12ObjectName(D3D12DescriptorHeap.Get(), Name);
	}

	FD3D12AddressHandle FD3D12DescriptorHeap::Allocate(uint32 DescriptorCount)
	{
		if (DescriptorIndex + DescriptorCount > Desc.Size)
			return { };

		FD3D12AddressHandle AddressHandle = FD3D12AddressHandle { Address, HandleSize } + DescriptorIndex;
		DescriptorIndex += DescriptorCount;
		return AddressHandle;
	}

	bool FD3D12OfflineDescriptorAllocator::Allocate(FD3D12DescriptorLocation & DescriptorLocation, uint32 DescriptorCount)
	{
		AssertExpr(DescriptorCount <= AllocatorDesc.HeapSize);

		uint32 DescriptorIndex = NullIndex;
		for (uintx Index = 0; Index + DescriptorCount <= DescriptorBits.Size; ++Index)
		{
			bool Found = true;
			for (uintx TestIndex = 0; TestIndex < DescriptorCount; ++TestIndex)
			{
				if (DescriptorBits[Index + TestIndex])
				{
					Found = false;
					break;
				}

				if (Found)
					break;
			}

			if (Found)
			{
				DescriptorIndex = uint32(Index);
				break;
			}
		}

		if (DescriptorIndex == NullIndex)
		{
			FDescriptorHeapDesc DescriptorHeapDesc;
			DescriptorHeapDesc.Type = AllocatorDesc.Type;
			DescriptorHeapDesc.Size = AllocatorDesc.HeapSize;
			FD3D12DescriptorHeapRef DescriptorHeap = new FD3D12DescriptorHeap(D3D12Devcie, DescriptorHeapDesc);
			DescriptorHeap->SetDebugName(Format(u8"{}.{}"V, DebugName, uintx(DescriptorHeaps.Size)));
			DescriptorHeaps.Add(DescriptorHeap);
			DescriptorIndex = uint32(DescriptorHeaps.Size - 1) * AllocatorDesc.HeapSize;
			DescriptorBits.AddN(false, AllocatorDesc.HeapSize);
#if D3D12_HEAP_DEBUG
			BlockObjects.AddN(nullptr, AllocatorDesc.HeapSize);
			BlockObjectsLast.AddN(nullptr, AllocatorDesc.HeapSize);
#endif
		}

		uint32 DescriptorHeapIndex = DescriptorIndex / AllocatorDesc.HeapSize;
		uint32 LocalDescriptorIndex = DescriptorIndex % AllocatorDesc.HeapSize;
		FD3D12DescriptorHeap * DescriptorHeap = DescriptorHeaps[DescriptorHeapIndex];

		DescriptorLocation.Allocator = this;
		DescriptorLocation.DescriptorHeap = DescriptorHeap;
		DescriptorLocation.BlockLocation.Offset = DescriptorIndex;
		DescriptorLocation.BlockLocation.Count = DescriptorCount;
		DescriptorLocation.Address.CPUAddress = DescriptorHeap->Address.CPUAddress + DescriptorHeap->HandleSize * LocalDescriptorIndex;
		DescriptorLocation.Address.GPUAddress = DescriptorHeap->Address.GPUAddress + DescriptorHeap->HandleSize * LocalDescriptorIndex;

		for (uintx TestIndex = 0; TestIndex < DescriptorCount; ++TestIndex)
		{
			AssertExpr(DescriptorBits[DescriptorIndex + TestIndex] == false);
#if D3D12_HEAP_DEBUG
			AssertExpr(BlockObjects[DescriptorIndex + TestIndex] == nullptr);
#endif
		}

		DescriptorBits.SetAt(DescriptorIndex, true, DescriptorCount);
		for (uintx TestIndex = 0; TestIndex < DescriptorCount; ++TestIndex)
			AssertExpr(DescriptorBits[DescriptorIndex + TestIndex] == true);
		return true;
	}

	void FD3D12OfflineDescriptorAllocator::Deallocate(const FD3D12DescriptorBlockLocation & BlockLocation)
	{
#if D3D12_HEAP_DEBUG
		AssertExpr(BlockObjects[BlockLocation.Offset]);
		BlockObjects[BlockLocation.Offset] = nullptr;
#endif
		for (uintx TestIndex = 0; TestIndex < BlockLocation.Count; ++TestIndex)
			AssertExpr(DescriptorBits[BlockLocation.Offset + TestIndex]);
		DescriptorBits.SetAt(BlockLocation.Offset, false, BlockLocation.Count);
	}
}
