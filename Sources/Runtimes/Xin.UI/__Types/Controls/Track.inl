// 0AD11F1D09EFF53B42AB9E54179B7ADF 2025-03-25 15:49:29
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Track.h"

namespace Xin
{
	class UTrack_Type : public TClass<UI::UTrack>
	{
	public:
		using UTrack = UI::UTrack;

		// --- 0 Constructors
		static inline const TConstructor<UTrack> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UTrack, FType &()> Method { u8"Type"N, &UTrack::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UTrack, void()> Method { u8"OnInitialize"N, &UTrack::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UTrack, void()> Method { u8"OnFinalize"N, &UTrack::OnFinalize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UTrack, FSize(FSize)> Method { u8"OnMeasure"N, &UTrack::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UTrack, void()> Method { u8"OnArrange"N, &UTrack::OnArrange, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UTrack, void(IPainter &)> Method { u8"OnPaint"N, &UTrack::OnPaint, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UTrack, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UTrack::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UTrack, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &UTrack::OnMouseUp, FMetadata::None };
		};
		struct Method___void___OnMouseEnter__FMouseEventArgs_lref
		{
			static inline const TMethod<UTrack, void(UI::FMouseEventArgs &)> Method { u8"OnMouseEnter"N, &UTrack::OnMouseEnter, FMetadata::None };
		};
		struct Method___void___OnMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<UTrack, void(UI::FMouseEventArgs &)> Method { u8"OnMouseLeave"N, &UTrack::OnMouseLeave, FMetadata::None };
		};
		struct Method___void___ArrangeParts
		{
			static inline const TMethod<UTrack, void()> Method { u8"ArrangeParts"N, &UTrack::ArrangeParts, FMetadata::None };
		};
		struct Method___void___OnThumbChanged__TReferPtr__UThumb__
		{
			static inline const TMethod<UTrack, void(UI::UThumbRef)> Method { u8"OnThumbChanged"N, &UTrack::OnThumbChanged, FMetadata::None };
		};
		struct Method___void___OnIncreaseButtonChanged__TReferPtr__URepeatButton__
		{
			static inline const TMethod<UTrack, void(UI::URepeatButtonRef)> Method { u8"OnIncreaseButtonChanged"N, &UTrack::OnIncreaseButtonChanged, FMetadata::None };
		};
		struct Method___void___OnDecreaseButtonChanged__TReferPtr__URepeatButton__
		{
			static inline const TMethod<UTrack, void(UI::URepeatButtonRef)> Method { u8"OnDecreaseButtonChanged"N, &UTrack::OnDecreaseButtonChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnPaint__IPainter_lref::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseEnter__FMouseEventArgs_lref::Method,
			Method___void___OnMouseLeave__FMouseEventArgs_lref::Method,
			Method___void___ArrangeParts::Method,
			Method___void___OnThumbChanged__TReferPtr__UThumb__::Method,
			Method___void___OnIncreaseButtonChanged__TReferPtr__URepeatButton__::Method,
			Method___void___OnDecreaseButtonChanged__TReferPtr__URepeatButton__::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Orientation
		{
			struct FPropertyMetadata : UTrack::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Orientation"N, &UTrack::__Orientation, &UTrack::__GetOrientation, &UTrack::__SetOrientationValue, UTrack::__Orientation__, Metadata };
		};
		struct Property___Background
		{
			struct FPropertyMetadata : UTrack::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					Hierarchical = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Background"N, &UTrack::__Background, &UTrack::__GetBackground, &UTrack::__SetBackgroundValue, UTrack::__Background__, Metadata };
		};
		struct Property___ViewportSize
		{
			struct FPropertyMetadata : UTrack::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"ViewportSize"N, &UTrack::__ViewportSize, &UTrack::__GetViewportSize, &UTrack::__SetViewportSizeValue, UTrack::__ViewportSize__, Metadata };
		};
		struct Property___Maximum
		{
			struct FPropertyMetadata : UTrack::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Maximum"N, &UTrack::__Maximum, &UTrack::__GetMaximum, &UTrack::__SetMaximumValue, UTrack::__Maximum__, Metadata };
		};
		struct Property___Minimum
		{
			struct FPropertyMetadata : UTrack::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Minimum"N, &UTrack::__Minimum, &UTrack::__GetMinimum, &UTrack::__SetMinimumValue, UTrack::__Minimum__, Metadata };
		};
		struct Property___Value
		{
			struct FPropertyMetadata : UTrack::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Value"N, &UTrack::__Value, &UTrack::__GetValue, &UTrack::__SetValueValue, UTrack::__Value__, Metadata };
		};
		struct Property___Thumb
		{
			static inline const TProxyProperty Property { u8"Thumb"N, &UTrack::__Thumb, &UTrack::__GetThumb, &UTrack::__SetThumb, UTrack::FPropertyMetadata::None };
		};
		struct Property___IncreaseButton
		{
			static inline const TProxyProperty Property { u8"IncreaseButton"N, &UTrack::__IncreaseButton, &UTrack::__GetIncreaseButton, &UTrack::__SetIncreaseButton, UTrack::FPropertyMetadata::None };
		};
		struct Property___DecreaseButton
		{
			static inline const TProxyProperty Property { u8"DecreaseButton"N, &UTrack::__DecreaseButton, &UTrack::__GetDecreaseButton, &UTrack::__SetDecreaseButton, UTrack::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Orientation::Property,
			Property___Background::Property,
			Property___ViewportSize::Property,
			Property___Maximum::Property,
			Property___Minimum::Property,
			Property___Value::Property,
			Property___Thumb::Property,
			Property___IncreaseButton::Property,
			Property___DecreaseButton::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		UTrack_Type(FName Name = u8"UTrack"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::EOrientation> & UI::UTrack::OrientationProperty() { return UTrack_Type::Property___Orientation::Property; }

	const TProperty<IBrushRef> & UI::UTrack::BackgroundProperty() { return UTrack_Type::Property___Background::Property; }

	const TProperty<dimenx> & UI::UTrack::ViewportSizeProperty() { return UTrack_Type::Property___ViewportSize::Property; }

	const TProperty<dimenx> & UI::UTrack::MaximumProperty() { return UTrack_Type::Property___Maximum::Property; }

	const TProperty<dimenx> & UI::UTrack::MinimumProperty() { return UTrack_Type::Property___Minimum::Property; }

	const TProperty<dimenx> & UI::UTrack::ValueProperty() { return UTrack_Type::Property___Value::Property; }

	const TProperty<UI::UThumbRef> & UI::UTrack::ThumbProperty() { return UTrack_Type::Property___Thumb::Property; }

	const TProperty<UI::URepeatButtonRef> & UI::UTrack::IncreaseButtonProperty() { return UTrack_Type::Property___IncreaseButton::Property; }

	const TProperty<UI::URepeatButtonRef> & UI::UTrack::DecreaseButtonProperty() { return UTrack_Type::Property___DecreaseButton::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UTrack>
	{
		static inline UTrack_Type UTrackType { u8"UTrack"N };

		static FType & Type()
		{
			return UTrackType;
		}
	};

	FType & UI::UTrack::StaticType()
	{
		return TStaticType<UI::UTrack>().Type();
	}

	static TTypeRegister<UI::UTrack> UTrackTypeRegister { TStaticType<UI::UTrack>().Type() };
}

// Generated code end.
