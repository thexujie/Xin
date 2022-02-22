#include "PCH.h"
#include "DXC.h"

#include "Xin.Core/Platform/Windows/WindowsPrivate.h"
#include "D3D12Reflect.h"
#include "SPIRVReflect.h"

#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")


#pragma warning(disable: 4505)

namespace Xin::ShaderCompiler
{
	static FStringV GetDXCDefaultTargetProfile(EShaderStage ShaderStage)
	{
		return GetTargetProfile_6_0(ShaderStage);
	}

	struct FDxcBlob : public Windows::TUnknown<IDxcBlob>
	{
	public:
		LPVOID STDMETHODCALLTYPE GetBufferPointer(void) override
		{
			return (LPVOID)BlobData.Data;
		}
		SIZE_T STDMETHODCALLTYPE GetBufferSize(void) override
		{
			return BlobData.Size;
		}

		TView<byte> BlobData;
	};

	class FDxcIncludeHandler final : public Windows::TUnknown<IDxcIncludeHandler>
	{
	public:
		FDxcIncludeHandler(FShaderCompilerContext & CompilerInput) : CompilerInput(CompilerInput)
		{
		}

		HRESULT STDMETHODCALLTYPE LoadSource(LPCWSTR pFilename, IDxcBlob ** ppIncludeSource) override
		{
			*ppIncludeSource = nullptr;

			FString FilePath = FEncoding::ToUtf8(pFilename);
			if (CompilerInput.IncludeFile(FilePath, FileContent))
			{
				DxcBlob.BlobData = { (const byte *)FileContent.Data, FileContent.Size };
				if (DxcBlob.BlobData.Size)
				{
					*ppIncludeSource = &DxcBlob;
					DxcBlob.AddRef();
				}
			}
			return S_OK;
		}

	private:
		FShaderCompilerContext & CompilerInput;

		FDxcBlob DxcBlob;
		TList<byte> FileContent;
	};

	FDXCShaderCompiler::FDXCShaderCompiler()
	{

	}

	bool FDXCShaderCompiler::CompilerFromMemory(FStringV ShaderSource, FShaderCompilerContext & CompilerContext)
	{
		EShaderArchitecture TargetArchitecture = CompilerContext.TargetArchitecture;
		if (CompilerContext.TargetArchitecture == EShaderArchitecture::Default)
		{
			switch (CompilerContext.DeviceRHI)
			{
			default:
			case EDeviceRHI::D3D12:
				TargetArchitecture = EShaderArchitecture::DXIL;
				break;
			case EDeviceRHI::Vulkan:
				TargetArchitecture = EShaderArchitecture::SPIRV;
				break;
			}
		}
		AssertExpr(ShaderSource.Size);
		AssertExpr(CompilerContext.EntryName.Length);

		FStringV FileName = FPath::GetFileName(CompilerContext.FilePath);

		FDxcIncludeHandler IncludeHandler(CompilerContext);

		TReferPtr<IDxcUtils> DxcUtils;
		TReferPtr<IDxcCompiler3> DxcCompiler3;
		DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(DxcUtils.GetPP()));
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(DxcCompiler3.GetPP()));

		DxcBuffer SourceBuffer;
		SourceBuffer.Ptr = ShaderSource.Data;
		SourceBuffer.Size = ShaderSource.Size;
		SourceBuffer.Encoding = DXC_CP_UTF8;

		FWString FileNameW = FEncoding::ToWide(FileName);
		FWString EntryPointW = FEncoding::ToWide(CompilerContext.EntryName);
		FWString TargetProfileW = FEncoding::ToWide(GetDXCDefaultTargetProfile(CompilerContext.ShaderStage));
		FWString ShaderPDBFileName = FEncoding::ToWide(CompilerContext.ModuleName + u8".pdb"V);

		// https://github.com/microsoft/DirectXShaderCompiler/blob/main/include/dxc/Support/HLSLOptions.h
		// https://github.com/microsoft/DirectXShaderCompiler/blob/main/include/dxc/Support/SPIRVOptions.h
		// https://simoncoenen.com/blog/programming/graphics/DxcCompiling
		TList<FWString> Arguments;
		Arguments.Add(FileNameW);

		// Pack matrices in row-major order
		Arguments.Add(L"-Zpr");

		if (CompilerContext.DebugCompile)
		{
			Arguments.Add(L"-Zi");
			// Embed PDB in shader container (must be used with /Zi)
			Arguments.Add(L"-Qembed_debug");
		}

		if (CompilerContext.OptimizationLevel == EOptimizationLevel::Level0)
			Arguments.Add(L"-O0");
		else if (CompilerContext.OptimizationLevel == EOptimizationLevel::Level1)
			Arguments.Add(L"-O1");
		else if (CompilerContext.OptimizationLevel == EOptimizationLevel::Level2)
			Arguments.Add(L"-O2");
		else if (CompilerContext.OptimizationLevel == EOptimizationLevel::Level3)
			Arguments.Add(L"-O3");
		else
			Arguments.Add(L"-O3");

		// Strip reflection data from shader bytecode (must be used with /Fo )
		//Arguments.Add(L"-Qstrip_reflect");

		Arguments.Add(L"-E");
		Arguments.Add(EntryPointW);
		Arguments.Add(L"-T");
		Arguments.Add(TargetProfileW);

		Arguments.Add(L"-DXIN_DXC=1");
		if (TargetArchitecture != EShaderArchitecture::SPIRV)
		{
			Arguments.Add(L"-DXIN_SPIRV=0");
			Arguments.Add(L"-Fd");
			Arguments.Add(ShaderPDBFileName);
		}
		else
		{
			Arguments.Add(L"-DXIN_SPIRV=1");
			Arguments.Add(L"-spirv");
			//Arguments.Add(L"-spirv-defaultRowMajor");
			//Arguments.Add(L"-fspv-print-all");
			//Arguments.Add(L"-fspv-reflect");
			Arguments.Add(L"-fspv-target-env=vulkan1.3");
		}

		#if !XIN_PUBLISH
			// Strip debug information from 4_0 + shader bytecode(must be used with / Fo)
			Arguments.Add(L"-Qstrip_debug");
		#endif


		TList<const charw *> CompileArguments(Arguments.Size, EInitializeMode::None);
		for (uintx ArgumentIndex = 0; ArgumentIndex < Arguments.Size; ++ArgumentIndex)
			CompileArguments[ArgumentIndex] = Arguments[ArgumentIndex].Data;

#if !XIN_PUBLISH
		TReferPtr<IDxcCompiler> DxcCompiler = DxcCompiler3;
		AssertExpr(DxcCompiler);

		FDxcBlob SourceBlob;
		SourceBlob.BlobData = { (const byte *)SourceBuffer.Ptr, SourceBuffer.Size };
		TReferPtr<IDxcOperationResult> PreprocessResult;
		DxcCompiler->Preprocess(&SourceBlob, FileNameW.Data, CompileArguments.Data, uint32(Arguments.Size), nullptr, 0, &IncludeHandler, PreprocessResult.GetPP());

		TReferPtr<IDxcBlob> HlslBlob;
		PreprocessResult->GetResult(HlslBlob.GetPP());
		if (HlslBlob)
		{
			FStringV ShaderSourceHLSL = { (const char8 *)HlslBlob->GetBufferPointer(), HlslBlob->GetBufferSize() };
			CompilerContext.OnProcessed(ShaderSourceHLSL);
		}
#endif

		TReferPtr<IDxcResult> CompileResult;
		HRESULT Result = DxcCompiler3->Compile(&SourceBuffer, CompileArguments.Data, uint32(Arguments.Size), &IncludeHandler, IID_PPV_ARGS(CompileResult.GetPP()));
		if (Windows::Failed(Result) || !CompileResult)
		{
			LogWarning(u8"IDxcCompiler::Compile [{}] 0x{:x}, {}: failed."V, CompilerContext.EntryName, uint32(Result), CompilerContext.FilePath);
			return false;
		}

		CompileResult->GetStatus(&Result);
		if (Windows::Failed(Result))
		{
			if (CompileResult->HasOutput(DXC_OUT_ERRORS))
			{
				TReferPtr<IDxcBlobUtf8> CompileError;
				CompileResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(CompileError.GetPP()), nullptr);

				FStringV BlobMessage { (const char8_t *)CompileError->GetStringPointer(), CompileError->GetStringLength() };
				LogWarning(u8"IDxcCompiler::Compile [{}] 0x{:x}, {}: {}"V, CompilerContext.EntryName, uint32(Result), CompilerContext.FilePath, BlobMessage);
				//FDebug::WriteLineF(u8"{}: {}"V, CompilerInput.FilePath, BlobMessage);
			}
			else
			{
				LogWarning(u8"IDxcCompiler::Compile [{}] 0x{:x}, {}: failed."V, CompilerContext.EntryName, uint32(Result), CompilerContext.FilePath);
				//FDebug::WriteLineF(u8"{}"V, CompilerInput.FilePath);
			}
			return false;
		}

		TReferPtr<IDxcBlob> CompiledShader, CompiledShaderPDB, ShaderHashBlob, ReflectionBlob;
		CompileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(CompiledShader.GetPP()), nullptr);
		CompileResult->GetOutput(DXC_OUT_SHADER_HASH, IID_PPV_ARGS(ShaderHashBlob.GetPP()), nullptr);
		CompileResult->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(ReflectionBlob.GetPP()), nullptr);

		TView<byte> ShaderCode = { (const byte *)CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize() };
		TView<byte> ShaderPDB;
		TView<byte> MetaData;

		FReflectParameters ReflectParameters;
		if (TargetArchitecture == EShaderArchitecture::SPIRV)
		{
			AssertExpr(CompilerContext.DeviceRHI == EDeviceRHI::None || CompilerContext.DeviceRHI == EDeviceRHI::Vulkan);
			if (CompilerContext.DeviceRHI == EDeviceRHI::Vulkan)
			{
				FSPIRVReflect SPIRVReflect;
				SPIRVReflect.Reflect(ShaderCode, ReflectParameters);
			}
		}
		else
		{
			const DxcShaderHash & ShaderHash = InterpretCastRef<DxcShaderHash>(ShaderHashBlob->GetBufferPointer());

			TReferPtr<IDxcBlobUtf16> CompiledPDBBlobName;
			CompileResult->GetOutput(DXC_OUT_PDB, IID_PPV_ARGS(CompiledShaderPDB.GetPP()), CompiledPDBBlobName.GetPP());
			FWStringV PDBFileName { CompiledPDBBlobName->GetStringPointer(), CompiledPDBBlobName->GetStringLength() };
			AssertExpr(PDBFileName == ShaderPDBFileName);

			ShaderPDB = { (const byte *)CompiledShaderPDB->GetBufferPointer(), CompiledShaderPDB->GetBufferSize() };
			MetaData = { (const byte *)ReflectionBlob->GetBufferPointer(), ReflectionBlob->GetBufferSize() };

			AssertExpr(CompilerContext.DeviceRHI == EDeviceRHI::None || CompilerContext.DeviceRHI == EDeviceRHI::D3D12);
			if (CompilerContext.DeviceRHI == EDeviceRHI::D3D12)
			{
				DxcBuffer ReflectionData;
				ReflectionData.Encoding = DXC_CP_ACP;
				ReflectionData.Ptr = MetaData.Data;
				ReflectionData.Size = MetaData.Size;

				TReferPtr<ID3D12ShaderReflection> D3D12ShaderReflection ;
				Result = DxcUtils->CreateReflection(&ReflectionData, IID_PPV_ARGS(D3D12ShaderReflection.GetPP()));
				D3D12GenerateReflectParameters(CompilerContext.ShaderStage, ReflectParameters, D3D12ShaderReflection);
			}
		}

		CompilerContext.OnCompiled(ShaderCode, ShaderPDB, ReflectParameters);
		return true;
	}
}

