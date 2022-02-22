// 746916787238E89A0872DAF43B726CF1 2025-03-06 15:45:55
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Media/Color.h"

namespace Xin
{
	class FColor_Type : public TClass<FColor>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FColor> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FColor, FColor const &> Constructor___FColor_const_lref { u8"!Constructor___FColor_const_lref"N, FMetadata::None };
		static inline const TConstructor<FColor, EColor> Constructor___EColor { u8"!Constructor___EColor"N, FMetadata::None };
		static inline const TConstructor<FColor, uint8, uint8, uint8, uint8> Constructor___uint8__uint8__uint8__uint8 { u8"!Constructor___uint8__uint8__uint8__uint8"N, FMetadata::None };
		static inline const TConstructor<FColor, TVec3<uint8>, uint8> Constructor___TVec3__uint8____uint8 { u8"!Constructor___TVec3__uint8____uint8"N, FMetadata::None };
		static inline const TConstructor<FColor, uint32> Constructor___uint32 { u8"!Constructor___uint32"N, FMetadata::None };
		static inline const TConstructor<FColor, FVariant> Constructor___FVariant { u8"!Constructor___FVariant"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FColor_const_lref,
			Constructor___EColor,
			Constructor___uint8__uint8__uint8__uint8,
			Constructor___TVec3__uint8____uint8,
			Constructor___uint32,
			Constructor___FVariant,
		};

		// --- 1 Methods
		struct Method___uint32___ToARGB____const
		{
			static inline const TMethod<FColor, uint32()> Method { u8"ToARGB"N, &FColor::ToARGB, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___uint32___ToARGB____const::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<TVec4<uint8>>(),
		};

		// --- 5 Constructor
		FColor_Type(FName Name = u8"FColor"N) : TClass(Name, Classof<TVec4<uint8>>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FColor>
	{
		static inline FColor_Type FColorType { u8"FColor"N };

		static FType & Type()
		{
			return FColorType;
		}
	};

	FType & FColor::StaticType()
	{
		return TStaticType<FColor>().Type();
	}

	static TTypeRegister<FColor> FColorTypeRegister { TStaticType<FColor>().Type() };

	class FColorF_Type : public TClass<FColorF>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FColorF> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FColorF, FColorF const &> Constructor___FColorF_const_lref { u8"!Constructor___FColorF_const_lref"N, FMetadata::None };
		static inline const TConstructor<FColorF, float32, float32, float32, float32> Constructor___float32__float32__float32__float32 { u8"!Constructor___float32__float32__float32__float32"N, FMetadata::None };
		static inline const TConstructor<FColorF, TVec3<float32>, float32> Constructor___TVec3__float32____float32 { u8"!Constructor___TVec3__float32____float32"N, FMetadata::None };
		static inline const TConstructor<FColorF, EColor> Constructor___EColor { u8"!Constructor___EColor"N, FMetadata::None };
		static inline const TConstructor<FColorF, FColor const &> Constructor___FColor_const_lref { u8"!Constructor___FColor_const_lref"N, FMetadata::None };
		static inline const TConstructor<FColorF, uint8, uint8, uint8, uint8> Constructor___uint8__uint8__uint8__uint8 { u8"!Constructor___uint8__uint8__uint8__uint8"N, FMetadata::None };
		static inline const TConstructor<FColorF, TVec3<uint8>, uint8> Constructor___TVec3__uint8____uint8 { u8"!Constructor___TVec3__uint8____uint8"N, FMetadata::None };
		static inline const TConstructor<FColorF, uint32> Constructor___uint32 { u8"!Constructor___uint32"N, FMetadata::None };
		static inline const TConstructor<FColorF, FVariant> Constructor___FVariant { u8"!Constructor___FVariant"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FColorF_const_lref,
			Constructor___float32__float32__float32__float32,
			Constructor___TVec3__float32____float32,
			Constructor___EColor,
			Constructor___FColor_const_lref,
			Constructor___uint8__uint8__uint8__uint8,
			Constructor___TVec3__uint8____uint8,
			Constructor___uint32,
			Constructor___FVariant,
		};

		// --- 1 Methods

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<TVec4<float32>>(),
		};

		// --- 5 Constructor
		FColorF_Type(FName Name = u8"FColorF"N) : TClass(Name, Classof<TVec4<float32>>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FColorF>
	{
		static inline FColorF_Type FColorFType { u8"FColorF"N };

		static FType & Type()
		{
			return FColorFType;
		}
	};

	FType & FColorF::StaticType()
	{
		return TStaticType<FColorF>().Type();
	}

	static TTypeRegister<FColorF> FColorFTypeRegister { TStaticType<FColorF>().Type() };
}

// Generated code end.
