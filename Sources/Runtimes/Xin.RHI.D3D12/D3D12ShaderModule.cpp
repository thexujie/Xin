#include "PCH.h"
#include "D3D12ShaderModule.h"

namespace Xin::RHI::D3D12
{
	FD3D12ShaderModule::FD3D12ShaderModule(FD3D12Device & Device, IShaderInstance & ShaderInstance)
		: D3D12Devcie(Device), IRHIShaderModule(ShaderInstance)
	{
		
	}
}
