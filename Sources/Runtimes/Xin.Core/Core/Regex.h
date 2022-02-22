#pragma once

#include "Core.Types.h"

namespace Xin
{
	enum class ERegexFlags
	{
		None = 0,
		IgnoreCase
	};
	template class TEnumWrapper<ERegexFlags>;

	class CORE_API FRegex
	{
	public:
		FRegex(FStringV Expression_, ERegexFlags RegexFlags_ = ERegexFlags::None) : Expression(Expression_), RegexFlags(RegexFlags_) {}

		bool Match(FStringV String);
		bool Search(FStringV String);
		FStringV operator[](uintx Index) const { return Result[Index]; }
		
		FString Expression;
		ERegexFlags RegexFlags = ERegexFlags::None;
		TList<FStringV> Result;
	};
}
