#include "PCH.h"
#include "FileVersion.h"

#include "Private/InternalNT.h"
#include "Winversion.h"

#pragma comment(lib, "version.lib")

namespace Xin::Windows::Internal
{
	static constexpr FWStringV VarFileInfo_Translation = L"\\VarFileInfo\\Translation";

	static constexpr FWStringV VersionBlockNames[uintx(EFileVersionBlock::Count)] =
	{
		L"Comments",
		L"InternalName",
		L"ProductName",
		L"CompanyName",
		L"LegalCopyright",
		L"ProductVersion",
		L"FileDescription",
		L"LegalTrademarks",
		L"PrivateBuild",
		L"FileVersion",
		L"OriginalFilename",
		L"SpecialBuild",
	};

	static FWString QueryVersionBlock(LPCVOID Block, EFileVersionBlock FileVersionBlock, const LANGANDCODEPAGE & LangandCodepage)
	{
		FWStringV QueryName = Format(L"StringFileInfo\\{:04x}{:04x}\\{}"V, LangandCodepage.Language, LangandCodepage.CodePage, VersionBlockNames[uintx(FileVersionBlock)]);

		charw * Version;
		uint32 VersionLength;
		if (VerQueryValueW(Block, QueryName.Data, (LPVOID *)&Version, &VersionLength))
		{
			if (VersionLength <= 1)
				return FWString::None;
			return { Version, VersionLength - 1 };
		}
		return FWString::None;
	}

	FFileVersion::FFileVersion(FStringV FilePath)
	{
		LoadFromFile(FilePath);
	}

	bool FFileVersion::NtLoadFromFile(FStringV FilePath)
	{
		FWString FilePathW = FEncoding::ToWide(FilePath);

		UNICODE_STRING NtFilePath;
		NTSTATUS Status = RtlDosLongPathNameToNtPathName_U_WithStatus(FilePathW.Data, &NtFilePath, NULL, NULL);
		if (!NT_SUCCESS(Status))
			return false;

		OBJECT_ATTRIBUTES ObjectAttributes;
		IO_STATUS_BLOCK IoStatusBlock;
		InitializeObjectAttributes(&ObjectAttributes, &NtFilePath, OBJ_CASE_INSENSITIVE, NULL, NULL);

		NTHandle FileHandle;
		Status = NtCreateFile(
			&FileHandle,
			FILE_READ_ATTRIBUTES | FILE_READ_DATA | SYNCHRONIZE,
			&ObjectAttributes,
			&IoStatusBlock,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			0);
		RtlFreeUnicodeString(&NtFilePath);

		if (!NT_SUCCESS(Status))
			return false;

		NTHandle SectionHandle;

		AssertExpr(FWinversion::Current > Winversion_8);
		Status = NtCreateSection(
			&SectionHandle,
			SECTION_QUERY | SECTION_MAP_READ,
			NULL,
			NULL,
			PAGE_READONLY,
			FWinversion::Current < Winversion_8 ? SEC_IMAGE : SEC_IMAGE_NO_EXECUTE,
			FileHandle
		);
		if (!NT_SUCCESS(Status))
			return false;

		//AssertExpresion(GWinversion > Winversion_10_22H2);
		//AssertExpresion(GWinversion == Winversion_11);
		//AssertExpresion(GWinversion < Winversion_11_22H2);

		PVOID ImageBaseAddress = nullptr;
		SIZE_T ImageBaseSize = 0;

		//NtSuppressDebugMessage(TRUE);
		Status = NtMapViewOfSection(
			SectionHandle,
			NtCurrentProcess(),
			&ImageBaseAddress,
			0,
			0,
			NULL,
			&ImageBaseSize,
			ViewUnmap,
			FWinversion::Current < Winversion_10_RS2 ? 0 : MEM_MAPPED,
			PAGE_READONLY
		);
		//NtSuppressDebugMessage(TRUE);

		if (Status == STATUS_IMAGE_NOT_AT_BASE)
			Status = STATUS_SUCCESS;

		PVOID BaseAddress = nullptr;
		if (NT_SUCCESS(Status))
		{
			// Windows returns the address with bitwise OR|2 for use with LDR_IS_IMAGEMAPPING (dmex)
			BaseAddress = LDR_MAPPEDVIEW_TO_IMAGEMAPPING(ImageBaseAddress);
		}

		if (!BaseAddress)
			return false;

		LDR_RESOURCE_INFO LdrResourceInfo;
		LdrResourceInfo.Type = (ULONG_PTR)VS_FILE_INFO;
		LdrResourceInfo.Name = (ULONG_PTR)MAKEINTRESOURCE(VS_VERSION_INFO);
		LdrResourceInfo.Language = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

		PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry;
		if (!NT_SUCCESS(LdrFindResource_U(BaseAddress, &LdrResourceInfo, RESOURCE_DATA_LEVEL, &ResourceDataEntry)))
			return false;

		ULONG ResourceLength;
		PVOID ResourceBuffer;
		if (!NT_SUCCESS(LdrAccessResource(BaseAddress, ResourceDataEntry, &ResourceBuffer, &ResourceLength)))
			return false;

		typedef struct _VS_VERSION_INFO_STRUCT16
		{
			USHORT Length;
			USHORT ValueLength;
			CHAR Key[1];
		} VS_VERSION_INFO_STRUCT16, * PVS_VERSION_INFO_STRUCT16;
		// Invalid version info
		if (((VS_VERSION_INFO_STRUCT16 *)ResourceBuffer)->Key[0] >= 32)
			return false;


		LANGANDCODEPAGE * LangandCodePage;
		UINT QueryLength;

		if (!VerQueryValueW(ResourceBuffer, VarFileInfo_Translation.Data, (LPVOID *)&LangandCodePage, &QueryLength))
			return false;
		AssertExpr(QueryLength % sizeof(LANGANDCODEPAGE) == 0);

		//uint32 LanguageCodePage = (MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) << 16) + 1252;
		//LanguageCodePage = uint32(LangandCodePage[0].Language) << 16 + LangandCodePage[0].CodePage;

		//LANGID LangId = GetUserDefaultLangID();

		LANGID LocalLangId = GetUserDefaultUILanguage();
		LANGANDCODEPAGE LocalLangandCodePage = LangandCodePage[0];
		for (uintx LangandCodePageIndex = 0; LangandCodePageIndex < QueryLength / sizeof(LANGANDCODEPAGE); ++LangandCodePageIndex)
		{
			if (LangandCodePage[LangandCodePageIndex].Language == LocalLangId)
			{
				LocalLangandCodePage = LangandCodePage[LangandCodePageIndex];
				break;
			}
		}
		for (uintx BlockIndex = 0; BlockIndex < uintx(EFileVersionBlock::Count); ++BlockIndex)
		{
			FWString Block = QueryVersionBlock(ResourceBuffer, EFileVersionBlock(BlockIndex), LocalLangandCodePage);
			Blocks[BlockIndex] = FEncoding::ToUtf8(Block);
		}

		return true;
	}

	bool FFileVersion::LoadFromFile(FStringV FilePath)
	{
		FWString FilePathW = FEncoding::ToWide(FilePath);

		DWORD FileVersionFlags = FILE_VER_GET_LOCALISED;
		DWORD DwHandle;
		DWORD FileInfoSize = GetFileVersionInfoSizeExW(FileVersionFlags, FilePathW.Data, &DwHandle);

		TList<byte> FileVersionInfoBytes(FileInfoSize, EInitializeMode::None);
		if (GetFileVersionInfoExW(FileVersionFlags, FilePathW.Data, DwHandle, FileInfoSize, FileVersionInfoBytes.Data))
		{
			LANGANDCODEPAGE * LangandCodePage;
			UINT QueryLength;

			if (!VerQueryValueW(FileVersionInfoBytes.Data, VarFileInfo_Translation.Data, (LPVOID *)&LangandCodePage, &QueryLength))
				return false;
			AssertExpr(QueryLength % sizeof(LANGANDCODEPAGE) == 0);

			//uint32 LanguageCodePage = (MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) << 16) + 1252;
			//LanguageCodePage = uint32(LangandCodePage[0].Language) << 16 + LangandCodePage[0].CodePage;

			//LANGID LangId = GetUserDefaultLangID();

			LANGID LocalLangId = GetUserDefaultUILanguage();
			LANGANDCODEPAGE LocalLangandCodePage = LangandCodePage[0];
			for (uintx LangandCodePageIndex = 0; LangandCodePageIndex < QueryLength / sizeof(LANGANDCODEPAGE); ++LangandCodePageIndex)
			{
				if (LangandCodePage[LangandCodePageIndex].Language == LocalLangId)
				{
					LocalLangandCodePage = LangandCodePage[LangandCodePageIndex];
					break;
				}
			}

			for (uintx BlockIndex = 0; BlockIndex < uintx(EFileVersionBlock::Count); ++BlockIndex)
			{
				FWString Block = QueryVersionBlock(FileVersionInfoBytes.Data, EFileVersionBlock(BlockIndex), LocalLangandCodePage);
				Blocks[BlockIndex] = FEncoding::ToUtf8(Block);
			}
			return true;
		}
		else { }
		return false;
	}

	const FFileVersion * FImageInfoCache::FetchFileVersion(FStringV ImageFilePath)
	{
		if (auto Iter = FileVersions.Find(ImageFilePath); Iter != FileVersions.End())
			return Iter->Second;

		FFileVersionRef FileVersion = MakeRefer<FFileVersion>();
		if (FileVersion->LoadFromFile(ImageFilePath))
		{
			FileVersions.Insert({ ImageFilePath , FileVersion });
			return FileVersion;
		}
		FileVersions.Insert({ ImageFilePath, nullptr });
		return nullptr;
	}
}
