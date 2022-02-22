// 71215929D05D9A270CA07A30322543F5 2025-03-24 18:06:01
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Border.h"

namespace Xin
{
	class UBorder_Type : public TClass<UI::UBorder>
	{
	public:
		using UBorder = UI::UBorder;

		// --- 0 Constructors
		static inline const TConstructor<UBorder> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UBorder, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UBorder, FType &()> Method { u8"Type"N, &UBorder::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UBorder, void()> Method { u8"OnConstruct"N, &UBorder::OnConstruct, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UBorder, FSize(FSize)> Method { u8"OnMeasure"N, &UBorder::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UBorder, void()> Method { u8"OnArrange"N, &UBorder::OnArrange, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UBorder, void(IPainter &)> Method { u8"OnPaint"N, &UBorder::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Fill
		{
			struct FPropertyMetadata : UBorder::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Fill"N, &UBorder::__Fill, &UBorder::__GetFill, &UBorder::__SetFillValue, UBorder::__Fill__, Metadata };
		};
		struct Property___Stroke
		{
			struct FPropertyMetadata : UBorder::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Stroke"N, &UBorder::__Stroke, &UBorder::__GetStroke, &UBorder::__SetStrokeValue, UBorder::__Stroke__, Metadata };
		};
		struct Property___Thickness
		{
			struct FPropertyMetadata : UBorder::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
					FriendlyName = u8"Border Thickness"V;
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Thickness"N, &UBorder::__Thickness, &UBorder::__GetThickness, &UBorder::__SetThicknessValue, UBorder::__Thickness__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Fill::Property,
			Property___Stroke::Property,
			Property___Thickness::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPresenter>(),
		};

		// --- 5 Constructor
		UBorder_Type(FName Name = u8"UBorder"N) : TClass(Name, Classof<UI::UPresenter>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<IBrushRef> & UI::UBorder::FillProperty() { return UBorder_Type::Property___Fill::Property; }

	const TProperty<IBrushRef> & UI::UBorder::StrokeProperty() { return UBorder_Type::Property___Stroke::Property; }

	const TProperty<FThickness> & UI::UBorder::ThicknessProperty() { return UBorder_Type::Property___Thickness::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UBorder>
	{
		static inline UBorder_Type UBorderType { u8"UBorder"N };

		static FType & Type()
		{
			return UBorderType;
		}
	};

	FType & UI::UBorder::StaticType()
	{
		return TStaticType<UI::UBorder>().Type();
	}

	static TTypeRegister<UI::UBorder> UBorderTypeRegister { TStaticType<UI::UBorder>().Type() };
}

// Generated code end.
