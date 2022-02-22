#pragma once

#include "Core.Types.h"

namespace Xin
{
	class CORE_API FIniSection
	{
	public:
		FString Name;
		TMap<FString, FString> Items;

		FStringV operator[](FStringV Key) const
		{
			if (auto Iter = Items.Find(Key); Iter != Items.End())
				return Iter->Second;
			return FStringV::None;
		}

		FString & operator[](FStringV Key)
		{
			if (auto Iter = Items.Find(Key); Iter != Items.End())
				return Iter->Second;
			return Items.Insert(MakePair(Key, FString())).First->Second;
		}

		static const FIniSection Empty;
	};


	class CORE_API FIniDocument
	{
	public:
		FIniDocument() = default;
		~FIniDocument() = default;

		FIniDocument(FStringV FilePath);

		EError LoadFromFile(FStringV FilePath);
		EError SaveFile(FStringV FilePath);

		const FIniSection & operator[](FStringV Key) const
		{
			if (auto Iter = Sections.Find(Key); Iter != Sections.End())
				return Iter->Second;
			return FIniSection::Empty;
		}

		FIniSection & operator[](FStringV Key)
		{
			if (auto Iter = Sections.Find(Key); Iter != Sections.End())
				return Iter->Second;
			auto Result = Sections.Insert(MakePair(Key, FIniSection()));
			return Result.First->Second;
		}


		TMap<FString, FIniSection> Sections;
	};
}
