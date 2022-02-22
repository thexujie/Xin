#pragma once

#include "File.h"

namespace Xin
{
	class BASE_API FFileTransaction
	{
	public:
		FFileTransaction();
		FFileTransaction(const FFileTransaction &) = delete;
		~FFileTransaction();

	public:
		bool CreateFile(FStringV FilePath, EFileCreate FileCreate = EFileCreate::None);
		bool MoveFile(FStringV SourcePath, FStringV DestinationPath);
		bool DeleteFile(FStringV FilePath);
		bool MoveDirectory(FStringV SourcePath, FStringV DestinationPath);
		bool DeleteDirectory(FStringV FilePath);

	public:
		bool Commit();
		void Close();

	private:
		voidp Handle = nullptr;
	};
}
