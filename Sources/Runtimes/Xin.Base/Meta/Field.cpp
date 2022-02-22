#include "PCH.h"
#include "Field.h"
#include "Type.h"

namespace Xin
{
	FField FField::None;

	FField::FField() : Type(FType::None) {  }

	FField::FField(const FType & Type) : Type(Type) {}

	FField::FField(const FType & Type, FName Name, const FMetadata & Metadata) : FMeta(Name, Metadata), Type(Type) {}
}
