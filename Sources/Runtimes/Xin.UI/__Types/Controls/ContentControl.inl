// 143D8BE2DDB3737C75BE2275D85BC3E1 2025-03-24 18:06:11
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ContentControl.h"

namespace Xin
{
	class UContentControl_Type : public TClass<UI::UContentControl>
	{
	public:
		using UContentControl = UI::UContentControl;

		// --- 0 Constructors
		static inline const TConstructor<UContentControl> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UContentControl, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UContentControl, FType &()> Method { u8"Type"N, &UContentControl::Type, FMetadata::None };
		};
		struct Method___TView__char8_____Describe____const
		{
			static inline const TMethod<UContentControl, FStringV()> Method { u8"Describe"N, &UContentControl::Describe, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UContentControl, void()> Method { u8"OnInitialize"N, &UContentControl::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnInitialized
		{
			static inline const TMethod<UContentControl, void()> Method { u8"OnInitialized"N, &UContentControl::OnInitialized, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UContentControl, void()> Method { u8"OnFinalize"N, &UContentControl::OnFinalize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UContentControl, FSize(FSize)> Method { u8"OnMeasure"N, &UContentControl::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnShown
		{
			static inline const TMethod<UContentControl, void()> Method { u8"OnShown"N, &UContentControl::OnShown, FMetadata::None };
		};
		struct Method___void___OnChildMeasured__UElement_ptr
		{
			static inline const TMethod<UContentControl, void(UI::UElement *)> Method { u8"OnChildMeasured"N, &UContentControl::OnChildMeasured, FMetadata::None };
		};
		struct Method___void___OnTemplated
		{
			static inline const TMethod<UContentControl, void()> Method { u8"OnTemplated"N, &UContentControl::OnTemplated, FMetadata::None };
		};
		struct Method___void___OnContentChanged__FVariant
		{
			static inline const TMethod<UContentControl, void(FVariant)> Method { u8"OnContentChanged"N, &UContentControl::OnContentChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TView__char8_____Describe____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnInitialized::Method,
			Method___void___OnFinalize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnShown::Method,
			Method___void___OnChildMeasured__UElement_ptr::Method,
			Method___void___OnTemplated::Method,
			Method___void___OnContentChanged__FVariant::Method,
		};

		// --- 2 Fields
		struct Field___ContentPresented
		{
			static inline const TField Field { u8"ContentPresented"N, &UContentControl::ContentPresented, FMetadata::None };
		};
		struct Field___ContentPresenter
		{
			static inline const TField Field { u8"ContentPresenter"N, &UContentControl::ContentPresenter, FMetadata::None };
		};
		struct Field___ContentElement
		{
			static inline const TField Field { u8"ContentElement"N, &UContentControl::ContentElement, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ContentPresented::Field,
			Field___ContentPresenter::Field,
			Field___ContentElement::Field,
		};

		// --- 3 Properties
		struct Property___Content
		{
			static inline const TDependencyProperty Property { u8"Content"N, &UContentControl::__Content, &UContentControl::__GetContent, &UContentControl::__SetContentValue, UContentControl::__Content__, UContentControl::FPropertyMetadata::None };
		};
		struct Property___ContentTemplate
		{
			static inline const TProxyProperty Property { u8"ContentTemplate"N, &UContentControl::__ContentTemplate, &UContentControl::__GetContentTemplate, &UContentControl::__SetContentTemplate, UContentControl::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Content::Property,
			Property___ContentTemplate::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UControl>(),
		};

		// --- 5 Constructor
		UContentControl_Type(FName Name = u8"UContentControl"N) : TClass(Name, Classof<UI::UControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FVariant> & UI::UContentControl::ContentProperty() { return UContentControl_Type::Property___Content::Property; }

	const TProperty<UI::IDataTemplateRef> & UI::UContentControl::ContentTemplateProperty() { return UContentControl_Type::Property___ContentTemplate::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UContentControl>
	{
		static inline UContentControl_Type UContentControlType { u8"UContentControl"N };

		static FType & Type()
		{
			return UContentControlType;
		}
	};

	FType & UI::UContentControl::StaticType()
	{
		return TStaticType<UI::UContentControl>().Type();
	}

	static TTypeRegister<UI::UContentControl> UContentControlTypeRegister { TStaticType<UI::UContentControl>().Type() };
}

// Generated code end.
