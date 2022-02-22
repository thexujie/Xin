#pragma once

#include "Basic.Types.h"

namespace Xin
{
	enum class ESwizzleComponnet
	{
		XXXX = 0,
	};

	template<typename T>
	using TVec1 = T;

	template<typename T>
	struct TVec2;

	template<typename T>
	inline constexpr bool IsVec2V = false;

	template<typename T>
	inline constexpr bool IsVec2V<TVec2<T>> = true;

	template<typename T>
	concept Vec2C = IsVec2V<T>;

	template<typename T>
	concept NoneVec2C = !IsVec2V<T>;

	template<typename T>
	struct [[Meta(SystemType)]] TVec2
	{
	public:
		constexpr TVec2() = default;
		constexpr TVec2(const TVec2 &) = default;
		constexpr TVec2(T X, ESwizzleComponnet) : X(X), Y(X) { }
		constexpr TVec2(T X, T Y) : X(X), Y(Y) { }
		constexpr ~TVec2() = default;
		constexpr TVec2 & operator =(const TVec2 &) = default;
		bool operator ==(const TVec2 & Other) const { return Equal(X, Other.X) && Equal(Y, Other.Y); }
		auto operator <=>(const TVec2 & Other) const { return Compare(X, Other.X, Y, Other.Y); }

		template<ConvertibleToC<T> Ty>
		constexpr TVec2(const TVec2<Ty> & That) : X(static_cast<T>(That.X)), Y(static_cast<T>(That.Y)) {}

		template<ConvertibleToC<T> Ty>
		explicit operator TVec2<Ty>() { return { static_cast<Ty>(X), static_cast<Ty>(Y) }; }

		void Clear()
		{
			X = 0;
			Y = 0;
		}

		bool AnyZero() const { return X == 0 || Y == 0; }
		bool AllZero() const { return X == 0 && Y == 0; }
		bool Empty() const { return AnyZero(); }

		bool AnyNaN() const
		{
			if constexpr (FloatingPointC<T>)
				return IsNaN(X) || IsNaN(Y);
			else
				return false;
		}

		bool AllNaN() const
		{
			if constexpr (FloatingPointC<T>)
				return IsNaN(X) && IsNaN(Y);
			else
				return false;
		}

		T LengthSquare() const
		{
			if constexpr (ArithmeticC<T>)
				return X * X + Y * Y;
			else
				return { };
		}

		T Length() const
		{
			if constexpr (ArithmeticC<T>)
				return (T)Sqrt(X * X + Y * Y);
			else
				return { };
		}

		bool Contains(const TVec2 & Point) const { return Point.X >= 0 && Point.Y >= 0 && Point.X < X && Point.Y < Y; }

	public:
		TVec2 & Normalize()
		{
			if constexpr (ArithmeticC<T>)
			{
				auto L = Length();
				X = T(X / L);
				Y = T(Y / L);
				return *this;
			}
			else
				return *this;
		}

		TVec2 Normalized() const
		{
			if constexpr (ArithmeticC<T>)
			{
				auto L = Sqrt(X * X + Y * Y);
				return { T(X / L), T(Y / L) };
			}
			else
				return *this;
		}

		T Dot(const TVec2 & Another)  const
		{
			if constexpr (ArithmeticC<T>)
				return X * Another.X + Y * Another.Y;
			else
				return { };
		}

		T Cross(const TVec2 & Another)  const
		{
			if constexpr (ArithmeticC<T>)
				return X * Another.Y - Y * Another.X;
			else
				return { };
		}

	public:
		const T & operator [](int32 Index) const { return reinterpret_cast<const T *>(this)[Index % 2]; }
		T & operator [](int32 Index) { return reinterpret_cast<T *>(this)[Index % 2]; }
		TVec2 operator -() const requires IsSignedV<T> { return { -X, -Y }; }
		TVec2 operator +() const { return *this; }

		template<typename Ty>
		TVec2<IsAddableT<T, Ty>> operator +(const TVec2<Ty> & Other) const requires AddableC<T, Ty> { return { X + Other.X, Y + Other.Y }; }

		template<typename Ty>
		TVec2<IsSubtractableT<T, Ty>> operator -(const TVec2<Ty> & Other) const requires SubtractableC<T, Ty> { return { X - Other.X, Y - Other.Y }; }

		template<typename Ty>
		TVec2<IsMultiplicableT<T, Ty>> operator *(const TVec2<Ty> & Other) const requires MultiplicableC<T, Ty> { return { X * Other.X, Y * Other.Y }; }

		template<typename Ty>
		TVec2<IsDivisibleT<T, Ty>> operator /(const TVec2<Ty> & Other) const requires DivisibleC<T, Ty> { return { X / Other.X, Y / Other.Y }; }

		template<typename Ty>
		TVec2 & operator +=(const TVec2<Ty> & Other) requires AddableC<T, Ty>
		{
			X += Other.X;
			Y += Other.Y;
			return *this;
		}

		template<typename Ty>
		TVec2 & operator -=(const TVec2<Ty> & Other) requires SubtractableC<T, Ty>
		{
			X -= Other.X;
			Y -= Other.Y;
			return *this;
		}

		template<typename Ty>
		TVec2 & operator *=(const TVec2<Ty> & Other) requires MultiplicableC<T, Ty>
		{
			X *= Other.X;
			Y *= Other.Y;
			return *this;
		}

		template<typename Ty>
		TVec2 & operator /=(const TVec2<Ty> & Other) requires DivisibleC<T, Ty>
		{
			X /= Other.X;
			Y /= Other.Y;
			return *this;
		}

		template<NoneVec2C Ty>
		TVec2<IsAddableT<T, Ty>> operator +(const Ty & Value) const requires AddableC<T, Ty> { return { X + Value, Y + Value }; }

		template<NoneVec2C Ty>
		TVec2<IsSubtractableT<T, Ty>> operator -(const Ty & Value) const requires SubtractableC<T, Ty> { return { X - Value, Y - Value }; }

		template<NoneVec2C Ty>
		TVec2<IsMultiplicableT<T, Ty>> operator *(const Ty & Value) const requires MultiplicableC<T, Ty> { return { X * Value, Y * Value }; }

		template<NoneVec2C Ty>
		TVec2<IsDivisibleT<T, Ty>> operator /(const Ty & Value) const requires DivisibleC<T, Ty> { return { X / Value, Y / Value }; }

		template<NoneVec2C Ty>
		TVec2 & operator +=(const Ty & Value) requires AddableC<T, Ty>
		{
			X += Value;
			Y += Value;
			return *this;
		}

		template<NoneVec2C Ty>
		TVec2 & operator -=(const Ty & Value) requires SubtractableC<T, Ty>
		{
			X -= Value;
			Y -= Value;
			return *this;
		}

		template<NoneVec2C Ty>
		TVec2 & operator *=(const Ty & Value) requires MultiplicableC<T, Ty>
		{
			X *= Value;
			Y *= Value;
			return *this;
		}

		template<NoneVec2C Ty>
		TVec2 & operator /=(const Ty & Value) requires DivisibleC<T, Ty>
		{
			X /= Value;
			Y /= Value;
			return *this;
		}

		template<typename = void>
		TVec2 operator |(T Value) const requires ArithmeticOrAbleC<T> { return TVec2 { X | Value, Y | Value }; }

		template<typename = void>
		TVec2 operator &(T Value) const requires ArithmeticAndAbleC<T> { return TVec2 { X & Value, Y & Value }; }

		template<typename = void>
		TVec2 operator ^(T Value) const requires ArithmeticXorAbleC<T> { return TVec2 { X ^ Value, Y ^ Value }; }

		template<typename = void>
		TVec2 & operator |=(int32 Shift) requires ArithmeticOrAbleC<T>
		{
			X |= Shift;
			Y |= Shift;
			return *this;
		}

		template<typename = void>
		TVec2 & operator &=(int32 Shift) requires ArithmeticAndAbleC<T>
		{
			X &= Shift;
			Y &= Shift;
			return *this;
		}

		template<typename = void>
		TVec2 & operator ^=(int32 Shift) requires ArithmeticXorAbleC<T>
		{
			X ^= Shift;
			Y ^= Shift;
			return *this;
		}

		template<typename = void>
		TVec2 operator >>(int32 Shift) const requires ArithmeticShiftableC<T> { return TVec2 { X >> Shift, Y >> Shift }; }

		template<typename = void>
		TVec2 operator <<(int32 Shift) const requires ArithmeticShiftableC<T> { return TVec2 { X << Shift, Y << Shift }; }

		template<typename = void>
		TVec2 & operator >>=(int32 Shift) requires ArithmeticShiftableC<T>
		{
			X >>= Shift;
			Y >>= Shift;
			return *this;
		}

		template<typename = void>
		TVec2 & operator <<=(int32 Shift) requires ArithmeticShiftableC<T>
		{
			X <<= Shift;
			Y <<= Shift;
			return *this;
		}

		template<typename = void>
		TVec2 operator ~() const requires ArithmeticNotAbleC<T> { return { ~X, ~Y }; }

		template<typename = void>
		TVec2 operator |(const TVec2 & That) const requires ArithmeticOrAbleC<T> { return TVec2 { X | That.X, Y | That.Y }; }

		template<typename = void>
		TVec2 operator &(const TVec2 & That) const requires ArithmeticAndAbleC<T> { return TVec2 { X & That.X, Y & That.Y }; }

		template<typename = void>
		TVec2 operator ^(const TVec2 & That) const requires ArithmeticXorAbleC<T> { return TVec2 { X ^ That.X, Y ^ That.Y }; }

		template<typename = void>
		TVec2 & operator |=(const TVec2 & That) requires ArithmeticOrAbleC<T>
		{
			X |= That.X;
			Y |= That.Y;
			return *this;
		}

		template<typename = void>
		TVec2 & operator &=(const TVec2 & That) requires ArithmeticAndAbleC<T>
		{
			X &= That.X;
			Y &= That.Y;
			return *this;
		}

		template<typename = void>
		TVec2 & operator ^=(const TVec2 & That) requires ArithmeticXorAbleC<T>
		{
			X ^= That.X;
			Y ^= That.Y;
			return *this;
		}

	public:
		union
		{
			struct
			{
				T X;
				T Y;
			};
			struct
			{
				T Left;
				T Top;
			};
			struct
			{
				T Width;
				T Height;
			};
		};

	public:
		static TVec2 Zero;
		static TVec2 Half;
		static TVec2 One;
		static TVec2 XAxis;
		static TVec2 YAxis;
		static TVec2 Infinity;
		static TVec2 NaN;
	};

	template<typename T>
	TVec2<T> TVec2<T>::Zero {};

	template<typename T>
	TVec2<T> TVec2<T>::Half { 0.5, 0.5 };

	template<typename T>
	TVec2<T> TVec2<T>::One { 1, 1 };

	template<typename T>
	TVec2<T> TVec2<T>::XAxis { 1, 0 };

	template<typename T>
	TVec2<T> TVec2<T>::YAxis { 0, 1 };

	template<typename T>
	TVec2<T> TVec2<T>::Infinity { Xin::Infinity<T>, Xin::Infinity<T> };

	template<typename T>
	TVec2<T> TVec2<T>::NaN { Xin::NaN<T>, Xin::NaN<T> };

	template<typename T>
	forceinline constexpr TVec2<T> Clamp(const TVec2<T> & Value, const TVec2<T> & ValueMin, const TVec2<T> & ValueMax) noexcept
	{
		return TVec2<T> { Clamp(Value.X, ValueMin.X, ValueMax.X), Clamp(Value.Y, ValueMin.Y, ValueMax.Y) };
	}

	template<typename T>
	forceinline constexpr TVec2<T> Max(const TVec2<T> & ValueA, const TVec2<T> & ValueB) noexcept
	{
		return TVec2<T> { Max(ValueA.X, ValueB.X), Max(ValueA.Y, ValueB.Y) };
	}

	template<typename T>
	forceinline constexpr TVec2<T> Min(const TVec2<T> & ValueA, const TVec2<T> & ValueB) noexcept
	{
		return TVec2<T> { Min(ValueA.X, ValueB.X), Min(ValueA.Y, ValueB.Y) };
	}
	template<typename T>
	forceinline constexpr TVec2<T> Floor(TVec2<T> Vec) { return { Floor(Vec.X), Floor(Vec.Y) }; }

	template<typename T>
	forceinline constexpr TVec2<T> Ceil(TVec2<T> Vec) { return { Ceil(Vec.X), Ceil(Vec.Y) }; }

	template<typename T>
	forceinline constexpr TVec2<T> Round(TVec2<T> Vec) { return { Round(Vec.X), Round(Vec.Y) }; }

	template<typename Ty, typename T> requires AddableC<Ty, T>
	TVec2<IsAddableT<Ty, T>> operator +(const Ty & Value, const TVec2<T> & Vec) { return { Value + Vec.X, Value + Vec.Y }; }

	template<typename Ty, typename T> requires SubtractableC<Ty, T>
	TVec2<IsSubtractableT<Ty, T>> operator -(const Ty & Value, const TVec2<T> & Vec) { return { Value - Vec.X, Value - Vec.Y }; }

	template<typename Ty, typename T> requires MultiplicableC<Ty, T>
	TVec2<IsMultiplicableT<Ty, T>> operator *(const Ty & Value, const TVec2<T> & Vec) { return { Value * Vec.X, Value * Vec.Y }; }

	template<typename Ty, typename T> requires DivisibleC<Ty, T>
	TVec2<IsDivisibleT<Ty, T>> operator /(const Ty & Value, const TVec2<T> & Vec) { return { Value / Vec.X, Value / Vec.Y }; }


	using int1 = int32;
	using int2 = TVec2<int32>;
	using llint2 = TVec2<int64_t>;
	using uint1 = uint32;
	using uint2 = TVec2<uint32>;
	using ullint2 = TVec2<uint64>;

	using Int2 = TVec2<int32>;
	using UInt2 = TVec2<uint32>;

	using Vec2SB = TVec2<int8>;
	using Vec2UB = TVec2<uint8>;
	using Vec2SS = TVec2<int16>;
	using Vec2US = TVec2<uint16>;
	using Vec2I = TVec2<int32>;
	using Vec2U = TVec2<uint32>;
	using Vec2SL = TVec2<int64>;
	using Vec2UL = TVec2<uint64>;
	using Vec2F = TVec2<float32>;
	using Vec2D = TVec2<float64>;

	using SizeI = TVec2<int32>;
	using SizeU = TVec2<uint32>;
	using SizeF = TVec2<float32>;
	using SizeD = TVec2<float64>;

	using Size2I = TVec2<int32>;
	using Size2U = TVec2<uint32>;
	using Size2F = TVec2<float32>;

	using PointI = TVec2<int32>;
	using PointU = TVec2<uint32>;
	using PointF = TVec2<float32>;
	using PointD = TVec2<float64>;
	using Point2I = TVec2<int32>;
	using Point2U = TVec2<uint32>;
	using Point2F = TVec2<float32>;

	using SizeI64 = TVec2<int64_t>;
	using SizeU64 = TVec2<uint64>;

	static_assert(sizeof(Vec2I) == sizeof(int32) * 2);
	static_assert(sizeof(Vec2F) == sizeof(float32) * 2);
}
