#include "PCH.h"
#include "Ini.h"

namespace Xin
{
	const FIniSection FIniSection::Empty;

	FIniDocument::FIniDocument(FStringV FilePath)
	{
		LoadFromFile(FilePath);
	}

	EError FIniDocument::LoadFromFile(FStringV FilePath)
	{
		FFileStream FileStream(FilePath, EFileModes::Input);
		if (!FileStream.Valid())
			return EError::IO;

		FString Line;
		TMap<FString, FIniSection>::Iterator SectionIter = Sections.End();
		while (FileStream.ReadLine(Line))
		{
			if (Line.StartsWith(u8"["V) && Line.EndsWith(u8"]"V))
			{
				FStringV SectionName = Line.View(1, Line.Length - 2).Trim();
				if (Sections.Contains(SectionName))
				{
					LogWarning(u8"Duplicated section name {}"V, SectionName);
					break;
				}
				SectionIter = Sections.Insert(MakePair(SectionName, FIniSection())).First;
			}
			else if (uintx Index = Line.FindFirst(u8'='); Index != NullIndex)
			{
				if (SectionIter == Sections.End())
					continue;

				FStringV Key = Line.View(0, Index).Trim();
				FStringV Value = Line.View(Index + 1).Trim();
				if (Key.Size)
				{
					SectionIter->Second[Key] = Value;
				}
			}
			else
				continue;
		}
		return EError::OK;
	}

	EError FIniDocument::SaveFile(FStringV FilePath)
	{
		return EError::OK;
	}
}
