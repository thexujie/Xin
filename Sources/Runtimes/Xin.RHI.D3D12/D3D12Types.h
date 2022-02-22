#pragma once

#include "Xin.Core/Xin.Core.h"
#include "Xin.RHI/Xin.RHI.h"
#include "Xin.Core/Platform/Windows/WindowsPrivate.h"

#include "Xin.RHI.D3D12.Config.h"

#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

#include <d3d12.h>
#include <d3d11on12.h>

#if __has_include(<PIX/Include/pix3.h>) && !XIN_PUBLISH
#define XIN_D3D12_PIX 1
#pragma push_macro("PROFILE_BUILD")
#define PROFILE_BUILD
#include <PIX/Include/pix3.h>
#pragma comment(lib, "PIX\\Lib\\" XIN_PLATFORM "\\WinPixEventRuntime.lib")
#pragma pop_macro("PROFILE_BUILD")
#else
#define XIN_D3D12_PIX 0
#endif

namespace Xin::RHI::D3D12
{
	using IDXGISwapChain1Ref = TReferPtr<IDXGISwapChain1>;
	using IDXGISwapChain2Ref = TReferPtr<IDXGISwapChain2>;
	using IDXGISwapChain3Ref = TReferPtr<IDXGISwapChain3>;

	using IDXGIFactoryRef = TReferPtr<IDXGIFactory>;
	using IDXGIFactory1Ref = TReferPtr<IDXGIFactory1>;
	using IDXGIFactory3Ref = TReferPtr<IDXGIFactory3>;

	using IDXGIAdapterRef = TReferPtr<IDXGIAdapter>;
	using IDXGIAdapter1Ref = TReferPtr<IDXGIAdapter1>;

	using IDXGIDeviceRef = TReferPtr<IDXGIDevice>;
	using IDXGISurfaceRef = TReferPtr<IDXGISurface>;

	forceinline DXGI_FORMAT FromFormat(EFormat Format)
	{
		return (DXGI_FORMAT)DXGIFormatFromFormat(Format);
	}

	forceinline EFormat ToFormat(DXGI_FORMAT Format)
	{
		return FormatFromDXGIFormat((EDXGIFormat)Format);
	}
}

namespace Xin::RHI::D3D12
{
	constexpr uint32 MaxD3D12ShaderParameters = 32;
	constexpr uint32 MaxD3D12RootParameters = 32;
	constexpr uint32 MaxD3D12ShaderBindingRanges = 4;

	class FD3D12Device;
	class FD3D12Context;

	using namespace Xin::RHI;
	using namespace Xin::Windows;

	using ID3DBlobRef = TReferPtr<ID3DBlob>;

	using IDXGISwapChainRef = TReferPtr<IDXGISwapChain>;
	using IDXGISwapChain1Ref = TReferPtr<IDXGISwapChain1>;
	using IDXGISwapChain2Ref = TReferPtr<IDXGISwapChain2>;
	using IDXGISwapChain3Ref = TReferPtr<IDXGISwapChain3>;

	using IDXGIFactoryRef = TReferPtr<IDXGIFactory>;
	using IDXGIFactory3Ref = TReferPtr<IDXGIFactory3>;

	using IDXGIAdapterRef = TReferPtr<IDXGIAdapter>;
	using IDXGIAdapter1Ref = TReferPtr<IDXGIAdapter1>;

	using ID3D12FenceRef = TReferPtr<ID3D12Fence>;
	using ID3D12Fence1Ref = TReferPtr<ID3D12Fence1>;
	using ID3D12Device1Ref = TReferPtr<ID3D12Device1>;
	using ID3D12DeviceRef = ID3D12Device1Ref;
	using ID3D11On12DeviceRef = TReferPtr<ID3D11On12Device>;

	using ID3D12ResourceRef = TReferPtr<ID3D12Resource>;
	using ID3D12HeapRef = TReferPtr<ID3D12Heap>;
	using ID3D12DescriptorHeapRef = TReferPtr<ID3D12DescriptorHeap>;
	using ID3D12CommandQueueRef = TReferPtr<ID3D12CommandQueue>;
	using ID3D12CommandAllocatorRef = TReferPtr<ID3D12CommandAllocator>;
	using ID3D12GraphicsCommandListRef = TReferPtr<ID3D12GraphicsCommandList>;

	using ID3D12RootSignatureRef = TReferPtr<ID3D12RootSignature>;
	using ID3D12PipelineStateRef = TReferPtr<ID3D12PipelineState>;

	using ID3D12CommandSignatureRef = TReferPtr<ID3D12CommandSignature>;

	// ------------
	using IDXGISurfaceRef = TReferPtr<IDXGISurface>;
	using ID3D11DeviceRef = TReferPtr<ID3D11Device>;
	using ID3D11DeviceContextRef = TReferPtr<ID3D11DeviceContext>;
	using ID3D11ResourceRef = TReferPtr<ID3D11Resource>;

	constexpr uint32 D3D12ConstBufferAlignment = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	constexpr uint32 D3D12VertexBufferAlignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	constexpr uint32 D3D12IndexBufferAlignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	constexpr D3D12_RESOURCE_STATES D3D12_RESOURCE_STATE_UNKONWN = (D3D12_RESOURCE_STATES)-1;

	struct FD3D12DescriptorHandle
	{
		D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE GPUDescriptorHandle;
	};

	struct FD3D12CPUDescriptor
	{
		D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHandle;
		uint32 InstanceIndex;
	};

	struct FD3D12GPUDescriptor
	{
		D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE GPUDescriptorHandle;

		ID3D12DescriptorHeap * D3D12DescriptorHeap = nullptr;
	};

	enum class ED3D12RootParameterType
	{
		None = -1,
		UInt32Constant = 0,
		ConstBuffer,
		ShaderResource,
		UnorderedAccess,
		DescriptorTable,
		Sampler,
	};

	enum ED3D12RootParameterTypeIndex
	{
		RPTI_UInt32Constant = 0,
		RPTI_ConstBuffer,
		RPTI_ShaderResource,
		RPTI_UnorderedAccess,
		RPTI_DescriptorTable,
		RPTI_Sampler,
		RPTI_Count,
	};

	constexpr uint32 D3D12RootParameterTypeCount = RPTI_Count;

	struct FD3D12ParameterRange
	{
		FString ParametersName;

		EShaderStage ShaderStage = EShaderStage::None;

		D3D12_DESCRIPTOR_RANGE_TYPE ParameterType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

		uint32 RegisterMask = 0;
		uint32 RegisterSpace = 0;

		uint32 DescriptorIndex = 0;
		uint32 DescriptorCount = 0;

		friend uintx Hash(const FD3D12ParameterRange & This) { return HashCombine(This.ShaderStage, This.ParameterType, This.RegisterMask, This.RegisterSpace, This.DescriptorIndex, This.DescriptorCount); }

		uint32 GetBaseRegisterIndex() const { return BitFindForward(RegisterMask); }
		uint32 GetNumShaderResources() const { return BitCount1(RegisterMask); }
	};

	struct FD3D12RootParameter
	{
		D3D12_ROOT_PARAMETER_TYPE RootParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;

		FName BindingName;
		EShaderStage ShaderStage = EShaderStage::None;

		uint32 ParameterIndex = NullIndex;

		uint32 RegisterIndex = NullIndex;
		uint32 RegisterSpace = 0;

		uint32 RangeIndex = 0;
		uint32 RangeCount = 0;

		friend uintx Hash(const FD3D12RootParameter & This)
		{
			return HashCombine(This.RootParameterType, 
				This.ShaderStage, This.ParameterIndex, This.RegisterIndex, This.RegisterSpace, 
				This.RangeIndex, This.RangeCount);
		}
	};

	enum class ED3D12RootSignatureFlags
	{
		None = 0,
		ReverseZ,
	};

	struct FD3D12ShaderParameter
	{
		FName BindingName;
		EShaderStage ShaderStage = EShaderStage::None;

		D3D12_DESCRIPTOR_RANGE_TYPE ParameterType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

		uint32 DescriptorIndex = NullIndex;

		uint32 RegisterIndex = NullIndex;
		uint32 RegisterSpace = 0;

		friend uintx Hash(const FD3D12ShaderParameter & This) { return HashCombine(This.BindingName, This.ShaderStage, This.ParameterType, This.DescriptorIndex, This.RegisterIndex, This.RegisterSpace); }
	};

	struct FD3D12ShaderParameters
	{
		TArray<FD3D12ShaderParameter, MaxD3D12ShaderParameters> Parameters;

		TArray<FD3D12ParameterRange, MaxD3D12ShaderBindingRanges> ParameterRanges;
		TArray<FD3D12RootParameter, MaxD3D12RootParameters> RootParameters;

		TArray<FSamplerDesc, MaxStaticSamplers> StaticSamplers;

		friend uintx Hash(const FD3D12ShaderParameters & This) { return HashCombine(This.Parameters, This.ParameterRanges, This.StaticSamplers); }
		uint32 GetNumDescriptors() const
		{
			uint32 NumDescriptors = 0;
			for (uint32 ParameterIndex = 0; ParameterIndex < MaxD3D12ShaderParameters; ++ParameterIndex)
			{
				const FD3D12ShaderParameter & ShaderParameter = Parameters[ParameterIndex];
				if (ShaderParameter.ShaderStage == EShaderStage::None)
					break;

				if (ShaderParameter.DescriptorIndex != NullIndex)
					++NumDescriptors;
			}
			return NumDescriptors;
		}
	};


	struct FHeapBlock
	{
		uint64 Offset = 0;
		uint64 Size = 0;

		explicit operator bool() const { return !!Size; }
		bool operator ==(const FHeapBlock &) const = default;
		auto operator <=>(const FHeapBlock &) const = default;
		friend uintx Hash(const FHeapBlock & HeapPosition) { return HashCombine(HeapPosition.Offset, HeapPosition.Size); }
	};

	using FCPUAddress = byte *;
	using FGPUAddress = D3D12_GPU_VIRTUAL_ADDRESS;

	using FD3D12DescriptorAddress = FRHIAddress;

	struct FD3D12AddressHandle
	{
		FRHIAddress RHIAddress;
		uint32 HandleIncrement = 1;

		FD3D12AddressHandle operator +(uint32 Offset) const
		{
			return
			{
				RHIAddress + Offset * HandleIncrement,
				HandleIncrement
			};
		}

		FD3D12AddressHandle & operator +=(uint32 Offset)
		{
			RHIAddress = RHIAddress + Offset * HandleIncrement;
			return *this;
		}
	};

	forceinline void SetD3D12ObjectName(ID3D12Object * D3D12Object, LPCWSTR Name)
	{
		if (Name && Name[0])
			D3D12Object->SetName(Name);
	}

	forceinline void SetD3D12ObjectName(ID3D12Object * D3D12Object, FStringV Name)
	{
		if (Name.Size)
			D3D12Object->SetName(FEncoding::ToWide(Name));
	}

	forceinline void SetD3D12ObjectName(ID3D12Object * D3D12Object, FWStringV Name)
	{
		if (Name.Size)
			D3D12Object->SetName(Name.Data);
	}

	forceinline void SetD3D12ObjectName(ID3D12Object * D3D12Object, FName Name)
	{
		SetD3D12ObjectName(D3D12Object, Name.Resolve());
	}

	forceinline FStringV D3D12BlobMessage(ID3DBlob * D3DBlob)
	{
		if (!D3DBlob || D3DBlob->GetBufferSize() < 2)
			return FStringV();

		return D3DBlob ? FEncoding::ToUtf8(FAStringV(static_cast<const char *>(D3DBlob->GetBufferPointer()), D3DBlob->GetBufferSize() - 1)) : FStringV();
	}

	forceinline D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHandle(byte * CPUAddress)
	{
		return D3D12_CPU_DESCRIPTOR_HANDLE { SIZE_T(CPUAddress) };
	}

	forceinline FStringV D3D12BlobMessage(TReferPtr<ID3DBlob> D3DBlob)
	{
		if (!D3DBlob || D3DBlob->GetBufferSize() < 2)
			return FStringV();

		return D3DBlob ? FEncoding::ToUtf8(FAStringV(static_cast<const char *>(D3DBlob->GetBufferPointer()), D3DBlob->GetBufferSize() - 1)) : FStringV();
	}

	forceinline DXGI_SWAP_EFFECT FromSwapEffect(EPresentMode SwapEffect)
	{
		switch (SwapEffect)
		{
		case EPresentMode::Discard:
			return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
		case EPresentMode::Sequential:
			return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_SEQUENTIAL;
		case EPresentMode::FlipSequential:
			return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		case EPresentMode::FlipDiscard:
			return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
		default:
			return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
		}
	}

	forceinline DXGI_FORMAT FromFormatSRV(EFormat Format)
	{
		switch (Format)
		{
		case EFormat::R8G8B8A8: return FromFormat(EFormat::R8G8B8A8UF);
		case EFormat::R8G8B8X8: return FromFormat(EFormat::R8G8B8A8UF);
		case EFormat::B8G8R8A8: return FromFormat(EFormat::B8G8R8A8UF);
		case EFormat::B8G8R8X8: return FromFormat(EFormat::B8G8R8X8UF);
		case EFormat::R10G10B10A2: return FromFormat(EFormat::R10G10B10A2UF);
		case EFormat::R16G16: return FromFormat(EFormat::R16G16UF);
		case EFormat::R16G16B16A16: return FromFormat(EFormat::R16G16B16A16UF);
		case EFormat::R32G32: return FromFormat(EFormat::R32G32F);
		case EFormat::R32G32B32: return FromFormat(EFormat::R32G32B32F);
		case EFormat::R32G32B32A32: return FromFormat(EFormat::R32G32B32A32F);
		case EFormat::A8: return FromFormat(EFormat::A8UF);
		case EFormat::R8: return FromFormat(EFormat::R8UF);
		case EFormat::R16: return FromFormat(EFormat::R16UF);
		case EFormat::R24G8: return FromFormat(EFormat::R16UF);
		case EFormat::R32: return FromFormat(EFormat::R32F);
		case EFormat::BC1: return FromFormat(EFormat::BC1UF);
		case EFormat::BC2: return FromFormat(EFormat::BC2UF);
		case EFormat::BC3: return FromFormat(EFormat::BC3UF);
		case EFormat::BC4: return FromFormat(EFormat::BC4UF);
		case EFormat::BC5: return FromFormat(EFormat::BC5UF);
		case EFormat::BC6H: return FromFormat(EFormat::BC6H_UF16);
		case EFormat::BC7: return FromFormat(EFormat::BC7UF);
		case EFormat::R8G8: return FromFormat(EFormat::R8G8UF);
		case EFormat::R5G6B5: return FromFormat(EFormat::R5G6B5UF);

		case EFormat::D16UF: return FromFormat(EFormat::R16UF);
		case EFormat::D24UF_S8UI: return FromFormat(EFormat::R24G8);
		case EFormat::D32F: return FromFormat(EFormat::R32F);
		default: return FromFormat(Format);
		}
	}

	forceinline DXGI_FORMAT FromFormatDSV(EFormat Format)
	{
		switch (Format)
		{
		case EFormat::D16UF: return DXGI_FORMAT_D16_UNORM;
		case EFormat::D24UF_S8UI: return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case EFormat::D32F: return DXGI_FORMAT_D32_FLOAT;
		case EFormat::R32: return DXGI_FORMAT_D32_FLOAT;
		default: return FromFormat(Format);
		}
	}

	forceinline DXGI_FORMAT FromFormatUAV(EFormat Format)
	{
		switch (Format)
		{
		case EFormat::D16UF: return FromFormat(EFormat::R16UF);
		case EFormat::D24UF_S8UI: return FromFormat(EFormat::R24G8);
		case EFormat::D32F: return FromFormat(EFormat::R32F);
		default: return FromFormat(Format);
		}
	}

	forceinline DXGI_FORMAT FromIndexFormat(EIndexFormat IndexFormat)
	{
		switch (IndexFormat)
		{
		case EIndexFormat::Index16: return DXGI_FORMAT_R16_UINT;
		case EIndexFormat::Index32: return DXGI_FORMAT_R32_UINT;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	}

	forceinline D3D12_HEAP_TYPE FromHeapType(EHeapUsage HeapUsage)
	{
		switch (HeapUsage & EHeapUsage::HostMask)
		{
		case EHeapUsage::None:
			return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		case EHeapUsage::HostRead:
			return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK;
		case EHeapUsage::HostWrite:
			return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
		default:
			return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		}
	}

	forceinline D3D12_HEAP_TYPE FromHeapType(EResourceUsage ResourceUsage)
	{
		if (ResourceUsage & EResourceUsage::HostRead)
			return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK;
		else if (ResourceUsage & EResourceUsage::HostWrite)
			return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
		else
			return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	}

	forceinline EHeapUsage ToHeapType(D3D12_HEAP_TYPE HeapType)
	{
		switch (HeapType)
		{
		case D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT:
			return EHeapUsage::None;
		case D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD:
			return EHeapUsage::HostWrite;
		case D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK:
			return EHeapUsage::HostRead;
		default:
			return EHeapUsage::None;
		}
	}

	forceinline D3D12_COMMAND_LIST_TYPE FromCommandType(ECommandType CommandType)
	{
		switch (CommandType)
		{
		case ECommandType::Direct:
			return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
		case ECommandType::Bundle:
			return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE;
		case ECommandType::Compute:
			return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE;
		case ECommandType::Copy:
			return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COPY;
		default:
			return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
		}
	}

	forceinline D3D12_RESOURCE_DIMENSION FromResourceDimension(EResourceDimension ResourceDimension)
	{
		switch (ResourceDimension)
		{
		case EResourceDimension::None:
			return D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_UNKNOWN;
		case EResourceDimension::Buffer:
			return D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
		case EResourceDimension::Texture1D:
			return D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		case EResourceDimension::Texture2D:
			return D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		case EResourceDimension::Texture3D:
			return D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE3D;
		default:
			return D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_UNKNOWN;
		}
	}

	forceinline EResourceDimension ToResourceDimension(D3D12_RESOURCE_DIMENSION ResourceDimension)
	{
		switch (ResourceDimension)
		{
		case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_UNKNOWN:
			return EResourceDimension::None;
		case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER:
			return EResourceDimension::Buffer;
		case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE1D:
			return EResourceDimension::Texture1D;
		case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D:
			return EResourceDimension::Texture2D;
		case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE3D:
			return EResourceDimension::Texture3D;
		default:
			return EResourceDimension::None;
		}
	}

	forceinline D3D12_RESOURCE_FLAGS FromResourceUsage(EResourceUsage Usage)
	{
		TEnumWrapper<D3D12_RESOURCE_FLAGS> Result;
		Result.Set(D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, Usage & EResourceUsage::RenderTarget);
		Result.Set(D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, Usage & EResourceUsage::DepthStencil);
		//Flags cannot have D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE set without D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY or D3D12_RESOURCE_FLAG_VIDEO_ENCODE_REFERENCE_ONLY.
		if (Usage & EResourceUsage::DepthStencil)
			Result.Set(D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE, !(Usage & EResourceUsage::SampledTexture));
		Result.Set(D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, AnyFlags(Usage, EResourceUsage::StorageBuffer | EResourceUsage::UniformTexelBuffer | EResourceUsage::StorageTexelBuffer | EResourceUsage::StorageTexture));
		Result.Set(D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER, Usage & EResourceUsage::CrossAdapter);
		Result.Set(D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS, Usage & EResourceUsage::Simultaneous);
		return Result;
	}


	forceinline EResourceUsage ToResourceUsage(D3D12_RESOURCE_FLAGS D3D12ResourceFlags)
	{
		TEnumWrapper<EResourceUsage> Result;
		Result.Set(EResourceUsage::RenderTarget, D3D12ResourceFlags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		Result.Set(EResourceUsage::DepthStencil, D3D12ResourceFlags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		Result.Set(EResourceUsage::SampledTexture, !(D3D12ResourceFlags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE));
		Result.Set(EResourceUsage::TexelMask, D3D12ResourceFlags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		Result.Set(EResourceUsage::CrossAdapter, D3D12ResourceFlags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER);
		return Result;
	}

	/**
	 * see https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_resource_heap_tier
	 * @param HeapUsage 
	 * @param HeapTier
	 * @param D3D12FeatureOptions7 
	 * @return 
	 */
	forceinline D3D12_HEAP_FLAGS FromHeapFlags(EHeapUsage HeapUsage, D3D12_RESOURCE_HEAP_TIER HeapTier, bool D3D12FeatureOptions7 = false)
	{
		D3D12_HEAP_FLAGS Result {};
		SetFlags(Result, D3D12_HEAP_FLAG_SHARED, HeapUsage & EHeapUsage::Interoperable);
		SetFlags(Result, D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER, HeapUsage & EHeapUsage::CrossAdapter);

		// https://devblogs.microsoft.com/directx/coming-to-directx-12-more-control-over-memory-allocation/
		// there’s no dedicated CheckFeatureSupport option for these, they’re available anytime that ID3D12Device8 is exposed, or a check for D3D12_FEATURE_D3D12_OPTIONS7 succeeds.
		if (D3D12FeatureOptions7)
		{
			SetFlags(Result, D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT, (HeapUsage & EHeapUsage::NotResident));
			SetFlags(Result, D3D12_HEAP_FLAG_CREATE_NOT_ZEROED, !(HeapUsage & EHeapUsage::ZeroInitialize));
		}

		if (HeapTier == D3D12_RESOURCE_HEAP_TIER_1)
		{
			AssertExpr((HeapUsage & EHeapUsage::AllowMask) != EHeapUsage::AllowMask);
			SetFlags(Result, D3D12_HEAP_FLAG_DENY_BUFFERS, !(HeapUsage & EHeapUsage::AllowBuffers));
			SetFlags(Result, D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES, !(HeapUsage & EHeapUsage::AllowTextures));
			SetFlags(Result, D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES, !(HeapUsage & EHeapUsage::AllowRTsDSs));
		}
		else
		{
			// D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES = 0;
			//SetFlags(Result, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,  true);
		}
		return Result;
	}

	forceinline D3D12_HEAP_FLAGS FromCommittedHeapFlags(EHeapFlag HeapFlag)
	{
		D3D12_HEAP_FLAGS Result {};
		SetFlags(Result, D3D12_HEAP_FLAG_SHARED, HeapFlag & EHeapFlag::Interoperable);
		SetFlags(Result, D3D12_HEAP_FLAG_ALLOW_WRITE_WATCH, HeapFlag & EHeapFlag::WriteWatch);
		SetFlags(Result, D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER, HeapFlag & EHeapFlag::CrossAdapter);
		return Result;
	}

	forceinline D3D12_CPU_PAGE_PROPERTY FromCPUPageProperty(EHeapUsage HeapUsage)
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

	forceinline D3D12_MEMORY_POOL FromMemoryPool(EHeapUsage HeapUsage)
	{
		if (HeapUsage & EHeapUsage::HostMask)
			return D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;

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

	forceinline EResourceAccess GetInitialAccess(const FResourceDesc & ResourceDesc)
	{
		if (ResourceDesc.Usage & EResourceUsage::DepthStencil)
			return EResourceAccess::DepthWrite;

		if (ResourceDesc.Usage & EResourceUsage::WriteMask)
		{
			if (ResourceDesc.Usage & EResourceUsage::Simultaneous)
				return EResourceAccess::Common;

			if (ResourceDesc.Usage & EResourceUsage::VertexBuffer)
				return EResourceAccess::VertexBuffer;

			if (ResourceDesc.Usage & EResourceUsage::UniformBuffer)
				return EResourceAccess::UniformBuffer;

			if (ResourceDesc.Usage & EResourceUsage::StorageBuffer)
				return EResourceAccess::ComputeStorageBuffer;

			if (ResourceDesc.Usage & EResourceUsage::UniformTexelBuffer)
				return EResourceAccess::UniformTexelBuffer;

			if (ResourceDesc.Usage & EResourceUsage::StorageTexelBuffer)
				return EResourceAccess::StorageTexelBuffer;

			if (ResourceDesc.Usage & EResourceUsage::IndexBuffer)
				return EResourceAccess::IndexBuffer;

			if (ResourceDesc.Usage & EResourceUsage::IndirectBuffer)
				return EResourceAccess::IndirectBuffer;

			if (ResourceDesc.Usage & EResourceUsage::ResolveRead)
				return EResourceAccess::ResolveRead;

			if (ResourceDesc.Usage & EResourceUsage::ResolveWrite)
				return EResourceAccess::ResolveWrite;

			if (ResourceDesc.Usage & EResourceUsage::RenderTarget)
				return EResourceAccess::RenderTarget;

			if (ResourceDesc.Usage & EResourceUsage::CopyMask)
				return EResourceAccess::Common;

			if (ResourceDesc.Usage & EResourceUsage::SampledTexture)
				return EResourceAccess::Common;

			return EResourceAccess::None;
		}
		else if (ResourceDesc.Usage & EResourceUsage::BufferMask)
		{
			return EResourceAccess::Common;
		}
		else
		{
			return EResourceAccess::None;
		}
	}

	forceinline D3D12_RESOURCE_STATES FromResourceAccess(EResourceAccess ResourceAccess, EResourceAccess InitialAccess = EResourceAccess::Common)
	{
		if (ResourceAccess == EResourceAccess::None)
		{
			if (InitialAccess == EResourceAccess::None)
				return D3D12_RESOURCE_STATE_COMMON;
			return FromResourceAccess(InitialAccess);
		}

		if (ResourceAccess == EResourceAccess::Common)
			return D3D12_RESOURCE_STATE_COMMON;

		if (ResourceAccess == EResourceAccess::Generic)
			return D3D12_RESOURCE_STATE_GENERIC_READ;

		if (ResourceAccess & EResourceAccess::IndirectBuffer)
			return D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;

		if (ResourceAccess & EResourceAccess::StorageMask)
			return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

		TEnumWrapper<D3D12_RESOURCE_STATES> Result;
		Result.Set(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ResourceAccess & (EResourceAccess::VertexBuffer | EResourceAccess::UniformBuffer));
		Result.Set(D3D12_RESOURCE_STATE_INDEX_BUFFER, ResourceAccess & EResourceAccess::IndexBuffer);
		Result.Set(D3D12_RESOURCE_STATE_RENDER_TARGET, ResourceAccess & EResourceAccess::RenderTarget);

		// invalid combine for D3D12_RESOURCE_STATE_DEPTH_WRITE | D3D12_RESOURCE_STATE_DEPTH_READ.
		Result.Set(D3D12_RESOURCE_STATE_DEPTH_WRITE, ResourceAccess & EResourceAccess::DepthWrite);
		Result.Set(D3D12_RESOURCE_STATE_DEPTH_READ, ResourceAccess & EResourceAccess::DepthRead);

		if (ResourceAccess & EResourceAccess::SampledTexture)
		{
			Result.Set(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, ResourceAccess & EResourceAccess::PixelStage);
			Result.Set(D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, ResourceAccess & EResourceAccess::NonePixelStage);
		}

		Result.Set(D3D12_RESOURCE_STATE_COPY_SOURCE, ResourceAccess & EResourceAccess::CopyRead);
		Result.Set(D3D12_RESOURCE_STATE_COPY_DEST, ResourceAccess & EResourceAccess::CopyWrite);
		Result.Set(D3D12_RESOURCE_STATE_RESOLVE_SOURCE, ResourceAccess & EResourceAccess::ResolveRead);
		Result.Set(D3D12_RESOURCE_STATE_RESOLVE_DEST, ResourceAccess & EResourceAccess::ResolveWrite);
		Result.Set(D3D12_RESOURCE_STATE_PRESENT, ResourceAccess & EResourceAccess::Present);
		return Result;
	}

	forceinline D3D12_SRV_DIMENSION FromResourceViewDimension(EResourceViewDimension ResourceViewDimension, bool MSAA = false)
	{
		switch (ResourceViewDimension)
		{
		case EResourceViewDimension::None:
			return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_UNKNOWN;
		case EResourceViewDimension::Buffer:
			return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
		case EResourceViewDimension::Texture1D:
			return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1D;
		case EResourceViewDimension::Texture1DArray:
			return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
		case EResourceViewDimension::Texture2D:
			return MSAA ? D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DMS : D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
		case EResourceViewDimension::Texture2DArray:
			return MSAA ? D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DARRAY : D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		case EResourceViewDimension::Texture3D:
			return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE3D;
		case EResourceViewDimension::TextureCube:
			return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBE;
		case EResourceViewDimension::TextureCubeArray:
			return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
		default:
			return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_UNKNOWN;
		}
	}

	forceinline EResourceViewDimension ToResourceViewDimension(D3D12_SRV_DIMENSION ResourceViewDimension, bool MSAA = false)
	{
		switch (ResourceViewDimension)
		{
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_UNKNOWN:
			return EResourceViewDimension::None;
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER:
			return EResourceViewDimension::Buffer;
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1D:
			return EResourceViewDimension::Texture1D;
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1DARRAY:
			return EResourceViewDimension::Texture1DArray;
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D:
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DMS:
			return EResourceViewDimension::Texture2D;
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DARRAY:
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY:
			return EResourceViewDimension::Texture2DArray;
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE3D:
			return EResourceViewDimension::Texture3D;
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBE:
			return EResourceViewDimension::TextureCube;
		case D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBEARRAY:
			return EResourceViewDimension::TextureCubeArray;
		default:
			return EResourceViewDimension::None;
		}
	}

	forceinline D3D12_RTV_DIMENSION FromResourceViewDimension_RenderTargetView(EResourceViewDimension ResourceViewDimension, bool MSAA = false)
	{
		switch (ResourceViewDimension)
		{
		case EResourceViewDimension::None:
			return D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_UNKNOWN;
		case EResourceViewDimension::Buffer:
			return D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_BUFFER;
		case EResourceViewDimension::Texture1D:
			return D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE1D;
		case EResourceViewDimension::Texture1DArray:
			return D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
		case EResourceViewDimension::Texture2D:
			return MSAA ? D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2DMS : D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
		case EResourceViewDimension::Texture2DArray:
			return MSAA ? D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY : D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
		case EResourceViewDimension::Texture3D:
			return D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE3D;
		default:
			return D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_UNKNOWN;
		}
	}

	forceinline D3D12_DSV_DIMENSION FromResourceViewDimension_DepthStencilView(EResourceViewDimension ResourceViewDimension, bool MSAA = false)
	{
		switch (ResourceViewDimension)
		{
		case EResourceViewDimension::None:
			return D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_UNKNOWN;
		case EResourceViewDimension::Texture1D:
			return D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE1D;
		case EResourceViewDimension::Texture1DArray:
			return D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
		case EResourceViewDimension::Texture2D:
			return MSAA ? D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D : D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2DMS;
		case EResourceViewDimension::Texture2DArray:
			return MSAA ? D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2DARRAY : D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
		default:
			return D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_UNKNOWN;
		}
	}

	forceinline D3D12_UAV_DIMENSION FromResourceViewDimension_UAV(EResourceViewDimension ResourceViewDimension)
	{
		switch (ResourceViewDimension)
		{
		case EResourceViewDimension::None:
			return D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_UNKNOWN;
		case EResourceViewDimension::Buffer:
			return D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
		case EResourceViewDimension::Texture1D:
			return D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE1D;
		case EResourceViewDimension::Texture1DArray:
			return D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
		case EResourceViewDimension::Texture2D:
			return D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
		case EResourceViewDimension::Texture2DArray:
			return D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
		case EResourceViewDimension::Texture3D:
			return D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE3D;
		default:
			return D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_UNKNOWN;
		}
	}

	forceinline D3D12_DESCRIPTOR_HEAP_TYPE FromDescriptorHeapType(EDescriptorHeapType DescriptorHeapType)
	{
		switch (DescriptorHeapType)
		{
		case EDescriptorHeapType::ShaderResource:
			return D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		case EDescriptorHeapType::RenderTarget:
			return D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		case EDescriptorHeapType::DepthStencil:
			return D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		case EDescriptorHeapType::Sampler:
			return D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		default:
			AssertFailed(u8"Invalid DescriptorHeapType"V);
			return D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		}
	}

	forceinline D3D12_DESCRIPTOR_HEAP_FLAGS FromDescriptorHeapFlags(EDescriptorHeapFlag ResourcePacketFlags)
	{
		TEnumWrapper<D3D12_DESCRIPTOR_HEAP_FLAGS> Result;
		Result.Set(D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, ResourcePacketFlags & EDescriptorHeapFlag::ShaderVisible);
		return Result;
	}

	forceinline D3D12_DESCRIPTOR_RANGE_TYPE FromDescriptorType_RangeType(EDescriptorType DescriptorType)
	{
		switch (DescriptorType)
		{
		case EDescriptorType::UniformBuffer:
			return D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		case EDescriptorType::SampledTexture:
			return D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		case EDescriptorType::StorageBuffer:
		case EDescriptorType::UniformTexelBuffer:
		case EDescriptorType::StorageTexelBuffer:
			return D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		case EDescriptorType::Sampler:
			return D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		default:
			return D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		}
	}

	forceinline D3D12_ROOT_PARAMETER_TYPE FromDescriptorType_RootParameter(EDescriptorType DescriptorType)
	{
		switch (DescriptorType)
		{
		case EDescriptorType::UniformBuffer: return D3D12_ROOT_PARAMETER_TYPE_CBV;
		case EDescriptorType::SampledTexture:return D3D12_ROOT_PARAMETER_TYPE_SRV;
		case EDescriptorType::StorageBuffer:
		case EDescriptorType::UniformTexelBuffer:
		case EDescriptorType::StorageTexelBuffer:
			return D3D12_ROOT_PARAMETER_TYPE_UAV;
		default:return D3D12_ROOT_PARAMETER_TYPE(-1);
		}
	}

	forceinline D3D12_ROOT_PARAMETER_TYPE FromRootParameterType(ED3D12RootParameterType RootDescriptorType)
	{
		switch (RootDescriptorType)
		{
		case ED3D12RootParameterType::DescriptorTable:
			return D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		case ED3D12RootParameterType::UInt32Constant:
			return D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		case ED3D12RootParameterType::ConstBuffer:
			return D3D12_ROOT_PARAMETER_TYPE_CBV;
		case ED3D12RootParameterType::ShaderResource:
			return D3D12_ROOT_PARAMETER_TYPE_SRV;
		case ED3D12RootParameterType::UnorderedAccess:
			return D3D12_ROOT_PARAMETER_TYPE_UAV;
		default:
			return D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		}
	}

	forceinline D3D12_SHADER_VISIBILITY FromShaderRelevance(EShaderRelevance ShaderRelevance)
	{
		switch (ShaderRelevance)
		{
		case EShaderRelevance::All:
			return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
		case EShaderRelevance::Vertex:
			return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		case EShaderRelevance::Hull:
			return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_HULL;
		case EShaderRelevance::Domain:
			return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_DOMAIN;
		case EShaderRelevance::Geometry:
			return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY;
		case EShaderRelevance::Pixel:
			return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
		default:
			return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
		}
	}

	forceinline D3D12_BLEND FromBlendFactor(EBlendFactor BlendFactor)
	{
		switch (BlendFactor)
		{
		case EBlendFactor::None:
			return D3D12_BLEND::D3D12_BLEND_ZERO;
		case EBlendFactor::Zero:
			return D3D12_BLEND::D3D12_BLEND_ZERO;
		case EBlendFactor::One:
			return D3D12_BLEND::D3D12_BLEND_ONE;
		case EBlendFactor::SrcColor:
			return D3D12_BLEND::D3D12_BLEND_SRC_COLOR;
		case EBlendFactor::SrcColorInv:
			return D3D12_BLEND::D3D12_BLEND_INV_SRC_COLOR;
		case EBlendFactor::SrcAlpha:
			return D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
		case EBlendFactor::SrcAlphaInv:
			return D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
		case EBlendFactor::DestAlpha:
			return D3D12_BLEND::D3D12_BLEND_DEST_ALPHA;
		case EBlendFactor::DestAlphaInv:
			return D3D12_BLEND::D3D12_BLEND_INV_DEST_ALPHA;
		case EBlendFactor::DestColor:
			return D3D12_BLEND::D3D12_BLEND_DEST_COLOR;
		case EBlendFactor::DestColorInv:
			return D3D12_BLEND::D3D12_BLEND_INV_DEST_COLOR;
		case EBlendFactor::BlendFactor:
			return D3D12_BLEND::D3D12_BLEND_BLEND_FACTOR;
		case EBlendFactor::BlendFactorInv:
			return D3D12_BLEND::D3D12_BLEND_INV_BLEND_FACTOR;
		case EBlendFactor::Src1Color:
			return D3D12_BLEND::D3D12_BLEND_SRC1_COLOR;
		case EBlendFactor::Src1ColorInv:
			return D3D12_BLEND::D3D12_BLEND_INV_SRC1_COLOR;
		case EBlendFactor::Src1Alpha:
			return D3D12_BLEND::D3D12_BLEND_SRC1_ALPHA;
		case EBlendFactor::Src1AlphaInv:
			return D3D12_BLEND::D3D12_BLEND_INV_SRC1_ALPHA;
		default:
			return D3D12_BLEND::D3D12_BLEND_ZERO;
		}
	}

	forceinline D3D12_BLEND_OP FromBlendOP(EBlendAlgorithm BlendOP)
	{
		switch (BlendOP)
		{
		case EBlendAlgorithm::Add:
			return D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
		case EBlendAlgorithm::Subtract:
			return D3D12_BLEND_OP::D3D12_BLEND_OP_SUBTRACT;
		case EBlendAlgorithm::Min:
			return D3D12_BLEND_OP::D3D12_BLEND_OP_MIN;
		case EBlendAlgorithm::Max:
			return D3D12_BLEND_OP::D3D12_BLEND_OP_MAX;
		default:
			return D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
		}
	}

	forceinline D3D12_CULL_MODE FromCullMode(ECullMode CullMode)
	{
		switch (CullMode)
		{
		case ECullMode::None:
			return D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
		case ECullMode::Front:
			return D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		case ECullMode::Back:
			return D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		default:
			return D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
		}
	}

	forceinline D3D12_COLOR_WRITE_ENABLE FromWriteMasks(EWriteMasks WriteMasks)
	{
		TEnumWrapper<D3D12_COLOR_WRITE_ENABLE> Result;
		Result.Set(D3D12_COLOR_WRITE_ENABLE_RED, WriteMasks & EWriteMasks::Red);
		Result.Set(D3D12_COLOR_WRITE_ENABLE_GREEN, WriteMasks & EWriteMasks::Green);
		Result.Set(D3D12_COLOR_WRITE_ENABLE_BLUE, WriteMasks & EWriteMasks::Blue);
		Result.Set(D3D12_COLOR_WRITE_ENABLE_ALPHA, WriteMasks & EWriteMasks::Alpha);
		return Result;
	}

	forceinline D3D12_PRIMITIVE_TOPOLOGY FromTopology(ETopology Topology, uint32 NumControlPoints)
	{
		switch (Topology)
		{
		default:
		case ETopology::None:
			return D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		case ETopology::PointList:
			return D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		case ETopology::LineList:
			return D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case ETopology::LineStrip:
			return D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case ETopology::TriangleList:
			return D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case ETopology::TriangleStrip:
			return D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case ETopology::PatchList :
			AssertExpr(NumControlPoints > 0);
			return D3D12_PRIMITIVE_TOPOLOGY(D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + NumControlPoints - 1);
		}
	}

	forceinline D3D12_PRIMITIVE_TOPOLOGY_TYPE FromTopology(ETopology Topology)
	{
		switch (Topology)
		{
		case ETopology::PointList: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		case ETopology::LineList:return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		case ETopology::LineStrip:return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		case ETopology::TriangleList:return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		case ETopology::TriangleStrip:return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		case ETopology::PatchList:return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
		default: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
		}
	}

	forceinline D3D12_FILTER FromFilter(ESampleFilter Filter)
	{
		switch (Filter)
		{
		case ESampleFilter::Point:
			return D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		case ESampleFilter::Linear:
			return D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		case ESampleFilter::Trilinear:
			return D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		default:
			return D3D12_FILTER::D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		}
	}

	forceinline D3D12_TEXTURE_ADDRESS_MODE FromAddressMode(EAddressMode AddressMode)
	{
		switch (AddressMode)
		{
		case EAddressMode::Clamp:
			return D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		case EAddressMode::Wrap:
			return D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		case EAddressMode::Mirror:
			return D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
		case EAddressMode::Border:
			return D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		default:
			return D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		}
	}

	forceinline D3D12_COMPARISON_FUNC FromComparison(EComparison Comparison)
	{
		switch (Comparison)
		{
		case EComparison::None:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
		case EComparison::Never:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
		case EComparison::Always:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
		case EComparison::Less:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS;
		case EComparison::LessEqual:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_LESS_EQUAL;
		case EComparison::Equal:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_EQUAL;
		case EComparison::GreaterEqual:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		case EComparison::Greater:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_GREATER;
		case EComparison::NotEqual:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NOT_EQUAL;
		default:
			return D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER;
		}
	}

	forceinline D3D12_STATIC_BORDER_COLOR FromBorderColor(EBorderColor BorderColor)
	{
		switch (BorderColor)
		{
		case EBorderColor::TransparentBlack:
			return D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		case EBorderColor::OpaqueBlack:
			return D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		case EBorderColor::OpaqueWhite:
			return D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		default:
			return D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		}
	}

	forceinline D3D12_INDIRECT_ARGUMENT_TYPE FromIndirectParameterType(EIndirectParameterType IndirectParameterType)
	{
		switch (IndirectParameterType)
		{
		default:
		case EIndirectParameterType::Draw: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
		case EIndirectParameterType::DrawIndexed: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
		case EIndirectParameterType::Dispatch: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;
		case EIndirectParameterType::VertexBufferView: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW;
		case EIndirectParameterType::IndexBufferView: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW;
		case EIndirectParameterType::Constannt: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT;
		case EIndirectParameterType::CBV: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
		case EIndirectParameterType::SRV: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW;
		case EIndirectParameterType::UAV: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
		case EIndirectParameterType::DispatchRays: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_RAYS;
		case EIndirectParameterType::DispatchMesh: return D3D12_INDIRECT_ARGUMENT_TYPE::D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_MESH;
		}
	}

	forceinline D3D12_ROOT_DESCRIPTOR_FLAGS FromRootDescriptorFlags(EDescriptorFlag DescriptorFlags)
	{
		TEnumWrapper<D3D12_ROOT_DESCRIPTOR_FLAGS> Result;
		Result.Set(D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, DescriptorFlags & EDescriptorFlag::Volatile);
		Result.Set(D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, DescriptorFlags & EDescriptorFlag::Static);
		return Result;
	}

	forceinline D3D12_DESCRIPTOR_RANGE_FLAGS FromDescriptorRangeFlags(EDescriptorRangeFlag DescriptorFlags)
	{
		TEnumWrapper<D3D12_DESCRIPTOR_RANGE_FLAGS> Result;
		Result.Set(D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE, DescriptorFlags & EDescriptorRangeFlag::DescriptorVolatile);
		Result.Set(D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE, DescriptorFlags & EDescriptorRangeFlag::Volatile);
		Result.Set(D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC, DescriptorFlags & EDescriptorRangeFlag::Static);
		return Result;
	}

	forceinline D3D12_FENCE_FLAGS FromFenceFlags(EFenceFlag FenceFlags)
	{
		TEnumWrapper<D3D12_FENCE_FLAGS> Result;
		Result.Set(D3D12_FENCE_FLAG_SHARED, FenceFlags & EFenceFlag::Shared);
		return Result;
	}

	forceinline D3D12_VIEWPORT FromViewport(const FViewport & Viewport)
	{
		return D3D12_VIEWPORT { Viewport.X, Viewport.Y, Viewport.Width, Viewport.Height, Viewport.NearZ, Viewport.FarZ };
	}

	forceinline D3D12_RECT FromRect(const RectI & Rect)
	{
		return D3D12_RECT { Rect.X, Rect.Y, Rect.Right, Rect.Bottom };
	}

	struct FD3D12ShaderRegisterRange
	{
		EShaderStage ShaderStage = EShaderStage::None;
		uint32 RegisterSpace = NullIndex;
		uint32 RegisterIndex = 0;
		uint32 RegisterCount = 0;

		auto operator <=>(const FD3D12ShaderRegisterRange &) const = default;
		bool operator ==(const FD3D12ShaderRegisterRange &) const = default;

		friend uintx Hash(const FD3D12ShaderRegisterRange & This) { return HashCombine(This.ShaderStage, This.RegisterSpace, This.RegisterIndex, This.RegisterCount); }
	};

	constexpr int MaxShaderRegisterRangeCount_CBV = 2;
	constexpr int MaxShaderRegisterRangeCount_SRV = 8;
	constexpr int MaxShaderRegisterRangeCount_UAV = 4;
	constexpr int MaxShaderRegisterRangeCount_SAM = 2;
	constexpr int MaxShaderRegisterRangeCount = MaxShaderRegisterRangeCount_CBV + MaxShaderRegisterRangeCount_SRV + MaxShaderRegisterRangeCount_UAV + MaxShaderRegisterRangeCount_SAM;

	constexpr int MaxShaderRegisterSpaceCount = 4;
	constexpr TArray<uint32, 4> RegisterMask8Bits = { 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 };

	struct FD3D12ShaderRegisterBits
	{
		uint32 RegisterMask = 0;
		uint32 RegisterSpace = NullIndex;

		auto operator <=>(const FD3D12ShaderRegisterBits &) const = default;
		bool operator ==(const FD3D12ShaderRegisterBits &) const = default;
		friend uintx Hash(const FD3D12ShaderRegisterBits & This) { return HashCombine(This.RegisterMask, This.RegisterSpace); }
	};

	struct FD3D12ShaderRegisterCount
	{
		TArray<FD3D12ShaderRegisterBits, MaxShaderRegisterRangeCount_CBV> CBVs {};
		TArray<FD3D12ShaderRegisterBits, MaxShaderRegisterRangeCount_SRV> SRVs {};
		TArray<FD3D12ShaderRegisterBits, MaxShaderRegisterRangeCount_UAV> UAVs {};
		TArray<FD3D12ShaderRegisterBits, MaxShaderRegisterRangeCount_SAM> SAMs {};
		friend uintx Hash(const FD3D12ShaderRegisterCount & This) { return HashCombine(This.CBVs, This.SRVs, This.UAVs, This.SAMs); }
	};

	struct FD3D12ShaderRegisterCounts
	{
		uint64 HashCode = 0;
		TArray<FD3D12ShaderRegisterCount, MaxShaderStages> RegisterCounts;

		auto operator <=>(const FD3D12ShaderRegisterCounts & Another) const noexcept { return HashCode <=> Another.HashCode; }
		bool operator ==(const FD3D12ShaderRegisterCounts & Another) const noexcept { return HashCode == Another.HashCode; }
		forceinline friend uint64 Hash(const FD3D12ShaderRegisterCounts & This) { return This.HashCode; }
	};

	forceinline D3D12_CLEAR_VALUE FromClearValue(const FClearValue & ClearValue, EFormat Format, EResourceUsage Usage)
	{
		D3D12_CLEAR_VALUE D3D12ClearValue = { DXGI_FORMAT_UNKNOWN };
		if (Usage & EResourceUsage::RenderTarget)
		{
			//AssertExpresion(ClearValue.ClearMode == EClearMode::Color || IsTypelessFormat(Format));
			if (ClearValue.ClearMode == EClearMode::Color)
			{
				AssertExpr(ClearValue.ClearMode == EClearMode::Color);
				D3D12ClearValue.Format = FromFormat(Format);
				AssertExpr(D3D12ClearValue.Format != DXGI_FORMAT_UNKNOWN);
				MemoryCopy(D3D12ClearValue.Color, ClearValue.Color);
			}
			else if (ClearValue.ClearMode == EClearMode::None)
			{
				D3D12ClearValue.Format = FromFormat(Format);
				AssertExpr(D3D12ClearValue.Format != DXGI_FORMAT_UNKNOWN);
				ArrayCopy(D3D12ClearValue.Color, { 0.f, 0.f, 0.f, 0.f }); // Black
			}
			else {}
		}
		else if (Usage & EResourceUsage::DepthStencil)
		{
			//AssertExpresion(ClearValue.ClearMode == EClearMode::DepthStencil || ClearValue.ClearMode == EClearMode::Depth || IsTypelessFormat(Format));
			if (ClearValue.ClearMode == EClearMode::DepthStencil)
			{
				D3D12ClearValue.Format = FromFormat(MappingDepthStencilFormat(Format));
				AssertExpr(D3D12ClearValue.Format != DXGI_FORMAT_UNKNOWN);
				D3D12ClearValue.DepthStencil.Depth = ClearValue.Depth;
				D3D12ClearValue.DepthStencil.Stencil = ClearValue.Stencil;
			}
			else if (ClearValue.ClearMode == EClearMode::None)
			{
				D3D12ClearValue.Format = FromFormat(MappingDepthStencilFormat(Format));
				AssertExpr(D3D12ClearValue.Format != DXGI_FORMAT_UNKNOWN);
				D3D12ClearValue.DepthStencil.Depth = 0.0f;
				D3D12ClearValue.DepthStencil.Stencil = 0;
			}
			else {}
		}
		else { }
		return D3D12ClearValue;
	}

	forceinline D3D12_RESOURCE_DESC FromResourceDesc(const FResourceDesc & ResourceDesc, uint32 Alignment)
	{
		D3D12_RESOURCE_DESC D3D12ResourceDesc;
		D3D12ResourceDesc.Dimension = FromResourceDimension(ResourceDesc.Dimension);
		D3D12ResourceDesc.Alignment = Alignment;

		if (ResourceDesc.Dimension == EResourceDimension::Buffer)
		{
			D3D12ResourceDesc.Width = ResourceDesc.Size.Y > 1 ? (ResourceDesc.Size.X * ResourceDesc.Size.Y) : ResourceDesc.Size.X;
			D3D12ResourceDesc.Height = 1;
		}
		else
		{
			D3D12ResourceDesc.Width = ResourceDesc.Size.X;
			D3D12ResourceDesc.Height = ResourceDesc.Size.Y;
		}
		D3D12ResourceDesc.DepthOrArraySize = uint16(ResourceDesc.Layers);
		D3D12ResourceDesc.MipLevels = uint16(ResourceDesc.Mipmaps);
		D3D12ResourceDesc.Format = ResourceDesc.Dimension == EResourceDimension::Buffer ? DXGI_FORMAT_UNKNOWN : FromFormat(ResourceDesc.Format);
		D3D12ResourceDesc.SampleDesc.Count = ResourceDesc.MSAA.Level;
		D3D12ResourceDesc.SampleDesc.Quality = ResourceDesc.MSAA.Quality;
		D3D12ResourceDesc.Layout = ResourceDesc.Dimension == EResourceDimension::Buffer ? D3D12_TEXTURE_LAYOUT_ROW_MAJOR : D3D12_TEXTURE_LAYOUT_UNKNOWN;
		D3D12ResourceDesc.Flags = FromResourceUsage(ResourceDesc.Usage);
		return D3D12ResourceDesc;
	}

	forceinline D3D12_RESOURCE_DESC FromBufferDesc(const FBufferDesc & BufferDesc)
	{
		D3D12_RESOURCE_DESC D3D12ResourceDesc;
		D3D12ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		D3D12ResourceDesc.Alignment = 0;
		D3D12ResourceDesc.Width = BufferDesc.Size.X;
		D3D12ResourceDesc.Height = 1;
		D3D12ResourceDesc.DepthOrArraySize = uint16(BufferDesc.Size.Y);
		D3D12ResourceDesc.MipLevels = 0;
		D3D12ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		D3D12ResourceDesc.SampleDesc.Count = 0;
		D3D12ResourceDesc.SampleDesc.Quality = 0;
		D3D12ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		D3D12ResourceDesc.Flags = FromResourceUsage(BufferDesc.Usage);
		return D3D12ResourceDesc;
	}

	forceinline D3D12_RESOURCE_DESC FromTextureDesc(const FTextureDesc & TextureDesc)
	{
		D3D12_RESOURCE_DESC D3D12ResourceDesc;
		D3D12ResourceDesc.Dimension = FromResourceDimension(TextureDesc.Dimension);
		D3D12ResourceDesc.Alignment = 0;
		D3D12ResourceDesc.Width = TextureDesc.Size.X;
		D3D12ResourceDesc.Height = TextureDesc.Size.Y;
		D3D12ResourceDesc.DepthOrArraySize = uint16(TextureDesc.Depth);
		D3D12ResourceDesc.MipLevels = uint16(TextureDesc.MipLevels);
		D3D12ResourceDesc.Format = FromFormat(TextureDesc.Format);
		D3D12ResourceDesc.SampleDesc.Count = TextureDesc.MSAA.Level;
		D3D12ResourceDesc.SampleDesc.Quality = TextureDesc.MSAA.Quality;
		D3D12ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		D3D12ResourceDesc.Flags = FromResourceUsage(TextureDesc.Usage);
		return D3D12ResourceDesc;
	}
}
