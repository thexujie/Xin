#include "PCH.h"
#include "FileTransaction.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsHeader.h"
#include <ktmw32.h>
#pragma comment(lib, "KtmW32.lib")
#endif

namespace Xin
{
	FFileTransaction::FFileTransaction()
	{
		Handle = ::CreateTransaction(NULL, 0, 0, 0, INFINITE, 0, nullptr);
	}
	FFileTransaction::~FFileTransaction()
	{
		Close();
	}

	bool FFileTransaction::CreateFile(FStringV FilePath, EFileCreate FileCreate)
	{
		uint32 DesiredAccess = 0;
		uint32 CreationDisposition = (FileCreate & EFileCreate::Override) ? CREATE_ALWAYS : CREATE_NEW;
		uint32 FlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
		uint32 ShareMode = 0;

		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		HANDLE hFile = CreateFileTransactedW(FilePathW, DesiredAccess, ShareMode, NULL, CreationDisposition, FlagsAndAttributes, NULL, Handle, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;
		::CloseHandle(hFile);
		return true;
	}

	bool FFileTransaction::MoveFile(FStringV SourcePath, FStringV DestinationPath)
	{
		FWStringV SourcePathW = FEncoding::ToWide(SourcePath);
		FWStringV DestinationPathW = FEncoding::ToWide(DestinationPath);
		return !!::MoveFileTransactedW(SourcePathW, DestinationPathW, nullptr, nullptr, 0, Handle);
	}

	bool FFileTransaction::DeleteFile(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		return !!::DeleteFileTransactedW(FilePathW.Data, Handle);
	}

	bool FFileTransaction::MoveDirectory(FStringV SourcePath, FStringV DestinationPath)
	{
		FWStringV SourcePathW = FEncoding::ToWide(SourcePath);
		FWStringV DestinationPathW = FEncoding::ToWide(DestinationPath);
		return !!::MoveFileTransactedW(SourcePathW, DestinationPathW.Data, nullptr, nullptr, 0, Handle);
	}

	bool FFileTransaction::DeleteDirectory(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		return !!::RemoveDirectoryTransactedW(FilePathW, Handle);
	}

	bool FFileTransaction::Commit()
	{
		return !!::CommitTransaction(Handle);
	}

	void FFileTransaction::Close()
	{
		if (Handle)
		{
			::CloseHandle(Handle);
			Handle = nullptr;
		}
	}
}
