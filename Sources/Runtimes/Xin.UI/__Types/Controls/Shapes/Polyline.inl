// D028A76B09C55504971479BBFB1B85CD 2025-03-24 18:07:02
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Shapes/Polyline.h"

namespace Xin
{
	class UPolyline_Type : public TClass<UI::UPolyline>
	{
	public:
		using UPolyline = UI::UPolyline;

		// --- 0 Constructors
		static inline const TConstructor<UPolyline> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UPolyline, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UPolyline, FType &()> Method { u8"Type"N, &UPolyline::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UPolyline, void()> Method { u8"OnInitialize"N, &UPolyline::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UPolyline, void()> Method { u8"OnFinalize"N, &UPolyline::OnFinalize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UPolyline, FSize(FSize)> Method { u8"OnMeasure"N, &UPolyline::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UPolyline, void(IPainter &)> Method { u8"OnPaint"N, &UPolyline::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields
		struct Field___Points
		{
			static inline const TField Field { u8"Points"N, &UPolyline::Points, FMetadata::None };
		};
		struct Field___Stretch
		{
			static inline const TField Field { u8"Stretch"N, &UPolyline::Stretch, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Points::Field,
			Field___Stretch::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UShape>(),
		};

		// --- 5 Constructor
		UPolyline_Type(FName Name = u8"UPolyline"N) : TClass(Name, Classof<UI::UShape>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UPolyline>
	{
		static inline UPolyline_Type UPolylineType { u8"UPolyline"N };

		static FType & Type()
		{
			return UPolylineType;
		}
	};

	FType & UI::UPolyline::StaticType()
	{
		return TStaticType<UI::UPolyline>().Type();
	}

	static TTypeRegister<UI::UPolyline> UPolylineTypeRegister { TStaticType<UI::UPolyline>().Type() };
}

// Generated code end.
