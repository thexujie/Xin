#pragma once
#include "PCH.h"

namespace Xin
{
	class FProgramOptionsParser
	{
	public:
		FProgramOptionsParser();
		FProgramOptionsParser(FStringV CommandLine);

		bool Parse();

		FStringV FindOption(FName Name, FStringV DefaultValue = FStringV::None);
		bool FindBoolOption(FName Name, bool DefaultValue = false);

	public:
		static constexpr FName AppName = u8"'AppName'"N;
		static constexpr FName AppTarget = u8"'AppTarget'"N;

		FString CommandLine;
		struct FProgramOption
		{
			FName Name;
			FStringV Value;
		};
		TList<FProgramOption> Options;
	};
}
