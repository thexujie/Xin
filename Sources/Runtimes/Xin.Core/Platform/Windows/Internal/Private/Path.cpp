#include "PCH.h"
#include "Path.h"

#include <pathcch.h>
#pragma comment(lib, "pathcch.lib")

namespace Xin::Windows::Internal
{
	FWString NtFilePathToDosFilePath(UNICODE_STRING & NtFilePath)
	{
		OBJECT_ATTRIBUTES ObjectAttributes;
		InitializeObjectAttributes(&ObjectAttributes, &NtFilePath, OBJ_CASE_INSENSITIVE, NULL, NULL);
		IO_STATUS_BLOCK IoStatusBlock;

		HANDLE ntFile = NULL;
		NTSTATUS OpenStatus = NtOpenFile(&ntFile, FILE_READ_ATTRIBUTES, &ObjectAttributes, &IoStatusBlock, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0);
		if (NT_SUCCESS(OpenStatus))
		{
			FWString FilanFilePath(NtFilePath.Length / 2 + 1);
			DWORD FinalLength = GetFinalPathNameByHandleW(ntFile, FilanFilePath.Data, FilanFilePath.Length, VOLUME_NAME_DOS);
			if (FinalLength >= FilanFilePath.Length)
			{
				FilanFilePath.Resize(FinalLength);
				FinalLength = GetFinalPathNameByHandleW(ntFile, FilanFilePath.Data, FilanFilePath.Length, VOLUME_NAME_DOS);
			}

			if (FinalLength < FilanFilePath.Length)
			{
				FWString Win32FilePath(FilanFilePath.Length);
				HRESULT CanonicalResult = PathCchCanonicalizeEx(Win32FilePath.Data, Win32FilePath.Length, FilanFilePath.Data, PATHCCH_NONE);
				if (SUCCEEDED(CanonicalResult))
				{
					Win32FilePath.Length = Length(Win32FilePath.Data);
					NtClose(ntFile);
					return Win32FilePath;
				}
			}
			NtClose(ntFile);
		}
		return { };
	}
}
