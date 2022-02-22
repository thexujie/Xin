#pragma once

#include "Path.h"

namespace Xin
{
	class BASE_API FDirectory
	{
	public:


	public:
		static void SetAttribute(FStringV DirectoryPath, EFileAttribute DirectoryAttribute);

		static bool Exists(FStringV DirectoryPath);
		// return true for exists or new, false for new one.
		enum EDirectoryEnsureResult
		{
			DER_Fail = 0, // not exists, and create failed.
			DER_Exists,
			DER_Created,
		};
		static EDirectoryEnsureResult EnsureExists(FStringV DirectoryPath);
		static bool Create(FStringV DirectoryPath);
		static bool Delete(FStringV DirectoryPath);
		static bool HasEntries(FStringV DirectoryPath);
		static bool HasFiles(FStringV DirectoryPath, bool Recursively = false);
		static bool HasDirectories(FStringV DirectoryPath);
		
		static IEnumerable<FStringV> EnumEntries(FStringV DirectoryPath, FStringV Filter = u8"*.*"V);
		static IEnumerable<FStringV> EnumFiles(FStringV DirectoryPath, FStringV Filter = u8"*.*"V);
		static IEnumerable<FStringV> EnumTotalFiles(FStringV DirectoryPath, FStringV Filter = u8"*.*"V)
		{
			for (FStringV FilePath : EnumFiles(DirectoryPath, Filter))
				co_yield FilePath;

			for (FStringV SubDirectoryPath : EnumDirectories(DirectoryPath, Filter))
			{
				for (FStringV SubFilePath : EnumTotalFiles(SubDirectoryPath, Filter))
					co_yield SubFilePath;
			}
		}

		static IEnumerable<FStringV> EnumDirectories(FStringV DirectoryPath, FStringV Filter = u8"*.*"V);
		static IEnumerable<FStringV> EnumTotalDirectories(FStringV DirectoryPath, FStringV Filter = u8"*.*"V)
		{
			for (FStringV SubDiretoryPath : EnumDirectories(DirectoryPath, Filter))
			{
				co_yield SubDiretoryPath;

				for (FStringV SubSubDirectoryPath : EnumTotalDirectories(SubDiretoryPath, Filter))
					co_yield SubSubDirectoryPath;
			}
		}

		static bool Move(FStringV SourcePath, FStringV DestinationPath);
	};
}
