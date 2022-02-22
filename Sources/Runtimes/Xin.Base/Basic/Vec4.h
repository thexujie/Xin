#pragma once

#include "Basic.Types.h"
#include "Vec2.h"
#include "Vec3.h"

namespace Xin
{
	template<typename T>
	struct TVec4;

	template<typename T>
	inline constexpr bool IsVec4V = false;

	template<typename T>
	inline constexpr bool IsVec4V<TVec4<T>> = true;

	template<typename T>
	concept Vec4C = IsVec4V<T>;

	template<typename T>
	concept NoneVec4C = !IsVec4V<T>;

	template<typename T>
	struct [[Meta(SystemType)]] TVec4
	{
	public:
		constexpr TVec4() = default;
		constexpr TVec4(const TVec4 &) = default;
		constexpr TVec4 & operator =(const TVec4 &) = default;
		constexpr ~TVec4() = default;
		constexpr bool operator ==(const TVec4 & Other) const { return Equal(X, Other.X) && Equal(Y, Other.Y) && Equal(Z, Other.Z) && Equal(W, Other.W); }
		constexpr auto operator <=>(const TVec4 & Other) const noexcept { return Compare(X, Other.X, Y, Other.Y, Z, Other.Z, W, Other.W); }

		constexpr TVec4(T X, T Y, T Z, T W = 0) : X(X), Y(Y), Z(Z), W(W) {}
		constexpr TVec4(const TVec2<T> & Vec2, T Z) : X(Vec2.X), Y(Vec2.Y), Z(Z), W() { }
		constexpr TVec4(const TVec2<T> & Vec2, T Z, T W) : X(Vec2.X), Y(Vec2.Y), Z(Z), W(W) { }
		constexpr TVec4(const TVec3<T> & Vec3) : X(Vec3.X), Y(Vec3.Y), Z(Vec3.Z), W() { }
		constexpr TVec4(const TVec3<T> & Vec3, T W) : X(Vec3.X), Y(Vec3.Y), Z(Vec3.Z), W(W) { }
		constexpr TVec4(const TVec2<T> & XY, const TVec2<T> & ZW) : X(XY.X), Y(XY.Y), Z(ZW.X), W(ZW.Y) { }

		template<typename Ty> requires (ConvertibleToC<T, Ty> && !SameC<T, Ty>)
		explicit constexpr TVec4(const TVec4<Ty> & That) : X(static_cast<T>(That.X)), Y(static_cast<T>(That.Y)), Z(static_cast<T>(That.Z)), W(static_cast<T>(That.W)) {}

		void Clear()
		{
			X = 0;
			Y = 0;
			Z = 0;
			W = 0;
		}

		bool AnyZero() const { return X == 0 || Y == 0 || Z == 0 || W == 0; }
		bool AllZero() const { return X == 0 && Y == 0 && Z == 0 && W == 0; }
		bool Empty() const { return Size.Empty(); }

		bool AnyNaN() const
		{
			if constexpr (FloatingPointC<T>)
				return IsNaN(X) || IsNaN(Y) || IsNaN(Width) || IsNaN(Height);
			else
				return false;
		}

		bool AllNaN() const
		{
			if constexpr (FloatingPointC<T>)
				return IsNaN(X) && IsNaN(Y) && IsNaN(Width) && IsNaN(Height);
			else
				return false;
		}

		T Length() const { return (T)Sqrt(LengthSquare()); }
		T LengthSquare() const { return X * X + Y * Y + Z * Z + W * W; }

	public: // just for rect...
		bool Contains(const TVec2<T> & Point) const { return Point.X >= X && Point.Y >= Y && Point.X < X + Width && Point.Y < Y + Height; }

		bool IntersectsWith(const TVec4 & Other) const
		{
			if (Empty() || Other.Empty())
				return false;

			return Min(Right, Other.Right) > Max(X, Other.X) && Min(Bottom, Other.Bottom) > Max(Y, Other.Y);
		}

		TVec4 Intersect(const TVec4 & Other) const
		{
			if (Empty() || Other.Empty())
				return Zero;
			else
			{
				T MaxLeft = Max(X, Other.X);
				T MinRight = Min(Right, Other.Right);
				T MaxTop = Max(Y, Other.Y);
				T MinBottom = Min(Bottom, Other.Bottom);
				if (MinRight <= MaxLeft || MinBottom <= MaxTop)
					return Zero;
				else
					return { MaxLeft, MaxTop, T(MinRight - MaxLeft), T(MinBottom - MaxTop) };
			}
		}

		TVec4 Offset(const TVec2<T> & Delta) const { return { Position + Delta, Size }; }

	public:
		TVec4 & Normalize()
		{
			auto L = Length();
			X = T(X / L);
			Y = T(Y / L);
			Z = T(Z / L);
			W = T(W / L);
			return *this;
		}
		TVec4 Normalized() const
		{
			auto L = Sqrt(X * X + Y * Y + Z * Z + W * W);
			return { T(X / L), T(Y / L), T(Z / L), T(W / L) };
		}

	public:
		const T & operator [](int32 Index) const { return reinterpret_cast<const T *>(this)[Index % 4]; }
		T & operator [](int32 Index) { return reinterpret_cast<T *>(this)[Index % 4]; }

		TVec4 operator -() const requires IsSignedV<T> { return TVec4(-X, -Y, -Z, -W); }
		TVec4 operator +() const { return *this; }

		template<typename Ty>
		TVec4<IsAddableT<T, Ty>> operator +(const TVec4<Ty> & Other) const requires AddableC<T, Ty> { return { X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W }; }

		template<typename Ty>
		TVec4<IsSubtractableT<T, Ty>> operator -(const TVec4<Ty> & Other) const requires SubtractableC<T, Ty> { return { X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W }; }

		template<typename Ty>
		TVec4<IsMultiplicableT<T, Ty>> operator *(const TVec4<Ty> & Other) const requires MultiplicableC<T, Ty> { return { X * Other.X, Y * Other.Y, Z * Other.Z, W * Other.W }; }

		template<typename Ty>
		TVec4<IsDivisibleT<T, Ty>> operator /(const TVec4<Ty> & Other) const requires DivisibleC<T, Ty> { return { X / Other.X, Y / Other.Y, Z / Other.Z, W / Other.W }; }

		template<typename Ty>
		TVec4 & operator +=(const TVec4<Ty> & Other) requires AddableC<T, Ty>
		{
			X += Other.X;
			Y += Other.Y;
			Z += Other.Z;
			W += Other.W;
			return *this;
		}

		template<typename Ty>
		TVec4 & operator -=(const TVec4<Ty> & Other) requires SubtractableC<T, Ty>
		{
			X -= Other.X;
			Y -= Other.Y;
			Z -= Other.Z;
			W -= Other.W;
			return *this;
		}

		template<typename Ty>
		TVec4 & operator *=(const TVec4<Ty> & Other) requires MultiplicableC<T, Ty>
		{
			X *= Other.X;
			Y *= Other.Y;
			Z *= Other.Z;
			W *= Other.W;
			return *this;
		}

		template<typename Ty>
		TVec4 & operator /=(const TVec4<Ty> & Other) requires DivisibleC<T, Ty>
		{
			X /= Other.X;
			Y /= Other.Y;
			Z /= Other.Z;
			W /= Other.W;
			return *this;
		}

		template<NoneVec4C Ty>
		TVec4<IsAddableT<T, Ty>> operator +(const Ty & Value) const requires AddableC<T, Ty> { return { X + Value, Y + Value, Z + Value, W + Value }; }

		template<NoneVec4C Ty>
		TVec4<IsSubtractableT<T, Ty>> operator -(const Ty & Value) const requires SubtractableC<T, Ty> { return { X - Value, Y - Value, Z - Value, W - Value }; }

		template<NoneVec4C Ty>
		TVec4<IsMultiplicableT<T, Ty>> operator *(const Ty & Value) const requires MultiplicableC<T, Ty> { return { X * Value, Y * Value, Z * Value, W * Value }; }

		template<NoneVec4C Ty>
		TVec4<IsDivisibleT<T, Ty>> operator /(const Ty & Value) const requires DivisibleC<T, Ty> { return { X / Value, Y / Value, Z / Value, W / Value }; }

		template<NoneVec4C Ty>
		TVec4 & operator +=(const Ty & Value) requires AddableC<T, Ty>
		{
			X += Value;
			Y += Value;
			Z += Value;
			W += Value;
			return *this;
		}

		template<NoneVec4C Ty>
		TVec4 & operator -=(const Ty & Value) requires SubtractableC<T, Ty>
		{
			X -= Value;
			Y -= Value;
			Z -= Value;
			W -= Value;
			return *this;
		}

		template<NoneVec4C Ty>
		TVec4 & operator *=(const Ty & Value) requires MultiplicableC<T, Ty>
		{
			X *= Value;
			Y *= Value;
			Z *= Value;
			W *= Value;
			return *this;
		}

		template<NoneVec4C Ty>
		TVec4 & operator /=(const Ty & Value) requires DivisibleC<T, Ty>
		{
			X /= Value;
			Y /= Value;
			Z /= Value;
			W /= Value;
			return *this;
		}

		template<typename = void>
		TVec4 operator |(T Value) const requires ArithmeticOrAbleC<T> { return TVec4 { X | Value, Y | Value, Z | Value, W | Value }; }

		template<typename = void>
		TVec4 operator &(T Value) const requires ArithmeticAndAbleC<T> { return TVec4 { X & Value, Y & Value, Z & Value, W & Value }; }

		template<typename = void>
		TVec4 operator ^(T Value) const requires ArithmeticXorAbleC<T> { return TVec4 { X ^ Value, Y ^ Value, Z ^ Value, W ^ Value }; }

		template<typename = void>
		TVec4 & operator |=(int32 Shift) requires ArithmeticOrAbleC<T>
		{
			X |= Shift;
			Y |= Shift;
			Z |= Shift;
			W |= Shift;
			return *this;
		}

		template<typename = void>
		TVec4 & operator &=(int32 Shift) requires ArithmeticAndAbleC<T>
		{
			X &= Shift;
			Y &= Shift;
			Z &= Shift;
			W &= Shift;
			return *this;
		}

		template<typename = void>
		TVec4 & operator ^=(int32 Shift) requires ArithmeticXorAbleC<T>
		{
			X ^= Shift;
			Y ^= Shift;
			Z ^= Shift;
			W ^= Shift;
			return *this;
		}

		template<typename = void>
		TVec4 operator >>(int32 Shift) const requires ArithmeticShiftableC<T> { return TVec4 { X >> Shift, Y >> Shift, Z >> Shift, W >> Shift }; }

		template<typename = void>
		TVec4 operator <<(int32 Shift) const requires ArithmeticShiftableC<T> { return TVec4 { X << Shift, Y << Shift, Z << Shift, W << Shift }; }

		template<typename = void>
		TVec4 & operator >>=(int32 Shift) requires ArithmeticShiftableC<T>
		{
			X >>= Shift;
			Y >>= Shift;
			Z >>= Shift;
			W >>= Shift;
			return *this;
		}

		template<typename = void>
		TVec4 & operator <<=(int32 Shift) requires ArithmeticShiftableC<T>
		{
			X <<= Shift;
			Y <<= Shift;
			Z <<= Shift;
			W <<= Shift;
			return *this;
		}

		template<typename = void>
		TVec4 operator ~() const requires ArithmeticNotAbleC<T> { return { ~X, ~Y, ~Z, ~W }; }

		template<typename = void>
		TVec4 operator |(const TVec4 & Other) const requires ArithmeticOrAbleC<T> { return TVec4 { X | Other.X, Y | Other.Y, Z | Other.Z, W | Other.W }; }

		template<typename = void>
		TVec4 operator &(const TVec4 & Other) const requires ArithmeticAndAbleC<T> { return TVec4 { X & Other.X, Y & Other.Y, Z & Other.Z, W & Other.W }; }

		template<typename = void>
		TVec4 operator ^(const TVec4 & Other) const requires ArithmeticXorAbleC<T> { return TVec4 { X ^ Other.X, Y ^ Other.Y, Z ^ Other.Z, W ^ Other.W }; }

		template<typename = void>
		TVec4 & operator |=(const TVec4 & Other) requires ArithmeticOrAbleC<T>
		{
			X |= Other.X;
			Y |= Other.Y;
			Z |= Other.Z;
			W |= Other.W;
			return *this;
		}

		template<typename = void>
		TVec4 & operator &=(const TVec4 & Other) requires ArithmeticAndAbleC<T>
		{
			X &= Other.X;
			Y &= Other.Y;
			Z &= Other.Z;
			W &= Other.W;
			return *this;
		}

		template<typename = void>
		TVec4 & operator ^=(const TVec4 & Other) requires ArithmeticXorAbleC<T>
		{
			X ^= Other.X;
			Y ^= Other.Y;
			Z ^= Other.Z;
			W ^= Other.W;
			return *this;
		}

	public:
		template<typename Ty>
		TVec4 Inflated(Ty Value) const requires (AddableC<T, Ty> && SubtractableC<T, Ty>) { return TVec4 { X - Value, Y - Value, Z + Value + Value, W + Value + Value }; }

		template<typename Ty>
		TVec4 & Inflate(Ty Value) requires (AddableC<T, Ty> && SubtractableC<T, Ty>) { X -= Value; Y -= Value, Z += Value; Z += Value, W += Value; W += Value; return *this; }

	public:
		union
		{
			struct
			{
				T X;
				T Y;
				T Z;
				T W;
			};

			struct
			{
				union
				{
					struct
					{
						T Left;
						T Top;
					};
					TVec2<T> LeftTop;
					TVec2<T> Position;
				};

				union
				{
					struct
					{
						T Width;
						T Height;
					};
					TVec2<T> Size;
				};
			};

			struct
			{
				TVec2<T> XY;
				TVec2<T> ZW;
			};

			struct
			{
				TVec2<T> XYZ;
				TVec2<T> __W;
			};

			struct
			{
				T R;
				T G;
				T B;
				T A;
			};

			struct
			{
				TVec2<T> RG;
				T __BA;
			};

			struct
			{
				TVec3<T> RGB;
				T __A;
			};

			T Values[4];
		};

		// for Rectangle
	public:
		constexpr T GetRight() const { return X + Width; }
		constexpr void SetRight(T Value) { Width = Value - X; }
		PropertyRW(T, GetRight, SetRight) Right;

		constexpr T GetBottom() const { return Y + Height; }
		constexpr void SetBottom(T Value) { Height = Value - Y; }
		PropertyRW(T, GetBottom, SetBottom) Bottom;

		constexpr TVec2<T> GetLeftBottom() const { return TVec2<T>(X, Y + Height); }
		PropertyR(TVec2<T>, GetLeftBottom) LeftBottom;

		constexpr TVec2<T> GetRightTop() const { return TVec2<T>(X + Width, Y); }
		PropertyR(TVec2<T>, GetRightTop) RightTop;

		constexpr TVec2<T> GetRightBottom() const { return TVec2<T>(X + Width, Y + Height); }
		PropertyR(TVec2<T>, GetRightBottom) RightBottom;

		constexpr TVec2<T> GetLeftCenter() const { return TVec2<T>(X, Y + Height / 2); }
		PropertyR(TVec2<T>, GetLeftCenter) LeftCenter;

		constexpr TVec2<T> GetTopCenter() const { return TVec2<T>(X + Width / 2, Y); }
		PropertyR(TVec2<T>, GetTopCenter) TopCenter;

		constexpr TVec2<T> GetRightCenter() const { return TVec2<T>(X + Width, Y + Height / 2); }
		PropertyR(TVec2<T>, GetRightCenter) RightCenter;

		constexpr TVec2<T> GetBottomCenter() const { return TVec2<T>(X + Width / 2, Y + Height); }
		PropertyR(TVec2<T>, GetBottomCenter) BottomCenter;

		constexpr TVec2<T> GetCenter() const { return TVec2<T>(X + Width / 2, Y + Height / 2); }
		PropertyR(TVec2<T>, GetCenter) Center;

		constexpr TVec2<T> GetRadius() const { return TVec2<T>(Width / 2, Height / 2); }
		PropertyR(TVec2<T>, GetCenter) Radius;

	public:
		static const TVec4 Zero;
		static const TVec4 Infinity;
		static const TVec4 NaN;
	};

	template<typename T>
	inline const TVec4<T> TVec4<T>::Zero {};

	template<typename T>
	inline const TVec4<T> TVec4<T>::Infinity { Xin::Infinity<T>, Xin::Infinity<T>, Xin::Infinity<T>, Xin::Infinity<T> };

	template<typename T>
	inline const TVec4<T> TVec4<T>::NaN { Xin::NaN<T>, Xin::NaN<T>, Xin::NaN<T>, Xin::NaN<T> };

	template<typename T>
	forceinline uintx Hash(const TVec4<T> & Vec)
	{
		return HashCombine(Vec.X, Vec.Y, Vec.Z, Vec.W);
	}

	template<typename T>
	forceinline constexpr TVec4<T> Clamp(const TVec4<T> & Value, const TVec4<T> & ValueMin, const TVec4<T> & ValueMax) noexcept
	{
		return TVec4<T>
		{
			Clamp(Value.X, ValueMin.X, ValueMax.X),
			Clamp(Value.Y, ValueMin.Y, ValueMax.Y),
			Clamp(Value.Z, ValueMin.Z, ValueMax.Z),
			Clamp(Value.W, ValueMin.W, ValueMax.W)
		};
	}


	template<typename Ty, typename T> requires AddableC<Ty, T>
	TVec4<IsAddableT<Ty, T>> operator +(const Ty & Value, const TVec4<T> & Vec) { return { Value + Vec.X, Value + Vec.Y, Value + Vec.Z, Value + Vec.W }; }

	template<typename Ty, typename T> requires SubtractableC<Ty, T>
	TVec4<IsSubtractableT<Ty, T>> operator -(const Ty & Value, const TVec4<T> & Vec) { return { Value - Vec.X, Value - Vec.Y, Value - Vec.Z, Value - Vec.W }; }

	template<typename Ty, typename T> requires MultiplicableC<Ty, T>
	TVec4<IsMultiplicableT<Ty, T>> operator *(const Ty & Value, const TVec4<T> & Vec) { return { Value * Vec.X, Value * Vec.Y, Value * Vec.Z, Value * Vec.W }; }

	template<typename Ty, typename T> requires DivisibleC<Ty, T>
	TVec4<IsDivisibleT<Ty, T>> operator /(const Ty & Value, const TVec4<T> & Vec) { return { Value / Vec.X, Value / Vec.Y, Value / Vec.Z, Value / Vec.W }; }

	using Vec4SB = TVec4<int8>;
	using Vec4UB = TVec4<uint8>;
	using Vec4SS = TVec4<int16>;
	using Vec4US = TVec4<uint16>;
	using Vec4I = TVec4<int32>;
	using Vec4U = TVec4<uint32>;
	using Vec4SL = TVec4<int64>;
	using Vec4UL = TVec4<uint64>;
	using Vec4F = TVec4<float32>;
	using Vec4D = TVec4<float64>;

	using Int4 = TVec4<int32>;
	using UInt4 = TVec4<uint32>;
	using int4 = TVec4<int32>;
	using uint4 = TVec4<uint32>;

	using Byte4 = TVec4<int8>;
	using UByte4 = TVec4<uint8>;

	using RectI = TVec4<int32>;
	using RectU = TVec4<uint32>;
	using RectF = TVec4<float32>;
	using RectD = TVec4<float64>;

	using Size4I = TVec4<int32>;
	using Size4U = TVec4<uint32>;
	using Size4F = TVec4<float32>;
	using Size4D = TVec4<float64>;

	static_assert(sizeof(uint4) == 4 * sizeof(float));
	static_assert(sizeof(int4) == 4 * sizeof(int));
}
