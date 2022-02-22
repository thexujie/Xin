#include "PCH.h"
#include "File.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsHeader.h"
#endif

namespace Xin
{
	FFile::FFile(FStringV FilePath)
	{
		Open(FilePath);
	}
	FFile::~FFile()
	{
		if (Handle)
		{
			CloseHandle(Handle);
			Handle = nullptr;
		}
	}

	bool FFile::Open(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		HANDLE hFile = CreateFile2(FilePathW, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, OPEN_EXISTING, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
			Handle = hFile;
		return !!Handle;
	}

	uintx FFile::GetSize() const
	{
		if (!Handle)
			return 0;
		
		LARGE_INTEGER FileSize;
		BOOL bGetFileSizeEx = GetFileSizeEx(Handle, &FileSize);
		assert(bGetFileSizeEx);
		return uintx(FileSize.QuadPart);
	}

	void FFile::SetAttribute(FStringV FilePath, EFileAttribute FileAttribute)
	{
		DWORD dwFileAttributes = 0;
		SetBits<DWORD>(dwFileAttributes, FILE_ATTRIBUTE_NORMAL, FileAttribute & EFileAttribute::Normal);
		SetBits<DWORD>(dwFileAttributes, FILE_ATTRIBUTE_HIDDEN, FileAttribute & EFileAttribute::Hidden);
		SetBits<DWORD>(dwFileAttributes, FILE_ATTRIBUTE_SYSTEM, FileAttribute & EFileAttribute::System);
		SetBits<DWORD>(dwFileAttributes, FILE_ATTRIBUTE_READONLY, FileAttribute & EFileAttribute::ReadOnly);

		SetFileAttributesW(FEncoding::ToWide(FilePath), dwFileAttributes);
	}

	uint64 FFile::FileSize(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);

		WIN32_FILE_ATTRIBUTE_DATA FileAttributeData = {};
		GetFileAttributesExW(FilePathW, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FileAttributeData);
		return (uint64(FileAttributeData.nFileSizeHigh) << 32) | FileAttributeData.nFileSizeLow;
	}

	uint64 FFile::AccessTime(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);

		WIN32_FILE_ATTRIBUTE_DATA FileAttributeData = {};
		GetFileAttributesExW(FilePathW, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FileAttributeData);
		return (uint64(FileAttributeData.ftLastAccessTime.dwHighDateTime) << 32) | FileAttributeData.ftLastAccessTime.dwLowDateTime;
	}

	uint64 FFile::CreationTime(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);

		WIN32_FILE_ATTRIBUTE_DATA FileAttributeData = {};
		GetFileAttributesExW(FilePathW, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FileAttributeData);
		return (uint64(FileAttributeData.ftCreationTime.dwHighDateTime) << 32) | FileAttributeData.ftCreationTime.dwLowDateTime;
	}

	uint64 FFile::ModifiedTime(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);

		WIN32_FILE_ATTRIBUTE_DATA FileAttributeData = {};
		GetFileAttributesExW(FilePathW, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FileAttributeData);
		return (uint64(FileAttributeData.ftLastWriteTime.dwHighDateTime) << 32) | FileAttributeData.ftLastWriteTime.dwLowDateTime;
	}

	bool FFile::Exists(FStringV FilePath)
	{
		if (FilePath.Contains(u8'*'))
		{
			WIN32_FIND_DATAW FindData = { };
			FWString FilePathW = FEncoding::ToWide(FilePath);
			HANDLE hFind = ::FindFirstFileExW(FilePathW.Data, FindExInfoBasic, &FindData, FindExSearchNameMatch, nullptr, 0);
			if (hFind == INVALID_HANDLE_VALUE)
				return false;
			::FindClose(hFind);
			return true;
		}
		else
		{
			DWORD FileAttribute = GetFileAttributesW(FEncoding::ToWide(FilePath));
			return (FileAttribute != INVALID_FILE_ATTRIBUTES) && ((FileAttribute & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE);
		}
	}

	bool FFile::Delete(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		return !!::DeleteFileW(FilePathW.Data);
	}

	bool FFile::Copy(FStringV SourcePath, FStringV DestinationPath, bool Override)
	{
		FWStringV SourcePathW = FEncoding::ToWide(SourcePath);
		FWStringV DestinationPathW = FEncoding::ToWide(DestinationPath);
		return !!::CopyFileW(SourcePathW.Data, DestinationPathW.Data, Override);
	}

	bool FFile::Move(FStringV SourcePath, FStringV DestinationPath)
	{
		FWStringV SourcePathW = FEncoding::ToWide(SourcePath);
		FWStringV DestinationPathW = FEncoding::ToWide(DestinationPath);
		return !!::MoveFileW(SourcePathW, DestinationPathW.Data);
	}
	
	FString FFile::ReadString(FStringV FilePath, EFileBOM * FileBOM)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		HANDLE hFile = CreateFileW(FilePathW, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return FString();

		LARGE_INTEGER FileSize;
		BOOL bGetFileSizeEx = GetFileSizeEx(hFile, &FileSize);
		assert(bGetFileSizeEx);

		if (FileSize.QuadPart > UInt32Max)
		{
			CloseHandle(hFile);
			return FString();
		}

		// BOM
		uint32 BOMSize = 0;
		{
			uint8 BOM[3] = {};
			DWORD NumberOfBytesReadBOM = 0;
			BOOL bReadFileBOM = ReadFile(hFile, BOM, 3, &NumberOfBytesReadBOM, NULL);
			if (!bReadFileBOM)
			{
				CloseHandle(hFile);
				return FString();
			}

			// UTF16BE
			if (NumberOfBytesReadBOM >= 2 && BOM[0] == 0xFE && BOM[1] == 0xFF)
			{
				if (FileBOM) *FileBOM = EFileBOM::UTF16BE;
				BOMSize = 2;
			}
			// UTF16LE
			else if (NumberOfBytesReadBOM >= 2 && BOM[0] == 0xFF && BOM[1] == 0xFE)
			{
				if (FileBOM) *FileBOM = EFileBOM::UTF16LE;
				BOMSize = 2;
			}
			// UTF8
			else if (NumberOfBytesReadBOM == 3 && BOM[0] == 0xEF && BOM[1] == 0xBB && BOM[2] == 0xBF)
			{
				if (FileBOM) *FileBOM = EFileBOM::UTF8;
				BOMSize = 3;
			}
			// ANSI
			else
			{
				if (FileBOM) *FileBOM = EFileBOM::None;
				BOMSize = 0;
			}
		}

		SetFilePointer(hFile, BOMSize, 0, FILE_BEGIN);
		FString String(FileSize.LowPart - BOMSize);
		DWORD NumberOfBytesRead = 0;
		BOOL bReadFile = ReadFile(hFile, String.GetData(), FileSize.LowPart - BOMSize, &NumberOfBytesRead, NULL);
		CloseHandle(hFile);

		if (!bReadFile || NumberOfBytesRead != FileSize.LowPart - BOMSize)
			return FString();
		return String;
	}

	uintx FFile::WriteString(FStringV FilePath, FStringV String)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		HANDLE hFile = CreateFileW(FilePathW, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		DWORD NumberOfBytesWriten = 0;
		[[maybe_unused]] BOOL bSaveFile = WriteFile(hFile, String.Data, DWORD(String.Size) * sizeof(char8), &NumberOfBytesWriten, NULL);
		CloseHandle(hFile);
		
		return NumberOfBytesWriten;
	}

	uintx FFile::WriteBytes(FStringV FilePath, TView<byte> Bytes)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		HANDLE hFile = CreateFileW(FilePathW, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		DWORD NumberOfBytesWriten = 0;
		[[maybe_unused]] BOOL bSaveFile = WriteFile(hFile, Bytes.Data, DWORD(Bytes.Size), &NumberOfBytesWriten, NULL);
		CloseHandle(hFile);

		return NumberOfBytesWriten;
	}

	TList<byte> FFile::ReadBytes(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		HANDLE hFile = CreateFileW(FilePathW, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return TList<byte>();

		LARGE_INTEGER FileSize;
		BOOL bGetFileSizeEx = GetFileSizeEx(hFile, &FileSize);
		assert(bGetFileSizeEx);

		if (FileSize.QuadPart > UInt32Max)
		{
			CloseHandle(hFile);
			return TList<byte>();
		}

		TList<byte> Vector(FileSize.LowPart);
		
		DWORD NumberOfBytesRead = 0;
		BOOL bReadFile = ReadFile(hFile, Vector.GetData(), FileSize.LowPart, &NumberOfBytesRead, NULL);
		CloseHandle(hFile);
		
		if (!bReadFile || NumberOfBytesRead != FileSize.LowPart)
			return TList<byte>();
		return Vector;
	}
}