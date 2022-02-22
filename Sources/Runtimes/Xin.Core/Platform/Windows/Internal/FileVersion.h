#pragma once

#include "Internal.Types.h"

namespace Xin::Windows::Internal
{
	enum class EFileVersionBlock
	{
		Comments = 0,
		InternalName,
		ProductName,
		CompanyName,
		LegalCopyright,
		ProductVersion,
		FileDescription,
		LegalTrademarks,
		PrivateBuild,
		FileVersion,
		OriginalFilename,
		SpecialBuild,
		Count
	};

	class INTERNAL_API FFileVersion : public IObject
	{
	public:
		FFileVersion() = default;
		FFileVersion(FStringV FilePath);

		bool NtLoadFromFile(FStringV FilePath);
		bool LoadFromFile(FStringV FilePath);

	public:
		TArray<FString, uintx(EFileVersionBlock::Count)> Blocks;
	};
	using FFileVersionRef = TReferPtr<FFileVersion>;

	class INTERNAL_API FImageInfoCache
	{
	public:
		FImageInfoCache() = default;
		~FImageInfoCache() = default;

	public:
		const FFileVersion * FetchFileVersion(FStringV ImageFilePath);

	private:
		TMap<FString, FFileVersionRef> FileVersions;
	};
}
