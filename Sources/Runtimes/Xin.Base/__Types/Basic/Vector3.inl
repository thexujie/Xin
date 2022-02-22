// 0BC54AC89960EAB7F45CF371F2353EA7 2024-12-25 11:51:27
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Vector3.h"

namespace Xin
{
	class FVector3_Type : public TClass<FVector3>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FVector3> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FVector3, FVector3 const &> Constructor___FVector3_const_lref { u8"!Constructor___FVector3_const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector3, float32, float32> Constructor___float32__float32 { u8"!Constructor___float32__float32"N, FMetadata::None };
		static inline const TConstructor<FVector3, float32, float32, float32> Constructor___float32__float32__float32 { u8"!Constructor___float32__float32__float32"N, FMetadata::None };
		static inline const TConstructor<FVector3, float32 const(&)[3]> Constructor___float32_const_lref_3 { u8"!Constructor___float32_const_lref_3"N, FMetadata::None };
		static inline const TConstructor<FVector3, FVector2 const &, float32> Constructor___FVector2_const_lref__float32 { u8"!Constructor___FVector2_const_lref__float32"N, FMetadata::None };
		static inline const TConstructor<FVector3, Vec3F const &> Constructor___TVec3__float32___const_lref { u8"!Constructor___TVec3__float32___const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector3, DirectX::XMVECTOR const &> Constructor____const_lref { u8"!Constructor____const_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FVector3_const_lref,
			Constructor___float32__float32,
			Constructor___float32__float32__float32,
			Constructor___float32_const_lref_3,
			Constructor___FVector2_const_lref__float32,
			Constructor___TVec3__float32___const_lref,
			Constructor____const_lref,
		};

		// --- 1 Methods
		struct Method___FVector3_lref___Set__FVector3_const_lref
		{
			static inline const TMethod<FVector3, FVector3 &(FVector3 const &)> Method { u8"Set"N, &FVector3::Set, FMetadata::None };
		};
		struct Method___bool___HasNan____const
		{
			static inline const TMethod<FVector3, bool()> Method { u8"HasNan"N, &FVector3::HasNan, FMetadata::None };
		};
		struct Method___FVector3_lref___Normalize
		{
			static inline const TMethod<FVector3, FVector3 &()> Method { u8"Normalize"N, &FVector3::Normalize, FMetadata::None };
		};
		struct Method___FVector3___Normalized____const
		{
			static inline const TMethod<FVector3, FVector3()> Method { u8"Normalized"N, &FVector3::Normalized, FMetadata::None };
		};
		struct Method___float32___Length____const
		{
			static inline const TMethod<FVector3, float32()> Method { u8"Length"N, &FVector3::Length, FMetadata::None };
		};
		struct Method___float32___LengthSquare____const
		{
			static inline const TMethod<FVector3, float32()> Method { u8"LengthSquare"N, &FVector3::LengthSquare, FMetadata::None };
		};
		struct Method___float32___Dot__FVector3_const_lref____const
		{
			static inline const TMethod<FVector3, float32(FVector3 const &)> Method { u8"Dot"N, &FVector3::Dot, FMetadata::None };
		};
		struct Method___FVector3___Cross__FVector3_const_lref____const
		{
			static inline const TMethod<FVector3, FVector3(FVector3 const &)> Method { u8"Cross"N, &FVector3::Cross, FMetadata::None };
		};
		struct Method___FVector3___Abs____const
		{
			static inline const TMethod<FVector3, FVector3()> Method { u8"Abs"N, &FVector3::Abs, FMetadata::None };
		};
		struct Method___FVector3___Negate____const
		{
			static inline const TMethod<FVector3, FVector3()> Method { u8"Negate"N, &FVector3::Negate, FMetadata::None };
		};
		struct Method___FVector3___Reciprocal____const
		{
			static inline const TMethod<FVector3, FVector3()> Method { u8"Reciprocal"N, &FVector3::Reciprocal, FMetadata::None };
		};
		struct Method___FVector3___Add__FVector3_const_lref____const
		{
			static inline const TMethod<FVector3, FVector3(FVector3 const &)> Method { u8"Add"N, &FVector3::Add, FMetadata::None };
		};
		struct Method___FVector3___Subtract__FVector3_const_lref____const
		{
			static inline const TMethod<FVector3, FVector3(FVector3 const &)> Method { u8"Subtract"N, &FVector3::Subtract, FMetadata::None };
		};
		struct Method___FVector3___Multiply__FVector3_const_lref____const
		{
			static inline const TMethod<FVector3, FVector3(FVector3 const &)> Method { u8"Multiply"N, &FVector3::Multiply, FMetadata::None };
		};
		struct Method___FVector3___Divide__FVector3_const_lref____const
		{
			static inline const TMethod<FVector3, FVector3(FVector3 const &)> Method { u8"Divide"N, &FVector3::Divide, FMetadata::None };
		};
		struct Method___FVector3___MultiplyAdd__FVector3_const_lref__FVector3_const_lref____const
		{
			static inline const TMethod<FVector3, FVector3(FVector3 const &, FVector3 const &)> Method { u8"MultiplyAdd"N, &FVector3::MultiplyAdd, FMetadata::None };
		};
		struct Method___FVector3_lref___Set__
		{
			static inline const TMethod<FVector3, FVector3 &(DirectX::XMVECTOR)> Method { u8"Set"N, &FVector3::Set, FMetadata::None };
		};
		struct Method______ToXMVector____const
		{
			static inline const TMethod<FVector3, DirectX::XMVECTOR()> Method { u8"ToXMVector"N, &FVector3::ToXMVector, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FVector3_lref___Set__FVector3_const_lref::Method,
			Method___bool___HasNan____const::Method,
			Method___FVector3_lref___Normalize::Method,
			Method___FVector3___Normalized____const::Method,
			Method___float32___Length____const::Method,
			Method___float32___LengthSquare____const::Method,
			Method___float32___Dot__FVector3_const_lref____const::Method,
			Method___FVector3___Cross__FVector3_const_lref____const::Method,
			Method___FVector3___Abs____const::Method,
			Method___FVector3___Negate____const::Method,
			Method___FVector3___Reciprocal____const::Method,
			Method___FVector3___Add__FVector3_const_lref____const::Method,
			Method___FVector3___Subtract__FVector3_const_lref____const::Method,
			Method___FVector3___Multiply__FVector3_const_lref____const::Method,
			Method___FVector3___Divide__FVector3_const_lref____const::Method,
			Method___FVector3___MultiplyAdd__FVector3_const_lref__FVector3_const_lref____const::Method,
			Method___FVector3_lref___Set__::Method,
			Method______ToXMVector____const::Method,
		};

		// --- 2 Fields
		struct Field___X
		{
			static inline const TField Field { u8"X"N, &FVector3::X, FMetadata::None };
		};
		struct Field___Y
		{
			static inline const TField Field { u8"Y"N, &FVector3::Y, FMetadata::None };
		};
		struct Field___Z
		{
			static inline const TField Field { u8"Z"N, &FVector3::Z, FMetadata::None };
		};
		struct Field___Pitch
		{
			static inline const TField Field { u8"Pitch"N, &FVector3::Pitch, FMetadata::None };
		};
		struct Field___Yaw
		{
			static inline const TField Field { u8"Yaw"N, &FVector3::Yaw, FMetadata::None };
		};
		struct Field___Roll
		{
			static inline const TField Field { u8"Roll"N, &FVector3::Roll, FMetadata::None };
		};
		struct Field___XY
		{
			static inline const TField Field { u8"XY"N, &FVector3::XY, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___X::Field,
			Field___Y::Field,
			Field___Z::Field,
			Field___Pitch::Field,
			Field___Yaw::Field,
			Field___Roll::Field,
			Field___XY::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		FVector3_Type(FName Name = u8"FVector3"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FVector3>
	{
		static FType & Type()
		{
			static FVector3_Type FVector3Type { u8"FVector3"N };
			return FVector3Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<FVector3>()
	{
		return TStaticType<FVector3>().Type();
	}

	static TTypeRegister<FVector3> FVector3TypeRegister { TStaticType<FVector3>().Type() };
}

// Generated code end.
