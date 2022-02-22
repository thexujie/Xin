// A739BAE484F3129D007FA927187337C2 2024-12-25 11:51:25
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Vec4.h"

namespace Xin
{
	template<typename T>
	class TVec4_Type : public TClass<TVec4<T>>
	{
	public:
		using TVec4 = TVec4<T>;

		// --- 0 Constructors
		static inline const TConstructor<TVec4> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<TVec4, TVec4 const &> Constructor___TVec4_const_lref { u8"!Constructor___TVec4_const_lref"N, FMetadata::None };
		static inline const TConstructor<TVec4, T, T, T, T> Constructor___T__T__T__T { u8"!Constructor___T__T__T__T"N, FMetadata::None };
		static inline const TConstructor<TVec4, TVec2<T> const &, T> Constructor___TVec2__T___const_lref__T { u8"!Constructor___TVec2__T___const_lref__T"N, FMetadata::None };
		static inline const TConstructor<TVec4, TVec2<T> const &, T, T> Constructor___TVec2__T___const_lref__T__T { u8"!Constructor___TVec2__T___const_lref__T__T"N, FMetadata::None };
		static inline const TConstructor<TVec4, TVec3<T> const &> Constructor___TVec3__T___const_lref { u8"!Constructor___TVec3__T___const_lref"N, FMetadata::None };
		static inline const TConstructor<TVec4, TVec3<T> const &, T> Constructor___TVec3__T___const_lref__T { u8"!Constructor___TVec3__T___const_lref__T"N, FMetadata::None };
		static inline const TConstructor<TVec4, TVec2<T> const &, TVec2<T> const &> Constructor___TVec2__T___const_lref__TVec2__T___const_lref { u8"!Constructor___TVec2__T___const_lref__TVec2__T___const_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___TVec4_const_lref,
			Constructor___T__T__T__T,
			Constructor___TVec2__T___const_lref__T,
			Constructor___TVec2__T___const_lref__T__T,
			Constructor___TVec3__T___const_lref,
			Constructor___TVec3__T___const_lref__T,
			Constructor___TVec2__T___const_lref__TVec2__T___const_lref,
		};

		// --- 1 Methods
		struct Method___void___Clear
		{
			static inline const TMethod<TVec4, void()> Method { u8"Clear"N, &TVec4::Clear, FMetadata::None };
		};
		struct Method___bool___AnyZero____const
		{
			static inline const TMethod<TVec4, bool()> Method { u8"AnyZero"N, &TVec4::AnyZero, FMetadata::None };
		};
		struct Method___bool___AllZero____const
		{
			static inline const TMethod<TVec4, bool()> Method { u8"AllZero"N, &TVec4::AllZero, FMetadata::None };
		};
		struct Method___bool___Empty____const
		{
			static inline const TMethod<TVec4, bool()> Method { u8"Empty"N, &TVec4::Empty, FMetadata::None };
		};
		struct Method___bool___AnyNaN____const
		{
			static inline const TMethod<TVec4, bool()> Method { u8"AnyNaN"N, &TVec4::AnyNaN, FMetadata::None };
		};
		struct Method___bool___AllNaN____const
		{
			static inline const TMethod<TVec4, bool()> Method { u8"AllNaN"N, &TVec4::AllNaN, FMetadata::None };
		};
		struct Method___T___Length____const
		{
			static inline const TMethod<TVec4, T()> Method { u8"Length"N, &TVec4::Length, FMetadata::None };
		};
		struct Method___T___LengthSquare____const
		{
			static inline const TMethod<TVec4, T()> Method { u8"LengthSquare"N, &TVec4::LengthSquare, FMetadata::None };
		};
		struct Method___bool___Contains__TVec2__T___const_lref____const
		{
			static inline const TMethod<TVec4, bool(TVec2<T> const &)> Method { u8"Contains"N, &TVec4::Contains, FMetadata::None };
		};
		struct Method___bool___IntersectsWith__TVec4_const_lref____const
		{
			static inline const TMethod<TVec4, bool(TVec4 const &)> Method { u8"IntersectsWith"N, &TVec4::IntersectsWith, FMetadata::None };
		};
		struct Method___TVec4___Intersect__TVec4_const_lref____const
		{
			static inline const TMethod<TVec4, TVec4(TVec4 const &)> Method { u8"Intersect"N, &TVec4::Intersect, FMetadata::None };
		};
		struct Method___TVec4___Offset__TVec2__T___const_lref____const
		{
			static inline const TMethod<TVec4, TVec4(TVec2<T> const &)> Method { u8"Offset"N, &TVec4::Offset, FMetadata::None };
		};
		struct Method___TVec4_lref___Normalize
		{
			static inline const TMethod<TVec4, TVec4 &()> Method { u8"Normalize"N, &TVec4::Normalize, FMetadata::None };
		};
		struct Method___TVec4___Normalized____const
		{
			static inline const TMethod<TVec4, TVec4()> Method { u8"Normalized"N, &TVec4::Normalized, FMetadata::None };
		};
		struct Method___T___GetRight____const
		{
			static inline const TMethod<TVec4, T()> Method { u8"GetRight"N, &TVec4::GetRight, FMetadata::None };
		};
		struct Method___void___SetRight__T
		{
			static inline const TMethod<TVec4, void(T)> Method { u8"SetRight"N, &TVec4::SetRight, FMetadata::None };
		};
		struct Method___T___GetBottom____const
		{
			static inline const TMethod<TVec4, T()> Method { u8"GetBottom"N, &TVec4::GetBottom, FMetadata::None };
		};
		struct Method___void___SetBottom__T
		{
			static inline const TMethod<TVec4, void(T)> Method { u8"SetBottom"N, &TVec4::SetBottom, FMetadata::None };
		};
		struct Method___TVec2__T_____GetLeftBottom____const
		{
			static inline const TMethod<TVec4, TVec2<T>()> Method { u8"GetLeftBottom"N, &TVec4::GetLeftBottom, FMetadata::None };
		};
		struct Method___TVec2__T_____GetRightTop____const
		{
			static inline const TMethod<TVec4, TVec2<T>()> Method { u8"GetRightTop"N, &TVec4::GetRightTop, FMetadata::None };
		};
		struct Method___TVec2__T_____GetRightBottom____const
		{
			static inline const TMethod<TVec4, TVec2<T>()> Method { u8"GetRightBottom"N, &TVec4::GetRightBottom, FMetadata::None };
		};
		struct Method___TVec2__T_____GetLeftCenter____const
		{
			static inline const TMethod<TVec4, TVec2<T>()> Method { u8"GetLeftCenter"N, &TVec4::GetLeftCenter, FMetadata::None };
		};
		struct Method___TVec2__T_____GetTopCenter____const
		{
			static inline const TMethod<TVec4, TVec2<T>()> Method { u8"GetTopCenter"N, &TVec4::GetTopCenter, FMetadata::None };
		};
		struct Method___TVec2__T_____GetRightCenter____const
		{
			static inline const TMethod<TVec4, TVec2<T>()> Method { u8"GetRightCenter"N, &TVec4::GetRightCenter, FMetadata::None };
		};
		struct Method___TVec2__T_____GetBottomCenter____const
		{
			static inline const TMethod<TVec4, TVec2<T>()> Method { u8"GetBottomCenter"N, &TVec4::GetBottomCenter, FMetadata::None };
		};
		struct Method___TVec2__T_____GetCenter____const
		{
			static inline const TMethod<TVec4, TVec2<T>()> Method { u8"GetCenter"N, &TVec4::GetCenter, FMetadata::None };
		};
		struct Method___TVec2__T_____GetRadius____const
		{
			static inline const TMethod<TVec4, TVec2<T>()> Method { u8"GetRadius"N, &TVec4::GetRadius, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___void___Clear::Method,
			Method___bool___AnyZero____const::Method,
			Method___bool___AllZero____const::Method,
			Method___bool___Empty____const::Method,
			Method___bool___AnyNaN____const::Method,
			Method___bool___AllNaN____const::Method,
			Method___T___Length____const::Method,
			Method___T___LengthSquare____const::Method,
			Method___bool___Contains__TVec2__T___const_lref____const::Method,
			Method___bool___IntersectsWith__TVec4_const_lref____const::Method,
			Method___TVec4___Intersect__TVec4_const_lref____const::Method,
			Method___TVec4___Offset__TVec2__T___const_lref____const::Method,
			Method___TVec4_lref___Normalize::Method,
			Method___TVec4___Normalized____const::Method,
			Method___T___GetRight____const::Method,
			Method___void___SetRight__T::Method,
			Method___T___GetBottom____const::Method,
			Method___void___SetBottom__T::Method,
			Method___TVec2__T_____GetLeftBottom____const::Method,
			Method___TVec2__T_____GetRightTop____const::Method,
			Method___TVec2__T_____GetRightBottom____const::Method,
			Method___TVec2__T_____GetLeftCenter____const::Method,
			Method___TVec2__T_____GetTopCenter____const::Method,
			Method___TVec2__T_____GetRightCenter____const::Method,
			Method___TVec2__T_____GetBottomCenter____const::Method,
			Method___TVec2__T_____GetCenter____const::Method,
			Method___TVec2__T_____GetRadius____const::Method,
		};

		// --- 2 Fields
		struct Field___X
		{
			static inline const TField Field { u8"X"N, &TVec4::X, FMetadata::None };
		};
		struct Field___Y
		{
			static inline const TField Field { u8"Y"N, &TVec4::Y, FMetadata::None };
		};
		struct Field___Z
		{
			static inline const TField Field { u8"Z"N, &TVec4::Z, FMetadata::None };
		};
		struct Field___W
		{
			static inline const TField Field { u8"W"N, &TVec4::W, FMetadata::None };
		};
		struct Field___Left
		{
			static inline const TField Field { u8"Left"N, &TVec4::Left, FMetadata::None };
		};
		struct Field___Top
		{
			static inline const TField Field { u8"Top"N, &TVec4::Top, FMetadata::None };
		};
		struct Field___LeftTop
		{
			static inline const TField Field { u8"LeftTop"N, &TVec4::LeftTop, FMetadata::None };
		};
		struct Field___Position
		{
			static inline const TField Field { u8"Position"N, &TVec4::Position, FMetadata::None };
		};
		struct Field___Width
		{
			static inline const TField Field { u8"Width"N, &TVec4::Width, FMetadata::None };
		};
		struct Field___Height
		{
			static inline const TField Field { u8"Height"N, &TVec4::Height, FMetadata::None };
		};
		struct Field___Size
		{
			static inline const TField Field { u8"Size"N, &TVec4::Size, FMetadata::None };
		};
		struct Field___XY
		{
			static inline const TField Field { u8"XY"N, &TVec4::XY, FMetadata::None };
		};
		struct Field___ZW
		{
			static inline const TField Field { u8"ZW"N, &TVec4::ZW, FMetadata::None };
		};
		struct Field___XYZ
		{
			static inline const TField Field { u8"XYZ"N, &TVec4::XYZ, FMetadata::None };
		};
		struct Field___R
		{
			static inline const TField Field { u8"R"N, &TVec4::R, FMetadata::None };
		};
		struct Field___G
		{
			static inline const TField Field { u8"G"N, &TVec4::G, FMetadata::None };
		};
		struct Field___B
		{
			static inline const TField Field { u8"B"N, &TVec4::B, FMetadata::None };
		};
		struct Field___A
		{
			static inline const TField Field { u8"A"N, &TVec4::A, FMetadata::None };
		};
		struct Field___RG
		{
			static inline const TField Field { u8"RG"N, &TVec4::RG, FMetadata::None };
		};
		struct Field___RGB
		{
			static inline const TField Field { u8"RGB"N, &TVec4::RGB, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___X::Field,
			Field___Y::Field,
			Field___Z::Field,
			Field___W::Field,
			Field___Left::Field,
			Field___Top::Field,
			Field___LeftTop::Field,
			Field___Position::Field,
			Field___Width::Field,
			Field___Height::Field,
			Field___Size::Field,
			Field___XY::Field,
			Field___ZW::Field,
			Field___XYZ::Field,
			Field___R::Field,
			Field___G::Field,
			Field___B::Field,
			Field___A::Field,
			Field___RG::Field,
			Field___RGB::Field,
		};

		// --- 3 Properties
		struct Property___Right
		{
			static inline const TFunctorProperty Property { u8"Right"N, &TVec4::GetRight, &TVec4::SetRight, FMetadata::None };
		};
		struct Property___Bottom
		{
			static inline const TFunctorProperty Property { u8"Bottom"N, &TVec4::GetBottom, &TVec4::SetBottom, FMetadata::None };
		};
		struct Property___LeftBottom
		{
			static inline const TFunctorProperty Property { u8"LeftBottom"N, &TVec4::GetLeftBottom, nullptr, FMetadata::None };
		};
		struct Property___RightTop
		{
			static inline const TFunctorProperty Property { u8"RightTop"N, &TVec4::GetRightTop, nullptr, FMetadata::None };
		};
		struct Property___RightBottom
		{
			static inline const TFunctorProperty Property { u8"RightBottom"N, &TVec4::GetRightBottom, nullptr, FMetadata::None };
		};
		struct Property___LeftCenter
		{
			static inline const TFunctorProperty Property { u8"LeftCenter"N, &TVec4::GetLeftCenter, nullptr, FMetadata::None };
		};
		struct Property___TopCenter
		{
			static inline const TFunctorProperty Property { u8"TopCenter"N, &TVec4::GetTopCenter, nullptr, FMetadata::None };
		};
		struct Property___RightCenter
		{
			static inline const TFunctorProperty Property { u8"RightCenter"N, &TVec4::GetRightCenter, nullptr, FMetadata::None };
		};
		struct Property___BottomCenter
		{
			static inline const TFunctorProperty Property { u8"BottomCenter"N, &TVec4::GetBottomCenter, nullptr, FMetadata::None };
		};
		struct Property___Center
		{
			static inline const TFunctorProperty Property { u8"Center"N, &TVec4::GetCenter, nullptr, FMetadata::None };
		};
		struct Property___Radius
		{
			static inline const TFunctorProperty Property { u8"Radius"N, &TVec4::GetCenter, nullptr, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Right::Property,
			Property___Bottom::Property,
			Property___LeftBottom::Property,
			Property___RightTop::Property,
			Property___RightBottom::Property,
			Property___LeftCenter::Property,
			Property___TopCenter::Property,
			Property___RightCenter::Property,
			Property___BottomCenter::Property,
			Property___Center::Property,
			Property___Radius::Property,
		};

		// --- 4 Base Types

		// --- 5 Constructor
		TVec4_Type(FName Name = u8"TVec4"N) : TClass<TVec4>(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
		}
	};

	template<typename T>
	struct TStaticType<TVec4<T>>
	{
		static FType & Type()
		{
			static TVec4_Type<T> TVec4Type { u8"TVec4"N };
			return TVec4Type;
		}
	};
}

// Generated code end.
