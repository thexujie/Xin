// 0BDAB61D14B04B9FB020EEEE8B88DDD6 2025-03-06 15:48:57
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Separator.h"

namespace Xin
{
	class USeparator_Type : public TClass<UI::USeparator>
	{
	public:
		using USeparator = UI::USeparator;

		// --- 0 Constructors
		static inline const TConstructor<USeparator> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<USeparator, FType &()> Method { u8"Type"N, &USeparator::Type, FMetadata::None };
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
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		USeparator_Type(FName Name = u8"USeparator"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::USeparator>
	{
		static inline USeparator_Type USeparatorType { u8"USeparator"N };

		static FType & Type()
		{
			return USeparatorType;
		}
	};

	FType & UI::USeparator::StaticType()
	{
		return TStaticType<UI::USeparator>().Type();
	}

	static TTypeRegister<UI::USeparator> USeparatorTypeRegister { TStaticType<UI::USeparator>().Type() };
}

// Generated code end.
