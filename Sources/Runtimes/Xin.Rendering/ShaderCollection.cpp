#include "PCH.h"
#include "ShaderCollection.h"

#include "BaseEngine.h"

namespace Xin::Rendering
{
	IDataStream & operator >>(IDataStream & DataStream, FShaderArchive & ShaderArchive)
	{
		uint32 MagicNumber = DataStream.ReadUInt32();

		if (MagicNumber != ShaderArchiveMagicNumber)
			return DataStream;

		DataStream >> ShaderArchive.ShaderStage;
		DataStream >> ShaderArchive.FilePath;
		DataStream >> ShaderArchive.EntryName;

		DataStream >> ShaderArchive.FilesInclude;

		DataStream >> ShaderArchive.ShaderCode;
		DataStream >> ShaderArchive.ReflectParameters;

		return DataStream;
	}

	IDataStream & operator <<(IDataStream & DataStream, FShaderArchive & ShaderArchive)
	{
		DataStream << ShaderArchiveMagicNumber;

		DataStream << ShaderArchive.ShaderStage;
		DataStream << ShaderArchive.FilePath;
		DataStream << ShaderArchive.EntryName;

		DataStream << ShaderArchive.FilesInclude;

		DataStream << ShaderArchive.ShaderCode;
		DataStream << ShaderArchive.ReflectParameters;

		return DataStream;
	}

	class FEngineShaderCompilerContext : public FShaderCompilerContext
	{
	public:
		FEngineShaderCompilerContext(FEngine & Interface, const IShaderSource & ShaderSource, const FShaderCompilerEnvironment & Environment, FString ShaderModuleFilePath)
			: Engine(Interface), ShaderSource(ShaderSource), ShaderModuleFilePath(ShaderModuleFilePath)
		{
			DefinesFileContent.Append(u8"#ifndef _SHADER_DEFINES_H_\r\n"V);
			DefinesFileContent.Append(u8"#define _SHADER_DEFINES_H_ 1\r\n"V);
			DefinesFileContent.Append(u8"// Auto genetared from FD3D12Compiler\r\n\r\n"V);
			for (uintx DefineIndex = 0; DefineIndex < Environment.Definitions.Size; ++DefineIndex)
			{
				const FShaderCompilerEnvironment::FDefine & RHIShaderDefines = Environment.Definitions[DefineIndex];
				DefinesFileContent.Append(u8"#define "V);
				DefinesFileContent.Append(RHIShaderDefines.Name);
				DefinesFileContent.Append(u8" "V);
				DefinesFileContent.Append(RHIShaderDefines.Value);
				DefinesFileContent.Append(u8"\r\n"V);
			}
			DefinesFileContent.Append(u8"\r\n#endif // _SHADER_DEFINES_H_\r\n"V);

			IncludeFileMapping = Environment.IncludeFileMapping;
		}

		bool IncludeFile(FStringV FilePath, TList<byte> & FileContent) override
		{
			if (FilePath.EndsWith(u8"Defines.xsh"V))
			{
				FileContent.ResizeTo(DefinesFileContent.Length, EInitializeMode::None);
				MemoryCopy(FileContent.Data, DefinesFileContent.Data, DefinesFileContent.Length);
				return true;
			}

			if (FilePath.StartsWith(u8"./"V))
				FilePath = FilePath.SubView(2);

			if (FilePath.StartsWith(u8".\\"V))
				FilePath = FilePath.SubView(2);

			if (auto Iter = IncludeFileMapping.Find(FilePath); Iter != IncludeFileMapping.End())
				FilePath = Iter->Second;

			// 1. Find in ShaderCollection.RelativePath
			FString AssetPath = FPath::Combine(FPath::GetDirectory(ShaderSource.FilePath), FilePath);
			IAssetStreamRef AssetStream = Engine.LoadStream(AssetPath);

			// 2. Find in SearchPaths
			if (!AssetStream)
			{
				for (const FString & SearchPath : SearchPaths)
				{
					AssetPath = FPath::Combine(SearchPath, FilePath);
					AssetStream = Engine.LoadStream(AssetPath);

					if (AssetStream)
						break;
				}
			}

			// 2. Find in Engine.Resources
			if (!AssetStream)
			{
				AssetStream = Engine.LoadStream(FilePath);
				if (!AssetStream)
					AssetStream = Engine.LoadStream(FPath::Combine(u8"Shaders"V, FilePath));

				if (!AssetStream)
					return false;
			}

			FilesInclude.Add(AssetStream->FilePath);
			uintx FileSize = AssetStream->Size();

			FileContent.ResizeTo(FileSize, EInitializeMode::None);
			AssetStream->ReadData(FileContent.Data, FileContent.Size);
			return true;
		}


		void OnProcessed(FStringV ProcessedSource) override
		{
			FString OutputDirectory = FPath::GetDirectory(ShaderModuleFilePath);
			FDirectory::EnsureExists(OutputDirectory);

			FString XsfFilePath = FPath::Combine(OutputDirectory, FPath::GetBaseName(ShaderModuleFilePath) + u8".xsf"V);
			FFile::WriteString(XsfFilePath, ProcessedSource);
		}

		void OnCompiled(TView<byte> ShaderCode, TView<byte> ShaderPDB, const FReflectParameters & ReflectParameters) override
		{
			FString OutputDirectory = FPath::GetDirectory(ShaderModuleFilePath);
			FDirectory::EnsureExists(OutputDirectory);
			{
				FFileStream FileStream { ShaderModuleFilePath, EFileModes::Output };

				FShaderArchive ShaderArchive;
				ShaderArchive.ShaderStage = ShaderStage;
				ShaderArchive.EntryName = EntryName;
				ShaderArchive.FilePath = FilePath;
				ShaderArchive.FilesInclude = TList(FilesInclude);
				ShaderArchive.ShaderCode = ShaderCode;
				ShaderArchive.ReflectParameters = ReflectParameters;

				FileStream << ShaderArchive;
			}

			if (ShaderPDB)
			{
				FString PDBFilePath = FPath::Combine(OutputDirectory, FPath::GetBaseName(ShaderModuleFilePath) + u8".pdb"V);
				FFile::WriteBytes(PDBFilePath, ShaderPDB);
			}
		}

	public:
		FEngine & Engine;
		const IShaderSource & ShaderSource;
		FString ShaderModuleFilePath;

		TMap<FString, FString> IncludeFileMapping;
		TList<FString> SearchPaths;

	public:
		FString DefinesFileContent;
		TSet<FString> FilesInclude;
	};

	FShaderInstance::FShaderInstance(FEngine & Engine, const FShaderCollection & ShaderCollection, const FShaderCompilerEnvironment & Environment)
		: Engine(Engine), ShaderCollection(ShaderCollection), Environment(Environment)
	{
		FStringV ShaderFileName = FPath::GetFileName(ShaderCollection.ShaderSource.FilePath);
		FStringV ShaderBaseName = FPath::GetBaseName(ShaderCollection.ShaderSource.FilePath);
		const EShaderArchitecture ShaderArchitecture = EShaderArchitecture::Default;

		FString ShaderModuleName = FString(FPath::GetDirectory(ShaderCollection.ShaderSource.FilePath)).Replace(u8"/"V, u8"_"V).Replace(u8":"V, u8"_"V).Replace(u8"\\"V, u8"_"V) + u8"__"V + FPath::GetBaseName(ShaderBaseName);

		if (ShaderCollection.ShaderSource.EntryName)
		{
			ShaderModuleName += u8"_"V;
			ShaderModuleName += ShaderCollection.ShaderSource.EntryName;
		}
		if (Environment.Definitions.Size)
			ShaderModuleName += Format(u8"_{0:0X}"V, HashCombine(ShaderCollection.ShaderSource.EntryName, Environment.Definitions));
		//FString ShaderBaseName = FString(FPath::GetDirectory(ShaderCollection.ShaderPath)).Replace(u8"/"V, u8"_"V);

		FString ShaderModuleFolederName =
			GetDeviceRHIName(Engine.Device.GetDeviceRHI()) + u8"_"V + GetShaderArchitectureName(ShaderArchitecture) +
#ifdef XIN_DEBUG
			u8"_Debug"V
#else
			u8"_Release"V
#endif
		;
		FString ShaderCodeOutputPath = FPath::Combine(Engine.GetDocumentsPath(), u8"ShaderModules"V, ShaderModuleFolederName);

		FString ShaderModuleFileName = ShaderModuleName + CompiledShaderExtention(ShaderCollection.ShaderSource.ShaderStage);
		FString ShaderModuleFilePath = FPath::Combine(ShaderCodeOutputPath, ShaderModuleFileName);

		bool NeedCompile = !FFile::Exists(ShaderModuleFilePath);
		if (!NeedCompile)
		{
			uint64 CompileTime = FFile::ModifiedTime(ShaderModuleFilePath);
			if (FFile::FileSize(ShaderModuleFilePath) < 4)
			{
				NeedCompile = true;
			}
			else if (IAssetStreamRef AssetStream = Engine.LoadStream(ShaderCollection.ShaderSource.FilePath);
				AssetStream && FFile::ModifiedTime(AssetStream->FilePath) > CompileTime)
			{
				NeedCompile = true;
			}
			else
			{
				FFileStream FileStream { ShaderModuleFilePath, EFileModes::Input };
				FileStream >> ShaderArchive;
				if (ShaderArchive.ShaderStage == EShaderStage::None)
				{
					NeedCompile = true;
				}
				else
				{
					for (FString & IncludeFilePath : ShaderArchive.FilesInclude)
					{
						if (IAssetStreamRef IncludeFileStream = Engine.LoadStream(IncludeFilePath);
							IncludeFileStream && FFile::ModifiedTime(IncludeFileStream->FilePath) > CompileTime)
						{
							NeedCompile = true;
							ShaderArchive = { };
							break;
						}
					}
				}
			}
		}

		if (NeedCompile)
		{
			FString ShaderEntry = ShaderCollection.ShaderSource.EntryName.Length ? FString(ShaderCollection.ShaderSource.EntryName) : FString(u8"Main"V + ShaderFriendlyName(ShaderCollection.ShaderSource.ShaderStage));
			FEngineShaderCompilerContext CompilerContext(Engine, ShaderCollection.ShaderSource, Environment, ShaderModuleFilePath);

			CompilerContext.FilePath = ShaderCollection.ShaderSource.FilePath;
			CompilerContext.ModuleName = ShaderModuleName;

			CompilerContext.DeviceRHI = EDeviceRHI::None;
			CompilerContext.SourceLanguage = EShaderLanguage::HLSL;
			CompilerContext.TargetArchitecture = ShaderArchitecture;

			CompilerContext.ShaderStage = ShaderCollection.ShaderSource.ShaderStage;
			CompilerContext.EntryName = ShaderEntry;

#if !XIN_PUBLISH
			CompilerContext.DebugCompile = true;
#endif
#if XIN_DEBUG
			CompilerContext.OptimizationLevel = EOptimizationLevel::Level0;
#endif

			if (IAssetStreamRef ResourceStream = Engine.LoadStream(ShaderCollection.ShaderSource.FilePath))
			{
				FString ShaderSource { ResourceStream->Size() };
				ResourceStream->ReadData(ShaderSource.Data, ShaderSource.Length);

				bool CompilerResult = Engine.Device.GetShaderCompiler().CompilerFromSource(ShaderSource, CompilerContext);

				LogInfo(u8"IRHICompiler::Compiler [{}] {} -> {}, {} Defines."V, ShaderEntry, ShaderBaseName, ShaderModuleFilePath, Environment.Definitions.Size);
				for (uintx DefineIndex = 0; DefineIndex < Environment.Definitions.Size; ++DefineIndex)
					LogInfo(u8"    ---> {} = {}"V, Environment.Definitions[DefineIndex].Name, Environment.Definitions[DefineIndex].Value);
			}
			else
				LogWarning(u8"Can't find file \"{}\"."V, ShaderCollection.ShaderSource.FilePath);
		}

		FFileStream FileStream { ShaderModuleFilePath, EFileModes::Input };
		FileStream >> ShaderArchive;

		ShaderModule =  Engine.Device.CreateShaderModule(*this);
	}
}
