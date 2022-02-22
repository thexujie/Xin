#pragma once

#include "RHITypes.h"

namespace Xin::RHI
{
	enum class EDeviceRHI : uint32
	{
		None = 0,
		D3D11 = MakeFourCC("DX11"),
		D3D12 = MakeFourCC("DX12"),
		Vulkan = MakeFourCC("VLK "),
		OpenGL = MakeFourCC("OGL "),
	};

	constexpr FStringV GetDeviceRHIName(EDeviceRHI DeviceRHI)
	{
		switch (DeviceRHI)
		{
		case EDeviceRHI::D3D11: return u8"D3D11"V;
		case EDeviceRHI::D3D12: return u8"D3D12"V;
		case EDeviceRHI::Vulkan: return u8"Vulkan"V;
		case EDeviceRHI::OpenGL: return u8"OpenGL"V;
		default: return u8"Unkonwn"V;
		}
	}

	enum class EShaderLanguage
	{
		HLSL = 0,
		GLSL,
	};

	enum class EShaderArchitecture
	{
		Default = 0,
		DXIL,
		DXBC,
		SPIRV
	};

	constexpr FStringV GetShaderArchitectureName(EShaderArchitecture ShaderArchitecture)
	{
		switch (ShaderArchitecture)
		{
		case EShaderArchitecture::Default: return u8"Default"V;
		case EShaderArchitecture::DXIL: return u8"DXIL"V;
		case EShaderArchitecture::DXBC: return u8"DXBC"V;
		case EShaderArchitecture::SPIRV: return u8"SPIRV"V;
		default: return u8"Unkonwn"V;
		}
	}

	enum class EOptimizationLevel
	{
		Level0 =0,
		Level1,
		Level2,
		Level3,
	};

	struct FRHIShaderDefines
	{
		FStringV Name;
		FStringV Value;
	};

	class RHI_API FShaderCompilerContext : public IRHIObject
	{
	public:
		FShaderCompilerContext() = default;

		virtual bool IncludeFile(FStringV FilePath, TList<byte> & FileContent) { return false; }
		virtual void OnProcessed(FStringV ProcessedSource) {}
		virtual void OnCompiled(TView<byte> ShaderCode, TView<byte> ShaderPDB, const FReflectParameters & ReflectParameters) {}

	public:
		FString FilePath;
		FString ModuleName;

		EDeviceRHI DeviceRHI = EDeviceRHI::None;
		EShaderLanguage SourceLanguage = EShaderLanguage::HLSL;
		EShaderArchitecture TargetArchitecture = EShaderArchitecture::Default;

		EShaderStage ShaderStage = EShaderStage::None;
		FString EntryName;

		bool DebugCompile = false;
		EOptimizationLevel OptimizationLevel = EOptimizationLevel::Level3;
	};

	class RHI_API IRHIShaderCompiler : public IRHIObject
	{
	public:
		IRHIShaderCompiler() = default;

		virtual bool CompilerFromFile(FStringV FilePath, FShaderCompilerContext & CompilerContext) = 0;
		virtual bool CompilerFromSource(FStringV ShaderSource, FShaderCompilerContext & CompilerContext) = 0;
	};
	using IRHICompilerRef = TReferPtr<IRHIShaderCompiler>;
}
