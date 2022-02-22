// 8D4CE13D757C1FEF0733058D8B64D06A 2025-03-06 15:49:36
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/ControlStyle.h"

namespace Xin
{
	class IControlStyle_Type : public TClass<UI::IControlStyle>
	{
	public:
		using IControlStyle = UI::IControlStyle;

		// --- 0 Constructors
		static inline const TConstructor<IControlStyle, FType const &> Constructor___FType_const_lref { u8"!Constructor___FType_const_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___FType_const_lref,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IControlStyle, FType &()> Method { u8"Type"N, &IControlStyle::Type, FMetadata::None };
		};
		struct Method___void___LoadStyle__UObject_lref
		{
			static inline const TMethod<IControlStyle, void(UI::UObject &)> Method { u8"LoadStyle"N, &IControlStyle::LoadStyle, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___LoadStyle__UObject_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::IResource>(),
		};

		// --- 5 Constructor
		IControlStyle_Type(FName Name = u8"IControlStyle"N) : TClass(Name, Classof<UI::IResource>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::IControlStyle>
	{
		static inline IControlStyle_Type IControlStyleType { u8"IControlStyle"N };

		static FType & Type()
		{
			return IControlStyleType;
		}
	};

	FType & UI::IControlStyle::StaticType()
	{
		return TStaticType<UI::IControlStyle>().Type();
	}

	static TTypeRegister<UI::IControlStyle> IControlStyleTypeRegister { TStaticType<UI::IControlStyle>().Type() };
}

// Generated code end.
