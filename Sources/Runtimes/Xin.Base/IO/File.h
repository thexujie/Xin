#pragma once

#include "IO.Types.h"

namespace Xin
{
	enum class EFileBOM
	{
		None = 0,
		UTF8,
		UTF16BE,
		UTF16LE,
	};

	enum class EFileCreate
	{
		None = 0,
		Override = 0x0001,
	};

	enum class EFileOpen
	{
		None = 0,
		Truncate = 0x0001,
		Append = 0x0002,
	};

	enum class EFileShare
	{
		None = 0,
		ShareRead = 0x0001,
		ShareWrite = 0x0002,
		ShareDelete = 0x0004,

		ShareReadWrite = ShareRead | ShareWrite,
		ShareReadWriteDelete = ShareRead | ShareWrite | ShareDelete,
	};

	enum class EFileModes
	{
		None = 0,
		Input = 0x0001, // Seek(0) and input
		Output = 0x0002, // Seek(0) and output
		Append = 0x0004,  // Seek(-1) and output

		ShareRead = 0x00010000,
		ShareWrite = 0x00020000,
		ShareDelete = 0x00040000,

		ShareReadWrite = ShareRead | ShareWrite,
		ShareReadWriteDelete = ShareRead | ShareWrite | ShareDelete,
		
		InputOutput = Input | Output,
	};
	template class TEnumWrapper<EFileModes>;

	class BASE_API FFile
	{
	public:
		FFile() = default;
		~FFile();
		FFile(const FFile &) = delete;
		FFile(FFile && That) noexcept : Handle(That.Handle) { That.Handle = nullptr; }
		FFile(FStringV FilePath);

		bool Open(FStringV FilePath);
		bool Valid() const { return !!Handle; }
		uintx GetSize() const;
		explicit operator bool() { return !!Handle; }

	private:
		voidp Handle = nullptr;

	public:
		static void SetAttribute(FStringV FilePath, EFileAttribute FileAttribute);

		static uint64 FileSize(FStringV FilePath);

		static uint64 AccessTime(FStringV FilePath);
		static uint64 CreationTime(FStringV FilePath);
		static uint64 ModifiedTime(FStringV FilePath);

		static bool Exists(FStringV FilePath);
		static bool Delete(FStringV FilePath);
		static bool Copy(FStringV SourcePath, FStringV DestinationPath, bool Override = true);
		static bool Move(FStringV SourcePath, FStringV DestinationPath);
		
		static FString ReadString(FStringV FilePath, EFileBOM * FileBOM = nullptr);
		static uintx WriteString(FStringV FilePath, FStringV String);
		static uintx WriteBytes(FStringV FilePath, TView<byte> Bytes);
		static TList<byte> ReadBytes(FStringV FilePath);
	};
}