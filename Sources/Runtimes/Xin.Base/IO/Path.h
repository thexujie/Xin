#pragma once

#include "IO.Types.h"

namespace Xin
{
	class BASE_API FPath
	{
	public:
		FPath() = default;
		FPath(const FPath &) = default;
		FPath(FStringV FilePathIn) : FilePath(FilePathIn) {}
		FPath & operator =(const FPath &) = default;

		FString FilePath;

	public:
		static char8 DirectorySeparatorChar;

		static FStringV Combine(FStringV BasePath, FStringV Path);

		template<typename... PathTs>
		static FStringV Combine(FStringV BasePath, FStringV Path, PathTs... Paths)
		{
			if constexpr (sizeof...(PathTs) == 0)
				return Combine(BasePath, Path);
			else
			{
				FStringV CombinedPath = Combine(BasePath, Path);
				return Combine(CombinedPath, Paths...);
			}
		}

		static FStringV GetAbsolutePath(FStringV Path);
		static FStringV GetAbsolutePath(FStringV BasePath, FStringV Path);
		static FStringV GetRelativePath(FStringV Path, FStringV BasePath);

		static FStringV GetWorkingDirectory();

		static bool IsAbsolute(FStringV Path);
		static bool Exists(FStringV Path);
		static bool IsFile(FStringV Path);
		static bool IsDirectory(FStringV Path);

		// Directory/BaseName.ext

		// Directory
		static FStringV GetDirectory(FStringV Path);

		// BaseName
		static FStringV GetBaseName(FStringV Path);

		// BaseName.ext
		static FStringV GetFileName(FStringV Path);

		// ext
		static FStringV GetExtention(FStringV Path);

		/**
		 * 
		 * @param Path origion path
		 * @param Extention new extention with '.'
		 * @return 
		 */
		static FStringV ChangeExtention(FStringV Path, FStringV Extention);

		static FStringV LexicallyNormal(FStringV Path);

		static bool Move(FStringV SourcePath, FStringV DestinationPath);

		static uint64 GetFileSize(FStringV Path);

		static FStringV OpenFileDialog(FStringV Title, FStringV FileFilter);
	};
}
