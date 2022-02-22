// 5767F31C11A5D03733B104F6B62A1431 2025-03-06 15:45:41
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/Freezable.h"

namespace Xin
{
	class IFreezable_Type : public TClass<IFreezable>
	{
	public:
		// --- 0 Constructors

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IFreezable, FType &()> Method { u8"Type"N, &IFreezable::Type, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IDependency>(),
		};

		// --- 5 Constructor
		IFreezable_Type(FName Name = u8"IFreezable"N) : TClass(Name, Classof<IDependency>())
		{
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<IFreezable>
	{
		static inline IFreezable_Type IFreezableType { u8"IFreezable"N };

		static FType & Type()
		{
			return IFreezableType;
		}
	};

	FType & IFreezable::StaticType()
	{
		return TStaticType<IFreezable>().Type();
	}

	static TTypeRegister<IFreezable> IFreezableTypeRegister { TStaticType<IFreezable>().Type() };
}

// Generated code end.
