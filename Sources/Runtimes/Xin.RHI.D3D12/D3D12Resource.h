#pragma once

#include "D3D12Types.h"
#include "D3D12ResourceView.h"
#include "D3D12Heap.h"

namespace Xin::RHI::D3D12
{
	class ID3D12HeapInstance;
	class FD3D12ResourceView;

	class RHID3D12_API FD3D12Resource : public IRHIResource
	{
	public:
		FD3D12Resource(FD3D12Device & D3D12Devcie, const FResourceDesc & ResourceDesc);
		FD3D12Resource(FD3D12Device & D3D12Devcie, ID3D12Resource * d3d12Resource, EResourceAccess InitialAccess);
		~FD3D12Resource();
		void SetDebugName(FStringV Name) override;

	public:
		forceinline IRHIResourceView * GetResourceView(uint32 ArrayIndex = NullIndex) const final { return ArrayIndex == NullIndex ? ResourceView : SubresourceViews[ArrayIndex]; }
		forceinline void * GetSharedHandle() const final { return d3d12Resource.Get(); }

	public:
		/**
		 * @link https://learn.microsoft.com/en-us/windows/win32/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12#implicit-state-transitions
		 * @param StateAfter 
		 * @return 
		 */
		bool IsImplicitStateTransition(EResourceAccess AccessAfter) const;
		bool IsImplicitStateTransition(D3D12_RESOURCE_STATES StateAfter) const;

	public:
		void CreateResource(IHeapAllocator & HeapAllocator);
		void CreateResource(const FRHIMemory & Memory);
		void CreateCommitedResource();
		void CreateReservedResource();
		void CreateResource();
		void CreateResourceViews();

	public:
		static D3D12_RESOURCE_STATES GetResourceDefaultState(const FResourceDesc & ResourceDesc);

	public:
		FD3D12Device & D3D12Devcie;

		EResourceAccess InitialAccess = EResourceAccess::Common;
		EResourceAccess ResourceAccess = EResourceAccess::None;

		D3D12_RESOURCE_STATES StatesBefore = D3D12_RESOURCE_STATE_COMMON;
		D3D12_RESOURCE_STATES StatesAfter = D3D12_RESOURCE_STATE_COMMON;

		ID3D12ResourceRef d3d12Resource;

	public:
		FD3D12ResourceViewRef ResourceView;
		TList<FD3D12ResourceViewRef> SubresourceViews;
	};
	using FD3D12ResourceRef = TReferPtr<FD3D12Resource>;
}
