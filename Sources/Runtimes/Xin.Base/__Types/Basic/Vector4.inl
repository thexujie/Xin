// 99EAEB3813BC7346A9EA9F6EDCE83562 2024-12-25 11:51:28
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Vector4.h"

namespace Xin
{
	class FVector4_Type : public TClass<FVector4>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FVector4> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FVector4, FVector4 const &> Constructor___FVector4_const_lref { u8"!Constructor___FVector4_const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector4, float32, float32> Constructor___float32__float32 { u8"!Constructor___float32__float32"N, FMetadata::None };
		static inline const TConstructor<FVector4, float32, float32, float32> Constructor___float32__float32__float32 { u8"!Constructor___float32__float32__float32"N, FMetadata::None };
		static inline const TConstructor<FVector4, float32, float32, float32, float32> Constructor___float32__float32__float32__float32 { u8"!Constructor___float32__float32__float32__float32"N, FMetadata::None };
		static inline const TConstructor<FVector4, FVector2 const &> Constructor___FVector2_const_lref { u8"!Constructor___FVector2_const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector4, FVector2 const &, float32> Constructor___FVector2_const_lref__float32 { u8"!Constructor___FVector2_const_lref__float32"N, FMetadata::None };
		static inline const TConstructor<FVector4, FVector2 const &, float32, float32> Constructor___FVector2_const_lref__float32__float32 { u8"!Constructor___FVector2_const_lref__float32__float32"N, FMetadata::None };
		static inline const TConstructor<FVector4, FVector2 const &, FVector2 const &> Constructor___FVector2_const_lref__FVector2_const_lref { u8"!Constructor___FVector2_const_lref__FVector2_const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector4, FVector3 const &> Constructor___FVector3_const_lref { u8"!Constructor___FVector3_const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector4, FVector3 const &, float32> Constructor___FVector3_const_lref__float32 { u8"!Constructor___FVector3_const_lref__float32"N, FMetadata::None };
		static inline const TConstructor<FVector4, Vec2F const &> Constructor___TVec2__float32___const_lref { u8"!Constructor___TVec2__float32___const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector4, Vec2F const &, float32> Constructor___TVec2__float32___const_lref__float32 { u8"!Constructor___TVec2__float32___const_lref__float32"N, FMetadata::None };
		static inline const TConstructor<FVector4, Vec2F const &, float32, float32> Constructor___TVec2__float32___const_lref__float32__float32 { u8"!Constructor___TVec2__float32___const_lref__float32__float32"N, FMetadata::None };
		static inline const TConstructor<FVector4, Vec2F const &, Vec2F const &> Constructor___TVec2__float32___const_lref__TVec2__float32___const_lref { u8"!Constructor___TVec2__float32___const_lref__TVec2__float32___const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector4, Vec3F const &> Constructor___TVec3__float32___const_lref { u8"!Constructor___TVec3__float32___const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector4, Vec3F const &, float32> Constructor___TVec3__float32___const_lref__float32 { u8"!Constructor___TVec3__float32___const_lref__float32"N, FMetadata::None };
		static inline const TConstructor<FVector4, Vec4F const &> Constructor___TVec4__float32___const_lref { u8"!Constructor___TVec4__float32___const_lref"N, FMetadata::None };
		static inline const TConstructor<FVector4, float32 const(&)[4]> Constructor___float32_const_lref_4 { u8"!Constructor___float32_const_lref_4"N, FMetadata::None };
		static inline const TConstructor<FVector4, DirectX::XMVECTOR const &> Constructor____const_lref { u8"!Constructor____const_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FVector4_const_lref,
			Constructor___float32__float32,
			Constructor___float32__float32__float32,
			Constructor___float32__float32__float32__float32,
			Constructor___FVector2_const_lref,
			Constructor___FVector2_const_lref__float32,
			Constructor___FVector2_const_lref__float32__float32,
			Constructor___FVector2_const_lref__FVector2_const_lref,
			Constructor___FVector3_const_lref,
			Constructor___FVector3_const_lref__float32,
			Constructor___TVec2__float32___const_lref,
			Constructor___TVec2__float32___const_lref__float32,
			Constructor___TVec2__float32___const_lref__float32__float32,
			Constructor___TVec2__float32___const_lref__TVec2__float32___const_lref,
			Constructor___TVec3__float32___const_lref,
			Constructor___TVec3__float32___const_lref__float32,
			Constructor___TVec4__float32___const_lref,
			Constructor___float32_const_lref_4,
			Constructor____const_lref,
		};

		// --- 1 Methods
		struct Method___FVector4_lref___Set__FVector4_const_lref
		{
			static inline const TMethod<FVector4, FVector4 &(FVector4 const &)> Method { u8"Set"N, &FVector4::Set, FMetadata::None };
		};
		struct Method___bool___HasNan____const
		{
			static inline const TMethod<FVector4, bool()> Method { u8"HasNan"N, &FVector4::HasNan, FMetadata::None };
		};
		struct Method___FVector4_lref___Normalize
		{
			static inline const TMethod<FVector4, FVector4 &()> Method { u8"Normalize"N, &FVector4::Normalize, FMetadata::None };
		};
		struct Method___float32___Length3____const
		{
			static inline const TMethod<FVector4, float32()> Method { u8"Length3"N, &FVector4::Length3, FMetadata::None };
		};
		struct Method___float32___Length3Square____const
		{
			static inline const TMethod<FVector4, float32()> Method { u8"Length3Square"N, &FVector4::Length3Square, FMetadata::None };
		};
		struct Method___float32___Length____const
		{
			static inline const TMethod<FVector4, float32()> Method { u8"Length"N, &FVector4::Length, FMetadata::None };
		};
		struct Method___float32___LengthSquare____const
		{
			static inline const TMethod<FVector4, float32()> Method { u8"LengthSquare"N, &FVector4::LengthSquare, FMetadata::None };
		};
		struct Method___float32___Dot__FVector4_const_lref____const
		{
			static inline const TMethod<FVector4, float32(FVector4 const &)> Method { u8"Dot"N, &FVector4::Dot, FMetadata::None };
		};
		struct Method___FVector4___Cross__FVector4_const_lref__FVector4_const_lref____const
		{
			static inline const TMethod<FVector4, FVector4(FVector4 const &, FVector4 const &)> Method { u8"Cross"N, &FVector4::Cross, FMetadata::None };
		};
		struct Method___FVector4___Abs____const
		{
			static inline const TMethod<FVector4, FVector4()> Method { u8"Abs"N, &FVector4::Abs, FMetadata::None };
		};
		struct Method___FVector4___Negate____const
		{
			static inline const TMethod<FVector4, FVector4()> Method { u8"Negate"N, &FVector4::Negate, FMetadata::None };
		};
		struct Method___FVector4___Reciprocal____const
		{
			static inline const TMethod<FVector4, FVector4()> Method { u8"Reciprocal"N, &FVector4::Reciprocal, FMetadata::None };
		};
		struct Method___FVector4___Add__FVector4_const_lref____const
		{
			static inline const TMethod<FVector4, FVector4(FVector4 const &)> Method { u8"Add"N, &FVector4::Add, FMetadata::None };
		};
		struct Method___FVector4___Subtract__FVector4_const_lref____const
		{
			static inline const TMethod<FVector4, FVector4(FVector4 const &)> Method { u8"Subtract"N, &FVector4::Subtract, FMetadata::None };
		};
		struct Method___FVector4___Multiply__FVector4_const_lref____const
		{
			static inline const TMethod<FVector4, FVector4(FVector4 const &)> Method { u8"Multiply"N, &FVector4::Multiply, FMetadata::None };
		};
		struct Method___FVector4___Divide__FVector4_const_lref____const
		{
			static inline const TMethod<FVector4, FVector4(FVector4 const &)> Method { u8"Divide"N, &FVector4::Divide, FMetadata::None };
		};
		struct Method___FVector4___MultiplyAdd__FVector4_const_lref__FVector4_const_lref____const
		{
			static inline const TMethod<FVector4, FVector4(FVector4 const &, FVector4 const &)> Method { u8"MultiplyAdd"N, &FVector4::MultiplyAdd, FMetadata::None };
		};
		struct Method___FVector4_lref___Set___const_lref
		{
			static inline const TMethod<FVector4, FVector4 &(DirectX::XMVECTOR const &)> Method { u8"Set"N, &FVector4::Set, FMetadata::None };
		};
		struct Method______ToXMVector____const
		{
			static inline const TMethod<FVector4, DirectX::XMVECTOR()> Method { u8"ToXMVector"N, &FVector4::ToXMVector, FMetadata::None };
		};
		struct Method____lref___AsXMVector
		{
			static inline const TMethod<FVector4, DirectX::XMVECTOR &()> Method { u8"AsXMVector"N, &FVector4::AsXMVector, FMetadata::None };
		};
		struct Method____const_lref___AsXMVector____const
		{
			static inline const TMethod<FVector4, DirectX::XMVECTOR const &()> Method { u8"AsXMVector"N, &FVector4::AsXMVector, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FVector4_lref___Set__FVector4_const_lref::Method,
			Method___bool___HasNan____const::Method,
			Method___FVector4_lref___Normalize::Method,
			Method___float32___Length3____const::Method,
			Method___float32___Length3Square____const::Method,
			Method___float32___Length____const::Method,
			Method___float32___LengthSquare____const::Method,
			Method___float32___Dot__FVector4_const_lref____const::Method,
			Method___FVector4___Cross__FVector4_const_lref__FVector4_const_lref____const::Method,
			Method___FVector4___Abs____const::Method,
			Method___FVector4___Negate____const::Method,
			Method___FVector4___Reciprocal____const::Method,
			Method___FVector4___Add__FVector4_const_lref____const::Method,
			Method___FVector4___Subtract__FVector4_const_lref____const::Method,
			Method___FVector4___Multiply__FVector4_const_lref____const::Method,
			Method___FVector4___Divide__FVector4_const_lref____const::Method,
			Method___FVector4___MultiplyAdd__FVector4_const_lref__FVector4_const_lref____const::Method,
			Method___FVector4_lref___Set___const_lref::Method,
			Method______ToXMVector____const::Method,
			Method____lref___AsXMVector::Method,
			Method____const_lref___AsXMVector____const::Method,
		};

		// --- 2 Fields
		struct Field___X
		{
			static inline const TField Field { u8"X"N, &FVector4::X, FMetadata::None };
		};
		struct Field___Y
		{
			static inline const TField Field { u8"Y"N, &FVector4::Y, FMetadata::None };
		};
		struct Field___Z
		{
			static inline const TField Field { u8"Z"N, &FVector4::Z, FMetadata::None };
		};
		struct Field___W
		{
			static inline const TField Field { u8"W"N, &FVector4::W, FMetadata::None };
		};
		struct Field___XYZ
		{
			static inline const TField Field { u8"XYZ"N, &FVector4::XYZ, FMetadata::None };
		};
		struct Field___XY
		{
			static inline const TField Field { u8"XY"N, &FVector4::XY, FMetadata::None };
		};
		struct Field___ZW
		{
			static inline const TField Field { u8"ZW"N, &FVector4::ZW, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___X::Field,
			Field___Y::Field,
			Field___Z::Field,
			Field___W::Field,
			Field___XYZ::Field,
			Field___XY::Field,
			Field___ZW::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		FVector4_Type(FName Name = u8"FVector4"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FVector4>
	{
		static FType & Type()
		{
			static FVector4_Type FVector4Type { u8"FVector4"N };
			return FVector4Type;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<FVector4>()
	{
		return TStaticType<FVector4>().Type();
	}

	static TTypeRegister<FVector4> FVector4TypeRegister { TStaticType<FVector4>().Type() };
}

// Generated code end.
