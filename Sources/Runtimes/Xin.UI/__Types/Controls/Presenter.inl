// 390B5E62F1D55AEC667ECB689595D8D8 2025-03-25 15:49:26
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Presenter.h"

namespace Xin
{
	class UPresenter_Type : public TClass<UI::UPresenter>
	{
	public:
		using UPresenter = UI::UPresenter;

		// --- 0 Constructors
		static inline const TConstructor<UPresenter> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UPresenter, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UPresenter, FType &()> Method { u8"Type"N, &UPresenter::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UPresenter, void()> Method { u8"OnInitialize"N, &UPresenter::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UPresenter, void()> Method { u8"OnFinalize"N, &UPresenter::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnChildMeasured__UElement_ptr
		{
			static inline const TMethod<UPresenter, void(UI::UElement *)> Method { u8"OnChildMeasured"N, &UPresenter::OnChildMeasured, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UPresenter, FSize(FSize)> Method { u8"OnMeasure"N, &UPresenter::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UPresenter, void()> Method { u8"OnArrange"N, &UPresenter::OnArrange, FMetadata::None };
		};
		struct Method___void___OnKeyDown__FKeyboardEventArgs_lref
		{
			static inline const TMethod<UPresenter, void(UI::FKeyboardEventArgs &)> Method { u8"OnKeyDown"N, &UPresenter::OnKeyDown, FMetadata::None };
		};
		struct Method___void___OnKeyUp__FKeyboardEventArgs_lref
		{
			static inline const TMethod<UPresenter, void(UI::FKeyboardEventArgs &)> Method { u8"OnKeyUp"N, &UPresenter::OnKeyUp, FMetadata::None };
		};
		struct Method___void___OnChildChanged__TReferPtr__UElement__
		{
			static inline const TMethod<UPresenter, void(UI::UElementRef)> Method { u8"OnChildChanged"N, &UPresenter::OnChildChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnChildMeasured__UElement_ptr::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnKeyDown__FKeyboardEventArgs_lref::Method,
			Method___void___OnKeyUp__FKeyboardEventArgs_lref::Method,
			Method___void___OnChildChanged__TReferPtr__UElement__::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Child
		{
			static inline const TProxyProperty Property { u8"Child"N, &UPresenter::__Child, &UPresenter::__GetChild, &UPresenter::__SetChild, UPresenter::FPropertyMetadata::None };
		};
		struct Property___Padding
		{
			struct FPropertyMetadata : UPresenter::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Padding"N, &UPresenter::__Padding, &UPresenter::__GetPadding, &UPresenter::__SetPaddingValue, UPresenter::__Padding__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Child::Property,
			Property___Padding::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		UPresenter_Type(FName Name = u8"UPresenter"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::UElementRef> & UI::UPresenter::ChildProperty() { return UPresenter_Type::Property___Child::Property; }

	const TProperty<FThickness> & UI::UPresenter::PaddingProperty() { return UPresenter_Type::Property___Padding::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UPresenter>
	{
		static inline UPresenter_Type UPresenterType { u8"UPresenter"N };

		static FType & Type()
		{
			return UPresenterType;
		}
	};

	FType & UI::UPresenter::StaticType()
	{
		return TStaticType<UI::UPresenter>().Type();
	}

	static TTypeRegister<UI::UPresenter> UPresenterTypeRegister { TStaticType<UI::UPresenter>().Type() };
}

// Generated code end.
