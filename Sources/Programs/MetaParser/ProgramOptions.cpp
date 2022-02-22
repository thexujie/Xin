#include "PCH.h"
#include "ProgramOptions.h"

namespace Xin
{
	FProgramOptionsParser::FProgramOptionsParser() {}

	FProgramOptionsParser::FProgramOptionsParser(FStringV CommandLine) : CommandLine(CommandLine)
	{
		Parse();
	}

	bool FProgramOptionsParser::Parse()
	{
		FStringV Name;

		sizet StringIndex = 0;
		bool BetweenQuat = false;

		auto PushName = [&](uintx EndIndex)
		{
			Name = CommandLine.View(StringIndex, EndIndex - StringIndex);
			StringIndex = EndIndex + 1;
		};

		auto PushValue = [&](uintx EndIndex)
		{
			if (StringIndex > EndIndex)
			{
				StringIndex = EndIndex;
				return;
			}

			FStringV Value = CommandLine.View(StringIndex, EndIndex - StringIndex);
			if (Name.Empty())
			{
				if (Value.Valid())
				{
					if (Options.Empty())
						Options.Add({ AppName, Value });
					else if (Options.Size == 1)
						Options.Add({ AppTarget, Value });
					else
						Options.Add({ Value, FStringV::None });
				}
			}
			else
				Options.Add({ Name, Value });

			Name = FString::None;
			StringIndex = EndIndex + 1;
		};

		bool Escaping = false;
		for (sizet Index = 0; Index < CommandLine.Length; ++Index)
		{
			char8 Ch = CommandLine[Index];
			if (Escaping)
			{
				Escaping = false;
				continue;
			}

			switch (Ch)
			{
			case u8'\\':
				Escaping = true;
				break;
			case u8' ':
			case u8'\0':
				PushValue(Index);
				StringIndex = Index + 1;
				break;
			case u8'\"':
				if (BetweenQuat)
					PushValue(Index);
				else
					StringIndex = Index + 1;
				BetweenQuat = !BetweenQuat;
				break;
			case u8'=':
				PushName(Index);
				StringIndex = Index + 1;
				break;
			case u8'-':
				AssertExpr(Name.Size == 0);
				StringIndex = Index + 1;
				break;
			default:
				break;
			}
		}
		PushValue(CommandLine.Length);
		return true;
	}

	FStringV FProgramOptionsParser::FindOption(FName Name, FStringV DefaultValue)
	{
		for (const FProgramOption & Option : Options)
		{
			if (Option.Name == Name)
				return Option.Value;
		}
		return DefaultValue;
	}

	bool FProgramOptionsParser::FindBoolOption(FName Name, bool DefaultValue)
	{
		for (const FProgramOption & Option : Options)
		{
			if (Option.Name == Name)
			{
				if (Option.Value.Empty())
					return true;
				return StringEqualIC(Option.Value, u8"true"V) || StringEqualIC(Option.Value, u8"1"V);
			}
		}
		return DefaultValue;
	}
}
