// 3B8E6FD0119B98AD95BDE38BE227B164 2025-02-14 16:16:18
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Vector2.h"

namespace Xin
{
	class FVector2_Type : public TClass<FVector2>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FVector2> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FVector2, FVector2 const &> Constructor___FVector2_const_lref { u8"!Constructor___FVector2_const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector2, Vec2F const &> Constructor___TVec2__float32___const_lref { u8"!Constructor___TVec2__float32___const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector2, float32, float32> Constructor___float32__float32 { u8"!Constructor___float32__float32"N, FMetadata::None };
		static inline const TConstructor<FVector2, float32 const(&)[2]> Constructor___float32_const_lref_2 { u8"!Constructor___float32_const_lref_2"N, FMetadata::None };
		static inline const TConstructor<FVector2, DirectX::XMVECTOR const &> Constructor____const_lref { u8"!Constructor____const_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FVector2_const_lref,
			Constructor___TVec2__float32___const_lref,
			Constructor___float32__float32,
			Constructor___float32_const_lref_2,
			Constructor____const_lref,
		};

		// --- 1 Methods
		struct Method___bool___HasNan____const
		{
			static inline const TMethod<FVector2, bool()> Method { u8"HasNan"N, &FVector2::HasNan, FMetadata::None };
		};
		struct Method___FVector2_lref___Normalize
		{
			static inline const TMethod<FVector2, FVector2 &()> Method { u8"Normalize"N, &FVector2::Normalize, FMetadata::None };
		};
		struct Method___FVector2___Normalized____const
		{
			static inline const TMethod<FVector2, FVector2()> Method { u8"Normalized"N, &FVector2::Normalized, FMetadata::None };
		};
		struct Method___float32___Length____const
		{
			static inline const TMethod<FVector2, float32()> Method { u8"Length"N, &FVector2::Length, FMetadata::None };
		};
		struct Method___float32___LengthSquare____const
		{
			static inline const TMethod<FVector2, float32()> Method { u8"LengthSquare"N, &FVector2::LengthSquare, FMetadata::None };
		};
		struct Method___float32___Dot__FVector2_const_lref____const
		{
			static inline const TMethod<FVector2, float32(FVector2 const &)> Method { u8"Dot"N, &FVector2::Dot, FMetadata::None };
		};
		struct Method___FVector2___Cross__FVector2_const_lref____const
		{
			static inline const TMethod<FVector2, FVector2(FVector2 const &)> Method { u8"Cross"N, &FVector2::Cross, FMetadata::None };
		};
		struct Method___FVector2___Abs____const
		{
			static inline const TMethod<FVector2, FVector2()> Method { u8"Abs"N, &FVector2::Abs, FMetadata::None };
		};
		struct Method___FVector2___Negate____const
		{
			static inline const TMethod<FVector2, FVector2()> Method { u8"Negate"N, &FVector2::Negate, FMetadata::None };
		};
		struct Method___FVector2___Reciprocal____const
		{
			static inline const TMethod<FVector2, FVector2()> Method { u8"Reciprocal"N, &FVector2::Reciprocal, FMetadata::None };
		};
		struct Method___FVector2___Add__FVector2_const_lref____const
		{
			static inline const TMethod<FVector2, FVector2(FVector2 const &)> Method { u8"Add"N, &FVector2::Add, FMetadata::None };
		};
		struct Method___FVector2___Subtract__FVector2_const_lref____const
		{
			static inline const TMethod<FVector2, FVector2(FVector2 const &)> Method { u8"Subtract"N, &FVector2::Subtract, FMetadata::None };
		};
		struct Method___FVector2___Multiply__FVector2_const_lref____const
		{
			static inline const TMethod<FVector2, FVector2(FVector2 const &)> Method { u8"Multiply"N, &FVector2::Multiply, FMetadata::None };
		};
		struct Method___FVector2___Divide__FVector2_const_lref____const
		{
			static inline const TMethod<FVector2, FVector2(FVector2 const &)> Method { u8"Divide"N, &FVector2::Divide, FMetadata::None };
		};
		struct Method___FVector2___MultiplyAdd__FVector2_const_lref__FVector2_const_lref____const
		{
			static inline const TMethod<FVector2, FVector2(FVector2 const &, FVector2 const &)> Method { u8"MultiplyAdd"N, &FVector2::MultiplyAdd, FMetadata::None };
		};
		struct Method___FVector2_lref___Set__
		{
			static inline const TMethod<FVector2, FVector2 &(DirectX::XMVECTOR)> Method { u8"Set"N, &FVector2::Set, FMetadata::None };
		};
		struct Method______ToXMVector____const
		{
			static inline const TMethod<FVector2, DirectX::XMVECTOR()> Method { u8"ToXMVector"N, &FVector2::ToXMVector, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___bool___HasNan____const::Method,
			Method___FVector2_lref___Normalize::Method,
			Method___FVector2___Normalized____const::Method,
			Method___float32___Length____const::Method,
			Method___float32___LengthSquare____const::Method,
			Method___float32___Dot__FVector2_const_lref____const::Method,
			Method___FVector2___Cross__FVector2_const_lref____const::Method,
			Method___FVector2___Abs____const::Method,
			Method___FVector2___Negate____const::Method,
			Method___FVector2___Reciprocal____const::Method,
			Method___FVector2___Add__FVector2_const_lref____const::Method,
			Method___FVector2___Subtract__FVector2_const_lref____const::Method,
			Method___FVector2___Multiply__FVector2_const_lref____const::Method,
			Method___FVector2___Divide__FVector2_const_lref____const::Method,
			Method___FVector2___MultiplyAdd__FVector2_const_lref__FVector2_const_lref____const::Method,
			Method___FVector2_lref___Set__::Method,
			Method______ToXMVector____const::Method,
		};

		// --- 2 Fields
		struct Field___X
		{
			static inline const TField Field { u8"X"N, &FVector2::X, FMetadata::None };
		};
		struct Field___Y
		{
			static inline const TField Field { u8"Y"N, &FVector2::Y, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___X::Field,
			Field___Y::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		FVector2_Type(FName Name = u8"FVector2"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FVector2>
	{
		static FType & Type()
		{
			static FVector2_Type FVector2Type { u8"FVector2"N };
			return FVector2Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<FVector2>()
	{
		return TStaticType<FVector2>().Type();
	}

	static TTypeRegister<FVector2> FVector2TypeRegister { TStaticType<FVector2>().Type() };
}

// Generated code end.
