#include "PCH.h"
#include "Property.h"

#include "Type.h"
#include "Class.h"

namespace Xin
{
	IPropertyValue IPropertyValue::None;
	FProperty FProperty::None;

	FProperty::FProperty() : Instance(FType::None), Type(FType::None)
	{
		
	}

	FProperty::FProperty(const FType & Instance, const FType & Type) : Instance(Instance), Type(Type)
	{
		//AssertExpresion(Instance.Valid());
	}

	FProperty::FProperty(const FType & Instance, const FType & Type, FName Name, const FMetadata & Metadata)
		: FMeta(Name, Metadata), Instance(Instance), Type(Type)
	{
		//AssertExpresion(Instance.Valid());
	}

	const FField & FProperty::FindField(FName FieldName) const
	{
		const FClass & Class = StaticCast<FClass>(Type);
		const FField & Field = Class.FindField(FieldName);
		return Field;
	}
}
