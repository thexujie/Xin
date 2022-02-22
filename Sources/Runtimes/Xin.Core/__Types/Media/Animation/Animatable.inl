// 484D6B5A1AB3D0A8720A414AFFEF7F1B 2025-03-06 15:45:59
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Media/Animation/Animatable.h"

namespace Xin
{
	class IAnimatable_Type : public TClass<IAnimatable>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<IAnimatable> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IAnimatable, FType &()> Method { u8"Type"N, &IAnimatable::Type, FMetadata::None };
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
			Classof<IFreezable>(),
		};

		// --- 5 Constructor
		IAnimatable_Type(FName Name = u8"IAnimatable"N) : TClass(Name, Classof<IFreezable>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<IAnimatable>
	{
		static inline IAnimatable_Type IAnimatableType { u8"IAnimatable"N };

		static FType & Type()
		{
			return IAnimatableType;
		}
	};

	FType & IAnimatable::StaticType()
	{
		return TStaticType<IAnimatable>().Type();
	}

	static TTypeRegister<IAnimatable> IAnimatableTypeRegister { TStaticType<IAnimatable>().Type() };
}

// Generated code end.
