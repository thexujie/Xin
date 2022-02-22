#include "PCH.h"
#include "Typeid.h"

#include "Strings.h"
#include "Encoding.h"

namespace Xin
{
	FTypeid FTypeid::None;

	FStringV FTypeid::ExtractTypeName(const std::type_info & StdTypeInfo)
	{
		FStringV NameString = FEncoding::ToUtf8(FAStringV(StdTypeInfo.name()));
		return ExtractTypeName(NameString);
	}

	FStringV FTypeid::ExtractTypeName(FStringV NameString)
	{
		if (NameString.StartsWith(u8"class "V))
			NameString = NameString.SubView(6);
		else if (NameString.StartsWith(u8"struct "V))
			NameString = NameString.SubView(7);
		else {}

		if (NameString.Size > 2)
		{
			if (uintx Index = NameString.FindLast(u8"::"V); Index != NullIndex)
				NameString = NameString.SubView(Index + 2);
			if (NameString[0] == u8'U' || NameString[0] == u8'F')
				NameString = NameString.SubView(1);
		}
		return NameString;
	}

	FTypeidEntry::FTypeidEntry(const std::type_info & StdTypeInfo)
		: Identifier(StdTypeInfo.hash_code())
#if XIN_DEBUG
	, DebugType(&StdTypeInfo)
#endif
	{
		
	}
}
