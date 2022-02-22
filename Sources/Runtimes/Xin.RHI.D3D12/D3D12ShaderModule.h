#pragma once

#include "D3D12Types.h"

namespace Xin::RHI::D3D12
{
	class FD3D12Device;

	class FD3D12ShaderModule : public IRHIShaderModule
	{
	public:
		FD3D12ShaderModule(FD3D12Device & D3D12Devcie, IShaderInstance & ShaderInstance);

	public:
		FD3D12Device & D3D12Devcie;
	};
}
