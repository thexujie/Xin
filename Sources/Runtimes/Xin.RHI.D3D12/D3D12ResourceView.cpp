#include "PCH.h"
#include "D3D12ResourceView.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

namespace Xin::RHI::D3D12
{
	FD3D12ResourceView::FD3D12ResourceView(FD3D12Resource & D3D12Resource, uint32 SubresourceIndex)
		: IRHIResourceView(D3D12Resource, FResourceViewDesc(SubresourceIndex, D3D12Resource.ResourceDesc)), D3D12Resource(D3D12Resource)
	{
		if (D3D12Resource.ResourceDesc.Dimension == EResourceDimension::Buffer)
		{
			CreateResourceViewInnner_SRV();
			CreateResourceViewInnner_UAV();
		}
		else if (D3D12Resource.ResourceDesc.Dimension == EResourceDimension::Texture2D)
		{
			CreateResourceViewInnner_SRV();
			CreateResourceViewInnner_RTV();
			CreateResourceViewInnner_DSV();
			CreateResourceViewInnner_UAV();
		}
		else
		{

		}
	}

	FD3D12ResourceView::FD3D12ResourceView(FD3D12Resource & D3D12Resource, const FResourceViewDesc & ResourceViewDesc)
		: IRHIResourceView(D3D12Resource, ResourceViewDesc), D3D12Resource(D3D12Resource)
	{
		if (D3D12Resource.ResourceDesc.Dimension == EResourceDimension::Buffer)
		{
			CreateResourceViewInnner_SRV();
			CreateResourceViewInnner_UAV();
		}
		else if (D3D12Resource.ResourceDesc.Dimension == EResourceDimension::Texture2D)
		{
			CreateResourceViewInnner_SRV();
			CreateResourceViewInnner_RTV();
			CreateResourceViewInnner_DSV();
			CreateResourceViewInnner_UAV();
		}
		else
		{

		}
	}

	void FD3D12ResourceView::CreateResourceViewInnner_SRV()
	{
		if (!AnyFlags(D3D12Resource.ResourceDesc.Usage, EResourceUsage::StorageBuffer | EResourceUsage::UniformTexelBuffer | EResourceUsage::StorageTexelBuffer | EResourceUsage::SampledTexture))
			return;

		D3D12Resource.D3D12Devcie.AllocateOfflineDescriptors(EResourceAspect::ShaderResource, SRV, 1);
#if D3D12_HEAP_DEBUG
		SRV.Allocator->SetBlockObject(SRV.BlockLocation, this);
#endif

		D3D12_SHADER_RESOURCE_VIEW_DESC D3D12ShaderResourceViewDesc = {};
		D3D12ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		switch(D3D12Resource.ResourceDesc.Dimension)
		{
		case EResourceDimension::Buffer:
			D3D12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			D3D12ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			D3D12ShaderResourceViewDesc.Buffer.FirstElement = 0;
			if (D3D12Resource.ResourceDesc.Format == EFormat::None)
			{
				// Raw buffer, MUST be R32_TYPELESS
				D3D12ShaderResourceViewDesc.Format = DXGI_FORMAT_R32_TYPELESS;
				D3D12ShaderResourceViewDesc.Buffer.StructureByteStride = 0;
				D3D12ShaderResourceViewDesc.Buffer.NumElements = D3D12Resource.ResourceDesc.Size.X * D3D12Resource.ResourceDesc.Size.Y / 4;
				D3D12ShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
			}
			else
			{
				AssertExpr(GetFormatBytesPerPixel(D3D12Resource.ResourceDesc.Format) == D3D12Resource.ResourceDesc.Size.Y);
				D3D12ShaderResourceViewDesc.Format = FromFormat(D3D12Resource.ResourceDesc.Format);
				D3D12ShaderResourceViewDesc.Buffer.StructureByteStride = 0;
				D3D12ShaderResourceViewDesc.Buffer.FirstElement = 0;
				D3D12ShaderResourceViewDesc.Buffer.NumElements = D3D12Resource.ResourceDesc.Size.X;
				D3D12ShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			}
			break;
		case EResourceDimension::Texture1D:
			D3D12ShaderResourceViewDesc.Format = FromFormatSRV(D3D12Resource.ResourceDesc.Format);
			D3D12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
			break;
		case EResourceDimension::Texture2D:
			D3D12ShaderResourceViewDesc.Format = FromFormatSRV(D3D12Resource.ResourceDesc.Format);
			if (D3D12Resource.ResourceDesc.Layers > 1)
			{
				if (ViewDesc.SubresourceIndex != NullIndex)
				{
					D3D12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
					D3D12ShaderResourceViewDesc.Texture2DArray.MipLevels = UInt32Max;
					D3D12ShaderResourceViewDesc.Texture2DArray.FirstArraySlice = ViewDesc.SubresourceIndex;
					D3D12ShaderResourceViewDesc.Texture2DArray.ArraySize = 1;
				}
				else
				{
					D3D12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
					D3D12ShaderResourceViewDesc.Texture2DArray.MipLevels = UInt32Max;
					D3D12ShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
					D3D12ShaderResourceViewDesc.Texture2DArray.ArraySize = D3D12Resource.ResourceDesc.Layers;
				}
			}
			else
			{
				D3D12ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				D3D12ShaderResourceViewDesc.Texture2D.MipLevels = UInt32Max;
			}
			break;
		default:
			AssertFailed(u8"Invalid D3D12ResourceDesc.Dimension for CreateShaderResourceView");
			break;
		}
		D3D12Resource.D3D12Devcie.d3d12Device->CreateShaderResourceView(D3D12Resource.d3d12Resource, &D3D12ShaderResourceViewDesc, { SIZE_T(SRV.Address.CPUAddress) });
	}

	void FD3D12ResourceView::CreateResourceViewInnner_RTV()
	{
		if (!AnyFlags(D3D12Resource.ResourceDesc.Usage, EResourceUsage::RenderTarget))
			return;

		D3D12Resource.D3D12Devcie.AllocateOfflineDescriptors(EResourceAspect::RenderTarget, RTV, 1);
#if D3D12_HEAP_DEBUG
		RTV.Allocator->SetBlockObject(RTV.BlockLocation, this);
#endif

		D3D12_RENDER_TARGET_VIEW_DESC D3D12RenderTargetViewDesc = {};
		D3D12RenderTargetViewDesc.Format = FromFormat(D3D12Resource.ResourceDesc.Format);

		switch (D3D12Resource.ResourceDesc.Dimension)
		{
		case EResourceDimension::Texture1D:
			D3D12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
			break;
		case EResourceDimension::Texture2D:
			if (D3D12Resource.ResourceDesc.Layers > 1)
			{
				if (ViewDesc.SubresourceIndex != NullIndex)
				{
					D3D12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
					D3D12RenderTargetViewDesc.Texture2DArray.MipSlice = 0;
					D3D12RenderTargetViewDesc.Texture2DArray.FirstArraySlice = ViewDesc.SubresourceIndex;
					D3D12RenderTargetViewDesc.Texture2DArray.ArraySize = 1;
				}
				else
				{
					D3D12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
					D3D12RenderTargetViewDesc.Texture2DArray.MipSlice = 0;
					D3D12RenderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
					D3D12RenderTargetViewDesc.Texture2DArray.ArraySize = D3D12Resource.ResourceDesc.Layers;
				}
			}
			else
			{
				D3D12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
				D3D12RenderTargetViewDesc.Texture2D.MipSlice = 0;
			}
			break;
		case EResourceDimension::Texture3D:
			D3D12RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
			break;
		default:
			AssertFailed(u8"Invalid D3D12ResourceDesc.Dimension for CreateRenderTargetView");
			break;
		}
		D3D12Resource.D3D12Devcie.d3d12Device->CreateRenderTargetView(D3D12Resource.d3d12Resource, &D3D12RenderTargetViewDesc, { SIZE_T(RTV.Address.CPUAddress) });
	}

	void FD3D12ResourceView::CreateResourceViewInnner_DSV()
	{
		if (!AnyFlags(D3D12Resource.ResourceDesc.Usage, EResourceUsage::DepthStencil))
			return;

		D3D12Resource.D3D12Devcie.AllocateOfflineDescriptors(EResourceAspect::DepthStencil, DSV, 1);
#if D3D12_HEAP_DEBUG
		DSV.Allocator->SetBlockObject(DSV.BlockLocation, this);
#endif

		D3D12_DEPTH_STENCIL_VIEW_DESC D3D12DepthStencilViewDesc = {};
		D3D12DepthStencilViewDesc.Format = FromFormat(MappingDepthStencilFormat(D3D12Resource.ResourceDesc.Format));
		//D3D12DepthStencilViewDesc.Flags = D3D12_DSV_FLAGS__From(ResourceViewDesc.Flags);
		D3D12_DSV_FLAGS Result = (D3D12_DSV_FLAGS)0;
		//SetFlag(Result, D3D12_DSV_FLAGS::D3D12_DSV_FLAG_READ_ONLY_DEPTH, ResourceViewFlag & EResourceViewFlag::DSV_ReadOnlyDepth);

		switch (D3D12Resource.ResourceDesc.Dimension)
		{
		case EResourceDimension::Texture1D:
			D3D12DepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
			break;
		case EResourceDimension::Texture2D:
			if (D3D12Resource.ResourceDesc.Layers > 1)
			{
				D3D12DepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				if (ViewDesc.SubresourceIndex != NullIndex)
				{
					D3D12DepthStencilViewDesc.Texture2DArray.MipSlice = 0;
					D3D12DepthStencilViewDesc.Texture2DArray.FirstArraySlice = ViewDesc.SubresourceIndex;
					D3D12DepthStencilViewDesc.Texture2DArray.ArraySize = 1;
				}
				else
				{
					D3D12DepthStencilViewDesc.Texture2DArray.MipSlice = 0;
					D3D12DepthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
					D3D12DepthStencilViewDesc.Texture2DArray.ArraySize = D3D12Resource.ResourceDesc.Layers;
				}
			}
			else
			{
				D3D12DepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				D3D12DepthStencilViewDesc.Texture2D.MipSlice = ViewDesc.SubresourceIndex == NullIndex ? 0 : ViewDesc.SubresourceIndex;
			}
			break;
		default:
			AssertFailed(u8"Invalid D3D12ResourceDesc.Dimension for CreateDepthStencilView");
			break;
		}
		D3D12Resource.D3D12Devcie.d3d12Device->CreateDepthStencilView(D3D12Resource.d3d12Resource, &D3D12DepthStencilViewDesc, { SIZE_T(DSV.Address.CPUAddress) });
	}

	void FD3D12ResourceView::CreateResourceViewInnner_UAV()
	{
		if (!AnyFlags(D3D12Resource.ResourceDesc.Usage, EResourceUsage::StorageBuffer | EResourceUsage::StorageTexture | EResourceUsage::UniformTexelBuffer | EResourceUsage::StorageTexelBuffer))
			return;

		D3D12Resource.D3D12Devcie.AllocateOfflineDescriptors(EResourceAspect::TexelAccess, UAV, 1);
#if D3D12_HEAP_DEBUG
		UAV.Allocator->SetBlockObject(UAV.BlockLocation, this);
#endif

		D3D12_UNORDERED_ACCESS_VIEW_DESC D3D12UnorderedAccessViewDesc = {};

		D3D12UnorderedAccessViewDesc.Format = FromFormat(D3D12Resource.ResourceDesc.Format);
		switch (D3D12Resource.ResourceDesc.Dimension)
		{
		case EResourceDimension::Buffer:
			D3D12UnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			D3D12UnorderedAccessViewDesc.Buffer.FirstElement = ViewDesc.ElementIndex;
			if (D3D12Resource.ResourceDesc.Format == EFormat::None)
			{
				// Raw buffer, MUST be R32_TYPELESS
				D3D12UnorderedAccessViewDesc.Format = DXGI_FORMAT_R32_TYPELESS;
				D3D12UnorderedAccessViewDesc.Buffer.StructureByteStride = 0;
				D3D12UnorderedAccessViewDesc.Buffer.NumElements = D3D12Resource.ResourceDesc.Size.X * D3D12Resource.ResourceDesc.Size.Y / 4;
				D3D12UnorderedAccessViewDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
			}
			else
			{
				AssertExpr(GetFormatBytesPerPixel(D3D12Resource.ResourceDesc.Format) == D3D12Resource.ResourceDesc.Size.Y);
				D3D12UnorderedAccessViewDesc.Format = FromFormat(D3D12Resource.ResourceDesc.Format);
				D3D12UnorderedAccessViewDesc.Buffer.StructureByteStride = 0;
				D3D12UnorderedAccessViewDesc.Buffer.FirstElement = 0;
				D3D12UnorderedAccessViewDesc.Buffer.NumElements = D3D12Resource.ResourceDesc.Size.X;
				D3D12UnorderedAccessViewDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
			}
			break;
		case EResourceDimension::Texture1D:
			D3D12UnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
			break;
		case EResourceDimension::Texture2D:
			if (D3D12Resource.ResourceDesc.Layers > 1)
			{
				if (ViewDesc.SubresourceIndex != NullIndex)
				{
					D3D12UnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
					D3D12UnorderedAccessViewDesc.Texture2DArray.MipSlice = 0;
					D3D12UnorderedAccessViewDesc.Texture2DArray.FirstArraySlice = ViewDesc.SubresourceIndex;
					D3D12UnorderedAccessViewDesc.Texture2DArray.ArraySize = 1;
				}
				else
				{
					D3D12UnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
					D3D12UnorderedAccessViewDesc.Texture2DArray.MipSlice = 0;
					D3D12UnorderedAccessViewDesc.Texture2DArray.FirstArraySlice = 0;
					D3D12UnorderedAccessViewDesc.Texture2DArray.ArraySize = D3D12Resource.ResourceDesc.Layers;
				}
			}
			else
			{
				D3D12UnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				D3D12UnorderedAccessViewDesc.Texture2D.MipSlice = 0;
				D3D12UnorderedAccessViewDesc.Texture2D.PlaneSlice = 0;
			}
			break;
		case EResourceDimension::Texture3D:
			D3D12UnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
			break;
		default:
			AssertFailed(u8"Invalid D3D12ResourceDesc.Dimension for CreateUnorderedAccessView");
			break;
		}
		D3D12Resource.D3D12Devcie.d3d12Device->CreateUnorderedAccessView(D3D12Resource.d3d12Resource, nullptr, &D3D12UnorderedAccessViewDesc, { SIZE_T(UAV.Address.CPUAddress) });
	}

	FD3D12ResourceView::~FD3D12ResourceView()
	{
	}

	IRHIResource * FD3D12ResourceView::GetResource() const
	{
		return &D3D12Resource;
	}
}
