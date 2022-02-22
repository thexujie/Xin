#pragma once

#include "D3D12Types.h"
#include "D3D12Heap.h"

namespace Xin::RHI::D3D12
{
	class FD3D12Device;
	class FD3D12Resource;

	/**
	 * @link https://darkcorners.dev/buffers-vs-structuredbuffers
	 */
	class RHID3D12_API FD3D12ResourceView : public IRHIResourceView
	{
	public:
		FD3D12ResourceView(FD3D12Resource & D3D12Resource, uint32 SubresourceIndex = NullIndex);
		FD3D12ResourceView(FD3D12Resource & D3D12Resource, const FResourceViewDesc & ResourceViewDesc);
		~FD3D12ResourceView();
		IRHIResource * GetResource() const override;

	private:
		void CreateResourceViewInnner_SRV();
		void CreateResourceViewInnner_RTV();
		void CreateResourceViewInnner_DSV();
		void CreateResourceViewInnner_UAV();

	public:
		FD3D12Resource & D3D12Resource;

		FD3D12DescriptorLocation SRV;
		FD3D12DescriptorLocation RTV;
		FD3D12DescriptorLocation DSV;
		FD3D12DescriptorLocation UAV;
	};
	using FD3D12ResourceViewRef = TReferPtr<FD3D12ResourceView>;
}
