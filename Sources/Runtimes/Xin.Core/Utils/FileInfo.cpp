#include "PCH.h"
#include "FileInfo.h"

#include "Platform/Windows/WindowsPrivate.h"
#include <shobjidl_core.h>

namespace Xin
{
	TList<FFileInfoRef> FFileInfo::EnumPath(FStringV DirectoryPath)
	{
		TList<FFileInfoRef> FileInfos;

		for (FStringV FilePath : FDirectory::EnumFiles(DirectoryPath))
		{
			FWStringV FilePathW = FEncoding::ToWide(FilePath);
			WIN32_FILE_ATTRIBUTE_DATA FileAttributeData = { };
			GetFileAttributesExW(FilePathW.Data, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FileAttributeData);

			FFileInfoRef FileInfo = MakeRefer<FFileInfo>();
			FileInfo->FilePath = FilePath;
			FileInfo->FileName = FPath::GetFileName(FilePath);
			FileInfo->FileSize = ((uint64)FileAttributeData.nFileSizeHigh << 32ull) | (uint64)FileAttributeData.nFileSizeLow;
			FileInfo->CreationTime = ((uint64)FileAttributeData.ftCreationTime.dwHighDateTime << 32ull) | (uint64)FileAttributeData.ftCreationTime.dwLowDateTime;
			FileInfos.Add(FileInfo);
		}

		return FileInfos;
	}

	IPixmapRef FFileInfo::GetFileIcon(FStringV FilePath, SizeU IconSize)
	{
		FWString FilePathW = FEncoding::ToWide(FilePath);
		FilePathW.Replace(L"/"V, L"\\"V);
		TReferPtr<IShellItemImageFactory> ItemImageFactory;
		HRESULT Result = SHCreateItemFromParsingName(FilePathW.Data, nullptr, IID_IShellItemImageFactory, ItemImageFactory.GetVV());
		if (Windows::Failed(Result))
			return nullptr;

		SIIGBF Flags = 0;
		HBITMAP hBitmap = NULL;
		Result = ItemImageFactory->GetImage({ (LONG)IconSize.Width, (LONG)IconSize.Height }, Flags, &hBitmap);
		if (Windows::Failed(Result))
			return nullptr;
		return new Windows::FPixmapHBITMAP(hBitmap);
	}
}
