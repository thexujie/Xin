#pragma once

#include "Basic.Types.h"

namespace Xin 
{
	struct BASE_API FLocale
	{
	public:
		FLocale() = default;
		FLocale(const std::locale & Locale) : Locale(Locale) {}

		operator std::locale & () { return Locale; }
		operator const std::locale & () const { return Locale; }

	public:
		std::locale Locale;

	public:
		static FLocale Current();
	};
}
