// 0D295C9D75615444D5374A8AEF48ABBE 2025-03-24 18:06:42
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/SplitterPanel.h"

namespace Xin
{
	class USplitterPanel_Type : public TClass<UI::USplitterPanel>
	{
	public:
		using USplitterPanel = UI::USplitterPanel;

		// --- 0 Constructors
		static inline const TConstructor<USplitterPanel> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<USplitterPanel, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<USplitterPanel, FType &()> Method { u8"Type"N, &USplitterPanel::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<USplitterPanel, void()> Method { u8"OnConstruct"N, &USplitterPanel::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<USplitterPanel, void()> Method { u8"OnInitialize"N, &USplitterPanel::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<USplitterPanel, FSize(FSize)> Method { u8"OnMeasure"N, &USplitterPanel::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<USplitterPanel, void()> Method { u8"OnArrange"N, &USplitterPanel::OnArrange, FMetadata::None };
		};
		struct Method___UElement_ptr___HitElement__TVec2__float64______const
		{
			static inline const TMethod<USplitterPanel, UI::UElement *(FPoint)> Method { u8"HitElement"N, &USplitterPanel::HitElement, FMetadata::None };
		};
		struct Method___ECursor___QueryCursor__TVec2__float64______const
		{
			static inline const TMethod<USplitterPanel, ECursor(FPoint)> Method { u8"QueryCursor"N, &USplitterPanel::QueryCursor, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<USplitterPanel, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &USplitterPanel::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<USplitterPanel, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &USplitterPanel::OnMouseUp, FMetadata::None };
		};
		struct Method___void___OnMouseMove__FMouseEventArgs_lref
		{
			static inline const TMethod<USplitterPanel, void(UI::FMouseEventArgs &)> Method { u8"OnMouseMove"N, &USplitterPanel::OnMouseMove, FMetadata::None };
		};
		struct Method___void___OnMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<USplitterPanel, void(UI::FMouseEventArgs &)> Method { u8"OnMouseLeave"N, &USplitterPanel::OnMouseLeave, FMetadata::None };
		};
		struct Method___void___OnIndicatingIndexChanged__uintx
		{
			static inline const TMethod<USplitterPanel, void(uintx)> Method { u8"OnIndicatingIndexChanged"N, &USplitterPanel::OnIndicatingIndexChanged, FMetadata::None };
		};
		struct Method___void___OnResigingIndexChanged__uintx
		{
			static inline const TMethod<USplitterPanel, void(uintx)> Method { u8"OnResigingIndexChanged"N, &USplitterPanel::OnResigingIndexChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___UElement_ptr___HitElement__TVec2__float64______const::Method,
			Method___ECursor___QueryCursor__TVec2__float64______const::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseMove__FMouseEventArgs_lref::Method,
			Method___void___OnMouseLeave__FMouseEventArgs_lref::Method,
			Method___void___OnIndicatingIndexChanged__uintx::Method,
			Method___void___OnResigingIndexChanged__uintx::Method,
		};

		// --- 2 Fields
		struct Field___SplitterSize
		{
			static inline const TField Field { u8"SplitterSize"N, &USplitterPanel::SplitterSize, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___SplitterSize::Field,
		};

		// --- 3 Properties
		struct Property___ResizeEnabled
		{
			static inline const TAttachedProperty<USplitterPanel, bool> Property { u8"ResizeEnabled"N, USplitterPanel::__ResizeEnabled__, USplitterPanel::FPropertyMetadata::None };
		};
		struct Property___IndicatingIndex
		{
			static inline const TProxyProperty Property { u8"IndicatingIndex"N, &USplitterPanel::__IndicatingIndex, &USplitterPanel::__GetIndicatingIndex, &USplitterPanel::__SetIndicatingIndex, USplitterPanel::FPropertyMetadata::None };
		};
		struct Property___ResizingIndex
		{
			static inline const TProxyProperty Property { u8"ResizingIndex"N, &USplitterPanel::__ResizingIndex, &USplitterPanel::__GetResizingIndex, &USplitterPanel::__SetResizingIndex, USplitterPanel::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___ResizeEnabled::Property,
			Property___IndicatingIndex::Property,
			Property___ResizingIndex::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UStackPanel>(),
		};

		// --- 5 Constructor
		USplitterPanel_Type(FName Name = u8"USplitterPanel"N) : TClass(Name, Classof<UI::UStackPanel>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<bool> & UI::USplitterPanel::ResizeEnabledProperty() { return USplitterPanel_Type::Property___ResizeEnabled::Property; }

	const TProperty<uintx> & UI::USplitterPanel::IndicatingIndexProperty() { return USplitterPanel_Type::Property___IndicatingIndex::Property; }

	const TProperty<uintx> & UI::USplitterPanel::ResizingIndexProperty() { return USplitterPanel_Type::Property___ResizingIndex::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::USplitterPanel>
	{
		static inline USplitterPanel_Type USplitterPanelType { u8"USplitterPanel"N };

		static FType & Type()
		{
			return USplitterPanelType;
		}
	};

	FType & UI::USplitterPanel::StaticType()
	{
		return TStaticType<UI::USplitterPanel>().Type();
	}

	static TTypeRegister<UI::USplitterPanel> USplitterPanelTypeRegister { TStaticType<UI::USplitterPanel>().Type() };
}

// Generated code end.
