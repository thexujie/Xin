#include "PCH.h"
#include "Regex.h"
#include <regex>

namespace Xin
{
	bool FRegex::Match(FStringV String)
	{
		std::regex_constants::syntax_option_type StdFlags = {};
		SetFlags(StdFlags, std::regex_constants::icase, HasFlags(RegexFlags, ERegexFlags::IgnoreCase));
		std::basic_regex<char, std::regex_traits<char>> StdRegex((const char *)Expression.Data, Expression.Length, StdFlags);
		return std::regex_match((const char *)String.Begin(), (const char *)String.End(), StdRegex);
	}
	
	bool FRegex::Search(FStringV String)
	{
		Result.Clear();
		std::regex_constants::syntax_option_type StdFlags = {};
		SetFlags(StdFlags, std::regex_constants::icase, HasFlags(RegexFlags, ERegexFlags::IgnoreCase));
		std::basic_regex<char, std::regex_traits<char>> StdRegex((const char *)Expression.Data, Expression.Length, StdFlags);
		std::match_results<const char *> StdResults;
		bool StdIsMatched = std::regex_search((const char *)String.Begin(), (const char *)String.End(), StdResults, StdRegex);
		if (!StdIsMatched)
			return false;

		for (auto & StdResult : StdResults)
		{
			if (StdResult.matched)
				Result.Add(FStringV((const char8 *)StdResult.first, StdResult.second - StdResult.first));
			else
				Result.Add(FStringV());
		}
		return true;
	}
}
