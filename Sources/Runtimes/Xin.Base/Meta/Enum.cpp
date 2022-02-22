#include "PCH.h"
#include "Enum.h"
#include "Type.h"

namespace Xin
{
	FEnum FEnum::None;

	FEnum::FEnum() : UnderlyingType(FType::None) {  }

	FEnum::FEnum(const FType & UnderlyingType, FName Name, FTypeid Typeid, const FMetadata & Metadata) : FType(Name, Typeid, Metadata), UnderlyingType(UnderlyingType) {}
}
