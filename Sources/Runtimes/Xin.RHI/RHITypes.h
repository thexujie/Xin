#pragma once

#include "Xin.Core/Xin.Core.h"

#if XIN_WINDOWS
#ifdef _Xin_RHI_Module_
#define RHI_API __declspec(dllexport)
#else
#define RHI_API __declspec(dllimport)
#endif
#endif

namespace Xin::RHI
{
	using FGPUAddress = uint64;
	using FCPUAddress = byte *;
	using FDescriptorIndex = uintx;

	class IRHIHeap;
	using IRHIHeapRef = TReferPtr<IRHIHeap>;

	class IRHIResource;
	using IRHIResourceRef = TReferPtr<IRHIResource>;

	constexpr uint32 MaxFrameBackBuffers = 3;
	constexpr uint32 MaxVertexBuffers = 32;
	constexpr uint32 MaxShaderParameters = 32;
	constexpr uint32 MaxInputElements = 32;
	constexpr uint32 MaxRenderTargets = 8;
	constexpr uint32 MaxConstBuffers = 16;
	constexpr uint32 MaxShaderResources = 32;
	constexpr uint32 MaxViewports = 8;
	constexpr uint32 MaxScissorRects = 16;

	struct FViewport
	{
		float32 X = 0;
		float32 Y = 0;
		float32 Width = 0;
		float32 Height = 0;
		float32 NearZ = 0;
		float32 FarZ = 0;
	};

	class RHI_API IRHIObject : public ISharedRefer
	{
	public:
		virtual void SetDebugName(FStringV Name) { DebugName = Name ; }

	public:
		FString DebugName;
	};

	using IRHIObjectRef = TReferPtr<IRHIObject>;

	enum class EShaderStage
	{
		None = -1,
		Vertex = 0,
		Hull,
		Domain,
		Geometry,
		Pixel,
		Compute,
		Count,
	};

	constexpr uint32 MaxShaderStages = uint32(EShaderStage::Count);

	enum class EShaderRelevance
	{
		None = 0,
		Vertex =   0x0001,
		Hull =     0x0002,
		Domain =   0x0004,
		Geometry = 0x0008,
		Pixel =    0x0010,
		Compute =  0x0020,
		All = 0xFFFF,
	};

	constexpr bool IsShaderRelevanceStage(EShaderStage ShaderStage, EShaderRelevance ShaderRelevance)
	{
		switch (ShaderStage)
		{
		case EShaderStage::Vertex: return HasFlags(ShaderRelevance, EShaderRelevance::Vertex);
		case EShaderStage::Hull:return HasFlags(ShaderRelevance, EShaderRelevance::Hull);
		case EShaderStage::Domain:return HasFlags(ShaderRelevance, EShaderRelevance::Domain);
		case EShaderStage::Geometry:return HasFlags(ShaderRelevance, EShaderRelevance::Geometry);
		case EShaderStage::Pixel:return HasFlags(ShaderRelevance, EShaderRelevance::Pixel);
		case EShaderStage::Compute:return HasFlags(ShaderRelevance, EShaderRelevance::Compute);
		default: return false;
		}
	}

	constexpr EShaderRelevance ShaderRelevanceFormShaderStage(EShaderStage ShaderStage)
	{
		switch (ShaderStage)
		{
		case EShaderStage::Vertex: return EShaderRelevance::Vertex;
		case EShaderStage::Pixel: return EShaderRelevance::Pixel;
		case EShaderStage::Geometry: return EShaderRelevance::Geometry;
		case EShaderStage::Compute: return EShaderRelevance::Compute;
		case EShaderStage::Hull: return EShaderRelevance::Hull;
		case EShaderStage::Domain: return EShaderRelevance::Domain;
		default: return EShaderRelevance::None;
		}
	}

	enum class EIndexFormat
	{
		None = 0,
		Index16 = uint32(EFormat::UShort1),
		Index32 = uint32(EFormat::UInt1)
	};

	enum class EDescriptorType
	{
		None = -1,

		UniformBuffer = 0,
		StorageBuffer,

		UniformTexelBuffer,
		StorageTexelBuffer,

		SampledTexture,
		StorageTexture,

		Sampler,
		Count,
	};

	constexpr uint32 DescriptorTypeCount = uint32(EDescriptorType::Count);


	enum class EDescriptorFlag
	{
		None = 0,
		Static = 0x0001,
		Volatile = 0x0002,
		DescriptorVolatile = 0x0004,
	};

	enum class EDescriptorRangeFlag
	{
		None = 0,
		Static = 0x0001,
		Volatile = 0x0002,
		DescriptorVolatile = 0x0004,
	};

	enum class ESampleFilter
	{
		// MIN_MAG_MIP_POINT
		Point = 0,
		// MIN_MAG_LINEAR_MIP_POINT
		Linear,
		// MIN_MAG_MIP_LINEAR
		Trilinear,
		Anisotropic,
	};

	enum class EAddressMode
	{
		Clamp = 0,
		Wrap,
		Mirror,
		Border,
	};

	enum class EComparison
	{
		None = 0,
		Never,
		Always,
		Less,
		LessEqual,
		Equal,
		GreaterEqual,
		Greater,
		NotEqual,
	};

	enum class EBlendFactor
	{
		None = 0,
		Zero,
		One,
		SrcColor,
		SrcColorInv,
		SrcAlpha,
		SrcAlphaInv,
		DestColor,
		DestColorInv,
		DestAlpha,
		DestAlphaInv,
		BlendFactor,
		BlendFactorInv,
		Src1Color,
		Src1ColorInv,
		Src1Alpha,
		Src1AlphaInv,
	};

	enum class EBlendAlgorithm
	{
		Add = 0,
		Subtract,
		Min,
		Max,
	};

	enum class ECullMode
	{
		None = 0,
		// specifies that front-facing triangles are discarded
		Front,
		// specifies that back-facing triangles are discarded
		Back
	};

	enum class EPrimitiveTopology
	{
		None = 0,
		Point,
		Line,
		Triangle,
		Patch,
	};

	enum class ETopology
	{
		None = 0,
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
		PatchList,
	};

	enum class EResourceAspect
	{
		None = 0,
		ShaderResource,
		RenderTarget,
		DepthStencil,
		TexelAccess,
	};

	inline EFormat ResourceViewFormat(EFormat Format, EResourceAspect ResourceView)
	{
		if (ResourceView == EResourceAspect::DepthStencil)
		{
			switch (Format)
			{
			case EFormat::R16:
			case EFormat::R16F:
			case EFormat::R16UI:
			case EFormat::R16SI:
				return EFormat::D16UF;
			case EFormat::R32:
			case EFormat::R32F:
			case EFormat::R32UI:
			case EFormat::R32SI:
				return EFormat::D32F;
			default:
				break;
			}
		}
		else
		{
			switch (Format)
			{
			case EFormat::R16:
			case EFormat::D16UF:
				return EFormat::R16UF;
			case EFormat::R32:
			case EFormat::D32F:
				return EFormat::R32F;
			case EFormat::BC1:
				return EFormat::BC1UF;
			case EFormat::D24UF_S8UI:
				//return EFormat::R24G8;
				return EFormat::R24X8;
			default:
				break;
			}
		}
		return Format;
	}

	enum class EResourceViewDimension
	{
		None = 0,
		Buffer,
		Texture1D,
		Texture1DArray,
		Texture2D,
		Texture2DArray,
		Texture3D,
		TextureCube,
		TextureCubeArray,
	};

	enum class EDescriptorHeapType
	{
		ShaderResource = 0,
		RenderTarget,
		DepthStencil,
		Sampler,
	};

	constexpr uint32 DescriptorHeapTypeCount = uint32(EDescriptorHeapType::Sampler) + 1;

	enum class EResourceType
	{
		None = 0,
		ConstBuffer,
		ShaderResource,
		UnorderedAccess,
		RenderTarget,
		DepthStencil,
	};

	enum class EPipelineType
	{
		Graphics = 0,
		Compute,
	};

	enum class ECommandType
	{
		Direct = 0,
		Bundle,
		Compute,
		Copy,
	};

	constexpr uint32 ECommandTypeCount = uint32(ECommandType::Copy) + 1;

	enum class ECommandExecutorFlags
	{
		None = 0,
		DisableGPUTimeout = 0x0001,
	};

	struct FCommandQueueDesc
	{
		ECommandType Type = ECommandType::Direct;
		ECommandExecutorFlags Flags = ECommandExecutorFlags::None;
	};

	enum class EPresentMode
	{
		Discard = 0,
		Sequential,
		FlipSequential,
		FlipDiscard,
	};

	enum class EAllocStrategy
	{
		Commited = 0,
		Placed,
		Located
	};

	enum class EHeapUsage
	{
		None = 0,
		HostRead = 0x0001,
		HostWrite = 0x0002,
		HostMask = HostRead | HostWrite,

		MemorySysRAM = 0x0010,
		MemoryVRAM = 0x0020,
		MemoryMask = MemorySysRAM | MemoryVRAM,

		// tier 2
		// https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_heap_flags
		AllowBuffers = 0x0100,
		AllowTextures = 0x0200,
		AllowRTsDSs = 0x0400,
		AllowMask = AllowBuffers | AllowTextures | AllowRTsDSs,

		Interoperable = 0x10000,
		CrossAdapter = 0x20000,
		SupportMSAA = 0x40000,

		ZeroInitialize = 0x80000,
		NotResident = 0x100000,
	};

	enum class EHeapFlag
	{
		None = 0,

		CPU_NotAvailabe = 0x0001,
		CPU_WriteCombine = 0x0002,
		CPU_WriteBack = 0x0004,
		CPU_Mask = 0x000F,

		MemoryPool_L0 = 0x0010,
		MemoryPool_L1 = 0x0020,
		MemoryPool_Mask = 0x00F0,

		Allow_Buffers = 0x10000,
		Allow_Textures = 0x20000,
		Allow_RTsDSs = 0x40000,
		Allow_All = Allow_Buffers | Allow_Textures | Allow_RTsDSs,
		Allow_Mask = Allow_All,

		Interoperable = 0x100000,
		WriteWatch = 0x200000,
		CrossAdapter = 0x400000,
		ZeroInitialize = 0x800000,
	};

	enum class ECPUPageProperty
	{
		None = 0,
		NotAvailable,
		WriteCombine,
		WriteBack,
	};

	enum class EMemoryPool
	{
		None,
		L0,
		L1,
	};

	enum class EResourceDimension
	{
		None = 0,
		Buffer,
		Texture1D,
		Texture2D,
		Texture3D,
	};

	enum class EResourceAllocateFlag
	{
		None = 0,
		Located = 0x0001
	};

	enum class EResourceLoad
	{
		None = 0,
		Load,
		Clear,
		Discard
	};

	enum class EResourceStoreAction
	{
		None = 0,
		Discard,
	};

	enum class EResourceUsage
	{
		None = 0,

		UniformBuffer = 0x1,
		StorageBuffer = 0x2,
		UniformTexelBuffer = 0x4,
		StorageTexelBuffer = 0x8,

		VertexBuffer = 0x10,
		IndexBuffer = 0x20,
		IndirectBuffer = 0x40,

		RenderTarget = 0x80,
		DepthStencil = 0x100,

		SampledTexture = 0x200,
		StorageTexture = 0x400,

		CopyRead = 0x1000,
		CopyWrite = 0x2000,
		HostRead = 0x4000,
		HostWrite = 0x8000,
		ResolveRead = 0x10000,
		ResolveWrite = 0x20000,

		Interoperable = 0x0100000,
		Simultaneous = 0x0200000,
		CrossAdapter = 0x0400000,
		Transisent = 0x0800000,

		CopyMask = CopyRead | CopyWrite,
		HostMask = HostRead | HostWrite,
		ResolveMask = ResolveRead | ResolveWrite,
		BufferMask = UniformBuffer | StorageBuffer | UniformTexelBuffer | StorageTexelBuffer| VertexBuffer | IndexBuffer | IndirectBuffer,
		TargetMask = RenderTarget | DepthStencil,
		TexelMask = UniformTexelBuffer,
		TextureMask = RenderTarget | DepthStencil | TexelMask | SampledTexture | StorageTexture,

		WriteMask = TextureMask | CopyMask | ResolveMask,
	};

	enum class EResourceFlags
	{
		None = 0,

		/**
		 * mutable view format
		 */
		MutableFormat = 0x0001,
	};

	enum class ERHIAdapterFlags
	{
		None = 0,
		CacheCoherentUMA = 0x0001,
		TileBasedRender = 0x0002,
		Software = 0x0040,
	};

	enum class EResourceAccess
	{
		None = 0,

		UniformBuffer = 0x1,
		StorageBuffer = 0x2,
		UniformTexelBuffer = 0x4,
		StorageTexelBuffer = 0x8,

		VertexBuffer = 0x10,
		IndexBuffer = 0x20,
		IndirectBuffer = 0x40,

		SampledTexture = 0x80,
		StorageTexture = 0x100,

		RenderTarget = 0x200,
		DepthRead = 0x400,
		DepthWrite = 0x800,

		DepthStencil = DepthWrite | DepthRead,

		CopyRead = 0x1000,
		CopyWrite = 0x2000,
		ResolveRead = 0x4000,
		ResolveWrite = 0x8000,

		Present = 0x10000,
		Common = 0x20000,
		Generic = 0x40000,

		UsageMask = 0xFFFFF,

		VertexStage = 0x100000,
		HullStage = 0x200000,
		DomainStage = 0x400000,
		GeometryStage = 0x800000,
		PixelStage = 0x1000000,
		ComputeStage = 0x2000000,

		NonePixelStage = VertexStage | HullStage | DomainStage | GeometryStage | ComputeStage,

		PixelUniformTexelBuffer = PixelStage | UniformTexelBuffer,

		ComputeIndirectBuffer = ComputeStage | IndirectBuffer,
		ComputeUniformTexelBuffer = ComputeStage | UniformTexelBuffer,
		ComputeStorageTexelBuffer = ComputeStage | StorageTexelBuffer,

		VertexSampledTexutre = VertexStage | SampledTexture,
		HullSampledTexture = HullStage | SampledTexture,
		DomainSampledTexture = DomainStage | SampledTexture,
		GeometrySampledTexture = GeometryStage | SampledTexture,
		PixelSampledTexture = PixelStage | SampledTexture,
		ComputeSampledTexture = ComputeStage | SampledTexture,

		ComputeStorageBuffer = ComputeStage | StorageBuffer,
		ComputeStorageTexture = ComputeStage | StorageTexture,

		StagesMask = NonePixelStage | PixelStage,
		TextureMask = StagesMask | SampledTexture,

		TexelMask = UniformTexelBuffer | StorageTexelBuffer,
		StorageMask = StorageTexelBuffer | StorageTexture,
		CopyMask = CopyRead | CopyWrite | ResolveRead | ResolveWrite,
		WriteMask = RenderTarget | StorageTexelBuffer | StorageTexture | DepthWrite,
	};

	constexpr EResourceAccess GetDefaultResourceAccess(EResourceUsage ResourceUsage)
	{
		if (ResourceUsage & EResourceUsage::HostMask)
			return EResourceAccess::Generic;

		if (ResourceUsage & EResourceUsage::RenderTarget)
			return EResourceAccess::RenderTarget;
		if (ResourceUsage & EResourceUsage::DepthStencil)
			return EResourceAccess::DepthWrite;

		if (ResourceUsage & EResourceUsage::UniformBuffer)
			return EResourceAccess::UniformBuffer;
		if (ResourceUsage & EResourceUsage::StorageBuffer)
			return EResourceAccess::StorageBuffer;
		if (ResourceUsage & EResourceUsage::UniformTexelBuffer)
			return EResourceAccess::UniformTexelBuffer;
		if (ResourceUsage & EResourceUsage::StorageTexelBuffer)
			return EResourceAccess::StorageTexelBuffer;

		if (ResourceUsage & EResourceUsage::SampledTexture)
			return EResourceAccess::PixelSampledTexture;
		if (ResourceUsage & EResourceUsage::StorageTexture)
			return EResourceAccess::ComputeStorageTexture;
		if (ResourceUsage & EResourceUsage::VertexBuffer)
			return EResourceAccess::VertexBuffer;
		if (ResourceUsage & EResourceUsage::IndexBuffer)
			return EResourceAccess::IndexBuffer;

		return EResourceAccess::Generic;
	}

	constexpr EResourceUsage GetResourceUsageFromResourceAccess(EResourceAccess ResourceAccess)
	{
		EResourceUsage ResourceUsage = EResourceUsage::None;
		SetFlags(ResourceUsage, EResourceUsage::UniformBuffer, ResourceAccess & EResourceAccess::UniformBuffer);
		SetFlags(ResourceUsage, EResourceUsage::StorageBuffer, ResourceAccess & EResourceAccess::StorageBuffer);
		SetFlags(ResourceUsage, EResourceUsage::UniformTexelBuffer, ResourceAccess & EResourceAccess::UniformTexelBuffer);
		SetFlags(ResourceUsage, EResourceUsage::StorageTexelBuffer, ResourceAccess & EResourceAccess::StorageTexelBuffer);

		SetFlags(ResourceUsage, EResourceUsage::VertexBuffer, ResourceAccess & EResourceAccess::VertexBuffer);
		SetFlags(ResourceUsage, EResourceUsage::IndexBuffer, ResourceAccess & EResourceAccess::IndexBuffer);
		SetFlags(ResourceUsage, EResourceUsage::IndirectBuffer, ResourceAccess & (EResourceAccess::IndirectBuffer));

		SetFlags(ResourceUsage, EResourceUsage::SampledTexture, ResourceAccess & (EResourceAccess::SampledTexture));

		SetFlags(ResourceUsage, EResourceUsage::RenderTarget, ResourceAccess & EResourceAccess::RenderTarget);
		SetFlags(ResourceUsage, EResourceUsage::DepthStencil, ResourceAccess & (EResourceAccess::DepthStencil));
		SetFlags(ResourceUsage, EResourceUsage::StorageTexture, ResourceAccess & EResourceAccess::StorageTexture);

		SetFlags(ResourceUsage, EResourceUsage::CopyRead, ResourceAccess & EResourceAccess::CopyRead);
		SetFlags(ResourceUsage, EResourceUsage::CopyWrite, ResourceAccess & EResourceAccess::CopyWrite);

		SetFlags(ResourceUsage, EResourceUsage::ResolveRead, ResourceAccess & EResourceAccess::ResolveRead);
		SetFlags(ResourceUsage, EResourceUsage::ResolveWrite, ResourceAccess & EResourceAccess::ResolveWrite);

		return ResourceUsage;
	}

	constexpr EResourceAccess GetResourceAccessFromResourceUsage(EResourceUsage ResourceUsage)
	{
		EResourceAccess ResourceAccess = EResourceAccess::None;
		SetFlags(ResourceAccess, EResourceAccess::UniformBuffer, ResourceUsage & EResourceUsage::UniformBuffer);
		SetFlags(ResourceAccess, EResourceAccess::StorageBuffer, ResourceUsage & EResourceUsage::StorageBuffer);
		SetFlags(ResourceAccess, EResourceAccess::UniformTexelBuffer, ResourceUsage & EResourceUsage::UniformTexelBuffer);
		SetFlags(ResourceAccess, EResourceAccess::StorageTexelBuffer, ResourceUsage & EResourceUsage::StorageTexelBuffer);

		SetFlags(ResourceAccess, EResourceAccess::VertexBuffer, ResourceUsage & EResourceUsage::VertexBuffer);
		SetFlags(ResourceAccess, EResourceAccess::IndexBuffer, ResourceUsage & EResourceUsage::IndexBuffer);

		// missing shader stages.
		//SetFlags(ResourceAccess, EResourceAccess::SampledTexture, ResourceUsage & EResourceUsage::SampledTexture);

		SetFlags(ResourceAccess, EResourceAccess::RenderTarget, ResourceUsage & EResourceUsage::RenderTarget);
		SetFlags(ResourceAccess, EResourceAccess::DepthStencil, ResourceUsage & EResourceUsage::DepthStencil);
		SetFlags(ResourceAccess, EResourceAccess::StorageTexture, ResourceUsage & EResourceUsage::StorageTexture);

		SetFlags(ResourceAccess, EResourceAccess::CopyRead, ResourceUsage & EResourceUsage::CopyRead);
		SetFlags(ResourceAccess, EResourceAccess::CopyWrite, ResourceUsage & EResourceUsage::CopyWrite);

		SetFlags(ResourceAccess, EResourceAccess::ResolveRead, ResourceUsage & EResourceUsage::ResolveRead);
		SetFlags(ResourceAccess, EResourceAccess::ResolveWrite, ResourceUsage & EResourceUsage::ResolveWrite);

		return ResourceAccess;
	}

	constexpr bool IsResourceAccessCompatibleWithResourceUsage(EResourceAccess ResourceAccess, EResourceUsage ResourceUsage)
	{
		if (ResourceAccess == EResourceAccess::Common)
			return true;

		if (ResourceAccess == EResourceAccess::Present && ResourceUsage & EResourceUsage::RenderTarget)
			return true;

		if (ResourceAccess & EResourceAccess::CopyMask)
			return true;
		return ResourceUsage & GetResourceUsageFromResourceAccess(ResourceAccess);
	}

	enum class EWriteMasks
	{
		None = 0,
		Red = 0x1,
		Green = 0x2,
		Blue = 0x4,
		Alpha = 0x8,
		All = Red | Green | Blue | Alpha,
	};

	enum class EClearMode
	{
		None = 0,
		Color,
		DepthStencil,
	};

	struct FClearValue
	{
		EClearMode ClearMode = EClearMode::None;

		union
		{
			FColorF Color = Colors::None;

			struct
			{
				float32 Depth;
				uint8 Stencil;
			};
		};

		RHI_API static const FClearValue None;

		FClearValue() = default;
		FClearValue(const FColor & Color) : ClearMode(EClearMode::Color), Color(Color) {}
		FClearValue(const FColorF & Color) : ClearMode(EClearMode::Color), Color(Color) {}
		FClearValue(const FClearValue & Another) noexcept : ClearMode(Another.ClearMode)
		{
			switch (ClearMode)
			{
			case EClearMode::Color:
				Color = Another.Color;
				break;
			case EClearMode::DepthStencil:
				Depth = Another.Depth;
				Stencil = Another.Stencil;
				break;
			default:
				break;
			}
		}

		FClearValue(float32 Depth) : ClearMode(EClearMode::DepthStencil), Depth(Depth), Stencil(0) {}
		FClearValue(float32 Depth, uint8 Stencil) : ClearMode(EClearMode::DepthStencil), Depth(Depth), Stencil(Stencil) {}

		constexpr operator bool() const { return ClearMode != EClearMode::None; }

		FClearValue & operator =(const FClearValue & Another) noexcept
		{
			ClearMode = Another.ClearMode;
			switch (ClearMode)
			{
			case EClearMode::Color:
				Color = Another.Color;
				break;
			case EClearMode::DepthStencil:
				Depth = Another.Depth;
				Stencil = Another.Stencil;
				break;
			default:
				break;
			}
			return *this;
		}

		bool operator ==(const FClearValue & Another) const
		{
			if (ClearMode != Another.ClearMode)
				return false;

			switch (ClearMode)
			{
			case EClearMode::Color:
				return Color == Another.Color;
			case EClearMode::DepthStencil:
				return Depth == Another.Depth && Stencil == Another.Stencil;
			default:
				return ClearMode == Another.ClearMode;
			}
		}

		CompareCategoryT<EClearMode, FColorF, float32, uint8> operator <=>(const FClearValue & Another) const noexcept
		{
			if (ClearMode != Another.ClearMode)
				return ClearMode <=> Another.ClearMode;

			switch (ClearMode)
			{
			case EClearMode::Color:
				return Color <=> Another.Color;
			case EClearMode::DepthStencil:
				return Compare(Depth, Another.Depth, Stencil, Another.Stencil);
			default:
				return ClearMode <=> Another.ClearMode;
			}
		}
	};

	struct FMSAA
	{
		uint32 Level = 1;
		uint32 Quality = 0;

		bool operator ==(const FMSAA &) const = default;
		auto operator <=>(const FMSAA &) const noexcept = default;

		friend uintx Hash(const FMSAA & MSAA)
		{
			return HashCombine(MSAA.Level, MSAA.Quality);
		}
	};

	struct FRHIAdapterDesc
	{
		uint64 VendorId = 0;
		uint64 DeviceId = 0;
		FString Name;
		ERHIAdapterFlags Flags = ERHIAdapterFlags::None;
	};

	struct FResourceAllocationInfo
	{
		uint64 Size = 0;
		uint32 Alignment = 0;
	};

	struct FHeapType
	{
		EHeapUsage Usage = EHeapUsage::None;
		EHeapFlag Flags = EHeapFlag::None;
	};

	struct FHeapDesc
	{
		EHeapUsage Usage = EHeapUsage::None;
		uint64 Alignment = 0;
		uint64 Size = 0;
	};

	enum class EResourceFlag
	{
		None = 0,
	};

	struct FSurfaceFormat
	{
		EFormat Format = EFormat::None;
		EColorSpace ColorSpace = EColorSpace::sRGB;
	};

	struct FAllocateInfo
	{
		uint64 Size = 0;
		uint32 Alignment = 0;
		uint32 Type = 0;
	};

	struct FCopyableFootprint
	{
		EFormat Format = EFormat::None;

		uint64 Offset = 0;
		Size2U Size;
		uint32 Depth = 1;
		uint32 RowPitch = 0;
		uint32 RowCount = 0;
	};

	struct FSurfaceDesc
	{
		EFormat Format = EFormat::None;
		Size2U Size = { 0, 1 };
		EResourceUsage Usage = EResourceUsage::None;
	};

	struct FBufferDesc
	{
		Size2U Size = { 0, 1 };
		EResourceUsage Usage = EResourceUsage::None;

		EFormat Format = EFormat::None;
		EResourceAccess InitialAccess = EResourceAccess::None;
		FClearValue ClearValue;

		bool operator ==(const FBufferDesc &) const noexcept = default;
		auto operator <=>(const FBufferDesc &) const noexcept = default;

		FBufferDesc() noexcept = default;
		FBufferDesc(const FBufferDesc &) noexcept = default;
		FBufferDesc & operator =(const FBufferDesc &) noexcept = default;

		// Texture Desc Starts With (Format, Size)
		FBufferDesc(uint32 Size, EResourceUsage Usage, EResourceAccess InitialAccess = EResourceAccess::None, FClearValue ClearValue = { }) noexcept
			: Size(Size, 1)
			, Usage(Usage)
			, InitialAccess(InitialAccess)
			, ClearValue(ClearValue)
		{
		}

		FBufferDesc(Size2U Size, EFormat Format, EResourceUsage Usage, EResourceAccess InitialAccess = EResourceAccess::None, FClearValue ClearValue = { }) noexcept
			: Size(Size)
			, Usage(Usage)
			, Format(Format)
			, InitialAccess(InitialAccess)
			, ClearValue(ClearValue)
		{
		}
	};

	struct FTextureDesc
	{
		EResourceDimension Dimension = EResourceDimension::None;
		EFormat Format = EFormat::None;
		EResourceUsage Usage = EResourceUsage::None;

		Size2U Size = { 0, 1 };
		uint32 MipLevels = 1;
		uint32 Depth = 1;
		FMSAA MSAA;

		FClearValue ClearValue;

		bool operator ==(const FTextureDesc &) const noexcept = default;
		auto operator <=>(const FTextureDesc &) const noexcept = default;

		FTextureDesc() noexcept = default;
		FTextureDesc(const FTextureDesc &) noexcept = default;
		FTextureDesc & operator =(const FTextureDesc &) noexcept = default;

		FTextureDesc(EFormat Format, Size2U Size, EResourceUsage Usage, uint32 Depth, uint32 MipLevels, FClearValue ClearValue = { }) noexcept
			: Dimension(EResourceDimension::Texture2D)
			, Format(Format)
			, Usage(Usage)
			, Size(Size)
			, MipLevels(MipLevels)
			, Depth(Depth)
			//, MSAA(TextureDesc.MSAA)
			, ClearValue(ClearValue)
		{
		}

		FTextureDesc(EFormat Format, Size2U Size, EResourceUsage Usage, FClearValue ClearValue = { }) noexcept
			: Dimension(EResourceDimension::Texture2D)
			, Format(Format)
			, Usage(Usage)
			, Size(Size)
			, MipLevels(1)
			, Depth(1)
			//, MSAA(TextureDesc.MSAA)
			, ClearValue(ClearValue)
		{
		}

		FTextureDesc(EFormat Format, Size4U Size, EResourceUsage Usage, FClearValue ClearValue = { }) noexcept
			: Dimension(EResourceDimension::Texture2D)
			, Format(Format)
			, Usage(Usage)
			, Size(Size.XY)
			, MipLevels(Size.Z ? Size.Z : 1)
			, Depth(Size.W ? Size.W : 1)
			//, MSAA(TextureDesc.MSAA)
			, ClearValue(ClearValue)
		{
		}
	};


	struct FResourceDesc
	{
		EResourceDimension Dimension = EResourceDimension::None;
		EResourceUsage Usage = EResourceUsage::None;
		EFormat Format = EFormat::None;

		Size2U Size = { 0, 1 };
		uint32 Layers = 1;
		uint32 Mipmaps = 1;
		FMSAA MSAA;

		FClearValue ClearValue;

		EResourceFlags Flags = EResourceFlags::None;

		bool operator ==(const FResourceDesc &) const noexcept = default;
		auto operator <=>(const FResourceDesc &) const noexcept = default;

		FResourceDesc() noexcept = default;
		FResourceDesc(const FResourceDesc &) noexcept = default;
		FResourceDesc & operator =(const FResourceDesc &) noexcept = default;

		FResourceDesc(const FBufferDesc & BufferDesc)
			: Dimension(EResourceDimension::Buffer)
			, Usage(BufferDesc.Usage)
			, Format(BufferDesc.Format)
			, Size(BufferDesc.Size)
			, Layers(1)
			, Mipmaps(1)
			, ClearValue(BufferDesc.ClearValue)
		{
		}

		operator FBufferDesc() const
		{
			FBufferDesc BufferDesc;
			BufferDesc.Size = Size;
			BufferDesc.Usage = Usage;
			BufferDesc.Format = Format;
			BufferDesc.ClearValue = ClearValue;
			return BufferDesc;
		}

		FResourceDesc(const FTextureDesc & TextureDesc)
			: Dimension(TextureDesc.Dimension)
			, Usage(TextureDesc.Usage)
			, Format(TextureDesc.Format)
			, Size(TextureDesc.Size)
			, Layers(TextureDesc.Depth)
			, Mipmaps(TextureDesc.MipLevels)
			, MSAA(TextureDesc.MSAA)
			, ClearValue(TextureDesc.ClearValue)
		{
		}

		operator FTextureDesc() const
		{
			FTextureDesc TextureDesc;
			TextureDesc.Dimension = Dimension;
			TextureDesc.Format = Format;
			TextureDesc.Usage = Usage;
			TextureDesc.Size = Size;
			TextureDesc.Depth = Layers;
			TextureDesc.MipLevels = Mipmaps;
			TextureDesc.MSAA = MSAA;
			TextureDesc.ClearValue = ClearValue;
			return TextureDesc;
		}
	};

	enum class EDescriptorHeapFlag
	{
		None = 0,
		ShaderVisible = 0x0001,
	};

	struct FResourceViewDesc
	{
		EFormat Format = EFormat::None;

		// structured buffer (FirstElement, ElementCount)
		// texture array(FirstElement, ElementCount)
		uint32 ElementIndex = 0;
		uint32 ElementCount = 0;

		// > 0  for structured
		uint32 StructureStride = 0;

		// != UInt32Max for subresource
		uint32 SubresourceIndex = NullIndex;

		FResourceViewDesc() = default;
		FResourceViewDesc(uint32 SubresourceIndex, const FResourceDesc & ResourceDesc) : Format(ResourceDesc.Format), SubresourceIndex(SubresourceIndex) {}
		FResourceViewDesc(EFormat Format, uint32 SubresourceIndex = NullIndex) : Format(Format), SubresourceIndex(SubresourceIndex) {}
		bool operator ==(const FResourceViewDesc &) const noexcept = default;
		auto operator <=>(const FResourceViewDesc &) const noexcept = default;
	};

	struct FDescriptorHeapDesc
	{
		EDescriptorHeapType Type = EDescriptorHeapType::ShaderResource;
		uint32 Size = 0;
		EDescriptorHeapFlag Flags = EDescriptorHeapFlag::None;
	};

	struct FSwapChainDesc
	{
		bytep WindowHandle = NullAddress;
		uint32 NumBuffers = 2;

		EFormat Format = EFormat::BGRA;
		SizeU Size;
		EPresentMode SwapEffect = EPresentMode::FlipDiscard;
		FMSAA MSAA = { 1, 0 };

		ECommandExecutorFlags Flags;
	};

	//VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK = 0,
 //   VK_BORDER_COLOR_INT_TRANSPARENT_BLACK = 1,
 //   VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK = 2,
 //   VK_BORDER_COLOR_INT_OPAQUE_BLACK = 3,
 //   VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE = 4,

	enum class EBorderColor
	{
		TransparentBlack = 0, // 0 0 0 0
		OpaqueBlack,
		OpaqueWhite,
	};

	struct FSamplerDesc
	{
		EShaderRelevance ShaderRelevance = EShaderRelevance::None;
		uint32 BindingIndex = 0;
		uint32 BindingSpace = 0;

		ESampleFilter SampleFilter = ESampleFilter::Point;
		EAddressMode AddressModeU = EAddressMode::Clamp;
		EAddressMode AddressModeV = EAddressMode::Clamp;
		EAddressMode AddressModeW = EAddressMode::Clamp;
		float32 MipLODBias = 0;
		uint32 MaxAnisotropy = 0;
		EComparison Comparison = EComparison::None;
		float32 MinLOD = 0;
		float32 MaxLOD = 0;
		EBorderColor BorderColor = EBorderColor::TransparentBlack;

		auto operator <=>(const FSamplerDesc &) const noexcept = default;

		friend uintx Hash(const FSamplerDesc & SamplerDesc)
		{
			return HashCombine(SamplerDesc.ShaderRelevance, SamplerDesc.BindingIndex, SamplerDesc.BindingSpace,
				SamplerDesc.SampleFilter, SamplerDesc.AddressModeU, SamplerDesc.AddressModeV, SamplerDesc.AddressModeW,
				SamplerDesc.MipLODBias, SamplerDesc.MaxAnisotropy, SamplerDesc.Comparison, SamplerDesc.MinLOD, SamplerDesc.MaxLOD,
				SamplerDesc.BorderColor);
		}
	};

	struct FDescriptorRangeDesc
	{
		EDescriptorType Type = EDescriptorType::UniformBuffer;
		uint32 NumDescriptors = 1;
		uint32 RegisterIndex = 0;
		uint32 RegisterSpace = 0;
		EDescriptorRangeFlag Flags = EDescriptorRangeFlag::None;

		uint32 PacketOffset = UInt32Max;

		auto operator <=>(const FDescriptorRangeDesc &) const noexcept = default;

		friend uintx Hash(const FDescriptorRangeDesc & DescriptorRangeDesc)
		{
			return HashCombine(DescriptorRangeDesc.Type, DescriptorRangeDesc.NumDescriptors, DescriptorRangeDesc.RegisterIndex, DescriptorRangeDesc.RegisterSpace, DescriptorRangeDesc.Flags, DescriptorRangeDesc.PacketOffset);
		}
	};

	enum class EInputSemantic
	{
		None = 0,
		Position,
		Texcoord,
		Normal,
		Tangent,
		BiTangent,

		Custom0,
		Custom1,
		Custom2,
		Custom3,
		Custom4,
		Custom5,
		Custom6,
		Custom7,
		Custom8,
		Custom9,
		Count,
	};

	constexpr uint32 InputSemanticCount = (uint32)EInputSemantic::Count;

	constexpr TArray<FStringV, InputSemanticCount> InputSemanticNames =
	{
		u8"None"V,
		u8"Position"V,
		u8"Texcoord"V,
		u8"Normal"V,
		u8"Tangent"V,
		u8"BiTangent"V,
	};

	struct FInputElement
	{
		EInputSemantic SemanticName = EInputSemantic::None;
		uint32 SemanticIndex = 0;
		EFormat Format = EFormat::None;
		uint32 StreamIndex = 0;
		uint32 ByteOffset = 0;

		auto operator <=>(const FInputElement &) const noexcept = default;

		friend uintx Hash(const FInputElement & InputElement)
		{
			return HashCombine(InputElement.SemanticName, InputElement.SemanticIndex, InputElement.Format, InputElement.StreamIndex, InputElement.ByteOffset);
		}
	};

	struct FInputStream
	{
		uint32 Stride = 0;
		bool Instanced = false;

		auto operator <=>(const FInputStream &) const noexcept = default;

		friend uintx Hash(const FInputStream & InputStream)
		{
			return HashCombine(InputStream.Stride, InputStream.Instanced);
		}
	};

	constexpr uint32 MaxInputStreams = 8;

	struct FInputFormat
	{
		EPrimitiveTopology PrimitiveTopology = EPrimitiveTopology::None;
		TArray<FInputStream, MaxInputStreams> Streams {};
		TArray<FInputElement, MaxInputElements> Elements {};

		auto operator <=>(const FInputFormat &) const noexcept = default;

		friend uintx Hash(const FInputFormat & InputFormat)
		{
			uintx HashCode = 0;
			for (uint32 InputStreamIndex : IRange(MaxInputStreams))
				HashCode = HashCombineValue(HashCode, HashCombine(InputFormat.Streams[InputStreamIndex]));
			for (uint32 InputElementIndex : IRange(MaxInputElements))
				HashCode = HashCombineValue(HashCode, HashCombine(InputFormat.Elements[InputElementIndex]));
			return HashCode;
		}
	};

	struct FDepthStencilState
	{
		bool DepthTest = false;
		bool DepthWrite = false;
		EComparison DepthComparison = EComparison::LessEqual;
		bool StencilTest = false;
		uint32 StencilRef = 0;

		auto operator <=>(const FDepthStencilState &) const = default;

		RHI_API static const FDepthStencilState Disabled;
		RHI_API static const FDepthStencilState Default;
		RHI_API static const FDepthStencilState DefaultReversedZ;

		FDepthStencilState() = default;
		FDepthStencilState(EComparison DepthComparison, bool DepthWrite) : DepthTest(DepthComparison != EComparison::None), DepthWrite(DepthWrite), DepthComparison(DepthComparison) {}
	};

	struct FBlendMethod
	{
		bool Enable = false;

		EBlendFactor SrcColor = EBlendFactor::One;
		EBlendFactor DstColor = EBlendFactor::Zero;
		EBlendAlgorithm ColorAlgorithm = EBlendAlgorithm::Add;

		EBlendFactor SrcAlpha = EBlendFactor::One;
		EBlendFactor DstAlpha = EBlendFactor::Zero;
		EBlendAlgorithm AlphaAlgorithm = EBlendAlgorithm::Add;

		EWriteMasks WriteMasks = EWriteMasks::All;

		auto operator <=>(const FBlendMethod &) const noexcept = default;

		RHI_API static const FBlendMethod Default;
		RHI_API static const FBlendMethod Transparent;
		RHI_API static const FBlendMethod Premultiplied;
	};

	struct FBlendState
	{
		bool AlphaToCoverage = false;
		bool IndependentBlend = false;
		TArray<float, 4> BlendFactor = { 1.0f, 1.0f, 1.0f, 1.0f };

		TArray<FBlendMethod, MaxRenderTargets> RenderTargets {};

		auto operator <=>(const FBlendState &) const noexcept = default;

		friend uintx Hash(const FBlendState & BlendState)
		{
			return HashCombine(
				BlendState.AlphaToCoverage,
				BlendState.IndependentBlend,
				BlendState.BlendFactor,
				BlendState.RenderTargets
			);
		}

		RHI_API static const FBlendState Default;
		RHI_API static const FBlendState Transparent;
		RHI_API static const FBlendState Premultiplied;
	};

	struct FRasterizeState
	{
		bool Wireframe = false;

		ECullMode CullMode = ECullMode::Back;
		bool FrontCCW = false;

		bool DepthClip = false;
		int32 DepthBias = 0;
		float32 DepthBiasClamp = 0.0f;
		float32 SlopScaledDepthBias = 0.0f;

		bool LineAA = false;
		bool MSAA = false;

		auto operator <=>(const FRasterizeState &) const noexcept = default;

		friend uintx Hash(const FRasterizeState & RasterizeState)
		{
			return HashCombine(
				RasterizeState.Wireframe,
				RasterizeState.CullMode,
				RasterizeState.FrontCCW,
				RasterizeState.DepthBias,
				RasterizeState.DepthBiasClamp,
				RasterizeState.SlopScaledDepthBias,
				RasterizeState.DepthClip,
				RasterizeState.MSAA
			);
		}

		RHI_API static const FRasterizeState Default;
		RHI_API static const FRasterizeState DefaultWireframe;
	};

	struct FOutputFormat
	{
		TArray<EFormat, MaxRenderTargets> RenderTargets {};
		FMSAA MSAA;
		EFormat DepthStencil = EFormat::None;
		uint32 SampleMask = 0xFFFFFFFF;

		auto operator <=>(const FOutputFormat &) const noexcept = default;

		friend uintx Hash(const FOutputFormat & OutputFormat)
		{
			uintx HashCode = 0;
			for (uint32 RenderTargetIndex : IRange(MaxRenderTargets))
				HashCode = HashCombineValue(HashCode, HashCombine(OutputFormat.RenderTargets[RenderTargetIndex]));
			return HashAppend(HashCode, OutputFormat.MSAA, OutputFormat.DepthStencil, OutputFormat.SampleMask);
		}
	};

	struct FRHILocation
	{
		bool operator ==(const FRHILocation &) const noexcept = default;

		uint64 Offset = 0;
		uint64 Size = 0;
		uint64 BlockSize = 0;
		uint32 Alignment = 0;
	};

	struct FRHIAddress
	{
		FCPUAddress CPUAddress = nullptr;
		FGPUAddress GPUAddress = 0;

		FRHIAddress() = default;
		FRHIAddress(NullptrT) {}
		FRHIAddress(FCPUAddress CPUAddress, FGPUAddress GPUAddress) : CPUAddress(CPUAddress), GPUAddress(GPUAddress) {}

		FRHIAddress & operator =(NullptrT)
		{
			CPUAddress = nullptr;
			GPUAddress = 0;
			return *this;
		}

		bool Offline() const noexcept { return CPUAddress != nullptr; }
		bool Online() const noexcept { return GPUAddress != 0; }
		bool Valid() const noexcept { return CPUAddress != nullptr || GPUAddress != 0; }
		explicit operator bool() const noexcept { return Valid(); }

		bool operator ==(const FRHIAddress &) const noexcept = default;
		FRHIAddress operator +(uint64 Offset) const noexcept { return FRHIAddress { CPUAddress ? CPUAddress + Offset : nullptr, GPUAddress ? GPUAddress + Offset : 0 }; }
		FRHIAddress operator -(uint64 Offset) const noexcept { return FRHIAddress { CPUAddress ? CPUAddress - Offset : nullptr, GPUAddress ? GPUAddress - Offset : 0 }; }
	};

	struct FRHIBlock
	{
		FRHIAddress BaseAddress;
		FRHILocation Location;

		FRHIBlock() = default;
		FRHIBlock(NullptrT) {}
		FRHIBlock(FRHIAddress BaseAddress, FRHILocation Location) : BaseAddress(BaseAddress), Location(Location) {}
		bool operator ==(const FRHIBlock &) const noexcept = default;

		FRHIBlock & operator =(NullptrT)
		{
			BaseAddress = nullptr;
			Location = {};
			return *this;
		}

		explicit operator bool() const noexcept { return BaseAddress.Valid() && Location.Size; }
	};

	struct FRHIMemory
	{
		class IHeapInstance * Instance = nullptr;

		IRHIHeapRef PlaceHeap;
		FRHILocation Location;

		explicit operator bool() const { return Instance != nullptr; }
	};

	struct FTextureRegion
	{
		EFormat Format = EFormat::None;
		uint32 Offset = 0;
		Size2U Size;
		uint32 Depth = 1;
		uint32 RowPitch = 0;
		uint32 RowCount = 0;
	};

	enum class EFenceFlag
	{
		None = 0,
		Shared = 0x1
	};

	struct FCopyableSubresource
	{
		EFormat Format = EFormat::None;
		Size2U Size;
		uint32 Depth = 1;
		uint32 RowPitch = 0;
		uint32 RowCount = 0;
	};

	enum class EIndirectParameterType
	{
		None = 0,
		Draw,
		DrawIndexed,
		Dispatch,
		VertexBufferView,
		IndexBufferView,
		Constannt,
		CBV,
		SRV,
		UAV,
		DispatchRays,
		DispatchMesh,
	};

	struct FIndirectParameter
	{
		EIndirectParameterType Type = EIndirectParameterType::None;

		union
		{
			struct
			{
				uint32 SlotIndex = 0;
			} VertexBuffer;

			struct
			{
				uint32 RootIndex;
			} CBVSRVUAV;
		};

		auto operator <=>(const FIndirectParameter & Another) const noexcept
		{
			if (Type != Another.Type)
				return Type <=> Another.Type;

			switch (Type)
			{
			case EIndirectParameterType::VertexBufferView:
				return VertexBuffer.SlotIndex <=> Another.VertexBuffer.SlotIndex;
			case EIndirectParameterType::CBV:
			case EIndirectParameterType::SRV:
			case EIndirectParameterType::UAV:
				return CBVSRVUAV.RootIndex <=> Another.CBVSRVUAV.RootIndex;
			default:
				return Type <=> Another.Type;
			}
		}

		friend uintx Hash(const FIndirectParameter & Parameter)
		{
			switch (Parameter.Type)
			{
			case EIndirectParameterType::VertexBufferView:
				return HashCombine(Parameter.Type, Parameter.VertexBuffer.SlotIndex);
			case EIndirectParameterType::CBV:
			case EIndirectParameterType::SRV:
			case EIndirectParameterType::UAV:
				return HashCombine(Parameter.Type, Parameter.CBVSRVUAV.RootIndex);
			default:
				return Xin::Hash(Parameter.Type);
			}
		}
	};

	struct FShaderParameter
	{
		FName BindingName;
		EShaderStage ShaderStage = EShaderStage::None;
		EDescriptorType BindingType = EDescriptorType::None;
		uint32 DescriptorIndex = 0;
		uint32 BindingIndex = 0;
		uint32 BindingSpace = 0;

		friend uintx Hash(const FShaderParameter & This) { return HashCombine(This.BindingName, This.ShaderStage, This.BindingType, This.DescriptorIndex, This.BindingIndex, This.BindingSpace); }
	};

	struct FIndirectSignature
	{
		auto operator <=>(const FIndirectSignature &) const noexcept = default;

		uint32 ParametersStride = 0; // Use 0 to generate automaticly
		TList<FIndirectParameter> Parameters;
		friend uintx Hash(const FIndirectSignature & IndirectSignature) { return HashCombine(IndirectSignature.ParametersStride, IndirectSignature.Parameters); }
	};

	struct FReflectParameter
	{
		EDescriptorType BindingType = EDescriptorType::None;
		EResourceDimension ResourceDimension = EResourceDimension::None;
		EShaderStage ShaderStage = EShaderStage::None;
		FString Name;
		uint32 BindingIndex = NullIndex;
		uint32 BindingSpace = 0;
		EFormat Format = EFormat::None;

		auto operator <=>(const FReflectParameter &) const noexcept = default;
		uintx Hash() const { return HashCombine(BindingType, ShaderStage, Name, BindingIndex, Format); }
		bool Bound() const { return BindingIndex != NullIndex; }

		friend IDataStream & operator >>(IDataStream & Stream, FReflectParameter & This)
		{
			Stream >> TWrappedValue<uint8, EDescriptorType>(This.BindingType);
			Stream >> TWrappedValue<uint8, EResourceDimension>(This.ResourceDimension);
			Stream >> This.ShaderStage;
			Stream >> This.Name;
			Stream >> This.BindingIndex;
			Stream >> This.BindingSpace;
			Stream >> This.Format;
			return Stream;
		}

		friend IDataStream & operator <<(IDataStream & Stream, const FReflectParameter & This)
		{
			Stream << TWrappedValue<uint8, const EDescriptorType>(This.BindingType);
			Stream << TWrappedValue<uint8, const EResourceDimension>(This.ResourceDimension);
			Stream << This.ShaderStage;
			Stream << This.Name;
			Stream << This.BindingIndex;
			Stream << This.BindingSpace;
			Stream << This.Format;
			return Stream;
		}
	};


	struct FReflectVariable
	{
		FString Name;
		uint32 BindingIndex = 0;

		auto operator <=>(const FReflectVariable &) const noexcept = default;
		bool operator ==(const FReflectVariable &) const noexcept = default;

		uintx Hash() const { return HashCombine(Name, BindingIndex); }

		friend IDataStream & operator >>(IDataStream & Stream, FReflectVariable & This)
		{
			Stream >> This.Name;
			Stream >> This.BindingIndex;
			return Stream;
		}

		friend IDataStream & operator <<(IDataStream & Stream, const FReflectVariable & This)
		{
			Stream << This.Name;
			Stream << This.BindingIndex;
			return Stream;
		}
	};

	class FReflectParameters
	{
	public:
		TList<FReflectVariable> InputVariables;
		TList<FReflectVariable> OutputVariables;
		TList<FReflectParameter> Parameters;

		auto operator <=>(const FReflectParameters &) const noexcept = default;
		bool operator ==(const FReflectParameters &) const noexcept = default;

		const FReflectParameter & FindParameter(EDescriptorType DescriptorType, FStringV Name) const
		{
			for (const FReflectParameter & ReflectParameter : Parameters)
			{
				if (ReflectParameter.BindingType == DescriptorType && ReflectParameter.Name == Name)
					return ReflectParameter;
			}

			static FReflectParameter UnboundParameter { EDescriptorType::None, EResourceDimension::None, EShaderStage::Vertex, FString::None, NullIndex };
			return UnboundParameter;
		}

		uintx Hash() const { return HashCombine(InputVariables, Parameters); }

		friend IDataStream & operator >>(IDataStream & Stream, FReflectParameters & This)
		{
			Stream >> This.InputVariables;
			Stream >> This.Parameters;
			return Stream;
		}

		friend IDataStream & operator <<(IDataStream & Stream, const FReflectParameters & This)
		{
			Stream << This.InputVariables;
			Stream << This.Parameters;
			return Stream;
		}
	};

	enum class ETextureCompression
	{
		None = 0,
		DXTC,
		ASTC,
	};

	constexpr FStringV GetTextureCompressionName(ETextureCompression TextureCompression)
	{
		switch (TextureCompression)
		{
		case ETextureCompression::DXTC: return u8"DXTC"V;
		case ETextureCompression::ASTC: return u8"ASTC"V;
		default: return u8"None"V;
		}
	}

	constexpr FStringV GetTextureCompressionFileExtention(ETextureCompression TextureCompression)
	{
		switch (TextureCompression)
		{
		case ETextureCompression::DXTC: return u8".dxtc"V;
		case ETextureCompression::ASTC: return u8".astc"V;
		default: return u8".none"V;
		}
	}
}
