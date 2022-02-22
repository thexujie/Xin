// FF5ACE62B395931473ADA3F4D66D030F 2025-03-31 14:40:17
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Popup.h"

namespace Xin
{
	class UPopup_Type : public TClass<UI::UPopup>
	{
	public:
		using UPopup = UI::UPopup;

		// --- 0 Constructors
		static inline const TConstructor<UPopup> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UPopup, FType &()> Method { u8"Type"N, &UPopup::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UPopup, void()> Method { u8"OnInitialize"N, &UPopup::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UPopup, void()> Method { u8"OnFinalize"N, &UPopup::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnDesiredSizeChanged__TVec2__float64__
		{
			static inline const TMethod<UPopup, void(FSize)> Method { u8"OnDesiredSizeChanged"N, &UPopup::OnDesiredSizeChanged, FMetadata::None };
		};
		struct Method___void___OnShowing__FEventArgs_lref
		{
			static inline const TMethod<UPopup, void(FEventArgs &)> Method { u8"OnShowing"N, &UPopup::OnShowing, FMetadata::None };
		};
		struct Method___void___OnTrackElementChanged__TEntryPtr__UElement__
		{
			static inline const TMethod<UPopup, void(UI::UElementObj)> Method { u8"OnTrackElementChanged"N, &UPopup::OnTrackElementChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnDesiredSizeChanged__TVec2__float64__::Method,
			Method___void___OnShowing__FEventArgs_lref::Method,
			Method___void___OnTrackElementChanged__TEntryPtr__UElement__::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___TrackElement
		{
			static inline const TProxyProperty Property { u8"TrackElement"N, &UPopup::__TrackElement, &UPopup::__GetTrackElement, &UPopup::__SetTrackElement, UPopup::FPropertyMetadata::None };
		};
		struct Property___PopupPosition
		{
			static inline const TDependencyProperty Property { u8"PopupPosition"N, &UPopup::__PopupPosition, &UPopup::__GetPopupPosition, &UPopup::__SetPopupPositionValue, UPopup::__PopupPosition__, UPopup::FPropertyMetadata::None };
		};
		struct Property___PlacementOffset
		{
			static inline const TDependencyProperty Property { u8"PlacementOffset"N, &UPopup::__PlacementOffset, &UPopup::__GetPlacementOffset, &UPopup::__SetPlacementOffsetValue, UPopup::__PlacementOffset__, UPopup::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___TrackElement::Property,
			Property___PopupPosition::Property,
			Property___PlacementOffset::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UWindow>(),
		};

		// --- 5 Constructor
		UPopup_Type(FName Name = u8"UPopup"N) : TClass(Name, Classof<UI::UWindow>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::UElementObj> & UI::UPopup::TrackElementProperty() { return UPopup_Type::Property___TrackElement::Property; }

	const TProperty<EPopupPosition> & UI::UPopup::PopupPositionProperty() { return UPopup_Type::Property___PopupPosition::Property; }

	const TProperty<FSize> & UI::UPopup::PlacementOffsetProperty() { return UPopup_Type::Property___PlacementOffset::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UPopup>
	{
		static inline UPopup_Type UPopupType { u8"UPopup"N };

		static FType & Type()
		{
			return UPopupType;
		}
	};

	FType & UI::UPopup::StaticType()
	{
		return TStaticType<UI::UPopup>().Type();
	}

	static TTypeRegister<UI::UPopup> UPopupTypeRegister { TStaticType<UI::UPopup>().Type() };
}

// Generated code end.
