// E13132D3556002DE9099F8813FAFB6AA 2025-03-25 15:49:28
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/PresenterControl.h"

namespace Xin
{
	class UPresenterControl_Type : public TClass<UI::UPresenterControl>
	{
	public:
		using UPresenterControl = UI::UPresenterControl;

		// --- 0 Constructors
		static inline const TConstructor<UPresenterControl> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UPresenterControl, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UPresenterControl, FType &()> Method { u8"Type"N, &UPresenterControl::Type, FMetadata::None };
		};
		struct Method___void___OnChildMeasured__UElement_ptr
		{
			static inline const TMethod<UPresenterControl, void(UI::UElement *)> Method { u8"OnChildMeasured"N, &UPresenterControl::OnChildMeasured, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UPresenterControl, void()> Method { u8"OnInitialize"N, &UPresenterControl::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UPresenterControl, void()> Method { u8"OnFinalize"N, &UPresenterControl::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnInitialized
		{
			static inline const TMethod<UPresenterControl, void()> Method { u8"OnInitialized"N, &UPresenterControl::OnInitialized, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UPresenterControl, FSize(FSize)> Method { u8"OnMeasure"N, &UPresenterControl::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UPresenterControl, void()> Method { u8"OnArrange"N, &UPresenterControl::OnArrange, FMetadata::None };
		};
		struct Method___void___OnKeyDown__FKeyboardEventArgs_lref
		{
			static inline const TMethod<UPresenterControl, void(UI::FKeyboardEventArgs &)> Method { u8"OnKeyDown"N, &UPresenterControl::OnKeyDown, FMetadata::None };
		};
		struct Method___void___OnKeyUp__FKeyboardEventArgs_lref
		{
			static inline const TMethod<UPresenterControl, void(UI::FKeyboardEventArgs &)> Method { u8"OnKeyUp"N, &UPresenterControl::OnKeyUp, FMetadata::None };
		};
		struct Method___void___OnChildChanged__TReferPtr__UElement__
		{
			static inline const TMethod<UPresenterControl, void(UI::UElementRef)> Method { u8"OnChildChanged"N, &UPresenterControl::OnChildChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnChildMeasured__UElement_ptr::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnInitialized::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnKeyDown__FKeyboardEventArgs_lref::Method,
			Method___void___OnKeyUp__FKeyboardEventArgs_lref::Method,
			Method___void___OnChildChanged__TReferPtr__UElement__::Method,
		};

		// --- 2 Fields
		struct Field___ContentPresenter
		{
			static inline const TField Field { u8"ContentPresenter"N, &UPresenterControl::ContentPresenter, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ContentPresenter::Field,
		};

		// --- 3 Properties
		struct Property___Child
		{
			static inline const TProxyProperty Property { u8"Child"N, &UPresenterControl::__Child, &UPresenterControl::__GetChild, &UPresenterControl::__SetChild, UPresenterControl::FPropertyMetadata::None };
		};
		struct Property___Padding
		{
			struct FPropertyMetadata : UPresenterControl::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Padding"N, &UPresenterControl::__Padding, &UPresenterControl::__GetPadding, &UPresenterControl::__SetPaddingValue, UPresenterControl::__Padding__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Child::Property,
			Property___Padding::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UControl>(),
		};

		// --- 5 Constructor
		UPresenterControl_Type(FName Name = u8"UPresenterControl"N) : TClass(Name, Classof<UI::UControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::UElementRef> & UI::UPresenterControl::ChildProperty() { return UPresenterControl_Type::Property___Child::Property; }

	const TProperty<FThickness> & UI::UPresenterControl::PaddingProperty() { return UPresenterControl_Type::Property___Padding::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UPresenterControl>
	{
		static inline UPresenterControl_Type UPresenterControlType { u8"UPresenterControl"N };

		static FType & Type()
		{
			return UPresenterControlType;
		}
	};

	FType & UI::UPresenterControl::StaticType()
	{
		return TStaticType<UI::UPresenterControl>().Type();
	}

	static TTypeRegister<UI::UPresenterControl> UPresenterControlTypeRegister { TStaticType<UI::UPresenterControl>().Type() };
}

// Generated code end.
