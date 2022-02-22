#pragma once

#include "D3D12Types.h"
#include "D3D12Heap.h"
#include "D3D12ShaderCompiler.h"
#include "D3D12CommandAllocator.h"

#if __has_include(<nvapi/Include/nvapi.h>)
#define XIN_D3D12_NVAPI 1
#else
#define XIN_D3D12_NVAPI 0
#endif

namespace Xin::RHI::D3D12
{
	class RHID3D12_API FD3D12Device : public IRHIDevice
	{
	public:
		FD3D12Device(IDXGIFactory3 * DXGIFactory3, IDXGIAdapter * DXGIAdapter, ECreateDeviceFlag Flags);
		~FD3D12Device();

		void Initialize(ECreateDeviceFlag Flags);

	public:
		void SetDebugName(FStringV Label) override;

		EDeviceRHI GetDeviceRHI() const override { return EDeviceRHI::D3D12; }
		ETextureCompression GetTextureCompression() const { return ETextureCompression::DXTC; }

		IRHIShaderCompiler & GetShaderCompiler() override { return D3D12Compiler; }

		IRHIFenceRef CreateFence() override;
		IRHIShaderModuleRef CreateShaderModule(IShaderInstance & ShaderInstance) override;
		IRHIPipelineStateRef CreatePipelineState(const FPipelineStateDesc & PipelineStateDesc) override;

		IRHICommandListRef CreateCommandList(ECommandType CommandType) override;
		IRHICommandQueueRef CreateCommandQueue(const FCommandQueueDesc & CommandQueueDesc) override;

		IRHIViewportRef CreateViewport(IRHICommandQueue * CommandQueue, voidp WindowHandle, SizeU Size, uint32 BackBufferCount,
			EFormat Format = EFormat::RGBA, EPresentMode SwapEffect = EPresentMode::FlipDiscard, FMSAA MSAA = {}) override;

		IRHIHeapRef CreateHeap(const FHeapDesc & HeapDesc) override;

		IRHIResourceRef CreateCommitedResource(const FResourceDesc & ResourceDesc) override;
		IRHIResourceRef CreateReservedResource(const FResourceDesc & ResourceDesc) override;
		IRHIResourceRef CreatePlacedResource(const FResourceDesc & ResourceDesc, const FRHIMemory & Memory) override;
		IRHIResourceRef CreatePlacedResource(const FResourceDesc & ResourceDesc, IHeapAllocator & HeapAllocator) override;
		IRHIResourceViewRef CreateResourceView(IRHIResource * RHIResource, const FResourceViewDesc & ResourceViewDesc) override;
		IRHIIndirectSignatureRef CreateIndirectSignature(const FIndirectSignature & IndirectSignature, IRHIPipelineState * RHIPipelineState) override;

	public:
		void ClearContext() override;
		void UpdateContext() override;

	public:
		uint64 GetMemoryUsage() const override;

	public:
		EResourceAccess GetForamtAccess(EFormat Format) const override;
		FAllocateInfo GetAllocateInfo(const FResourceDesc & ResourceDesc, bool LocatedResource = false) const override;
		// return RequiredSize;
		uint64 GetCopyableFootprints(uint64 IntermediateOffset, const FResourceDesc & ResourceDesc, TSlice<FCopyableFootprint> CopyableFootprints, uint32 SubresourceIndex = 0) const;

	private:
		class FHashedShaderParameters
		{
		public:
			uintx HashCode = 0;
			FD3D12ShaderParameters ShaderParameters;

			FHashedShaderParameters() = default;
			FHashedShaderParameters(const FD3D12ShaderParameters & ShaderParameters) :HashCode(Hash(ShaderParameters)), ShaderParameters(ShaderParameters) {}

			bool operator ==(const FHashedShaderParameters & Another) const { return HashCode == Another.HashCode; }
			auto operator <=>(const FHashedShaderParameters & Another) const noexcept { return HashCode <=> Another.HashCode; }
			friend uintx Hash(const FHashedShaderParameters & Index) { return Index.HashCode; }
		};
		mutable TMap<FHashedShaderParameters, ID3D12RootSignatureRef> RootSignatureCache;

	public:
		ID3D12RootSignatureRef FetchRootSinature(const FD3D12ShaderParameters & ShaderParameters) const;

	public:
		FD3D12CommandAllocatorRef AquireCommandAllocator(ECommandType CommandType);
		void FreeCommandAllocator(FD3D12CommandAllocator * CommandAllocator, ECommandType CommandType);
		TArray<TList<FD3D12CommandAllocatorRef>, ECommandTypeCount> FreeCommandAllocators;
		uint32 CommandAllocatorIndex = 0;

	public:
		FD3D12DescriptorHeapRef AquireOnlineDescriptorHeap();
		void FreeOnlineDescriptorHeap(FD3D12DescriptorHeapRef DescriptorHeap);

		TList<FD3D12DescriptorHeapRef> FreeOnlineDescriptorHeaps;

	public:
		void AllocateOfflineDescriptors(EResourceAspect ResourceView, FD3D12DescriptorLocation & DescriptorLocation, uint32 DescriptorCount);
		FD3D12OfflineDescriptorAllocator DescriptorAllocatorCommon;
		FD3D12OfflineDescriptorAllocator DescriptorAllocatorRTV;
		FD3D12OfflineDescriptorAllocator DescriptorAllocatorDSV;

	private:
		uint32 OnlineDescriptorHeapIndex = 0;

	public:
		IDXGIFactory3Ref dxgiFactory3;
		IDXGIAdapterRef dxgiAdapter;

		ID3D12DeviceRef d3d12Device;
#if XIN_DEBUG
		TReferPtr<ID3D12InfoQueue> d3d12InfoQueue;
		TReferPtr<ID3D12InfoQueue1> d3d12InfoQueue1;
		DWORD d3d12MessageCallbackCookie = 0;
#endif

		FD3D12ShaderCompiler D3D12Compiler;

		FString DeviceName;
		uint64 VendorId = 0;
		uint64 DeviceId = 0;

		bool IsNvidiaDevice() const { return VendorId == 0x10DE; }

		uint32 CommomResourceIncrement = 0;

		D3D12_FEATURE_DATA_SHADER_MODEL ShaderModelFeature = { D3D_SHADER_MODEL_6_0 };
		D3D12_FEATURE_DATA_D3D12_OPTIONS D3D12Options = {};
		D3D12_FEATURE_DATA_D3D12_OPTIONS1 D3D12Options1 = {};
		bool FeatureOptions7 = false;
		D3D12_FEATURE_DATA_D3D12_OPTIONS7 FeatureDataOptions7 = {};
		D3D12_FEATURE_DATA_ROOT_SIGNATURE RootSignatureFeature = {};
	};
	using FD3D12DeviceRef = TReferPtr<FD3D12Device>;
}
