// 0CD669A4DD584863028BD63CA526074F 2025-03-24 18:06:36
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ScrollArea.h"

namespace Xin
{
	class UScrollArea_Type : public TClass<UI::UScrollArea>
	{
	public:
		using UScrollArea = UI::UScrollArea;

		// --- 0 Constructors
		static inline const TConstructor<UScrollArea> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UScrollArea, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UScrollArea, FType &()> Method { u8"Type"N, &UScrollArea::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UScrollArea, void()> Method { u8"OnConstruct"N, &UScrollArea::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UScrollArea, void()> Method { u8"OnInitialize"N, &UScrollArea::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UScrollArea, FSize(FSize)> Method { u8"OnMeasure"N, &UScrollArea::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UScrollArea, void()> Method { u8"OnArrange"N, &UScrollArea::OnArrange, FMetadata::None };
		};
		struct Method___void___OnArranged
		{
			static inline const TMethod<UScrollArea, void()> Method { u8"OnArranged"N, &UScrollArea::OnArranged, FMetadata::None };
		};
		struct Method___void___OnMouseWhell__FMouseWhellEventArgs_lref
		{
			static inline const TMethod<UScrollArea, void(UI::FMouseWhellEventArgs &)> Method { u8"OnMouseWhell"N, &UScrollArea::OnMouseWhell, FMetadata::None };
		};
		struct Method___void___OnScrollChanged__FEventArgs_lref
		{
			static inline const TMethod<UScrollArea, void(FEventArgs &)> Method { u8"OnScrollChanged"N, &UScrollArea::OnScrollChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnArranged::Method,
			Method___void___OnMouseWhell__FMouseWhellEventArgs_lref::Method,
			Method___void___OnScrollChanged__FEventArgs_lref::Method,
		};

		// --- 2 Fields
		struct Field___ScrollInfo
		{
			static inline const TField Field { u8"ScrollInfo"N, &UScrollArea::ScrollInfo, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ScrollInfo::Field,
		};

		// --- 3 Properties
		struct Property___ViewportSize
		{
			static inline const TDependencyProperty Property { u8"ViewportSize"N, &UScrollArea::__ViewportSize, &UScrollArea::__GetViewportSize, &UScrollArea::__SetViewportSizeValue, UScrollArea::__ViewportSize__, UScrollArea::FPropertyMetadata::None };
		};
		struct Property___ScrollExtent
		{
			static inline const TDependencyProperty Property { u8"ScrollExtent"N, &UScrollArea::__ScrollExtent, &UScrollArea::__GetScrollExtent, &UScrollArea::__SetScrollExtentValue, UScrollArea::__ScrollExtent__, UScrollArea::FPropertyMetadata::None };
		};
		struct Property___ScrollOffset
		{
			struct FPropertyMetadata : UScrollArea::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"ScrollOffset"N, &UScrollArea::__ScrollOffset, &UScrollArea::__GetScrollOffset, &UScrollArea::__SetScrollOffsetValue, UScrollArea::__ScrollOffset__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___ViewportSize::Property,
			Property___ScrollExtent::Property,
			Property___ScrollOffset::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPresenter>(),
		};

		// --- 5 Constructor
		UScrollArea_Type(FName Name = u8"UScrollArea"N) : TClass(Name, Classof<UI::UPresenter>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FSize> & UI::UScrollArea::ViewportSizeProperty() { return UScrollArea_Type::Property___ViewportSize::Property; }

	const TProperty<FVec2> & UI::UScrollArea::ScrollExtentProperty() { return UScrollArea_Type::Property___ScrollExtent::Property; }

	const TProperty<FVec2> & UI::UScrollArea::ScrollOffsetProperty() { return UScrollArea_Type::Property___ScrollOffset::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UScrollArea>
	{
		static inline UScrollArea_Type UScrollAreaType { u8"UScrollArea"N };

		static FType & Type()
		{
			return UScrollAreaType;
		}
	};

	FType & UI::UScrollArea::StaticType()
	{
		return TStaticType<UI::UScrollArea>().Type();
	}

	static TTypeRegister<UI::UScrollArea> UScrollAreaTypeRegister { TStaticType<UI::UScrollArea>().Type() };
}

// Generated code end.
