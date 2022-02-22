#include "PCH.h"
#include "D3D12Device.h"
#include "D3D12Heap.h"
#include "D3D12CommandQueue.h"
#include "D3D12PipelineState.h"
#include "D3D12ShaderModule.h"
#include "D3D12Viewport.h"

#if XIN_D3D12_NVAPI
#pragma warning(disable: 4819)
#include <nvapi/Include/nvapi.h>
#pragma comment(lib, "nvapi\\Lib\\" XIN_PLATFORM "\\nvapi64.lib")
#endif

namespace Xin::RHI::D3D12
{
	// Note that Windows 10 Creator Update SDK is required for enabling Shader Model 6 feature.
	static HRESULT EnableExperimentalShaderModels() {
		static constexpr GUID D3D12ExperimentalShaderModelsID = { /* 76f5573e-f13a-40f5-b297-81ce9e18933f */
			0x76f5573e,
			0xf13a,
			0x40f5,
			{ 0xb2, 0x97, 0x81, 0xce, 0x9e, 0x18, 0x93, 0x3f }
		};

		return ::D3D12EnableExperimentalFeatures(1, &D3D12ExperimentalShaderModelsID, nullptr, nullptr);
	}

	void __stdcall D3D12DeviceMessageCallBack(D3D12_MESSAGE_CATEGORY Category, D3D12_MESSAGE_SEVERITY Severity, D3D12_MESSAGE_ID ID, LPCSTR pDescription, void *pContext)
	{
		if (Severity == D3D12_MESSAGE_SEVERITY_ERROR)
		{
			LogError(FEncoding::AsUtf8(pDescription));
		}
	}

	static FD3D12OfflineDescriptorAllocatorDesc DescriptorAllcaotorCommonConfiguration =
	{
		.HeapSize = 1024,
		.Type = EDescriptorHeapType::ShaderResource,
	};

	static FD3D12OfflineDescriptorAllocatorDesc DescriptorAllcaotorRTVConfiguration =
	{
		.HeapSize = 256,
		.Type = EDescriptorHeapType::RenderTarget,
	};

	static FD3D12OfflineDescriptorAllocatorDesc DescriptorAllcaotorDSVConfiguration =
	{
		.HeapSize = 256,
		.Type = EDescriptorHeapType::DepthStencil,
	};

	FD3D12Device::FD3D12Device(IDXGIFactory3 * DXGIFactory3, IDXGIAdapter * DXGIAdapter, ECreateDeviceFlag Flags)
		: dxgiFactory3(DXGIFactory3), dxgiAdapter(DXGIAdapter), D3D12Compiler(*this)
		, DescriptorAllocatorCommon(*this, DescriptorAllcaotorCommonConfiguration)
		, DescriptorAllocatorRTV(*this, DescriptorAllcaotorRTVConfiguration)
		, DescriptorAllocatorDSV(*this, DescriptorAllcaotorDSVConfiguration)
	{
		DXGI_ADAPTER_DESC DXGIAdapterDesc = {};
		DXGIAdapter->GetDesc(&DXGIAdapterDesc);

		DeviceName = FEncoding::ToUtf8(DXGIAdapterDesc.Description);
		VendorId = DXGIAdapterDesc.VendorId;
		DeviceId = DXGIAdapterDesc.DeviceId;

		Initialize(Flags);
	}

	FD3D12Device::~FD3D12Device()
	{
#if XIN_DEBUG
		if (d3d12MessageCallbackCookie)
		{
			if (d3d12InfoQueue1)
				d3d12InfoQueue1->UnregisterMessageCallback(d3d12MessageCallbackCookie);
			d3d12MessageCallbackCookie = 0;
		}
#endif
	}

	void FD3D12Device::Initialize(ECreateDeviceFlag Flags)
	{
		HRESULT Result = S_OK;
#if XIN_DEBUG
		TReferPtr<ID3D12Debug1> d3d12Debug1;
		Result = D3D12GetDebugInterface(IID_PPV_ARGS(d3d12Debug1.GetPP()));
		if (SUCCEEDED(Result))
		{
			d3d12Debug1->EnableDebugLayer();
		}
#endif

		//Result = EnableExperimentalShaderModels();
		//if (FAILED(Result))
		//{
		//	LogError(u8"EnableExperimentalShaderModels faild, {}"V, ErrorDescription(GetLastError()));
		//	return;
		//}

		Result = D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), d3d12Device.GetVV());
		if (FAILED(Result))
		{
			LogWarning(u8" D3D12CreateDevice failed: {}"V, ErrorDescription(Result & 0xFFFF));
			return;
		}

#if XIN_DEBUG
		d3d12Device->QueryInterface(d3d12InfoQueue1.GetPP());
		if (d3d12InfoQueue1)
		{
			d3d12InfoQueue1->RegisterMessageCallback(D3D12DeviceMessageCallBack, D3D12_MESSAGE_CALLBACK_FLAG_NONE, nullptr, &d3d12MessageCallbackCookie);
		}
		else
		{
			d3d12Device->QueryInterface(d3d12InfoQueue.GetPP());
			if (d3d12InfoQueue)
			{
				d3d12InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			}
		}
#endif

		Result = d3d12Device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &ShaderModelFeature, sizeof(ShaderModelFeature));
		Result = d3d12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &D3D12Options, sizeof(D3D12Options));
		Result = d3d12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &D3D12Options1, sizeof(D3D12Options1));
		FeatureOptions7 = d3d12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &FeatureDataOptions7, sizeof(FeatureDataOptions7)) == S_OK;

		// This is the highest version we currently support. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
		RootSignatureFeature.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		if (FAILED(d3d12Device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &RootSignatureFeature, sizeof(RootSignatureFeature))))
		{
			RootSignatureFeature.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}

		CommomResourceIncrement = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

#if XIN_D3D12_NVAPI
		NvAPI_Initialize();
#endif
	}

	void FD3D12Device::SetDebugName(FStringV Name)
	{
		IRHIDevice::SetDebugName(Name);
		DescriptorAllocatorCommon.SetDebugName(Format(u8"{}.OfflineDescriptorHeapCommon"V, Name));
		DescriptorAllocatorRTV.SetDebugName(Format(u8"{}.OfflineDescriptorHeapRTV"V, Name));
		DescriptorAllocatorDSV.SetDebugName(Format(u8"{}.OfflineDescriptorHeapDSV"V, Name));
	}

	IRHIFenceRef FD3D12Device::CreateFence()
	{
		return new FD3D12Fence(*this);
	}

	IRHIShaderModuleRef FD3D12Device::CreateShaderModule(IShaderInstance & ShaderInstance)
	{
		return new FD3D12ShaderModule(*this, ShaderInstance);
	}

	IRHICommandQueueRef FD3D12Device::CreateCommandQueue(const FCommandQueueDesc & CommandQueueDesc)
	{
		return new FD3D12CommandQueue(*this, CommandQueueDesc);
	}

	IRHICommandListRef FD3D12Device::CreateCommandList(ECommandType CommandType)
	{
		return new FD3D12CommandList(*this, CommandType);

	}


	IRHIViewportRef FD3D12Device::CreateViewport(IRHICommandQueue * CommandQueue, voidp WindowHandle, SizeU Size, uint32 BackBufferCount, EFormat Format, EPresentMode SwapEffect, FMSAA MSAA)
	{
		if (Size == SizeU::Zero)
			Size = Windows::GetClientSize(HWND(WindowHandle));

		FD3D12CommandQueue & D3D12CommandQueue = StaticCastRef<FD3D12CommandQueue>(CommandQueue);
		return new FD3D12Viewport(D3D12CommandQueue, WindowHandle, Size, BackBufferCount, Format, SwapEffect, MSAA);
	}

	IRHIHeapRef FD3D12Device::CreateHeap(const FHeapDesc & HeapDesc)
	{
		return new FD3D12Heap(*this, HeapDesc);
	}

	IRHIPipelineStateRef FD3D12Device::CreatePipelineState(const FPipelineStateDesc & PipelineStateDesc)
	{
		return new FD3D12PipelineState(*this, PipelineStateDesc);
	}

	IRHIResourceRef FD3D12Device::CreateCommitedResource(const FResourceDesc & ResourceDesc)
	{
		FD3D12ResourceRef D3D12Resource = new FD3D12Resource(*this, ResourceDesc);
		D3D12Resource->CreateCommitedResource();
		return D3D12Resource;
	}

	IRHIResourceRef FD3D12Device::CreateReservedResource(const FResourceDesc & ResourceDesc)
	{
		FD3D12ResourceRef D3D12Resource = new FD3D12Resource(*this, ResourceDesc);
		D3D12Resource->CreateReservedResource();
		return D3D12Resource;
	}

	IRHIResourceRef FD3D12Device::CreatePlacedResource(const FResourceDesc & ResourceDesc, const FRHIMemory & Memory)
	{
		FD3D12ResourceRef D3D12Resource = new FD3D12Resource(*this, ResourceDesc);
		D3D12Resource->CreateResource(Memory);
		return D3D12Resource;
	}

	IRHIResourceRef FD3D12Device::CreatePlacedResource(const FResourceDesc & ResourceDesc, IHeapAllocator & HeapAllocator)
	{
		FD3D12ResourceRef D3D12Resource = new FD3D12Resource(*this, ResourceDesc);
		D3D12Resource->CreateResource(HeapAllocator);
		return D3D12Resource;
	}

	IRHIResourceViewRef FD3D12Device::CreateResourceView(IRHIResource * RHIResource, const FResourceViewDesc & ResourceViewDesc)
	{
		return new FD3D12ResourceView(StaticCastRef<FD3D12Resource>(RHIResource), ResourceViewDesc);
	}

	IRHIIndirectSignatureRef FD3D12Device::CreateIndirectSignature(const FIndirectSignature & IndirectSignature, IRHIPipelineState * RHIPipelineState)
	{
		return new FD3D12IndirectSignature(*this, IndirectSignature, StaticCast<FD3D12PipelineState>(RHIPipelineState));
	}

	void FD3D12Device::ClearContext()
	{
		FreeCommandAllocators[ECommandType::Direct].Clear();
	}

	void FD3D12Device::UpdateContext()
	{
	}

	uint64 FD3D12Device::GetMemoryUsage() const
	{
		TReferPtr<IDXGIAdapter3> DXGIAdapter3 = dxgiAdapter.QueryInterface<IDXGIAdapter3>();
		DXGI_QUERY_VIDEO_MEMORY_INFO DXGIQueryVideoMemoryInfo;
		DXGIAdapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &DXGIQueryVideoMemoryInfo);
		return DXGIQueryVideoMemoryInfo.CurrentUsage;
	}

	EResourceAccess FD3D12Device::GetForamtAccess(EFormat Format) const
	{
		D3D12_FEATURE_DATA_FORMAT_SUPPORT FeatureDataFormatSupport;
		FeatureDataFormatSupport.Format = FromFormat(Format);
		d3d12Device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &FeatureDataFormatSupport, sizeof(D3D12_FEATURE_DATA_FORMAT_SUPPORT));

		EResourceAccess ResourceAccess = EResourceAccess::None;
		SetFlags(ResourceAccess, EResourceAccess::RenderTarget, FeatureDataFormatSupport.Support1 & D3D12_FORMAT_SUPPORT1_RENDER_TARGET);
		SetFlags(ResourceAccess, EResourceAccess::DepthStencil, FeatureDataFormatSupport.Support1 & D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL);
		return ResourceAccess;
	}

	FAllocateInfo FD3D12Device::GetAllocateInfo(const FResourceDesc & ResourceDesc, bool LocatedResource) const
	{
		AssertExpr(ResourceDesc.Size.Width > 0);
		AssertExpr(ResourceDesc.Size.Height > 0);
		AssertExpr(ResourceDesc.Mipmaps > 0);
		AssertExpr(ResourceDesc.Layers > 0);

		D3D12_RESOURCE_ALLOCATION_INFO D3DResourceAllocateInfo;
		D3D12_RESOURCE_DESC D3D12ResourceDesc = FromResourceDesc(ResourceDesc, 0);
		if (!LocatedResource
			&& ResourceDesc.Dimension == EResourceDimension::Texture2D
			&& !AnyFlags(ResourceDesc.Usage, EResourceUsage::RenderTarget | EResourceUsage::DepthStencil)
			&& GetTilingInfo4K(ResourceDesc.Format, ResourceDesc.Size).TotalBytes <= D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT)
		{
			// Try to use small resource.
			D3D12ResourceDesc.Alignment = ResourceDesc.MSAA.Level ? D3D12_SMALL_MSAA_RESOURCE_PLACEMENT_ALIGNMENT : D3D12_SMALL_RESOURCE_PLACEMENT_ALIGNMENT;
			D3DResourceAllocateInfo = d3d12Device->GetResourceAllocationInfo(0, 1, &D3D12ResourceDesc);
			if (D3DResourceAllocateInfo.Alignment != D3D12ResourceDesc.Alignment)
			{
				D3D12ResourceDesc.Alignment = 0;
				D3DResourceAllocateInfo = d3d12Device->GetResourceAllocationInfo(0, 1, &D3D12ResourceDesc);
			}
		}
		else
			D3DResourceAllocateInfo = d3d12Device->GetResourceAllocationInfo(0, 1, &D3D12ResourceDesc);
		FAllocateInfo ResourceAllocateInfo;
		ResourceAllocateInfo.Size = D3DResourceAllocateInfo.SizeInBytes;
		ResourceAllocateInfo.Alignment = uint32(D3DResourceAllocateInfo.Alignment);
		return ResourceAllocateInfo;
	}

	uint64 FD3D12Device::GetCopyableFootprints(uint64 IntermediateOffset, const FResourceDesc & ResourceDesc, TSlice<FCopyableFootprint> CopyableFootprints, uint32 SubresourceIndex) const
	{
		constexpr uint32 MaxSubresources = 64;
		AssertExpr(CopyableFootprints.Size <= MaxSubresources);

		D3D12_RESOURCE_DESC D3D12ResourceDesc = FromResourceDesc(ResourceDesc, 0);

		TArray<D3D12_PLACED_SUBRESOURCE_FOOTPRINT, MaxSubresources> D3D12PlacedSubresourceFootPrints {};
		TArray<uint64, MaxSubresources> RowPitchs {};
		TArray<uint32, MaxSubresources> RowCounts {};
		uint64 RequiredSize = 0;

		uint32 SubresourceCount = Clamp<uint32>(ResourceDesc.Mipmaps * ResourceDesc.Layers, 1, uint32(CopyableFootprints.Size));
		d3d12Device->GetCopyableFootprints(&D3D12ResourceDesc, SubresourceIndex, SubresourceCount, IntermediateOffset, D3D12PlacedSubresourceFootPrints.Data, RowCounts.Data, RowPitchs.Data, &RequiredSize);
		for (uint32 FootprintIndex = 0; FootprintIndex < SubresourceCount; ++FootprintIndex)
		{
			const D3D12_PLACED_SUBRESOURCE_FOOTPRINT & D3D12PlacedSubresourceFootPrint = D3D12PlacedSubresourceFootPrints[FootprintIndex];
			//AssertExpresion(D3D12PlacedSubresourceFootPrint.Footprint.RowPitch == RowPitchs[FootprintIndex]); !!!
			FCopyableFootprint & CopyableFootprint = CopyableFootprints[FootprintIndex];
			CopyableFootprint.Format = ToFormat(D3D12PlacedSubresourceFootPrint.Footprint.Format);
			CopyableFootprint.Offset = D3D12PlacedSubresourceFootPrint.Offset;
			CopyableFootprint.Size = { D3D12PlacedSubresourceFootPrint.Footprint.Width, D3D12PlacedSubresourceFootPrint.Footprint.Height };
			CopyableFootprint.Depth = D3D12PlacedSubresourceFootPrint.Footprint.Depth;
			//CopyableFootprint.RowPitch = D3D12PlacedSubresourceFootPrint.Footprint.RowPitch;
			CopyableFootprint.RowPitch = D3D12PlacedSubresourceFootPrint.Footprint.RowPitch;
			CopyableFootprint.RowCount = RowCounts[FootprintIndex];
		}
		return RequiredSize;
	}

	FD3D12CommandAllocatorRef FD3D12Device::AquireCommandAllocator(ECommandType CommandType)
	{
		if (FreeCommandAllocators[CommandType].Size)
			return FreeCommandAllocators[CommandType].PopBack();

		FD3D12CommandAllocatorRef CommandAllocator = new FD3D12CommandAllocator(*this);
		CommandAllocator->SetDebugName(u8"CommandAllocator[{}]"V.Format(CommandAllocatorIndex++));
		return CommandAllocator;
	}

	void FD3D12Device::FreeCommandAllocator(FD3D12CommandAllocator * CommandAllocator, ECommandType CommandType)
	{
		CommandAllocator->ResetAllocator();
		FreeCommandAllocators[CommandType].Add(CommandAllocator);
	}

	FD3D12DescriptorHeapRef FD3D12Device::AquireOnlineDescriptorHeap()
	{
		if (FreeOnlineDescriptorHeaps.Size)
			return FreeOnlineDescriptorHeaps.PopBack();

		constexpr uint32 DefaultHeapSize = 2048;

		FDescriptorHeapDesc DescriptorHeapDesc;
		DescriptorHeapDesc.Type = EDescriptorHeapType::ShaderResource;
		DescriptorHeapDesc.Size = DefaultHeapSize;
		DescriptorHeapDesc.Flags = EDescriptorHeapFlag::ShaderVisible;
		FD3D12DescriptorHeapRef D3D12DescriptorHeap = new FD3D12DescriptorHeap(*this, DescriptorHeapDesc);
		D3D12DescriptorHeap->SetDebugName(Format(u8"{}.OnlineDescriptorHeap.{}"V, DebugName, OnlineDescriptorHeapIndex++));
		return D3D12DescriptorHeap;
	}

	void FD3D12Device::FreeOnlineDescriptorHeap(FD3D12DescriptorHeapRef DescriptorHeap)
	{
		DescriptorHeap->ResetAllocate();
		FreeOnlineDescriptorHeaps.Add(DescriptorHeap);
	}

	void FD3D12Device::AllocateOfflineDescriptors(EResourceAspect ResourceView, FD3D12DescriptorLocation & DescriptorLocation, uint32 DescriptorCount)
	{
		switch (ResourceView)
		{
		case EResourceAspect::ShaderResource:
		case EResourceAspect::TexelAccess:
			DescriptorAllocatorCommon.Allocate(DescriptorLocation, DescriptorCount);
			break;
		case EResourceAspect::RenderTarget:
			DescriptorAllocatorRTV.Allocate(DescriptorLocation, DescriptorCount);
			break;
		case EResourceAspect::DepthStencil:
			DescriptorAllocatorDSV.Allocate(DescriptorLocation, DescriptorCount);
			break;
		default:
			break;
		}
	}

	ID3D12RootSignatureRef FD3D12Device::FetchRootSinature(const FD3D12ShaderParameters & ShaderParameters) const
	{
		FHashedShaderParameters HashedRootSignature { ShaderParameters };

		//HashedRootSignature.RootSignature.Flags = RootSignaureKey.Flags;

		if (auto Iter = RootSignatureCache.Find(HashedRootSignature); Iter != RootSignatureCache.End())
			return Iter->Second.Get();


		const D3D12_DESCRIPTOR_RANGE_FLAGS CBVDescriptorRangeFlags = (D3D12Options.ResourceBindingTier <= D3D12_RESOURCE_BINDING_TIER_2) ?
			D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE :
			D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

		const D3D12_DESCRIPTOR_RANGE_FLAGS SRVDescriptorRangeFlags = (D3D12Options.ResourceBindingTier <= D3D12_RESOURCE_BINDING_TIER_1) ?
			D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE :
			D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

		const D3D12_DESCRIPTOR_RANGE_FLAGS UAVDescriptorRangeFlags = (D3D12Options.ResourceBindingTier <= D3D12_RESOURCE_BINDING_TIER_2) ?
			D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE :
			D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

		const D3D12_DESCRIPTOR_RANGE_FLAGS SamplerDescriptorRangeFlags = (D3D12Options.ResourceBindingTier <= D3D12_RESOURCE_BINDING_TIER_1) ?
			D3D12_DESCRIPTOR_RANGE_FLAG_NONE :
			D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

		TArray<D3D12_DESCRIPTOR_RANGE_FLAGS, DescriptorTypeCount> DescriptorRangeFlags = { CBVDescriptorRangeFlags, SRVDescriptorRangeFlags, UAVDescriptorRangeFlags, SamplerDescriptorRangeFlags };

		//struct FDescriptorRange
		//{
		//	EShaderStage ShaderStage;
		//	D3D12_DESCRIPTOR_RANGE_TYPE RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		//	uint32 RegisterIndex = 0;
		//	uint32 RegisterCount = 0;
		//};

		const D3D12_ROOT_DESCRIPTOR_FLAGS CBVRootDescriptorFlags = D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC;
		const D3D12_ROOT_DESCRIPTOR_FLAGS SRVRootDescriptorFlags = D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC;

		TArray<CD3DX12_DESCRIPTOR_RANGE1, MaxShaderStages> D3D12DescriptorRanges;
		TArray<CD3DX12_ROOT_PARAMETER1, MaxD3D12RootParameters> D3D12RootParameters;

		uint32 NumRootParameters = 0;
		for (uintx RootParameterIndex = 0; RootParameterIndex < ShaderParameters.RootParameters.Size; ++RootParameterIndex, ++NumRootParameters)
		{
			const FD3D12RootParameter & RootParameter = ShaderParameters.RootParameters[RootParameterIndex];
			if (RootParameter.ShaderStage == EShaderStage::None)
				break;

			switch (RootParameter.RootParameterType)
			{
			case D3D12_ROOT_PARAMETER_TYPE_CBV:
			{
				D3D12RootParameters[RootParameterIndex].InitAsConstantBufferView
				(
					RootParameter.RegisterIndex,
					RootParameter.RegisterSpace,
					CBVRootDescriptorFlags,
					FromShaderRelevance(ShaderRelevanceFormShaderStage(RootParameter.ShaderStage))
				);
				break;
			}
			case D3D12_ROOT_PARAMETER_TYPE_SRV:
			{
				AssertFailed(u8"SRV or UAV root descriptors can only be Raw or Structured buffers."V);
				break;
			}
			case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
			{
				const FD3D12ParameterRange & ResourceRange = ShaderParameters.ParameterRanges[RootParameter.RangeIndex];

				D3D12DescriptorRanges[RootParameter.RangeIndex].Init(ResourceRange.ParameterType, ResourceRange.GetNumShaderResources(), ResourceRange.GetBaseRegisterIndex(), 0, SRVDescriptorRangeFlags, 0);

				D3D12RootParameters[RootParameterIndex].InitAsDescriptorTable
				(
					1,
					D3D12DescriptorRanges.Data + RootParameter.RangeIndex,
					FromShaderRelevance(ShaderRelevanceFormShaderStage(EShaderStage(RootParameter.ShaderStage)))
				);
				break;
			}
			default:
				break;
			}
		}


		//const D3D12_STATIC_SAMPLER_DESC StaticSamplerDescs[] =
		//{
		//	MakeStaticSampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_SSRI_PointWrap, 1000),
		//	MakeStaticSampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_SSRI_PointClamp, 1000),
		//	MakeStaticSampler(D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_BORDER, D3D12_SSRI_PointBorder, 1000),
		//	MakeStaticSampler(D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_SSRI_LinearWrap, 1000),
		//	MakeStaticSampler(D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_SSRI_LinearClamp, 1000),
		//	MakeStaticSampler(D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_BORDER, D3D12_SSRI_LinearBorder, 1000),
		//	MakeStaticSampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_SSRI_TrilinearWrap, 1000),
		//	MakeStaticSampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_SSRI_TrilinearClamp, 1000),
		//	MakeStaticSampler(D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_BORDER, D3D12_SSRI_TrilinearBorder, 1000),
		//	MakeStaticShadowComparisonSampler(D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		//		RootSignature.Flags & ED3D12RootSignatureFlags::ReverseZ ? D3D12_COMPARISON_FUNC_GREATER_EQUAL : D3D12_COMPARISON_FUNC_LESS_EQUAL, D3D12_SSRI_Shadow, 1000),
		//};

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC RootDesc;
		D3D12_ROOT_SIGNATURE_FLAGS Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
		Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		if (D3D12Options.ResourceBindingTier > D3D12_RESOURCE_BINDING_TIER_1)
		{
			TArray<D3D12_STATIC_SAMPLER_DESC, MaxStaticSamplers> StaticSamplerDescs;
			uint32 NumStaticSamplers = 0;
			for (uintx SamplerIndex = 0; SamplerIndex < MaxStaticSamplers; ++SamplerIndex)
			{
				const FSamplerDesc & SamplerDesc = ShaderParameters.StaticSamplers[SamplerIndex];
				if (SamplerDesc.ShaderRelevance == EShaderRelevance::None)
					break;

				D3D12_STATIC_SAMPLER_DESC & D3D12SamplerDesc = StaticSamplerDescs[SamplerIndex];
				D3D12SamplerDesc.Filter = FromFilter(SamplerDesc.SampleFilter);
				D3D12SamplerDesc.AddressU = FromAddressMode(SamplerDesc.AddressModeU);
				D3D12SamplerDesc.AddressV = FromAddressMode(SamplerDesc.AddressModeV);
				D3D12SamplerDesc.AddressW = FromAddressMode(SamplerDesc.AddressModeW);
				D3D12SamplerDesc.MipLODBias = SamplerDesc.MipLODBias;
				D3D12SamplerDesc.MaxAnisotropy = SamplerDesc.MaxAnisotropy;
				D3D12SamplerDesc.ComparisonFunc = FromComparison(SamplerDesc.Comparison);
				D3D12SamplerDesc.BorderColor = FromBorderColor(SamplerDesc.BorderColor);
				D3D12SamplerDesc.MinLOD = SamplerDesc.MinLOD;
				D3D12SamplerDesc.MaxLOD = SamplerDesc.MaxLOD;
				D3D12SamplerDesc.ShaderRegister = SamplerDesc.BindingIndex;
				D3D12SamplerDesc.RegisterSpace = SamplerDesc.BindingSpace;
				D3D12SamplerDesc.ShaderVisibility = FromShaderRelevance(SamplerDesc.ShaderRelevance);
				++NumStaticSamplers;
			}
			RootDesc.Init_1_1(NumRootParameters, D3D12RootParameters.Data, NumStaticSamplers, StaticSamplerDescs.Data, Flags);
		}
		else
		{
			RootDesc.Init_1_1(NumRootParameters, D3D12RootParameters.Data, 0, nullptr, Flags);
		}

		ID3DBlobRef RootSignatureBlob;
		ID3DBlobRef ErrorBlob;
		HRESULT Result = D3DX12SerializeVersionedRootSignature(&RootDesc, RootSignatureFeature.HighestVersion, RootSignatureBlob.GetPP(), ErrorBlob.GetPP());
		if (SUCCEEDED(Result))
		{
			ID3D12RootSignatureRef D3D12RootSignature;
			Result = d3d12Device->CreateRootSignature(0,
				RootSignatureBlob->GetBufferPointer(),
				RootSignatureBlob->GetBufferSize(),
				IID_PPV_ARGS(D3D12RootSignature.GetPP()));

			if (FAILED(Result))
			{
				LogWarning(u8"ID3D12Device::CreateRootSignature failed."V);
			}

			RootSignatureCache.Insert({ HashedRootSignature, D3D12RootSignature });
			return D3D12RootSignature;
		}
		else
		{
			LogWarning(u8"ID3D12DeviceID3D12Device::CreateRootSignature failed. {0}"V, D3D12BlobMessage(ErrorBlob));
			RootSignatureCache.Insert({ HashedRootSignature, nullptr });
			return nullptr;
		}
	}
}
