// 2C4BE97C06077FD03A2449A0D7B69950 2025-03-24 18:06:58
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Shapes/Line.h"

namespace Xin
{
	class ULine_Type : public TClass<UI::ULine>
	{
	public:
		using ULine = UI::ULine;

		// --- 0 Constructors
		static inline const TConstructor<ULine> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<ULine, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<ULine, FType &()> Method { u8"Type"N, &ULine::Type, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<ULine, FSize(FSize)> Method { u8"OnMeasure"N, &ULine::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<ULine, void(IPainter &)> Method { u8"OnPaint"N, &ULine::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___From
		{
			static inline const TDependencyProperty Property { u8"From"N, &ULine::__From, &ULine::__GetFrom, &ULine::__SetFromValue, ULine::__From__, ULine::FPropertyMetadata::None };
		};
		struct Property___To
		{
			static inline const TDependencyProperty Property { u8"To"N, &ULine::__To, &ULine::__GetTo, &ULine::__SetToValue, ULine::__To__, ULine::FPropertyMetadata::None };
		};
		struct Property___CoordinateMode
		{
			struct FPropertyMetadata : ULine::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"CoordinateMode"N, &ULine::__CoordinateMode, &ULine::__GetCoordinateMode, &ULine::__SetCoordinateModeValue, ULine::__CoordinateMode__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___From::Property,
			Property___To::Property,
			Property___CoordinateMode::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UShape>(),
		};

		// --- 5 Constructor
		ULine_Type(FName Name = u8"ULine"N) : TClass(Name, Classof<UI::UShape>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FPoint> & UI::ULine::FromProperty() { return ULine_Type::Property___From::Property; }

	const TProperty<FPoint> & UI::ULine::ToProperty() { return ULine_Type::Property___To::Property; }

	const TProperty<UI::ECoordinateMode> & UI::ULine::CoordinateModeProperty() { return ULine_Type::Property___CoordinateMode::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::ULine>
	{
		static inline ULine_Type ULineType { u8"ULine"N };

		static FType & Type()
		{
			return ULineType;
		}
	};

	FType & UI::ULine::StaticType()
	{
		return TStaticType<UI::ULine>().Type();
	}

	static TTypeRegister<UI::ULine> ULineTypeRegister { TStaticType<UI::ULine>().Type() };

	class UHLine_Type : public TClass<UI::UHLine>
	{
	public:
		using UHLine = UI::UHLine;

		// --- 0 Constructors
		static inline const TConstructor<UHLine> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UHLine, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UHLine, FType &()> Method { u8"Type"N, &UHLine::Type, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UHLine, FSize(FSize)> Method { u8"OnMeasure"N, &UHLine::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UHLine, void(IPainter &)> Method { u8"OnPaint"N, &UHLine::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UShape>(),
		};

		// --- 5 Constructor
		UHLine_Type(FName Name = u8"UHLine"N) : TClass(Name, Classof<UI::UShape>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UHLine>
	{
		static inline UHLine_Type UHLineType { u8"UHLine"N };

		static FType & Type()
		{
			return UHLineType;
		}
	};

	FType & UI::UHLine::StaticType()
	{
		return TStaticType<UI::UHLine>().Type();
	}

	static TTypeRegister<UI::UHLine> UHLineTypeRegister { TStaticType<UI::UHLine>().Type() };

	class UVLine_Type : public TClass<UI::UVLine>
	{
	public:
		using UVLine = UI::UVLine;

		// --- 0 Constructors
		static inline const TConstructor<UVLine> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UVLine, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UVLine, FType &()> Method { u8"Type"N, &UVLine::Type, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UVLine, FSize(FSize)> Method { u8"OnMeasure"N, &UVLine::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UVLine, void(IPainter &)> Method { u8"OnPaint"N, &UVLine::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UShape>(),
		};

		// --- 5 Constructor
		UVLine_Type(FName Name = u8"UVLine"N) : TClass(Name, Classof<UI::UShape>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UVLine>
	{
		static inline UVLine_Type UVLineType { u8"UVLine"N };

		static FType & Type()
		{
			return UVLineType;
		}
	};

	FType & UI::UVLine::StaticType()
	{
		return TStaticType<UI::UVLine>().Type();
	}

	static TTypeRegister<UI::UVLine> UVLineTypeRegister { TStaticType<UI::UVLine>().Type() };
}

// Generated code end.
