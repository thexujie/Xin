#include "PCH.h"
#include "D3D12Reflect.h"

#include "Xin.Core/Platform/Windows/WindowsPrivate.h"

#include <d3d12shader.h>

namespace Xin::ShaderCompiler
{
	static EFormat FromD3DReturnType(D3D_RESOURCE_RETURN_TYPE ReturnType)
	{
		switch (ReturnType)
		{
		case D3D_RETURN_TYPE_SINT: return EFormat::Int1;
		case D3D_RETURN_TYPE_UINT: return EFormat::UInt1;
		case D3D_RETURN_TYPE_FLOAT: return EFormat::Float1;
		default: return EFormat::None;
		}
	}

	void D3D12GenerateReflectParameters(EShaderStage ShaderStage, FReflectParameters & ReflectParameters, ID3D12ShaderReflection * D3D12ShaderReflection)
	{
		D3D12_SHADER_DESC D3D12ShaderDesc = {};
		D3D12ShaderReflection->GetDesc(&D3D12ShaderDesc);

		//switch(D3D12ShaderDesc.Version)
		//{
		//default: 
		//case D3D12_SHVER_PIXEL_SHADER: ShaderType = EShaderType::Pixel; break;
		//case D3D12_SHVER_VERTEX_SHADER: ShaderType = EShaderType::Vertex; break;
		//case D3D12_SHVER_GEOMETRY_SHADER: ShaderType = EShaderType::Geoetry; break;
		//case D3D12_SHVER_HULL_SHADER: ShaderType = EShaderType::Hull; break;
		//case D3D12_SHVER_DOMAIN_SHADER: ShaderType = EShaderType::Domain; break;
		//case D3D12_SHVER_COMPUTE_SHADER: ShaderType = EShaderType::Compute; break;
		//}

		for (uint32 ResourceIndex = 0; ResourceIndex < D3D12ShaderDesc.BoundResources; ++ResourceIndex)
		{
			D3D12_SHADER_INPUT_BIND_DESC D3D12ResourceBindDesc = {};
			D3D12ShaderReflection->GetResourceBindingDesc(ResourceIndex, &D3D12ResourceBindDesc);

			switch (D3D12ResourceBindDesc.Type)
			{
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER:
				ReflectParameters.Parameters.Add({ EDescriptorType::UniformBuffer, EResourceDimension::None, ShaderStage, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)), D3D12ResourceBindDesc.BindPoint, D3D12ResourceBindDesc.Space });
				break;
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE:
				ReflectParameters.Parameters.Add({ EDescriptorType::SampledTexture, EResourceDimension::None, ShaderStage, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)), D3D12ResourceBindDesc.BindPoint, D3D12ResourceBindDesc.Space });
				break;
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED:
				ReflectParameters.Parameters.Add({ EDescriptorType::SampledTexture, EResourceDimension::None, ShaderStage, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)), D3D12ResourceBindDesc.BindPoint, D3D12ResourceBindDesc.Space });
				break;
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_BYTEADDRESS:
				ReflectParameters.Parameters.Add({ EDescriptorType::SampledTexture, EResourceDimension::None, ShaderStage, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)), D3D12ResourceBindDesc.BindPoint, D3D12ResourceBindDesc.Space });
				break;
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWTYPED:
				ReflectParameters.Parameters.Add({ EDescriptorType::StorageBuffer, EResourceDimension::None, ShaderStage, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)), D3D12ResourceBindDesc.BindPoint, D3D12ResourceBindDesc.Space, FromD3DReturnType(D3D12ResourceBindDesc.ReturnType)});
				break;
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED:
				ReflectParameters.Parameters.Add({ EDescriptorType::StorageBuffer, EResourceDimension::None, ShaderStage, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)), D3D12ResourceBindDesc.BindPoint, D3D12ResourceBindDesc.Space, FromD3DReturnType(D3D12ResourceBindDesc.ReturnType) });
				break;
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWBYTEADDRESS:
				ReflectParameters.Parameters.Add({ EDescriptorType::StorageBuffer, EResourceDimension::None, ShaderStage, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)), D3D12ResourceBindDesc.BindPoint, D3D12ResourceBindDesc.Space, FromD3DReturnType(D3D12ResourceBindDesc.ReturnType) });
				break;
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER:
				ReflectParameters.Parameters.Add({ EDescriptorType::Sampler, EResourceDimension::None, ShaderStage, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)), D3D12ResourceBindDesc.BindPoint, D3D12ResourceBindDesc.Space });
				break;
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
				ReflectParameters.Parameters.Add({ EDescriptorType::StorageBuffer, EResourceDimension::None, ShaderStage, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)), D3D12ResourceBindDesc.BindPoint, D3D12ResourceBindDesc.Space, FromD3DReturnType(D3D12ResourceBindDesc.ReturnType) });
				break;
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_APPEND_STRUCTURED:
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_UAV_CONSUME_STRUCTURED:
			case D3D_SHADER_INPUT_TYPE::D3D_SIT_TBUFFER:
			default:
				LogWarning(u8"Unkonwn shader parameter {}."V, FEncoding::ToUtf8(FAStringV(D3D12ResourceBindDesc.Name)));
				break;
			}
		}
	}
}

