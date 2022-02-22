#include "PCH.h"
#include "SPIRVReflect.h"

#include "SPIRV-Reflect/spirv_reflect.h"

namespace Xin::ShaderCompiler
{
	static EFormat ToFormat(SpvImageFormat spvImageFormat)
	{
		constexpr EFormat InvalidFormat = EFormat::None;
		switch (spvImageFormat)
		{
		case SpvImageFormatRgba32f: return EFormat::R32G32B32A32F;
		case SpvImageFormatRgba16f: return EFormat::R16G16B16A16F;
		case SpvImageFormatR32f: return EFormat::R32F;
		case SpvImageFormatRgba8: return EFormat::R8G8B8A8;
		case SpvImageFormatRgba8Snorm: return EFormat::R8G8B8A8SF;
		case SpvImageFormatRg32f: return EFormat::R32G32F;
		case SpvImageFormatRg16f: return EFormat::R16G16F;
		case SpvImageFormatR11fG11fB10f: return EFormat::R11G11B10F;
		case SpvImageFormatR16f: return EFormat::R16F;
		case SpvImageFormatRgba16: return EFormat::R16G16B16A16;
		case SpvImageFormatRgb10A2: return EFormat::R10G10B10A2;
		case SpvImageFormatRg16: return EFormat::R16G16;
		case SpvImageFormatRg8: return EFormat::R8G8;
		case SpvImageFormatR16: return EFormat::R16;
		case SpvImageFormatR8: return EFormat::R8;
		case SpvImageFormatRgba16Snorm: return EFormat::R16G16B16A16UF;
		case SpvImageFormatRg16Snorm: return EFormat::R16G16UF;
		case SpvImageFormatRg8Snorm: return EFormat::R8G8UF;
		case SpvImageFormatR16Snorm: return EFormat::R16UF;
		case SpvImageFormatR8Snorm: return EFormat::R8SF;
		case SpvImageFormatRgba32i: return EFormat::R32G32B32A32SI;
		case SpvImageFormatRgba16i: return EFormat::R16G16B16A16SI;
		case SpvImageFormatRgba8i: return EFormat::R8G8B8A8SI;
		case SpvImageFormatR32i: return EFormat::R32SI;
		case SpvImageFormatRg32i: return EFormat::R32G32SI;
		case SpvImageFormatRg16i: return EFormat::R16G16SI;
		case SpvImageFormatRg8i: return EFormat::R8G8SI;
		case SpvImageFormatR16i: return EFormat::R16SI;
		case SpvImageFormatR8i: return EFormat::R8SI;
		case SpvImageFormatRgba32ui: return EFormat::R32G32B32A32UI;
		case SpvImageFormatRgba16ui: return EFormat::R16G16B16A16UI;
		case SpvImageFormatRgba8ui: return EFormat::R8G8B8A8UI;
		case SpvImageFormatR32ui: return EFormat::R32UI;
		case SpvImageFormatRgb10a2ui: return EFormat::R10G10B10A2UI;
		case SpvImageFormatRg32ui: return EFormat::R32UI;
		case SpvImageFormatRg16ui: return EFormat::R16G16UI;
		case SpvImageFormatRg8ui: return EFormat::R8G8UI;
		case SpvImageFormatR16ui: return EFormat::R16UI;
		case SpvImageFormatR8ui: return EFormat::R8UI;
		//case SpvImageFormatR64ui:return EFormat::R64UI;
		//case SpvImageFormatR64i: return EFormat::R64SI;
		default:
			return InvalidFormat;
		}
	}

	static EShaderStage FromShaderStage(SpvReflectShaderStageFlagBits ReflectShaderStage)
	{
		if (ReflectShaderStage & SPV_REFLECT_SHADER_STAGE_VERTEX_BIT)
			return EShaderStage::Vertex;

		if (ReflectShaderStage & SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT)
			return EShaderStage::Geometry;

		if (ReflectShaderStage & SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT)
			return EShaderStage::Pixel;

		if (ReflectShaderStage & SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT)
			return EShaderStage::Compute;

		return EShaderStage::None;
	}

	bool FSPIRVReflect::Reflect(TView<byte> ShaderCode, FReflectParameters & ReflectParameters)
	{
		spv_reflect::ShaderModule ShaderModule(ShaderCode.Size, ShaderCode.Data, SPV_REFLECT_MODULE_FLAG_NO_COPY);
		if (ShaderModule.GetResult() != SPV_REFLECT_RESULT_SUCCESS)
			return false;

		EShaderStage ShaderStage = FromShaderStage(ShaderModule.GetShaderStage());

		ShaderModule.GetEntryPointName();

		SpvReflectResult Result;

		{
			uint32 NumInputVariables;
			Result = ShaderModule.EnumerateInputVariables(&NumInputVariables, nullptr);
			AssertExpr(Result == SPV_REFLECT_RESULT_SUCCESS);
			TList<SpvReflectInterfaceVariable *> InputInterfaceVariables(NumInputVariables, EInitializeMode::None);
			Result = ShaderModule.EnumerateInputVariables(&NumInputVariables, InputInterfaceVariables.Data);
			for (SpvReflectInterfaceVariable * InputInterfaceVariable : InputInterfaceVariables)
			{
				FStringV VariableName = FEncoding::AsUtf8(InputInterfaceVariable->name);
				if (VariableName.StartsWith(u8"in.var."V))
					VariableName = VariableName.SubView(7);

				FReflectVariable & InputVariable = ReflectParameters.InputVariables.AddDefault();
				InputVariable.Name = VariableName;
				InputVariable.BindingIndex = InputInterfaceVariable->location;
			}
		}

		if constexpr (false)
		{
			uint32 NumOutputVariables;
			Result = ShaderModule.EnumerateOutputVariables(&NumOutputVariables, nullptr);
			AssertExpr(Result == SPV_REFLECT_RESULT_SUCCESS);
			TList<SpvReflectInterfaceVariable *> OutputInterfaceVariables(NumOutputVariables, EInitializeMode::None);
			Result = ShaderModule.EnumerateOutputVariables(&NumOutputVariables, OutputInterfaceVariables.Data);
			for (SpvReflectInterfaceVariable * OutputInterfaceVariable : OutputInterfaceVariables)
			{
				FStringV VariableName = FEncoding::AsUtf8(OutputInterfaceVariable->name);
				if (VariableName.StartsWith(u8"in.var."V))
					VariableName = VariableName.SubView(7);

				FReflectVariable & OutputVariable = ReflectParameters.OutputVariables.AddDefault();
				OutputVariable.Name = VariableName;
				OutputVariable.BindingIndex = OutputInterfaceVariable->location;
			}
		}

		uint32 NumDescriptorBindings;
		Result = ShaderModule.EnumerateDescriptorBindings(&NumDescriptorBindings, nullptr);
		TList<SpvReflectDescriptorBinding *> DescriptorBindings(NumDescriptorBindings, EInitializeMode::None);
		Result = ShaderModule.EnumerateDescriptorBindings(&NumDescriptorBindings, DescriptorBindings.Data);
		for (SpvReflectDescriptorBinding * DescriptorBinding : DescriptorBindings)
		{
			if (!DescriptorBinding->accessed)
				continue;

			switch (DescriptorBinding->descriptor_type)
			{
			case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:
			{
				FReflectParameter & ReflectParameter = ReflectParameters.Parameters.AddDefault();
				ReflectParameter.BindingType = EDescriptorType::Sampler;
				ReflectParameter.ShaderStage = ShaderStage;
				ReflectParameter.BindingIndex = DescriptorBinding->binding;
				ReflectParameter.Name = FEncoding::ToUtf8(DescriptorBinding->name);
				break;
			}
			case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			{
				FReflectParameter & ReflectParameter = ReflectParameters.Parameters.AddDefault();
				ReflectParameter.BindingType = EDescriptorType::SampledTexture;
				ReflectParameter.ShaderStage = ShaderStage;
				ReflectParameter.BindingIndex = DescriptorBinding->binding;
				ReflectParameter.Name = FEncoding::AsUtf8(DescriptorBinding->name);
				ReflectParameter.Format = ToFormat(DescriptorBinding->image.image_format);
				break;
			}
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:
			{
				FReflectParameter & ReflectParameter = ReflectParameters.Parameters.AddDefault();
				ReflectParameter.BindingType = EDescriptorType::StorageTexture;
				ReflectParameter.ResourceDimension = EResourceDimension::Texture2D;
				ReflectParameter.ShaderStage = ShaderStage;
				ReflectParameter.BindingIndex = DescriptorBinding->binding;
				ReflectParameter.Name = FEncoding::ToUtf8(DescriptorBinding->name);
				ReflectParameter.Format = ToFormat(DescriptorBinding->image.image_format);
				break;
			}
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			{
				FReflectParameter & ReflectParameter = ReflectParameters.Parameters.AddDefault();
				ReflectParameter.BindingType = EDescriptorType::UniformTexelBuffer;
				ReflectParameter.ResourceDimension = EResourceDimension::Buffer;
				ReflectParameter.ShaderStage = ShaderStage;
				ReflectParameter.BindingIndex = DescriptorBinding->binding;
				ReflectParameter.Name = FEncoding::AsUtf8(DescriptorBinding->name);
				break;
			}
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
			{
				FReflectParameter & ReflectParameter = ReflectParameters.Parameters.AddDefault();
				ReflectParameter.BindingType = EDescriptorType::StorageTexelBuffer;
				ReflectParameter.ResourceDimension = EResourceDimension::Buffer;
				ReflectParameter.ShaderStage = ShaderStage;
				ReflectParameter.BindingIndex = DescriptorBinding->binding;
				ReflectParameter.Name = FEncoding::AsUtf8(DescriptorBinding->name);
				break;
			}
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			{
				FReflectParameter & ReflectParameter = ReflectParameters.Parameters.AddDefault();
				ReflectParameter.BindingType = EDescriptorType::UniformBuffer;
				ReflectParameter.ResourceDimension = EResourceDimension::None;
				ReflectParameter.ShaderStage = ShaderStage;
				ReflectParameter.BindingIndex = DescriptorBinding->binding;
				ReflectParameter.Name = FEncoding::AsUtf8(DescriptorBinding->name);
				break;
			}
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			{
				FReflectParameter & ReflectParameter = ReflectParameters.Parameters.AddDefault();
				ReflectParameter.BindingType = EDescriptorType::StorageBuffer;
				ReflectParameter.ResourceDimension = EResourceDimension::Buffer;
				ReflectParameter.ShaderStage = ShaderStage;
				ReflectParameter.BindingIndex = DescriptorBinding->binding;
				ReflectParameter.Name = FEncoding::ToUtf8(DescriptorBinding->name);
				break;
			}
			default:
				LogInfo(u8"Unkonwn SPIRV DescriptorBinding {}, type {}."V, FEncoding::AsUtf8(DescriptorBinding->name), DescriptorBinding->descriptor_type);
				break;
			}
		}

		uint32 NumDescriptorSets;
		Result = ShaderModule.EnumerateDescriptorSets(&NumDescriptorSets, nullptr);
		TList<SpvReflectDescriptorBinding *> DescriptorSets(NumDescriptorSets, EInitializeMode::None);
		Result = ShaderModule.EnumerateDescriptorBindings(&NumDescriptorSets, DescriptorSets.Data);

		uint32 NumBlockVariables;
		Result = ShaderModule.EnumeratePushConstantBlocks(&NumBlockVariables, nullptr);
		TList<SpvReflectBlockVariable *> BlockVariables(NumBlockVariables, EInitializeMode::None);
		Result = ShaderModule.EnumeratePushConstantBlocks(&NumBlockVariables, BlockVariables.Data);

		return true;
	}
}
