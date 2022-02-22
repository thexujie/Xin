#include "PCH.h"
#include "D3D12PipelineState.h"
#include "D3D12Device.h"
#include "D3D12ShaderModule.h"

#if XIN_D3D12_NVAPI
#pragma warning(disable: 4819)
#include <nvapi/Include/nvapi.h>
#endif

namespace Xin::RHI::D3D12
{
	struct FPrefebRootParameter
	{
		EShaderStage ShaderStage = EShaderStage::None;
		ED3D12RootParameterType RootParameterType = ED3D12RootParameterType::None;
	};

	static constexpr FPrefebRootParameter PrefebRootParameters[]
	{
		{ EShaderStage::Pixel, ED3D12RootParameterType::DescriptorTable },
		{ EShaderStage::Pixel, ED3D12RootParameterType::ConstBuffer },
		{ EShaderStage::Vertex, ED3D12RootParameterType::ConstBuffer },
		{ EShaderStage::Compute, ED3D12RootParameterType::ConstBuffer },
		{ EShaderStage::Compute, ED3D12RootParameterType::DescriptorTable },
	};

	struct FShaderRegisterSignature
	{
		uint32 RegisterMask = 0;
		uint32 RegisterSpace = NullIndex;

		auto operator <=>(const FShaderRegisterSignature &) const = default;
		bool operator ==(const FShaderRegisterSignature &) const = default;
		friend uintx Hash(const FShaderRegisterSignature & This) { return HashCombine(This.RegisterMask, This.RegisterSpace); }
	};

	constexpr int MaxShaderRegisterCount_CBV = 2;
	constexpr int MaxShaderRegisterCount_SRV = 8;
	constexpr int MaxShaderRegisterCount_UAV = 4;
	constexpr int MaxShaderRegisterCount_SAM = 2;

	struct FShaderRegisterSignatures
	{
		TArray<FShaderRegisterSignature, MaxShaderRegisterCount_CBV> CBVs {};
		TArray<FShaderRegisterSignature, MaxShaderRegisterCount_SRV> SRVs {};
		TArray<FShaderRegisterSignature, MaxShaderRegisterCount_UAV> UAVs {};
		TArray<FShaderRegisterSignature, MaxShaderRegisterCount_SAM> SAMs {};
	};

	static void GetPipelineLayoutFromPipelineState(const FPipelineStateDesc & PipelineStateDesc, FD3D12ShaderParameters & ShaderParameters)
	{
		ShaderParameters.StaticSamplers = PipelineStateDesc.StaticSamplers;

		uint32 NumShaderParameters = 0;
		uint32 NumRootParameters = 0;
		for (uintx ShaderStageIndex = 0; ShaderStageIndex < MaxShaderStages; ++ShaderStageIndex)
		{
			if (PipelineStateDesc.Shaders[ShaderStageIndex] == nullptr)
				continue;

			const FReflectParameters & ReflectParameters = PipelineStateDesc.Shaders[ShaderStageIndex]->ShaderInstance.GetReflectParameters();

			for (const FReflectParameter & ReflectParameter : ReflectParameters.Parameters)
			{
				FD3D12ShaderParameter & ShaderParameter = ShaderParameters.Parameters[NumShaderParameters];
				ShaderParameter.BindingName = FName(ReflectParameter.Name);
				ShaderParameter.ShaderStage = EShaderStage(ShaderStageIndex);
				ShaderParameter.ParameterType = FromDescriptorType_RangeType(ReflectParameter.BindingType);
				// We calculate it on root signature generating.
				ShaderParameter.DescriptorIndex = NullIndex;
				ShaderParameter.RegisterIndex = ReflectParameter.BindingIndex;
				ShaderParameter.RegisterSpace = ReflectParameter.BindingSpace;
				//ShaderBindings.BindingMasks[ShaderStageIndex][ReflectParameter.BindingIndex] = true;

				if (ReflectParameter.BindingType == EDescriptorType::UniformBuffer)
				{
					FD3D12RootParameter & RootParameter = ShaderParameters.RootParameters[NumRootParameters++];
					RootParameter.RootParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
					RootParameter.BindingName = FName(ReflectParameter.Name);
					RootParameter.ShaderStage = EShaderStage(ShaderStageIndex);

					RootParameter.ParameterIndex = NumShaderParameters;
					RootParameter.RegisterIndex = ReflectParameter.BindingIndex;
					RootParameter.RegisterSpace = ReflectParameter.BindingSpace;
				}

				++NumShaderParameters;
			}
		}

		uint32 NumResourceRanges = 0;
		// D3D12_DESCRIPTOR_RANGE_TYPE_SRV D3D12_DESCRIPTOR_RANGE_TYPE_UAV
		TArray<FD3D12ParameterRange *, MaxShaderStages> ResourceRanges[2] {};
		TArray<uint32, MaxShaderStages> ResourceRangeIndices[2] = { { NullIndex, EElementFill::Fill }, { NullIndex, EElementFill::Fill } };

		for (uintx ParameterIndex = 0; ParameterIndex < MaxD3D12ShaderParameters; ++ParameterIndex)
		{
			const FD3D12ShaderParameter & ShaderParameter = ShaderParameters.Parameters[ParameterIndex];
			if (ShaderParameter.ShaderStage == EShaderStage::None)
				break;

			if (ShaderParameter.ParameterType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV ||
				ShaderParameter.ParameterType == D3D12_DESCRIPTOR_RANGE_TYPE_UAV)
			{
				FD3D12ParameterRange *& ResourceRange = ResourceRanges[ShaderParameter.ParameterType][ShaderParameter.ShaderStage];
				if (!ResourceRange)
				{
					ResourceRangeIndices[ShaderParameter.ParameterType][ShaderParameter.ShaderStage] = NumResourceRanges;
					ResourceRange = ShaderParameters.ParameterRanges.Data + NumResourceRanges++;
				}

				{
					if (ResourceRange->ParametersName.Length)
						ResourceRange->ParametersName += u8"|"V;
					ResourceRange->ParametersName += ShaderParameter.BindingName.Resolve();
					ResourceRange->ShaderStage = ShaderParameter.ShaderStage;
					ResourceRange->ParameterType = ShaderParameter.ParameterType;
					ResourceRange->RegisterMask |= (1 << ShaderParameter.RegisterIndex);
				}
			}
		}

		auto FindParameterIndex = [&](EShaderStage ShaderStage, D3D12_DESCRIPTOR_RANGE_TYPE BindingType, uint32 RegisterIndex, uint32 RegisterSpace)
		{
			for (uint32 ParameterIndex = 0; ParameterIndex < MaxD3D12ShaderParameters; ++ParameterIndex)
			{
				const FD3D12ShaderParameter & ShaderParameter = ShaderParameters.Parameters[ParameterIndex];
				if (ShaderParameter.ShaderStage == ShaderStage && ShaderParameter.ParameterType == BindingType
					&& ShaderParameter.RegisterIndex == RegisterIndex && ShaderParameter.RegisterSpace == RegisterSpace)
					return ParameterIndex;
			}
			return uint32(NullIndex);
		};

		uint32 NumDescriptors = 0;
		for (uintx ResourceRangeTypeIndex = 0; ResourceRangeTypeIndex < ArraySize(ResourceRanges); ++ResourceRangeTypeIndex)
		{
			for (uintx ShaderStageIndex = 0; ShaderStageIndex < MaxShaderStages; ++ShaderStageIndex)
			{
				FD3D12ParameterRange * ResourceRange = ResourceRanges[ResourceRangeTypeIndex][ShaderStageIndex];
				if (!ResourceRange)
					continue;

				FD3D12ParameterRange & ParameterRange = *ResourceRange;
				uint32 NumShaderResources = ParameterRange.GetNumShaderResources();
				AssertExpr(NumShaderResources > 0);
				uint32 BaseRegisterIndex = ParameterRange.GetBaseRegisterIndex();
				{
					ParameterRange.DescriptorIndex = NumDescriptors;
					ParameterRange.DescriptorCount = NumShaderResources;

					for (uint32 ResourceIndex = 0; ResourceIndex < NumShaderResources; ++ResourceIndex)
					{
						uint32 RegisterIndex = BaseRegisterIndex + ResourceIndex;
						if (ParameterRange.RegisterMask & (1 << RegisterIndex))
						{
							uint32 ParameterIndex = FindParameterIndex(ParameterRange.ShaderStage, ParameterRange.ParameterType, RegisterIndex, ParameterRange.RegisterSpace);
							if (ParameterIndex != NullIndex)
							{
								ShaderParameters.Parameters[ParameterIndex].DescriptorIndex = ParameterRange.DescriptorIndex + ResourceIndex;
							}
						}
					}
				}

				FD3D12RootParameter & RootParameter = ShaderParameters.RootParameters[NumRootParameters++];
				{
					RootParameter.RootParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
					RootParameter.BindingName = FName(ParameterRange.ParametersName);
					RootParameter.ShaderStage = ParameterRange.ShaderStage;

					RootParameter.RegisterIndex = BaseRegisterIndex;
					RootParameter.RegisterSpace = NumShaderResources;

					RootParameter.RangeIndex = ResourceRangeIndices[ResourceRangeTypeIndex][ShaderStageIndex];
					RootParameter.RangeCount = 1;

					AssertExpr(RootParameter.RangeIndex != NullIndex);
				}

				NumDescriptors += NumShaderResources;
			}
		}
	}

	static D3D12_SHADER_BYTECODE GetByteCodeFromRHIShader(IRHIShaderModule * Shader)
	{
		if (!Shader)
			return {};
		TView<byte> ShaderCode = StaticCastRef<FD3D12ShaderModule>(Shader).ShaderInstance.GetShaderCode();
		return D3D12_SHADER_BYTECODE { ShaderCode.Data, ShaderCode.Size };
	}

	FD3D12PipelineState::FD3D12PipelineState(FD3D12Device & Device, const FPipelineStateDesc & PipelineStateDesc)
		: Device(Device), PipelineStateDesc(PipelineStateDesc)
	{
		GetPipelineLayoutFromPipelineState(PipelineStateDesc, ShaderParameters);

		d3d12RootSignature = Device.FetchRootSinature(ShaderParameters);

		if (PipelineStateDesc.Shaders[EShaderStage::Compute])
		{
			D3D12_COMPUTE_PIPELINE_STATE_DESC D3D12ComputePipelineStateDesc = {};
			D3D12ComputePipelineStateDesc.pRootSignature = d3d12RootSignature.Get();
			D3D12ComputePipelineStateDesc.CS = GetByteCodeFromRHIShader(PipelineStateDesc.Shaders[EShaderStage::Compute]);
			D3D12ComputePipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;

			HRESULT Result = Device.d3d12Device->CreateComputePipelineState(&D3D12ComputePipelineStateDesc, __uuidof(ID3D12PipelineState), d3d12PipelineState.GetVV());
			if (FAILED(Result))
			{
				LogWarning(u8" CreateComputePipelineState failed, {}"V, ErrorDescription(Result));
			}
		}
		else
		{
			uintx NumInputElements = FindIndex(PipelineStateDesc.InputFormat.Elements, [](const FInputElement & InputElement) { return InputElement.SemanticName == EInputSemantic::None; });
			AssertExpr(NumInputElements != NullIndex);

			TList<D3D12_INPUT_ELEMENT_DESC> InputElementDescs(NumInputElements);
			for (uintx ElementIndex = 0; ElementIndex < NumInputElements; ++ElementIndex)
			{
				InputElementDescs[ElementIndex].SemanticName = (LPCSTR)InputSemanticNames[PipelineStateDesc.InputFormat.Elements[ElementIndex].SemanticName].GetData();
				InputElementDescs[ElementIndex].SemanticIndex = PipelineStateDesc.InputFormat.Elements[ElementIndex].SemanticIndex;
				InputElementDescs[ElementIndex].Format = FromFormat(PipelineStateDesc.InputFormat.Elements[ElementIndex].Format);
				InputElementDescs[ElementIndex].InputSlot = PipelineStateDesc.InputFormat.Elements[ElementIndex].StreamIndex;
				InputElementDescs[ElementIndex].AlignedByteOffset = /*RHIPipelineStateDesc.InputFormat.Elements[ElementIndex].ByteOffset*/D3D12_APPEND_ALIGNED_ELEMENT;
				InputElementDescs[ElementIndex].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
				InputElementDescs[ElementIndex].InstanceDataStepRate = 0;
			}

			D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsPipelineStateDesc = { };
			GraphicsPipelineStateDesc.pRootSignature = d3d12RootSignature.Get();
			GraphicsPipelineStateDesc.VS = GetByteCodeFromRHIShader(PipelineStateDesc.Shaders[EShaderStage::Vertex]);
			GraphicsPipelineStateDesc.HS = GetByteCodeFromRHIShader(PipelineStateDesc.Shaders[EShaderStage::Hull]);
			GraphicsPipelineStateDesc.DS = GetByteCodeFromRHIShader(PipelineStateDesc.Shaders[EShaderStage::Domain]);
			GraphicsPipelineStateDesc.GS = GetByteCodeFromRHIShader(PipelineStateDesc.Shaders[EShaderStage::Geometry]);
			GraphicsPipelineStateDesc.PS = GetByteCodeFromRHIShader(PipelineStateDesc.Shaders[EShaderStage::Pixel]);

			GraphicsPipelineStateDesc.BlendState.AlphaToCoverageEnable = PipelineStateDesc.BlendState.AlphaToCoverage;
			GraphicsPipelineStateDesc.BlendState.IndependentBlendEnable = PipelineStateDesc.BlendState.IndependentBlend;
			for (uint32 RenderTargetIndex = 0; RenderTargetIndex < MaxRenderTargets; ++RenderTargetIndex)
			{
				if (PipelineStateDesc.OutputFormat.RenderTargets[RenderTargetIndex] == EFormat::None)
				{
					GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].BlendEnable = false;
					continue;
				}

				GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].BlendEnable = PipelineStateDesc.BlendState.RenderTargets[RenderTargetIndex].Enable;
				GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].SrcBlend = FromBlendFactor(PipelineStateDesc.BlendState.RenderTargets[RenderTargetIndex].SrcColor);
				GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].DestBlend = FromBlendFactor(PipelineStateDesc.BlendState.RenderTargets[RenderTargetIndex].DstColor);
				GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].BlendOp = FromBlendOP(PipelineStateDesc.BlendState.RenderTargets[RenderTargetIndex].ColorAlgorithm);
				GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].SrcBlendAlpha = FromBlendFactor(PipelineStateDesc.BlendState.RenderTargets[RenderTargetIndex].SrcAlpha);
				GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].DestBlendAlpha = FromBlendFactor(PipelineStateDesc.BlendState.RenderTargets[RenderTargetIndex].DstAlpha);
				GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].BlendOpAlpha = FromBlendOP(PipelineStateDesc.BlendState.RenderTargets[RenderTargetIndex].AlphaAlgorithm);
				GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].LogicOp = D3D12_LOGIC_OP_NOOP;
				GraphicsPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex].RenderTargetWriteMask = (UINT8)FromWriteMasks(PipelineStateDesc.BlendState.RenderTargets[RenderTargetIndex].WriteMasks);
			}
			GraphicsPipelineStateDesc.SampleMask = PipelineStateDesc.OutputFormat.SampleMask;

			GraphicsPipelineStateDesc.DSVFormat = FromFormatDSV(PipelineStateDesc.OutputFormat.DepthStencil);
			GraphicsPipelineStateDesc.DepthStencilState.DepthEnable = PipelineStateDesc.DepthStencilState.DepthTest;
			GraphicsPipelineStateDesc.DepthStencilState.DepthWriteMask = PipelineStateDesc.DepthStencilState.DepthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
			GraphicsPipelineStateDesc.DepthStencilState.DepthFunc = FromComparison(PipelineStateDesc.DepthStencilState.DepthComparison);
			GraphicsPipelineStateDesc.DepthStencilState.StencilEnable = PipelineStateDesc.DepthStencilState.StencilTest;

			GraphicsPipelineStateDesc.InputLayout = { InputElementDescs.Data, uint32(NumInputElements) };
			GraphicsPipelineStateDesc.PrimitiveTopologyType = FromTopology(PipelineStateDesc.Topology);

			for (uint32 RenderTargetIndex = 0; RenderTargetIndex < MaxRenderTargets; ++RenderTargetIndex)
			{
				if (PipelineStateDesc.OutputFormat.RenderTargets[RenderTargetIndex] == EFormat::None)
				{
					GraphicsPipelineStateDesc.RTVFormats[RenderTargetIndex] = DXGI_FORMAT_UNKNOWN;
					continue;
				}

				GraphicsPipelineStateDesc.RTVFormats[RenderTargetIndex] = FromFormat(PipelineStateDesc.OutputFormat.RenderTargets[RenderTargetIndex]);
				GraphicsPipelineStateDesc.NumRenderTargets = RenderTargetIndex + 1;
			}
			AssertExpr(GraphicsPipelineStateDesc.NumRenderTargets > 0 || GraphicsPipelineStateDesc.DSVFormat != DXGI_FORMAT_UNKNOWN);

			GraphicsPipelineStateDesc.SampleDesc.Count = PipelineStateDesc.OutputFormat.MSAA.Level;
			GraphicsPipelineStateDesc.SampleDesc.Quality = PipelineStateDesc.OutputFormat.MSAA.Quality;

			GraphicsPipelineStateDesc.RasterizerState.FillMode = PipelineStateDesc.RasterizeState.Wireframe ? D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			GraphicsPipelineStateDesc.RasterizerState.CullMode = FromCullMode(PipelineStateDesc.RasterizeState.CullMode);
			GraphicsPipelineStateDesc.RasterizerState.FrontCounterClockwise = PipelineStateDesc.RasterizeState.FrontCCW;
			GraphicsPipelineStateDesc.RasterizerState.DepthBias = PipelineStateDesc.RasterizeState.DepthBias;
			GraphicsPipelineStateDesc.RasterizerState.DepthBiasClamp = PipelineStateDesc.RasterizeState.DepthBiasClamp;
			GraphicsPipelineStateDesc.RasterizerState.SlopeScaledDepthBias = PipelineStateDesc.RasterizeState.SlopScaledDepthBias;
			GraphicsPipelineStateDesc.RasterizerState.DepthClipEnable = PipelineStateDesc.RasterizeState.DepthClip;
			GraphicsPipelineStateDesc.RasterizerState.MultisampleEnable = PipelineStateDesc.RasterizeState.MSAA;
			GraphicsPipelineStateDesc.RasterizerState.AntialiasedLineEnable = PipelineStateDesc.RasterizeState.LineAA;

#if XIN_D3D12_NVAPI
			if (Device.IsNvidiaDevice())
			{
				NVAPI_D3D12_PSO_SET_SHADER_EXTENSION_SLOT_DESC ExtensionDesc { };
				ExtensionDesc.baseVersion = NV_PSO_EXTENSION_DESC_VER;
				ExtensionDesc.psoExtension = NV_PSO_SET_SHADER_EXTNENSION_SLOT_AND_SPACE;
				ExtensionDesc.version = NV_SET_SHADER_EXTENSION_SLOT_DESC_VER;
				ExtensionDesc.uavSlot = 0;
				ExtensionDesc.registerSpace = 1;

				// Put the pointer to the extension into an array - there can be multiple extensions enabled at once.
				// Other supported extensions are: 
				//     - Extended rasterizer state
				//  - Pass-through geometry shader, implicit or explicit
				//  - Depth bound test
				const NVAPI_D3D12_PSO_EXTENSION_DESC * pExtensions[] = { &ExtensionDesc };

				// Now create the PSO.
				NvAPI_Status NvapiStatus = NvAPI_D3D12_CreateGraphicsPipelineState(Device.d3d12Device.Get(), &GraphicsPipelineStateDesc, ARRAYSIZE(pExtensions), pExtensions, d3d12PipelineState.GetPP());
				if (NvapiStatus)
				{
					LogWarning(u8" NvAPI_D3D12_CreateGraphicsPipelineState failed, {}"V, NvapiStatus);
				}
			}
			else
#endif
			{
				HRESULT Result = Device.d3d12Device->CreateGraphicsPipelineState(&GraphicsPipelineStateDesc, __uuidof(ID3D12PipelineState), d3d12PipelineState.GetVV());
				if (FAILED(Result))
				{
					LogWarning(u8"CreateGraphicsPipelineState failed, {}", ErrorDescription(Result));
					AssertFailed(u8"CreateGraphicsPipelineState failed");
				}
			}
		}
	}

	FD3D12IndirectSignature::FD3D12IndirectSignature(FD3D12Device & D3D12Devcie, const FIndirectSignature & IndirectSignature, FD3D12PipelineState * PipelineState)
		: D3D12Devcie(D3D12Devcie), IndirectSignature(IndirectSignature)
	{
		D3D12RootSignature = PipelineState ? PipelineState->d3d12RootSignature.Get() : nullptr;

		UINT ByteStride = 0;
		TList<D3D12_INDIRECT_ARGUMENT_DESC> D3D12IndirectArgumentDescs(IndirectSignature.Parameters.Size, EInitializeMode::Zero);
		for (uintx ParameterIndex = 0; ParameterIndex < IndirectSignature.Parameters.Size; ++ParameterIndex)
		{
			const FIndirectParameter & RHIIndirectSignatureParameterDesc = IndirectSignature.Parameters[ParameterIndex];
			D3D12IndirectArgumentDescs[ParameterIndex].Type = FromIndirectParameterType(RHIIndirectSignatureParameterDesc.Type);
			switch (RHIIndirectSignatureParameterDesc.Type)
			{
			case EIndirectParameterType::Draw:
				ByteStride += sizeof(D3D12_DRAW_ARGUMENTS);
				break;
			case EIndirectParameterType::DrawIndexed:
				ByteStride += sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);
				break;
			case EIndirectParameterType::Dispatch:
				ByteStride += sizeof(D3D12_DISPATCH_ARGUMENTS);
				break;
			case EIndirectParameterType::VertexBufferView:
				ByteStride += sizeof(D3D12_VERTEX_BUFFER_VIEW);
				D3D12IndirectArgumentDescs[ParameterIndex].VertexBuffer.Slot = RHIIndirectSignatureParameterDesc.VertexBuffer.SlotIndex;
				break;
			case EIndirectParameterType::IndexBufferView:
				ByteStride += sizeof(D3D12_INDEX_BUFFER_VIEW);
				break;
			case EIndirectParameterType::Constannt:
				break;
			case EIndirectParameterType::CBV:
				ByteStride += 8;
				D3D12IndirectArgumentDescs[ParameterIndex].ConstantBufferView.RootParameterIndex = RHIIndirectSignatureParameterDesc.CBVSRVUAV.RootIndex;
				break;
			case EIndirectParameterType::SRV:
				ByteStride += 8;
				D3D12IndirectArgumentDescs[ParameterIndex].ShaderResourceView.RootParameterIndex = RHIIndirectSignatureParameterDesc.CBVSRVUAV.RootIndex;
				break;
			case EIndirectParameterType::UAV:
				ByteStride += 8;
				D3D12IndirectArgumentDescs[ParameterIndex].UnorderedAccessView.RootParameterIndex = RHIIndirectSignatureParameterDesc.CBVSRVUAV.RootIndex;
				break;
			case EIndirectParameterType::DispatchRays:
				break;
			case EIndirectParameterType::DispatchMesh:
				break;
			default:
				break;
			}
		}

		AssertExpr(IndirectSignature.ParametersStride == 0 || IndirectSignature.ParametersStride >= ByteStride);
		D3D12_COMMAND_SIGNATURE_DESC CommandSignatureDesc {};
		CommandSignatureDesc.ByteStride = IndirectSignature.ParametersStride == 0 ? ByteStride : IndirectSignature.ParametersStride;
		CommandSignatureDesc.NumArgumentDescs = (uint32)(D3D12IndirectArgumentDescs.Size);
		CommandSignatureDesc.pArgumentDescs = D3D12IndirectArgumentDescs.Data;

		TReferPtr<ID3DBlob> ErrorBlob;
		HRESULT Result = D3D12Devcie.d3d12Device->CreateCommandSignature(&CommandSignatureDesc, D3D12RootSignature, IID_PPV_ARGS(D3D12CommandSignature.GetPP()));
		if (Failed(Result))
		{
			LogWarning(u8"CreateCommandSignature failed, 0x{:x}, \n{}"V, uint32(Result), D3D12BlobMessage(ErrorBlob));
		}
	}
}
