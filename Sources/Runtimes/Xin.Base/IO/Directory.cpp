#include "PCH.h"
#include "Directory.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsHeader.h"
#endif

namespace Xin
{
#pragma optimize("", off)
	static IEnumerable<WIN32_FIND_DATAW> EnumPath(FWStringV PathPattern)
	{
		WIN32_FIND_DATAW FindData = {};
		HANDLE hFind = ::FindFirstFileExW(PathPattern.Data, FindExInfoBasic, &FindData, FindExSearchNameMatch, nullptr, 0);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == 0)
					continue;

				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == L'.' && FindData.cFileName[2] == 0)
					continue;

				TSlice FindPath { FindData.cFileName };
				std::replace(FindPath.Begin(), FindPath.End(), L'\\', (charw)FPath::DirectorySeparatorChar);
				co_yield FindData;
			}
			while (::FindNextFileW(hFind, &FindData));
			::FindClose(hFind);
		}
		co_return;
	}

	void FDirectory::SetAttribute(FStringV DirectoryPath, EFileAttribute DirectoryAttribute)
	{
		DWORD dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		SetBits<DWORD>(dwFileAttributes, FILE_ATTRIBUTE_NORMAL, DirectoryAttribute & EFileAttribute::Normal);
		SetBits<DWORD>(dwFileAttributes, FILE_ATTRIBUTE_HIDDEN, DirectoryAttribute & EFileAttribute::Hidden);
		SetBits<DWORD>(dwFileAttributes, FILE_ATTRIBUTE_SYSTEM, DirectoryAttribute & EFileAttribute::System);
		SetBits<DWORD>(dwFileAttributes, FILE_ATTRIBUTE_READONLY, DirectoryAttribute & EFileAttribute::ReadOnly);

		SetFileAttributesW(FEncoding::ToWide(DirectoryPath), dwFileAttributes);
	}

	bool FDirectory::Exists(FStringV DirectoryPath)
	{
		if (DirectoryPath.Contains(u8'*'))
		{
			WIN32_FIND_DATAW FindData = { };
			FWString DirectoryPathW = FEncoding::ToWide(DirectoryPath);
			HANDLE hFind = ::FindFirstFileExW(DirectoryPathW.Data, FindExInfoBasic, &FindData, FindExSearchNameMatch, nullptr, 0);
			if (hFind == INVALID_HANDLE_VALUE)
				return false;
			::FindClose(hFind);
			return true;
		}
		else
		{
			DWORD FileAttribute = GetFileAttributesW(FEncoding::ToWide(DirectoryPath));
			return (FileAttribute != INVALID_FILE_ATTRIBUTES) && ((FileAttribute & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
		}
	}

	FDirectory::EDirectoryEnsureResult FDirectory::EnsureExists(FStringV DirectoryPath)
	{
		if (!DirectoryPath.Size)
			return DER_Fail;
		FWString DirectoryPathW = FEncoding::ToWide(DirectoryPath);

		{
			DWORD FileAttribute = GetFileAttributesW(DirectoryPathW.Data);
			if (FileAttribute != INVALID_FILE_ATTRIBUTES)
				return (FileAttribute & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY ? DER_Exists : DER_Fail;
		}

		for (uintx CharIndex = 0; CharIndex < DirectoryPathW.Length; ++CharIndex)
		{
			if (CharIndex + 1 == DirectoryPathW.Length|| DirectoryPathW[CharIndex] == L'/' || DirectoryPathW[CharIndex] == L'\\')
			{
				if (CharIndex + 1 < DirectoryPathW.Length)
					DirectoryPathW[CharIndex] = L'\0';
				DWORD FileAttribute = GetFileAttributesW(DirectoryPathW.Data);
				if (FileAttribute == INVALID_FILE_ATTRIBUTES)
				{
					if (!::CreateDirectoryW(DirectoryPathW.Data, NULL))
						return FDirectory::EDirectoryEnsureResult::DER_Fail;
				}
				else
				{
					if ((FileAttribute & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
						return DER_Fail;
				}
				if (CharIndex + 1 < DirectoryPathW.Length)
					DirectoryPathW[CharIndex] = L'\\';
			}
		}
		return FDirectory::EDirectoryEnsureResult::DER_Exists;
	}

	bool FDirectory::Create(FStringV DirectoryPath)
	{
		FStringV DirectoryParentPath = FPath::GetDirectory(DirectoryPath);
		if (!Exists(DirectoryParentPath))
			Create(DirectoryParentPath);
		
		FWString DirectoryPathW = FEncoding::ToWide(DirectoryPath);
		return !!::CreateDirectoryW(DirectoryPathW.Data, NULL);
	}

	bool FDirectory::Delete(FStringV DirectoryPath)
	{
		FWStringV DirectoryPathW = FEncoding::ToWide(DirectoryPath);
		auto FunDelete = [](FWStringV Path, auto && Self)->bool
		{
			if (Path.Empty())
				return true;

			if (Path.Last() == L'\\' || Path.Last() == L'/')
				Path = Path.SubView(0, Path.Size - 1);

			FWString PathPattern = Path + TView(L"\\*.*");
			for (const WIN32_FIND_DATAW & FindData : EnumPath(PathPattern))
			{
				FWString SubPath = Path + L"\\" + FindData.cFileName;
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
				{
					bool IsFileDeleted = !!::DeleteFileW(SubPath.Data);
					if (!IsFileDeleted)
						return false;
				}
				else if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					bool IsSubFolderDeleted = Self(SubPath, Self);
					if (!IsSubFolderDeleted)
						return false;
				}
				else {}
			}
			return !!::RemoveDirectoryW(Path.Data);
		};
		
		return FunDelete(DirectoryPathW.Data, FunDelete);
	}

	bool FDirectory::HasEntries(FStringV DirectoryPath)
	{
		FWString DirectoryPathW = FEncoding::ToWide(FPath::Combine(DirectoryPath, u8"*.*"V));
		WIN32_FIND_DATAW FindData = {};
		HANDLE hFind = ::FindFirstFileExW(DirectoryPathW.Data, FindExInfoBasic, &FindData, FindExSearchNameMatch, nullptr, 0);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == 0)
					continue;

				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == L'.' && FindData.cFileName[2] == 0)
					continue;

				if (FindData.dwFileAttributes & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_DIRECTORY))
				{
					::FindClose(hFind);
					return true;
				}
			}
			while (::FindNextFileW(hFind, &FindData));
			::FindClose(hFind);
		}
		return false;
	}
	
	bool FDirectory::HasFiles(FStringV DirectoryPath, bool Recursively)
	{
		FWString DirectoryPathW = FEncoding::ToWide(FPath::Combine(DirectoryPath, u8"*.*"V));
		WIN32_FIND_DATAW FindData = {};
		HANDLE hFind = ::FindFirstFileExW(DirectoryPathW.Data, FindExInfoBasic, &FindData, FindExSearchNameMatch, nullptr, 0);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == 0)
					continue;

				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == L'.' && FindData.cFileName[2] == 0)
					continue;

				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
				{
					::FindClose(hFind);
					return true;
				}

				if (Recursively && (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					if (HasFiles(FEncoding::ToUtf8(FWStringV(FindData.cFileName))))
					{
						::FindClose(hFind);
						return true;
					}
				}
			}
			while (::FindNextFileW(hFind, &FindData));
			::FindClose(hFind);
		}
		return false;
	}
	
	bool FDirectory::HasDirectories(FStringV DirectoryPath)
	{
		FWString DirectoryPathW = FEncoding::ToWide(FPath::Combine(DirectoryPath, u8"*.*"V));
		WIN32_FIND_DATAW FindData = {};
		HANDLE hFind = ::FindFirstFileExW(DirectoryPathW.Data, FindExInfoBasic, &FindData, FindExSearchNameMatch, nullptr, 0);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == 0)
					continue;

				if (FindData.cFileName[0] == L'.' && FindData.cFileName[1] == L'.' && FindData.cFileName[2] == 0)
					continue;

				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					::FindClose(hFind);
					return true;
				}
			}
			while (::FindNextFileW(hFind, &FindData));
			::FindClose(hFind);
		}
		return false;
	}
	
	IEnumerable<FStringV> FDirectory::EnumEntries(FStringV DirectoryPath, FStringV Filter)
	{
		FWString DirectoryPathW = FEncoding::ToWide(FPath::Combine(DirectoryPath, Filter));
		for (const WIN32_FIND_DATAW & FindData : EnumPath(DirectoryPathW))
		{
			if (FindData.dwFileAttributes & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_DIRECTORY))
			{
				FString Entry = FPath::Combine(DirectoryPath, FEncoding::ToUtf8(FindData.cFileName));
				co_yield Entry;
			}
		}
	}

	IEnumerable<FStringV> FDirectory::EnumFiles(FStringV DirectoryPath, FStringV Filter)
	{
		FWString DirectoryPathW = FEncoding::ToWide(FPath::Combine(DirectoryPath, Filter));
		for (const WIN32_FIND_DATAW & FindData : EnumPath(DirectoryPathW))
		{
			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				FString Entry = FPath::Combine(DirectoryPath, FEncoding::ToUtf8(FindData.cFileName));
				co_yield Entry;
			}
		}
	}

	IEnumerable<FStringV> FDirectory::EnumDirectories(FStringV DirectoryPath, FStringV Filter)
	{
		FWString DirectoryPathW = FEncoding::ToWide(FPath::Combine(DirectoryPath, Filter));
		for (const WIN32_FIND_DATAW & FindData : EnumPath(DirectoryPathW))
		{
			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				FString Entry = FPath::Combine(DirectoryPath, FEncoding::ToUtf8(FindData.cFileName));
				co_yield Entry;
			}
		}
	}
	
	bool FDirectory::Move(FStringV SourcePath, FStringV DestinationPath)
	{
		FWStringV SourcePathW = FEncoding::ToWide(SourcePath);
		FWStringV DestinationPathW = FEncoding::ToWide(DestinationPath);
		return !!::MoveFileW(SourcePathW.Data, DestinationPathW.Data);
	}
}
