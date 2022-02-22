// 64134A0D327D9370D442EA7C0D254A75 2025-03-06 15:45:58
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Media/Pen.h"

namespace Xin
{
	class IPen_Type : public TClass<IPen>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<IPen> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<IPen, IBrush const &, float32, FStrokeStyle> Constructor___IBrush_const_lref__float32__FStrokeStyle { u8"!Constructor___IBrush_const_lref__float32__FStrokeStyle"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___IBrush_const_lref__float32__FStrokeStyle,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IPen, FType &()> Method { u8"Type"N, &IPen::Type, FMetadata::None };
		};
		struct Method___bool___IsValid____const
		{
			static inline const TMethod<IPen, bool()> Method { u8"IsValid"N, &IPen::IsValid, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___bool___IsValid____const::Method,
		};

		// --- 2 Fields
		struct Field___StrokeStyle
		{
			static inline const TField Field { u8"StrokeStyle"N, &IPen::StrokeStyle, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___StrokeStyle::Field,
		};

		// --- 3 Properties
		struct Property___Thickness
		{
			static inline const TDependencyProperty Property { u8"Thickness"N, &IPen::__Thickness, &IPen::__GetThickness, &IPen::__SetThicknessValue, IPen::__Thickness__, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Thickness::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IAnimatable>(),
		};

		// --- 5 Constructor
		IPen_Type(FName Name = u8"IPen"N) : TClass(Name, Classof<IAnimatable>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<float32> & IPen::ThicknessProperty() { return IPen_Type::Property___Thickness::Property; }

	template<> struct XIN_EXPORT TStaticType<IPen>
	{
		static inline IPen_Type IPenType { u8"IPen"N };

		static FType & Type()
		{
			return IPenType;
		}
	};

	FType & IPen::StaticType()
	{
		return TStaticType<IPen>().Type();
	}

	static TTypeRegister<IPen> IPenTypeRegister { TStaticType<IPen>().Type() };
}

// Generated code end.
