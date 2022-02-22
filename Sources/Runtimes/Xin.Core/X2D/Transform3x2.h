#pragma once

#include "X2D.Types.h"

namespace Xin
{
	template<typename T>
	struct TTransform3x2
	{
	public:
		TTransform3x2() : Row0(1, 0), Row1(0, 1), Trans(0, 0) {}
		TTransform3x2(const TTransform3x2 &) noexcept = default;
		TTransform3x2 & operator =(const TTransform3x2 &) = default;
		~TTransform3x2() = default;
		bool operator ==(const TTransform3x2 & Other) const { return Equal(Row0, Other.Row0, Row1, Other.Row1, Trans, Other.Trans); }
		auto operator <=>(const TTransform3x2 & Other) const { return Compare(Row0, Other.Row0, Row1, Other.Row1, Trans, Other.Trans); }

		TTransform3x2(const TVec2<T> & Translation) : Row0(1, 0), Row1(0, 1), Trans(Translation) {}
		TTransform3x2(const T & Scale, const TVec2<T> & Translation) : Row0(Scale, 0), Row1(0, Scale), Trans(Translation) {}
		TTransform3x2(const TVec2<T> & Scale, const TVec2<T> & Translation) : Row0(Scale.X, 0), Row1(0, Scale.Y), Trans(Translation) {}

		TTransform3x2(float32 M11, float32 M12, float32 M21, float32 M22, float32 OffsetX, float32 OffsetY)
			: M11(M11), M12(M12), M21(M21), M22(M22), M31(OffsetX), M32(OffsetY) { }

		TTransform3x2(TVec2<T> Row0, TVec2<T> Row1, TVec2<T> Trans) : Row0(Row0), Row1(Row1), Trans(Trans) { }

	public:
		TVec2<T> GetScale() const { return { Sx, Sy }; }
		TVec2<T> & SetScale(TVec2<T> S) const { Sx = S.X; Sy = S.Y; return *this; }
		PropertyRW(TVec2<T>, GetScale, SetScale) Scale;

		TVec2<T> GetSkew() const { return { Kx, Ky }; }
		TVec2<T> & SetSkew(TVec2<T> K) const { Kx = K.X; Ky = K.Y; return *this; }
		PropertyRW(TVec2<T>, GetSkew, SetSkew) Skew;

		bool IsIdentity() const { return Equal(Row0, TVec2<T>::XAxis, Row1, TVec2<T>::YAxis, Trans, TVec2<T>::Zero); }

		TVec2<T> TransformCoord(TVec2<T> Point) const
		{
			return { Point.X * Sx + Point.Y * Kx + Tx, Point.Y * Sy + Point.X * Ky + Ty };
		}

	public:
		union
		{
			T Data[6];

			struct
			{
				TVec2<T> Rows[3];
			};

			struct
			{
				TVec2<T> Row0;
				TVec2<T> Row1;
				TVec2<T> Trans;
			};

			struct
			{
				float32 Sx, Kx;
				float32 Ky, Sy;
				float32 Tx, Ty;
			};

			struct
			{
				float32 M11, M12;
				float32 M21, M22;
				float32 M31, M32;
			};
		};

	public:
		static const TTransform3x2 Identity;
	};

	template<typename T>
	const TTransform3x2<T> TTransform3x2<T>::Identity { TVec2<T>::XAxis, TVec2<T>::YAxis, TVec2<T>::Zero };

	using FTransform3x2F = TTransform3x2<float32>;
	//using FTransform3x2D = TTransform2D<float64>;
}
