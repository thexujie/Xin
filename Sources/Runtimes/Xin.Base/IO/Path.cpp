#include "PCH.h"
#include "Path.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsHeader.h"
#include <commdlg.h>
#endif

namespace Xin
{
	char8 FPath::DirectorySeparatorChar = u8'/';

	template<typename CharT>
	static bool IsSlash(const CharT & Ch)
	{
		return Ch == '/' || Ch == '\\';
	}

	template<typename CharT>
	static bool IsDrivePrefix(const CharT & Drive)
	{
		return IsBetween<CharT>(ToUpper(Drive), 'A', 'Z');
	}

	template<typename CharT>
	static intx PathBackward(const CharT * Path, const CharT * Begin)
	{
		const CharT * Curr = Path - 1;
		while(Curr >= Begin)
		{
			if (IsSlash(*Curr))
				return Curr - Path;
			--Curr;
		}
		return Begin - Path;
	}

	template<typename CharT>
	static intx PathForward(const CharT * Path, const CharT * Begin, const CharT * End)
	{
		while (Path != End)
		{
			if (IsSlash(*Path))
				return Path - Begin;
			++Path;
		}
		return NullIndex;
	}
	
	FStringV FPath::LexicallyNormal(FStringV Path)
	{
		FString & Buffer = GetThreadLocalStringBuffer<char8>();
		if (Buffer.Capacity <= Path.Size)
			Buffer.Resize(Path.Size, Path.Size + 1);

		const char8 * ReadBegin = Path.Begin();
		const char8 * Read = Path.Begin();
		const char8 * End = Path.End() + 1;
		char8 * Write = Buffer.Data;

		uintx DotIndex = 0;
		for (; Read != End; ++Read)
		{
			if (Read == End - 1 || IsSlash(Read[0]))
			{
				if (DotIndex == 1 && Read - ReadBegin == 1)
				{
					ReadBegin = Read + 1;
					DotIndex = 0;
					continue;
				}
				
				if (DotIndex == 2 && Read - ReadBegin == 2)
				{
					if (Write != Buffer.Data)
					{
						intx BackwardIndex = PathBackward(Write, Buffer.Data);
						if (BackwardIndex == -2 && Write[-1] == '.' && Write[-2] == '.')
						{
							
						}
						else
						{
							Write = Write + BackwardIndex;
#ifdef _DEBUG
							* Write = 0;
#endif
							ReadBegin = Read + 1;

							DotIndex = 0;
							continue;
						}
					}
				}
				else
				{
				}

				if (Write != Buffer.Data)
					*Write++ = DirectorySeparatorChar;

				while (ReadBegin < Read)
					*Write++ = *ReadBegin++;
#ifdef _DEBUG
				* Write = 0;
#endif
				++ReadBegin;
			}

			if (Read[0] == '.')
				++DotIndex;
			else
				DotIndex = 0;
		}

		Buffer.SetSize(Write - Buffer.Data);
		return Buffer;
	}
	
	FStringV FPath::Combine(FStringV BasePath, FStringV Path)
	{
		if (BasePath.Empty())
			return Path;

		if (Path.Empty())
			return BasePath;

		char8 BasePathLastChar = BasePath[BasePath.Size - 1];
		bool NeedSeparator = BasePathLastChar != u8'\\' && BasePathLastChar != u8'/';

		char8 PathFirstChar = Path[0];

		if (PathFirstChar == u8'\\' || PathFirstChar == u8'/')
			Path = Path.SubView(1);

		uintx Length = BasePath.Size + (NeedSeparator ? 1 : 0) + Path.Size;
		
		FString & ResultPath = GetThreadLocalStringBuffer<char8>();
		ResultPath.Resize(Length);

		ArrayCopy(ResultPath.Data, BasePath.Data, BasePath.Size);
		if (NeedSeparator)
		{
			ArrayCopy(ResultPath.Data + BasePath.Size, &DirectorySeparatorChar, 1);
			ArrayCopy(ResultPath.Data + BasePath.Size + 1, Path.Data, Path.Size);
		}
		else
			ArrayCopy(ResultPath.Data + BasePath.Size, Path.Data, Path.Size);
		return LexicallyNormal(ResultPath);
	}

	FStringV FPath::GetAbsolutePath(FStringV Path)
	{
		if (IsAbsolute(Path))
			return Path;
		return LexicallyNormal(Combine(GetWorkingDirectory(), Path));
	}
	
	FStringV FPath::GetAbsolutePath(FStringV BasePath, FStringV Path)
	{
		if (BasePath.Empty())
			return GetAbsolutePath(Path);

		return GetAbsolutePath(Combine(BasePath, Path));
	}

	FStringV FPath::GetRelativePath(FStringV Path, FStringV BasePath)
	{
		std::filesystem::path base(std::u8string_view { BasePath.Data, BasePath.Size });
		std::filesystem::path path(std::u8string_view { Path.Data, Path.Size });

		//std::u8string result = std::filesystem::relative(path, base).u8string();
		std::u8string result = path.lexically_relative(base).u8string();
		std::ranges::replace(result, u8'\\', u8'/');
		FString & ResultPath = GetThreadLocalStringBuffer<char8>();
		ResultPath = result;
		return ResultPath;
	}

	FStringV FPath::GetWorkingDirectory()
	{
		// If the buffer that is pointed to by lpBuffer is not large enough,
		// the return value specifies the required size of the buffer, in characters, including the null-terminating character.
		TString<charw> & Buffer = GetThreadLocalStringBuffer<wchar_t>();
		DWORD SizeIfSuccessOrCapacity = ::GetCurrentDirectoryW(DWORD(Buffer.Capacity), Buffer.GetData());
		if (!SizeIfSuccessOrCapacity)
			return FStringV::None;

		if (SizeIfSuccessOrCapacity > Buffer.Capacity)
		{
			Buffer.Reserve(SizeIfSuccessOrCapacity);
			SizeIfSuccessOrCapacity = ::GetCurrentDirectoryW(DWORD(Buffer.Capacity), Buffer.GetData());
			AssertExpr(SizeIfSuccessOrCapacity < Buffer.Capacity);
			Buffer.SetSize(SizeIfSuccessOrCapacity);
		}
		else
		{
			AssertExpr(SizeIfSuccessOrCapacity < Buffer.Capacity);
			Buffer.SetSize(SizeIfSuccessOrCapacity);
		}
		return FEncoding::ToUtf8(Buffer);
	}

	bool FPath::IsAbsolute(FStringV Path)
	{
		return Path.Size >= 2 && IsDrivePrefix(Path[0]) && Path[1] == ':';
	}
	
	bool FPath::Exists(FStringV Path)
	{
		DWORD FileAttribute = GetFileAttributesW(FEncoding::ToWide(Path).Data);
		return FileAttribute != INVALID_FILE_ATTRIBUTES;
	}

	bool FPath::IsFile(FStringV Path)
	{
		DWORD FileAttribute = GetFileAttributesW(FEncoding::ToWide(Path).Data);
		return (FileAttribute != INVALID_FILE_ATTRIBUTES) && !!(FileAttribute & FILE_ATTRIBUTE_ARCHIVE);
	}

	bool FPath::IsDirectory(FStringV Path)
	{
		DWORD FileAttribute = GetFileAttributesW(FEncoding::ToWide(Path).Data);
		return (FileAttribute != INVALID_FILE_ATTRIBUTES) && !!(FileAttribute & FILE_ATTRIBUTE_DIRECTORY);
	}

	template<typename CharT>
	static const CharT * FindRootEnd(const CharT * Path, const CharT * PathEnd)
	{
		if (PathEnd - Path < 2)
			return Path;

		if (IsDrivePrefix(Path[0]) && Path[1] == ':')
			return Path + 2;

		return Path;
	}
	
	FStringV FPath::GetDirectory(FStringV Path)
	{
		const char8 * PathEnd = Path.End();
		const char8 * PathBegin = FindRootEnd(Path.Begin(), PathEnd);
		while(PathEnd != PathBegin)
		{
			if (IsSlash(PathEnd[-1]))
				return FStringV(Path.Begin(), PathEnd - Path.Begin() - 1);
			--PathEnd;
		}
		return FStringV::None;
	}

	FStringV FPath::GetBaseName(FStringV Path)
	{
		while (Path.EndsWith(u8'/') || Path.EndsWith(u8'\\'))
			Path = Path.SubView(0, Path.Size - 1);
		return FEncoding::ToUtf8(std::filesystem::path(FEncoding::ToStdWString(Path)).stem().wstring());
	}

	FStringV FPath::GetFileName(FStringV Path)
	{
		while (Path.EndsWith(u8'/') || Path.EndsWith(u8'\\'))
			Path = Path.SubView(0, Path.Size - 1);
		return FEncoding::ToUtf8(std::filesystem::path(FEncoding::ToStdWString(Path)).filename().wstring());
	}
	
	FStringV FPath::GetExtention(FStringV Path)
	{
		while (Path.EndsWith(u8'/') || Path.EndsWith(u8'\\'))
			Path = Path.SubView(0, Path.Size - 1);

		return FEncoding::ToUtf8(std::filesystem::path(FEncoding::ToStdWString(Path)).extension().wstring());
	}

	FStringV FPath::ChangeExtention(FStringV Path, FStringV Extention)
	{
		while (Path.EndsWith(u8'/') || Path.EndsWith(u8'\\'))
			Path = Path.SubView(0, Path.Size - 1);

		FStringV Directory = GetDirectory(Path);
		if (Directory.Size)
			return Combine(GetDirectory(Path), GetBaseName(Path) + Extention);
		else
			return GetBaseName(Path) + Extention;
	}

	bool FPath::Move(FStringV SourcePath, FStringV DestinationPath)
	{
		FWStringV SourcePathW = FEncoding::ToWide(SourcePath);
		FWStringV DestinationPathW = FEncoding::ToWide(DestinationPath);
		return !!::MoveFileW(SourcePathW, DestinationPathW);
	}
	
	uint64 FPath::GetFileSize(FStringV Path)
	{
		FWStringV PathW = FEncoding::ToWide(Path);

		WIN32_FILE_ATTRIBUTE_DATA FileAttributeData ={};
		GetFileAttributesExW(PathW, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &FileAttributeData);
		return ((uint64)FileAttributeData.nFileSizeHigh << 32) | ((uint64)FileAttributeData.nFileSizeLow);
		
		//HANDLE hFile = ::CreateFileW(PathW.Data,
		//	0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		//	nullptr, OPEN_EXISTING, 0, NULL);

		//if (hFile == INVALID_HANDLE_VALUE)
		//	return 0;

		//LARGE_INTEGER FileSize = {};
		//::GetFileSizeEx(hFile, &FileSize);
		//::CloseHandle(hFile);
		//return (uint64)FileSize.QuadPart;
	}

	FStringV FPath::OpenFileDialog(FStringV Title, FStringV FileFilter)
	{
		FWString FilePath { 2048 };
		FWString FileFilterW = FEncoding::ToWide(FileFilter);
		FWString TitleW = FEncoding::ToWide(Title);
		OPENFILENAMEW OpenFileName { sizeof(OPENFILENAMEW) };
		OpenFileName.lpstrFilter = FileFilterW.Data;
		OpenFileName.lpstrFile = FilePath.Data;
		OpenFileName.nMaxFile = DWORD(FilePath.Length);
		OpenFileName.lpstrTitle = TitleW.Data;
		OpenFileName.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameW(&OpenFileName))
			return FEncoding::ToUtf8(FWStringV { FilePath });
		return {};
	}
}