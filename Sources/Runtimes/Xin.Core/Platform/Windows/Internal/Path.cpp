#include "PCH.h"
#include "Path.h"

#include "Private/Path.h"

namespace Xin::Windows::Internal
{
	FWString NtFilePathToDosFilePath(FWString FilePath)
	{
		UNICODE_STRING NtFilePath;
		NtFilePath.Buffer = FilePath.Data;
		NtFilePath.Length = USHORT(FilePath.Length * 2);
		NtFilePath.MaximumLength = USHORT(FilePath.Length * 2);
		return NtFilePathToDosFilePath(NtFilePath);
	}
}
