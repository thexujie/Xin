// A7142EB0153E474DB8464B356758B4A0 2024-12-25 11:51:24
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Vec3.h"

namespace Xin
{
	template<typename T>
	class TVec3_Type : public TClass<TVec3<T>>
	{
	public:
		using TVec3 = TVec3<T>;

		// --- 0 Constructors
		static inline const TConstructor<TVec3> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<TVec3, TVec3 const &> Constructor___TVec3_const_lref { u8"!Constructor___TVec3_const_lref"N, FMetadata::None };
		static inline const TConstructor<TVec3, T, T> Constructor___T__T { u8"!Constructor___T__T"N, FMetadata::None };
		static inline const TConstructor<TVec3, T, T, T> Constructor___T__T__T { u8"!Constructor___T__T__T"N, FMetadata::None };
		static inline const TConstructor<TVec3, TVec2<T> const &> Constructor___TVec2__T___const_lref { u8"!Constructor___TVec2__T___const_lref"N, FMetadata::None };
		static inline const TConstructor<TVec3, TVec2<T> const &, T> Constructor___TVec2__T___const_lref__T { u8"!Constructor___TVec2__T___const_lref__T"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___TVec3_const_lref,
			Constructor___T__T,
			Constructor___T__T__T,
			Constructor___TVec2__T___const_lref,
			Constructor___TVec2__T___const_lref__T,
		};

		// --- 1 Methods
		struct Method___void___Clear
		{
			static inline const TMethod<TVec3, void()> Method { u8"Clear"N, &TVec3::Clear, FMetadata::None };
		};
		struct Method___bool___IsZero____const
		{
			static inline const TMethod<TVec3, bool()> Method { u8"IsZero"N, &TVec3::IsZero, FMetadata::None };
		};
		struct Method___bool___IsEmpty____const
		{
			static inline const TMethod<TVec3, bool()> Method { u8"IsEmpty"N, &TVec3::IsEmpty, FMetadata::None };
		};
		struct Method___bool___AllNan____const
		{
			static inline const TMethod<TVec3, bool()> Method { u8"AllNan"N, &TVec3::AllNan, FMetadata::None };
		};
		struct Method___T___LengthSquare____const
		{
			static inline const TMethod<TVec3, T()> Method { u8"LengthSquare"N, &TVec3::LengthSquare, FMetadata::None };
		};
		struct Method___T___Length____const
		{
			static inline const TMethod<TVec3, T()> Method { u8"Length"N, &TVec3::Length, FMetadata::None };
		};
		struct Method___TVec3_lref___Normalize
		{
			static inline const TMethod<TVec3, TVec3 &()> Method { u8"Normalize"N, &TVec3::Normalize, FMetadata::None };
		};
		struct Method___TVec3___Normalized____const
		{
			static inline const TMethod<TVec3, TVec3()> Method { u8"Normalized"N, &TVec3::Normalized, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___void___Clear::Method,
			Method___bool___IsZero____const::Method,
			Method___bool___IsEmpty____const::Method,
			Method___bool___AllNan____const::Method,
			Method___T___LengthSquare____const::Method,
			Method___T___Length____const::Method,
			Method___TVec3_lref___Normalize::Method,
			Method___TVec3___Normalized____const::Method,
		};

		// --- 2 Fields
		struct Field___X
		{
			static inline const TField Field { u8"X"N, &TVec3::X, FMetadata::None };
		};
		struct Field___Y
		{
			static inline const TField Field { u8"Y"N, &TVec3::Y, FMetadata::None };
		};
		struct Field___Z
		{
			static inline const TField Field { u8"Z"N, &TVec3::Z, FMetadata::None };
		};
		struct Field___Width
		{
			static inline const TField Field { u8"Width"N, &TVec3::Width, FMetadata::None };
		};
		struct Field___Height
		{
			static inline const TField Field { u8"Height"N, &TVec3::Height, FMetadata::None };
		};
		struct Field___Depth
		{
			static inline const TField Field { u8"Depth"N, &TVec3::Depth, FMetadata::None };
		};
		struct Field___R
		{
			static inline const TField Field { u8"R"N, &TVec3::R, FMetadata::None };
		};
		struct Field___G
		{
			static inline const TField Field { u8"G"N, &TVec3::G, FMetadata::None };
		};
		struct Field___B
		{
			static inline const TField Field { u8"B"N, &TVec3::B, FMetadata::None };
		};
		struct Field___XY
		{
			static inline const TField Field { u8"XY"N, &TVec3::XY, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___X::Field,
			Field___Y::Field,
			Field___Z::Field,
			Field___Width::Field,
			Field___Height::Field,
			Field___Depth::Field,
			Field___R::Field,
			Field___G::Field,
			Field___B::Field,
			Field___XY::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		TVec3_Type(FName Name = u8"TVec3"N) : TClass<TVec3>(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<typename T>
	struct TStaticType<TVec3<T>>
	{
		static FType & Type()
		{
			static TVec3_Type<T> TVec3Type { u8"TVec3"N };
			return TVec3Type;
		}
	};
}

// Generated code end.
