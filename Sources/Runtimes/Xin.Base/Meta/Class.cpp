#include "PCH.h"
#include "Class.h"

#define weak_symbol __declspec(selectany)

namespace Xin
{
	FClass FClass::None;


	const FConstructor & FClass::FindConstructor(TView<const FType *> ParameterTypes) const
	{
		for (const FConstructor & Constructor : Constructors)
		{
			if (Constructor.IsInvocable(ParameterTypes))
			return Constructor;
		}
		return FConstructor::None;
	}

	const FMethod & FClass::FindMethod(FName Name) const
	{
		for (const FMethod & Method : Methods)
		{
			if (Method.Name == Name)
				return Method;
		}
		return FMethod::None;
	}

	const FField & FClass::FindField(FName Name) const
	{
		for (const FField & Field : Fields)
		{
			if (Field.Name == Name)
				return Field;
		}
		return FField::None;
	}

	const FProperty & FClass::FindProperty(FName Name) const
	{
		for (const FProperty & Property : Properties)
		{
			if (Property.Name == Name)
				return Property;
		}
		return FProperty::None;
	}
}
