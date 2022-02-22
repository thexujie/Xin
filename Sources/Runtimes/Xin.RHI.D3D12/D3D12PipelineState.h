#pragma once

#include "D3D12Types.h"

namespace Xin::RHI::D3D12
{
	class FD3D12Device;

	constexpr int RegisterRangeUnit_CBV = 4;
	constexpr int RegisterRangeUnit_SRV = 4;
	constexpr int RegisterRangeUnit_UAV = 4;
	constexpr int RegisterRangeUnit_SAM = 4;


	class RHID3D12_API FD3D12PipelineState : public IRHIPipelineState
	{
	public:
		FD3D12PipelineState(FD3D12Device & Device, const FPipelineStateDesc & PipelineStateDesc);

	public:
		FD3D12Device & Device;
		FPipelineStateDesc PipelineStateDesc;

		FD3D12ShaderParameters ShaderParameters;

		ID3D12RootSignatureRef d3d12RootSignature;
		ID3D12PipelineStateRef d3d12PipelineState;

	};
	using IRHIPipelineStateRef = TReferPtr<IRHIPipelineState>;


	class RHID3D12_API FD3D12IndirectSignature : public IRHIIndirectSignature
	{
	public:
		FD3D12IndirectSignature(FD3D12Device & D3D12Devcie, const FIndirectSignature & IndirectSignature, FD3D12PipelineState * PipelineState);

	public:
		FD3D12Device & D3D12Devcie;
		FIndirectSignature IndirectSignature;

		ID3D12RootSignatureRef D3D12RootSignature;
		ID3D12CommandSignatureRef D3D12CommandSignature;
	};
	using FD3D12IndirectSignatureRef = TReferPtr<FD3D12IndirectSignature>;
}
