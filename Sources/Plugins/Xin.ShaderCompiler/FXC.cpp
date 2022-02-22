#include "PCH.h"
#include "FXC.h"

#include "Xin.Core/Platform/Windows/WindowsPrivate.h"
#include "D3D12Reflect.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#pragma warning(disable: 4505)

namespace Xin::ShaderCompiler
{
	forceinline FStringV GetBlobMessageDXC(TReferPtr<ID3DBlob> D3DBlob)
	{
		if (!D3DBlob || D3DBlob->GetBufferSize() < 2)
			return FStringV();

		return D3DBlob ? FEncoding::ToUtf8(FAStringV(static_cast<const char *>(D3DBlob->GetBufferPointer()), D3DBlob->GetBufferSize() - 1)) : FStringV();
	}

	static FStringV GetFXCDefaultTargetProfile(EShaderStage ShaderStage)
	{
		return GetTargetProfile_5_0(ShaderStage);
	}

	class FD3DIncludeFile : public ISharedRefer
	{
	public:
		FD3DIncludeFile(TList<byte> && FileContent) : FileContent(Move(FileContent)) {}
		TList<byte> FileContent;
	};

	class FD3DInclude final : public ID3DInclude
	{
	public:
		FD3DInclude(FShaderCompilerContext & CompilerInput) : CompilerInput(CompilerInput)
		{
		}

		HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID * ppData, UINT * pBytes) override
		{
			FString FilePath = FEncoding::AsUtf8(pFileName);

			TList<byte> FileContent;
			if (CompilerInput.IncludeFile(FilePath, FileContent))
			{
				TReferPtr<FD3DIncludeFile> D3DIncludeFile = new FD3DIncludeFile(Move(FileContent));
				*ppData = D3DIncludeFile->FileContent.Data;
				*pBytes = uint32(D3DIncludeFile->FileContent.Size);
				IncludeFiles.Add(D3DIncludeFile);
				return S_OK;
			}
			return ERROR_NO_MORE_FILES;
		}

		HRESULT Close(LPCVOID pData) override
		{
			auto Iter = IncludeFiles.FindBy([pData](const TReferPtr<FD3DIncludeFile> & D3DIncludeFile) { return pData == D3DIncludeFile->FileContent.Data; });
			AssertExpr(Iter != IncludeFiles.End());
			if (Iter != IncludeFiles.End())
				IncludeFiles.Erase(Iter);
			return S_OK;
		}

	private:
		FShaderCompilerContext & CompilerInput;
		TList<TReferPtr<FD3DIncludeFile>> IncludeFiles;
	};

	FFXCShaderCompiler::FFXCShaderCompiler()
	{

	}

	bool FFXCShaderCompiler::CompilerFromMemory(FStringV ShaderSource, FShaderCompilerContext & CompilerContext)
	{
		FStringV FileName = FPath::GetFileName(CompilerContext.FilePath);

		TReferPtr<ID3DBlob> ErrorBlob;
		TReferPtr<ID3DBlob> ShaderBlob;

		FD3DInclude D3DInclude(CompilerContext);
		uint32 D3D12CompileFlags = 0;
		D3D12CompileFlags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
		SetBits<uint32>(D3D12CompileFlags, D3DCOMPILE_DEBUG, CompilerContext.DebugCompile);
		SetBits<uint32>(D3D12CompileFlags, D3DCOMPILE_SKIP_OPTIMIZATION, CompilerContext.DebugCompile);
		SetBits<uint32>(D3D12CompileFlags, D3DCOMPILE_OPTIMIZATION_LEVEL0, CompilerContext.OptimizationLevel == EOptimizationLevel::Level0);
		SetBits<uint32>(D3D12CompileFlags, D3DCOMPILE_OPTIMIZATION_LEVEL1, CompilerContext.OptimizationLevel == EOptimizationLevel::Level1);
		SetBits<uint32>(D3D12CompileFlags, D3DCOMPILE_OPTIMIZATION_LEVEL2, CompilerContext.OptimizationLevel == EOptimizationLevel::Level2);
		SetBits<uint32>(D3D12CompileFlags, D3DCOMPILE_OPTIMIZATION_LEVEL3, CompilerContext.OptimizationLevel == EOptimizationLevel::Level3);

		std::string ShaderName = FEncoding::ToStdString(FileName);
		FAStringV MainEntryA = FEncoding::AsAnsi(CompilerContext.EntryName);
		FAStringV TargetA = FEncoding::AsAnsi(GetFXCDefaultTargetProfile(CompilerContext.ShaderStage));
		HRESULT Result = D3DCompile2(ShaderSource.Data, ShaderSource.Size, ShaderName.c_str(), nullptr, &D3DInclude, MainEntryA, TargetA, D3D12CompileFlags, 0, 0, nullptr, 0, ShaderBlob.GetPP(), ErrorBlob.GetPP());

		if (Result)
		{
			FStringV BlobMessage = GetBlobMessageDXC(ErrorBlob);
			if (BlobMessage)
			{
				if (BlobMessage.EndsWith(u8'\n'))
					--BlobMessage.Size;
				LogWarning(u8"D3DCompileFromFile failed, 0x{:x}.\n{}"V, uint32(Result), BlobMessage);
			}
			else
				LogWarning(u8"D3DCompileFromFile failed, 0x{:x}."V, uint32(Result));
			AssertFailed(u8"D3DCompileFromFile failed"V);
			return false;
		}

		TView<byte> ShaderCode = { (const byte *)ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize() };

		FReflectParameters ReflectParameters;
		if (CompilerContext.TargetArchitecture == EShaderArchitecture::DXBC)
		{
			AssertExpr(CompilerContext.DeviceRHI == EDeviceRHI::None 
				|| CompilerContext.DeviceRHI == EDeviceRHI::D3D12
				|| CompilerContext.DeviceRHI == EDeviceRHI::D3D12);
			if (CompilerContext.DeviceRHI == EDeviceRHI::D3D12)
			{
				TReferPtr<ID3D12ShaderReflection> D3D12ShaderReflection;
				Result = ::D3DReflect(ShaderCode.Data, ShaderCode.Size, __uuidof(ID3D12ShaderReflection), D3D12ShaderReflection.GetVV());
				if (Windows::Failed(Result))
				{
					LogWarning(u8"D3DReflect failed {0}", Windows::ErrorDescription(Result));
				}

				D3D12GenerateReflectParameters(CompilerContext.ShaderStage, ReflectParameters, D3D12ShaderReflection.Get());
			}
		}
		CompilerContext.OnCompiled(ShaderCode, {}, ReflectParameters);
		return true;
	}
}

