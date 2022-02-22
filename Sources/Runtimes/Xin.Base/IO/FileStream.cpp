#include "PCH.h"
#include "FileStream.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsHeader.h"
#endif

namespace Xin
{
	FFileStream::FFileStream(FStringV FilePath, EFileModes FileModes)
	{
		Open(FilePath, FileModes);
	}

	FFileStream::~FFileStream()
	{
		if (Handle)
		{
			CloseHandle(Handle);
			Handle = nullptr;
		}
	}

	uintx FFileStream::Read(byte * Data, uintx Size)
	{
		if (!Handle)
			return 0;

		DWORD NumberOfBytesRead = 0;
		BOOL bReadFile = ReadFile(Handle, Data, DWORD(Size), &NumberOfBytesRead, NULL);
		if (!bReadFile)
			return 0;
		return NumberOfBytesRead;
	}

	uintx FFileStream::Write(const byte * Data, uintx Size)
	{
		if (!Handle)
			return 0;

		DWORD NumberOfBytesWriten = 0;
		BOOL bReadFile = WriteFile(Handle, Data, DWORD(Size), &NumberOfBytesWriten, NULL);
		if (!bReadFile)
			return 0;
		assert(NumberOfBytesWriten == Size);
		return NumberOfBytesWriten;
	}

	uintx FFileStream::Peek(byte * Data, uintx Size)
	{
		if (!Handle)
			return 0;

		DWORD NumberOfBytesRead = 0;
		BOOL bReadFile = ReadFile(Handle, Data, DWORD(Size), &NumberOfBytesRead, NULL);
		if (!bReadFile || !NumberOfBytesRead)
			return 0;

		Seek(EStreamSeek::Current, -(intx)NumberOfBytesRead);
		return NumberOfBytesRead;
	}

	uintx FFileStream::Seek(EStreamSeek StreamSeek, intx Distance)
	{
		if (!Handle)
			return 0;

		DWORD dwMoveMethod = 0;
		switch (StreamSeek)
		{
		case EStreamSeek::Begin:
			dwMoveMethod = FILE_BEGIN;
			break;
		case EStreamSeek::End:
			dwMoveMethod = FILE_END;
			break;
		case EStreamSeek::Current:
			dwMoveMethod = FILE_CURRENT;
			break;
		default:
			return 0;
		}

		LARGE_INTEGER SeekDistance {};
		LARGE_INTEGER SeekedPosition {};
		SeekDistance.QuadPart = Distance;
		if (::SetFilePointerEx(Handle, SeekDistance, &SeekedPosition, dwMoveMethod))
		{
#ifdef XE_X86
			AssertExpresion(SeekedPosition.HighPart == 0);
#endif
			return SeekedPosition.QuadPart;
		}
		else
			return 0;
	}

	uintx FFileStream::Position() const
	{
		if (!Handle)
			return 0;

		LARGE_INTEGER Position;
		Position.QuadPart = 0;
		LARGE_INTEGER CurrentPosition;
		if (::SetFilePointerEx(Handle, Position, &CurrentPosition, FILE_CURRENT))
		{
#ifdef XE_X86
			AssertExpresion(CurrentPosition.HighPart == 0);
#endif
			return CurrentPosition.QuadPart;
		}
		return 0;
	}

	uintx FFileStream::Size() const
	{
		if (!Handle)
			return 0;

		LARGE_INTEGER FileSize;
		if (GetFileSizeEx(Handle, &FileSize))
		{
#ifdef XE_X86
			AssertExpresion(FileSize.HighPart == 0);
#endif
			return uintx(FileSize.QuadPart);
		}
		return 0;
	}

	EError FFileStream::Open(FStringV FilePath, EFileModes FileModes)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);

		//DesiredAccess
		uint32 DesiredAccess = 0;
		SetBits<uint32>(DesiredAccess, FILE_READ_DATA, FileModes & EFileModes::Input);
		SetBits<uint32>(DesiredAccess, FILE_WRITE_DATA, FileModes & EFileModes::Output);
		SetBits<uint32>(DesiredAccess, FILE_APPEND_DATA, FileModes & EFileModes::Append);

		//CreationDisposition
		uint32 CreationDisposition = 0;
		if (FileModes & EFileModes::Input)
		{
			CreationDisposition = OPEN_EXISTING;
		}
		else if (FileModes & (EFileModes::Output | EFileModes::Append))
		{
			if (!FFile::Exists(FilePath))
				CreationDisposition = CREATE_NEW;
			else if (FileModes & EFileModes::Append)
				CreationDisposition = OPEN_EXISTING;
			else/* if (FileModes * EFileModes::Output)*/
				CreationDisposition = CREATE_ALWAYS;
		}
		else {}

		//ShareMode
		TEnumWrapper<uint32> ShareMode = 0;
		//ShareMode.Set(FILE_SHARE_READ, !!(FileModes & EFileModes::ShareRead));
		//ShareMode.Set(FILE_SHARE_WRITE, !!(FileModes & EFileModes::ShareWrite));
		//ShareMode.Set(FILE_SHARE_DELETE, !!(FileModes & EFileModes::ShareDelete));
		if (FileModes & EFileModes::Input)
		{
			ShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		}
		else if (FileModes & (EFileModes::Output | EFileModes::Append))
		{
			ShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		}
		else {}
		
		uint32 FlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
		HANDLE hFile = CreateFileW(FilePathW, DesiredAccess, ShareMode, NULL, CreationDisposition, FlagsAndAttributes, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return EError::Inner;

		if (FileModes & EFileModes::Append)
		{
			LARGE_INTEGER Position;
			Position.QuadPart = 0;
			::SetFilePointerEx(hFile, Position, nullptr, FILE_END);
		}
		Handle = hFile;
		return EError::OK;
	}

	void FFileStream::Close()
	{
		if (Handle)
		{
			CloseHandle(Handle);
			Handle = nullptr;
		}
	}

	void FFileStream::Flush()
	{
		if (Handle)
		{
			FlushFileBuffers(Handle);
		}
	}
}
