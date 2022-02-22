// 8B3BBA302A04F6F90E15E954186DFF20 2025-03-24 18:07:13
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/ControlTemplate.h"

namespace Xin
{
	class IControlTemplate_Type : public TClass<UI::IControlTemplate>
	{
	public:
		using IControlTemplate = UI::IControlTemplate;

		// --- 0 Constructors

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IControlTemplate, FType &()> Method { u8"Type"N, &IControlTemplate::Type, FMetadata::None };
		};
		struct Method___TReferPtr__UElement_____LoadTemplate__UControl_lref
		{
			static inline const TMethod<IControlTemplate, UI::UElementRef(UI::UControl &)> Method { u8"LoadTemplate"N, &IControlTemplate::LoadTemplate, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TReferPtr__UElement_____LoadTemplate__UControl_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::IResource>(),
		};

		// --- 5 Constructor
		IControlTemplate_Type(FName Name = u8"IControlTemplate"N) : TClass(Name, Classof<UI::IResource>())
		{
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::IControlTemplate>
	{
		static inline IControlTemplate_Type IControlTemplateType { u8"IControlTemplate"N };

		static FType & Type()
		{
			return IControlTemplateType;
		}
	};

	FType & UI::IControlTemplate::StaticType()
	{
		return TStaticType<UI::IControlTemplate>().Type();
	}

	static TTypeRegister<UI::IControlTemplate> IControlTemplateTypeRegister { TStaticType<UI::IControlTemplate>().Type() };
}

// Generated code end.
