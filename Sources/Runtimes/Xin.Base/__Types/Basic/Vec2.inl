// D0119CB17C16DFFE1B5D0F48CE4B6640 2024-12-25 11:51:23
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Vec2.h"

namespace Xin
{
	template<typename T>
	class TVec2_Type : public TClass<TVec2<T>>
	{
	public:
		using TVec2 = TVec2<T>;

		// --- 0 Constructors
		static inline const TConstructor<TVec2> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<TVec2, TVec2 const &> Constructor___TVec2_const_lref { u8"!Constructor___TVec2_const_lref"N, FMetadata::None };
		static inline const TConstructor<TVec2, T, T> Constructor___T__T { u8"!Constructor___T__T"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___TVec2_const_lref,
			Constructor___T__T,
		};

		// --- 1 Methods
		struct Method___void___Clear
		{
			static inline const TMethod<TVec2, void()> Method { u8"Clear"N, &TVec2::Clear, FMetadata::None };
		};
		struct Method___bool___AnyZero____const
		{
			static inline const TMethod<TVec2, bool()> Method { u8"AnyZero"N, &TVec2::AnyZero, FMetadata::None };
		};
		struct Method___bool___AllZero____const
		{
			static inline const TMethod<TVec2, bool()> Method { u8"AllZero"N, &TVec2::AllZero, FMetadata::None };
		};
		struct Method___bool___Empty____const
		{
			static inline const TMethod<TVec2, bool()> Method { u8"Empty"N, &TVec2::Empty, FMetadata::None };
		};
		struct Method___bool___AnyNaN____const
		{
			static inline const TMethod<TVec2, bool()> Method { u8"AnyNaN"N, &TVec2::AnyNaN, FMetadata::None };
		};
		struct Method___bool___AllNaN____const
		{
			static inline const TMethod<TVec2, bool()> Method { u8"AllNaN"N, &TVec2::AllNaN, FMetadata::None };
		};
		struct Method___T___LengthSquare____const
		{
			static inline const TMethod<TVec2, T()> Method { u8"LengthSquare"N, &TVec2::LengthSquare, FMetadata::None };
		};
		struct Method___T___Length____const
		{
			static inline const TMethod<TVec2, T()> Method { u8"Length"N, &TVec2::Length, FMetadata::None };
		};
		struct Method___bool___Contains__TVec2_const_lref____const
		{
			static inline const TMethod<TVec2, bool(TVec2 const &)> Method { u8"Contains"N, &TVec2::Contains, FMetadata::None };
		};
		struct Method___TVec2_lref___Normalize
		{
			static inline const TMethod<TVec2, TVec2 &()> Method { u8"Normalize"N, &TVec2::Normalize, FMetadata::None };
		};
		struct Method___TVec2___Normalized____const
		{
			static inline const TMethod<TVec2, TVec2()> Method { u8"Normalized"N, &TVec2::Normalized, FMetadata::None };
		};
		struct Method___T___Dot__TVec2_const_lref____const
		{
			static inline const TMethod<TVec2, T(TVec2 const &)> Method { u8"Dot"N, &TVec2::Dot, FMetadata::None };
		};
		struct Method___T___Cross__TVec2_const_lref____const
		{
			static inline const TMethod<TVec2, T(TVec2 const &)> Method { u8"Cross"N, &TVec2::Cross, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___void___Clear::Method,
			Method___bool___AnyZero____const::Method,
			Method___bool___AllZero____const::Method,
			Method___bool___Empty____const::Method,
			Method___bool___AnyNaN____const::Method,
			Method___bool___AllNaN____const::Method,
			Method___T___LengthSquare____const::Method,
			Method___T___Length____const::Method,
			Method___bool___Contains__TVec2_const_lref____const::Method,
			Method___TVec2_lref___Normalize::Method,
			Method___TVec2___Normalized____const::Method,
			Method___T___Dot__TVec2_const_lref____const::Method,
			Method___T___Cross__TVec2_const_lref____const::Method,
		};

		// --- 2 Fields
		struct Field___X
		{
			static inline const TField Field { u8"X"N, &TVec2::X, FMetadata::None };
		};
		struct Field___Y
		{
			static inline const TField Field { u8"Y"N, &TVec2::Y, FMetadata::None };
		};
		struct Field___Left
		{
			static inline const TField Field { u8"Left"N, &TVec2::Left, FMetadata::None };
		};
		struct Field___Top
		{
			static inline const TField Field { u8"Top"N, &TVec2::Top, FMetadata::None };
		};
		struct Field___Width
		{
			static inline const TField Field { u8"Width"N, &TVec2::Width, FMetadata::None };
		};
		struct Field___Height
		{
			static inline const TField Field { u8"Height"N, &TVec2::Height, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___X::Field,
			Field___Y::Field,
			Field___Left::Field,
			Field___Top::Field,
			Field___Width::Field,
			Field___Height::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		TVec2_Type(FName Name = u8"TVec2"N) : TClass<TVec2>(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<typename T>
	struct TStaticType<TVec2<T>>
	{
		static FType & Type()
		{
			static TVec2_Type<T> TVec2Type { u8"TVec2"N };
			return TVec2Type;
		}
	};
}

// Generated code end.
