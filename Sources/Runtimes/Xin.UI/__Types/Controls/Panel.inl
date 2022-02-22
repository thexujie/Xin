// 4EA17AD961B92E0F9DE0A04AE51F2028 2025-03-25 15:49:25
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Panel.h"

namespace Xin
{
	class UPanel_Type : public TClass<UI::UPanel>
	{
	public:
		using UPanel = UI::UPanel;

		// --- 0 Constructors
		static inline const TConstructor<UPanel> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UPanel, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UPanel, FType &()> Method { u8"Type"N, &UPanel::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UPanel, void()> Method { u8"OnConstruct"N, &UPanel::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UPanel, void()> Method { u8"OnInitialize"N, &UPanel::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UPanel, void()> Method { u8"OnFinalize"N, &UPanel::OnFinalize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UPanel, FSize(FSize)> Method { u8"OnMeasure"N, &UPanel::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UPanel, void()> Method { u8"OnArrange"N, &UPanel::OnArrange, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UPanel, void(IPainter &)> Method { u8"OnPaint"N, &UPanel::OnPaint, FMetadata::None };
		};
		struct Method___void___AddChild__UElement_ptr
		{
			static inline const TMethod<UPanel, void(UI::UElement *)> Method { u8"AddChild"N, &UPanel::AddChild, FMetadata::None };
		};
		struct Method___void___RemoveChild__UElement_ptr
		{
			static inline const TMethod<UPanel, void(UI::UElement *)> Method { u8"RemoveChild"N, &UPanel::RemoveChild, FMetadata::None };
		};
		struct Method___void___RemoveChildren
		{
			static inline const TMethod<UPanel, void()> Method { u8"RemoveChildren"N, &UPanel::RemoveChildren, FMetadata::None };
		};
		struct Method___void___OnSizeChanged__TVec2__FDimen___const_lref
		{
			static inline const TMethod<UPanel, void(UI::FDimen2 const &)> Method { u8"OnSizeChanged"N, &UPanel::OnSizeChanged, FMetadata::None };
		};
		struct Method___void___OnAddItem__TReferPtr__UElement___lref
		{
			static inline const TMethod<UPanel, void(TReferPtr<UI::UElement> &)> Method { u8"OnAddItem"N, &UPanel::OnAddItem, FMetadata::None };
		};
		struct Method___void___OnRemoveItem__TReferPtr__UElement___lref
		{
			static inline const TMethod<UPanel, void(TReferPtr<UI::UElement> &)> Method { u8"OnRemoveItem"N, &UPanel::OnRemoveItem, FMetadata::None };
		};
		struct Method___void___OnSortComparerChanged__TFunction__bool___TReferPtr__UElement___const_lref__TReferPtr__UElement___const_lref_____
		{
			static inline const TMethod<UPanel, void(TFunction<bool(UI::UElementRef const &, UI::UElementRef const &)>)> Method { u8"OnSortComparerChanged"N, &UPanel::OnSortComparerChanged, FMetadata::None };
		};
		struct Method___void___SortElements
		{
			static inline const TMethod<UPanel, void()> Method { u8"SortElements"N, &UPanel::SortElements, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnPaint__IPainter_lref::Method,
			Method___void___AddChild__UElement_ptr::Method,
			Method___void___RemoveChild__UElement_ptr::Method,
			Method___void___RemoveChildren::Method,
			Method___void___OnSizeChanged__TVec2__FDimen___const_lref::Method,
			Method___void___OnAddItem__TReferPtr__UElement___lref::Method,
			Method___void___OnRemoveItem__TReferPtr__UElement___lref::Method,
			Method___void___OnSortComparerChanged__TFunction__bool___TReferPtr__UElement___const_lref__TReferPtr__UElement___const_lref_____::Method,
			Method___void___SortElements::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___SortComparer
		{
			static inline const TProxyProperty Property { u8"SortComparer"N, &UPanel::__SortComparer, &UPanel::__GetSortComparer, &UPanel::__SetSortComparer, UPanel::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___SortComparer::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		UPanel_Type(FName Name = u8"UPanel"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<TFunction<bool(UI::UElementRef const &, UI::UElementRef const &)>> & UI::UPanel::SortComparerProperty() { return UPanel_Type::Property___SortComparer::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UPanel>
	{
		static inline UPanel_Type UPanelType { u8"UPanel"N };

		static FType & Type()
		{
			return UPanelType;
		}
	};

	FType & UI::UPanel::StaticType()
	{
		return TStaticType<UI::UPanel>().Type();
	}

	static TTypeRegister<UI::UPanel> UPanelTypeRegister { TStaticType<UI::UPanel>().Type() };
}

// Generated code end.
