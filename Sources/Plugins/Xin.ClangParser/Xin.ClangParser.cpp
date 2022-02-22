#include "PCH.h"
#include "Xin.ClangParser.h"
#include "LLVMIncludes.h"

#include "Visitor.h"
#include "Generator.h"
#include "Utils.h"

namespace Xin
{
	constexpr FStringV TypesFolderName = u8"__Types"V;
	constexpr FStringV PCHFolderPath = u8"Documents/Build/ReflectorPCHs"V;

	bool ReflectModule(FReflectSettings & ReflectSettings)
	{
		FString ModuleName = FPath::GetFileName(ReflectSettings.ModuleRoot);
		FString RLogFilePath = FPath::Combine(ReflectSettings.ProjectRoot, PCHFolderPath, ModuleName + u8".rlog"V);
		TList<FString> RLogFiles;
		uint64_t RLogFileModifiedTime = 0;
		if (FFile::Exists(RLogFilePath))
		{
			FFileStream RLogFile { RLogFilePath, EFileModes::Input };
			FString Line;
			while (RLogFile.ReadLine(Line))
				RLogFiles.Add(Line);
			RLogFileModifiedTime = FFile::ModifiedTime(RLogFilePath);
		}

		FString PCHFilePath;
		FString PCHIncludeFilePath;
		if (ReflectSettings.PCHIncludeFile)
		{
			PCHIncludeFilePath = FPath::Combine(ReflectSettings.ModuleRoot, ReflectSettings.PCHIncludeFile);
			PCHFilePath = FPath::Combine(ReflectSettings.ProjectRoot, PCHFolderPath, ModuleName + u8".pch"V);

			if (not RunPCHAction(PCHIncludeFilePath, PCHFilePath, ReflectSettings))
			{
				PCHIncludeFilePath.Clear();
				PCHFilePath.Clear();
			}
		}

		TList<FString> HeaderFilePaths;
		//FString ModuleHeaderFilePath = FPath::Combine(ReflectSettings.ModuleRoot, ModuleName + u8".h"V);

		auto TravalDirectory = [&](this auto & This, FStringV DirectoryPath) -> void
			{
				FStringV DirectoryName = FPath::GetBaseName(DirectoryPath);
				if (DirectoryName == TypesFolderName)
					return;

				for (FStringV FilePath : FDirectory::EnumFiles(DirectoryPath))
				{
					if (FilePath.EndsWith(u8".h"V))
						HeaderFilePaths.Add(FilePath);
				}

				for (FStringV SubDirectoryPath : FDirectory::EnumDirectories(DirectoryPath))
				{
					This(SubDirectoryPath);
				}
			};
		TravalDirectory(ReflectSettings.ModuleRoot);

		TList<FString> ReflectedFilePaths;
		TList<FString> ReflectedRelativeFilePaths;
		for (sizet Index = 0; Index < HeaderFilePaths.Size; ++Index)
		{
			FStringV HeaderFilePath = HeaderFilePaths[Index];
			if (HeaderFilePath == PCHIncludeFilePath)
			{
				FConsole::WriteLine(u8"[{}/{}] skiped: \"{}\""V, Index + 1, uintx(HeaderFilePaths.Size), HeaderFilePath);
				continue;
			}

			FString RelativeFilePath = FPath::GetRelativePath(HeaderFilePath, ReflectSettings.ModuleRoot);
			FString ReflectedRelativeFilePath = FPath::Combine(TypesFolderName, FPath::ChangeExtention(RelativeFilePath, u8".inl"V));
			FString ReflectedFilePath = FPath::Combine(ReflectSettings.ModuleRoot, ReflectedRelativeFilePath);

			if (!ReflectSettings.ForceGenerate && RLogFiles.Contains(HeaderFilePath))
			{
				if (FFile::ModifiedTime(HeaderFilePath) < RLogFileModifiedTime)
				{
					FConsole::WriteLine(u8"[{}/{}] skiped: \"{}\""V, Index + 1, uintx(HeaderFilePaths.Size), HeaderFilePath);

					if (FFile::Exists(ReflectedFilePath))
					{
						ReflectedFilePaths.Add(ReflectedFilePath);
						ReflectedRelativeFilePaths.Add(ReflectedRelativeFilePath);
					}
					continue;
				}
			}

			FConsole::WriteLine(u8"[{}/{}] reflecting: \"{}\""V, Index + 1, uintx(HeaderFilePaths.Size), HeaderFilePath);

			FActionSink ActionSink { ReflectSettings };
			ActionSink.IncludeFilePath = RelativeFilePath;
			ActionSink.ReflectedFilePath = ReflectedFilePath;
			RunASTAction(HeaderFilePath, PCHFilePath, ActionSink);
			if (ActionSink.Sinked)
			{
				ReflectedFilePaths.Add(ReflectedFilePath);
				ReflectedRelativeFilePaths.Add(ReflectedRelativeFilePath);
			}
		}

		// Write rlog
		{
			FFileStream RLogFile { RLogFilePath, EFileModes::Output };
			for (sizet Index = 0; Index < HeaderFilePaths.Size; ++Index)
				RLogFile.WriteLine(HeaderFilePaths[Index]);
		}

		// Write Module.Types.cpp
		{
			FReflectBuilder Builder { ReflectSettings.LineEnd, ReflectSettings.TabChar };
			Builder.AppendLine(u8"// Generated code begin."V);
			Builder.AppendLine();
			if (PCHIncludeFilePath.Length)
				Builder.AppendLine(u8R"(#include "{0}")"V.Format(ReflectSettings.PCHIncludeFile));

			if (FFile::Exists(FPath::Combine(ReflectSettings.ModuleRoot, ModuleName + u8".h"V)))
				Builder.AppendLine(u8R"(#include "{0}.h")"V.Format(ModuleName));

			Builder.AppendLine();
			for (FStringV ReflectedRelativeFilePath : ReflectedRelativeFilePaths)
				Builder.AppendLine(u8R"(#include "{0}")"V.Format(ReflectedRelativeFilePath));
			Builder.AppendLine();
			Builder.AppendLine(u8"// Generated code end."V);

			FString TypesFilePath = FPath::Combine(ReflectSettings.ModuleRoot, ModuleName + u8".Types.cpp"V);
			Utils::SaveFileWithMD5(ReflectSettings, TypesFilePath, Builder.Result);
		}

		{
			FString TypesDirectory = FPath::Combine(ReflectSettings.ModuleRoot, TypesFolderName);
			TList<FString> TypeFiles = FDirectory::EnumTotalFiles(TypesDirectory);
			for (FString & TypeFile : TypeFiles)
			{
				if (ReflectedFilePaths.Contains(TypeFile))
					continue;

				FFile::Delete(TypeFile);
			}
		}
		return true;
	}

	bool ReflectFile(FReflectSettings & ReflectSettings, FStringV FilePath)
	{
		FString RelativeFilePath = FPath::GetRelativePath(FilePath, ReflectSettings.ModuleRoot);
		FString ReflectedFilePath = FPath::Combine(ReflectSettings.ModuleRoot, TypesFolderName, FPath::ChangeExtention(RelativeFilePath, u8".inl"V));

		FString PCHFilePath;
		if (ReflectSettings.PCHIncludeFile)
		{
			FStringV ModuleName = FPath::GetFileName(ReflectSettings.ModuleRoot);
			FString PCHIncludeFilePath = FPath::Combine(ReflectSettings.ModuleRoot, ReflectSettings.PCHIncludeFile);
			PCHFilePath = FPath::Combine(ReflectSettings.ProjectRoot, PCHFolderPath, ModuleName + u8".pch"V);
			if (not RunPCHAction(PCHIncludeFilePath, PCHFilePath, ReflectSettings))
			{
				PCHIncludeFilePath.Clear();
				PCHFilePath.Clear();
			}
		}

		FActionSink ActionSink { ReflectSettings };
		ActionSink.IncludeFilePath = RelativeFilePath;
		ActionSink.ReflectedFilePath = ReflectedFilePath;
		return RunASTAction(FilePath, PCHFilePath, ActionSink);
	}
}
