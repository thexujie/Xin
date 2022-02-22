// 3B56B90572C6F86444D88D765A8D362C 2025-03-07 19:45:09
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Media/Brush.h"

namespace Xin
{
	class IBrush_Type : public TClass<IBrush>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<IBrush> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<IBrush, nullp> Constructor___nullp { u8"!Constructor___nullp"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___nullp,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IBrush, FType &()> Method { u8"Type"N, &IBrush::Type, FMetadata::None };
		};
		struct Method___bool___IsValid____const
		{
			static inline const TMethod<IBrush, bool()> Method { u8"IsValid"N, &IBrush::IsValid, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___bool___IsValid____const::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IAnimatable>(),
		};

		// --- 5 Constructor
		IBrush_Type(FName Name = u8"IBrush"N) : TClass(Name, Classof<IAnimatable>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<IBrush>
	{
		static inline IBrush_Type IBrushType { u8"IBrush"N };

		static FType & Type()
		{
			return IBrushType;
		}
	};

	FType & IBrush::StaticType()
	{
		return TStaticType<IBrush>().Type();
	}

	static TTypeRegister<IBrush> IBrushTypeRegister { TStaticType<IBrush>().Type() };

	class FSolidColorBrush_Type : public TClass<FSolidColorBrush>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FSolidColorBrush> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FSolidColorBrush, FColor> Constructor___FColor { u8"!Constructor___FColor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FColor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<FSolidColorBrush, FType &()> Method { u8"Type"N, &FSolidColorBrush::Type, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Color
		{
			static inline const TDependencyProperty Property { u8"Color"N, &FSolidColorBrush::__Color, &FSolidColorBrush::__GetColor, &FSolidColorBrush::__SetColorValue, FSolidColorBrush::__Color__, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Color::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IBrush>(),
		};

		// --- 5 Constructor
		FSolidColorBrush_Type(FName Name = u8"FSolidColorBrush"N) : TClass(Name, Classof<IBrush>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FColor> & FSolidColorBrush::ColorProperty() { return FSolidColorBrush_Type::Property___Color::Property; }

	template<> struct XIN_EXPORT TStaticType<FSolidColorBrush>
	{
		static inline FSolidColorBrush_Type FSolidColorBrushType { u8"FSolidColorBrush"N };

		static FType & Type()
		{
			return FSolidColorBrushType;
		}
	};

	FType & FSolidColorBrush::StaticType()
	{
		return TStaticType<FSolidColorBrush>().Type();
	}

	static TTypeRegister<FSolidColorBrush> FSolidColorBrushTypeRegister { TStaticType<FSolidColorBrush>().Type() };
}

// Generated code end.
