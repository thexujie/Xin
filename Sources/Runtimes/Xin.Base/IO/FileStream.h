#pragma once

#include "File.h"

namespace Xin
{
	class BASE_API FFileStream : public IDataStream
	{
	public:
		FFileStream() = default;
		~FFileStream();
		FFileStream(const FFileStream &) = delete;
		FFileStream(FFileStream && That) noexcept : Handle(That.Handle) { That.Handle = nullptr; }
		FFileStream(FStringV FilePath, EFileModes FileModes);

	public:
		uintx Read(byte * Data, uintx Size) override;
		uintx Write(const byte * Data, uintx Size) override;
		uintx Peek(byte * Data, uintx Size) override;

		uintx Seek(EStreamSeek StreamSeek, intx Distance) override;
		uintx Position() const override;
		uintx Size() const override;

	public:
		bool Valid() const { return !!Handle; }
		explicit operator bool() { return Valid(); }

		EError Open(FStringV FilePath, EFileModes FileModes);
		void Close();
		void Flush();

	private:
		voidp Handle = nullptr;
	};
}
