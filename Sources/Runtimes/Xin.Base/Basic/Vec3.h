#pragma once

#include "Basic.Types.h"
#include "Vec2.h"

namespace Xin
{
	template<typename T>
	struct TVec3;

	template<typename T>
	inline constexpr bool IsVec3V = false;

	template<typename T>
	inline constexpr bool IsVec3V<TVec3<T>> = true;

	template<typename T>
	concept Vec3C = IsVec3V<T>;

	template<typename T>
	concept NoneVec3C = !IsVec3V<T>;

	template<Vec3C VecT>
	using VecItemT = typename VecT::ItemType;

	template<typename T>
	struct [[Meta(SystemType)]] TVec3
	{
	public:
		using ItemType = T;

	public:
		TVec3() = default;
		TVec3(const TVec3 &) = default;
		TVec3 & operator =(const TVec3 &) = default;
		~TVec3() = default;
		bool operator ==(const TVec3 & Other) const { return Equal(X, Other.X) && Equal(Y, Other.Y) && Equal(Z, Other.Z); }
		auto operator <=>(const TVec3 & Other) const { return Compare(X, Other.X, Y, Other.Y, Z, Other.Z); }

		constexpr TVec3(T X, T Y) : X(X), Y(Y), Z() { }
		constexpr TVec3(T X, T Y, T Z) : X(X), Y(Y), Z(Z) { }
		constexpr TVec3(const TVec2<T> & Vec2) : X(Vec2.X), Y(Vec2.Y), Z() {}
		constexpr TVec3(const TVec2<T> & Vec2, T Z) : X(Vec2.X), Y(Vec2.Y), Z(Z) {}

		template<typename Ty> requires ConvertibleToC<Ty, T>
		explicit constexpr TVec3(const TVec3<Ty> & Other) : X(static_cast<T>(Other.X)), Y(static_cast<T>(Other.Y)), Z(static_cast<T>(Other.Z)) {}

		template<typename Ty>
		explicit operator TVec3<Ty>() requires ConvertibleToC<T, Ty> { return { static_cast<Ty>(X), static_cast<Ty>(Y), static_cast<Ty>(Z) }; }

		void Clear()
		{
			X = 0;
			Y = 0;
			Z = 0;
		}

		bool IsZero() const { return X == 0 && Y == 0 && Z == 0; }
		bool IsEmpty() const { return X == 0 || Y == 0 || Z == 0; }

		template<typename = void>
		bool AnyNan() const
		{
			if constexpr (FloatingPointC<T>)
				return IsNaN(X) || IsNaN(Y) || IsNaN(Z);
			else
				return false;
		}

		bool AllNan() const
		{
			if constexpr (FloatingPointC<T>)
				return IsNaN(X) && IsNaN(Y) && IsNaN(Y);
			else
				return false;
		}

		T LengthSquare() const { return X * X + Y * Y + Z * Z; }
		T Length() const { return (T)Sqrt(X * X + Y * Y + Z * Z); }

	public:
		TVec3 & Normalize()
		{
			auto L = Length();
			X = T(X / L);
			Y = T(Y / L);
			Z = T(Z / L);
			return *this;
		}
		TVec3 Normalized() const
		{
			auto L = Sqrt(X * X + Y * Y + Z * Z);
			return { T(X / L), T(Y / L), T(Z / L) };
		}

	public:
		const T & operator [](int32 Index) const { return Values[Index % 3]; }
		T & operator [](int32 Index) { return Values[Index % 3]; }
		TVec3 operator -() const requires IsSignedV<T> { return TVec3(-X, -Y, -Z); }
		TVec3 operator +() const { return *this; }

		template<typename Ty>
		TVec3<IsAddableT<T, Ty>> operator +(const TVec3<Ty> & Other) const requires AddableC<T, Ty> { return { X + Other.X, Y + Other.Y, Z + Other.Z }; }

		template<typename Ty>
		TVec3<IsSubtractableT<T, Ty>> operator -(const TVec3<Ty> & Other) const requires SubtractableC<T, Ty> { return { X - Other.X, Y - Other.Y, Z - Other.Z }; }

		template<typename Ty>
		TVec3<IsMultiplicableT<T, Ty>> operator *(const TVec3<Ty> & Other) const requires MultiplicableC<T, Ty> { return { X * Other.X, Y * Other.Y, Z * Other.Z }; }

		template<typename Ty>
		TVec3<IsDivisibleT<T, Ty>> operator /(const TVec3<Ty> & Other) const requires DivisibleC<T, Ty> { return { X / Other.X, Y / Other.Y, Z / Other.Z }; }

		template<typename Ty>
		TVec3 & operator +=(const TVec3<Ty> & Other) requires AddableC<T, Ty>
		{
			X += Other.X;
			Y += Other.Y;
			Z += Other.Z;
			return *this;
		}

		template<typename Ty>
		TVec3 & operator -=(const TVec3<Ty> & Other) requires SubtractableC<T, Ty>
		{
			X -= Other.X;
			Y -= Other.Y;
			Z -= Other.Z;
			return *this;
		}

		template<typename Ty>
		TVec3 & operator *=(const TVec3<Ty> & Other) requires MultiplicableC<T, Ty>
		{
			X *= Other.X;
			Y *= Other.Y;
			Z *= Other.Z;
			return *this;
		}

		template<typename Ty>
		TVec3 & operator /=(const TVec3<Ty> & Other) requires DivisibleC<T, Ty>
		{
			X /= Other.X;
			Y /= Other.Y;
			Z /= Other.Z;
			return *this;
		}

		template<typename Ty>
		TVec3<IsAddableT<T, Ty>> operator +(const Ty & Value) const requires AddableC<T, Ty> { return { X + Value, Y + Value, Z + Value }; }

		template<typename Ty>
		TVec3<IsSubtractableT<T, Ty>> operator -(const Ty & Value) const requires SubtractableC<T, Ty> { return { X - Value, Y - Value, Z - Value }; }

		template<typename Ty>
		TVec3<IsMultiplicableT<T, Ty>> operator *(const Ty & Value) const requires MultiplicableC<T, Ty> { return { X * Value, Y * Value, Z * Value }; }

		template<typename Ty>
		TVec3<IsDivisibleT<T, Ty>> operator /(const Ty & Value) const requires DivisibleC<T, Ty> { return { X / Value, Y / Value, Z / Value }; }

		template<typename Ty>
		TVec3 & operator +=(const Ty & Value) requires AddableC<T, Ty>
		{
			X += Value;
			Y += Value;
			Z += Value;
			return *this;
		}

		template<typename Ty>
		TVec3 & operator -=(const Ty & Value) requires SubtractableC<T, Ty>
		{
			X -= Value;
			Y -= Value;
			Z -= Value;
			return *this;
		}

		template<typename Ty>
		TVec3 & operator *=(const Ty & Value) requires MultiplicableC<T, Ty>
		{
			X *= Value;
			Y *= Value;
			Z *= Value;
			return *this;
		}

		template<typename Ty>
		TVec3 & operator /=(const Ty & Value) requires DivisibleC<T, Ty>
		{
			X /= Value;
			Y /= Value;
			Z /= Value;
			return *this;
		}

		template<typename = void>
		TVec3 operator |(T Value) const requires ArithmeticOrAbleC<T> { return TVec3 { X | Value, Y | Value, Z | Value }; }

		template<typename = void>
		TVec3 operator &(T Value) const requires ArithmeticAndAbleC<T> { return TVec3 { X & Value, Y & Value, Z & Value }; }

		template<typename = void>
		TVec3 operator ^(T Value) const requires ArithmeticXorAbleC<T> { return TVec3 { X ^ Value, Y ^ Value, Z ^ Value }; }

		template<typename = void>
		TVec3 & operator |=(int32 Shift) requires ArithmeticOrAbleC<T>
		{
			X |= Shift;
			Y |= Shift;
			Z |= Shift;
			return *this;
		}

		template<typename = void>
		TVec3 & operator &=(int32 Shift) requires ArithmeticAndAbleC<T>
		{
			X &= Shift;
			Y &= Shift;
			Z &= Shift;
			return *this;
		}

		template<typename = void>
		TVec3 & operator ^=(int32 Shift) requires ArithmeticXorAbleC<T>
		{
			X ^= Shift;
			Y ^= Shift;
			Z ^= Shift;
			return *this;
		}

		template<typename = void>
		TVec3 operator >>(int32 Shift) const requires ArithmeticShiftableC<T> { return TVec3 { X >> Shift, Y >> Shift, Z >> Shift }; }

		template<typename = void>
		TVec3 operator <<(int32 Shift) const requires ArithmeticShiftableC<T> { return TVec3 { X << Shift, Y << Shift, Z << Shift }; }

		template<typename = void>
		TVec3 & operator >>=(int32 Shift) requires ArithmeticShiftableC<T>
		{
			X >>= Shift;
			Y >>= Shift;
			Z >>= Shift;
			return *this;
		}

		template<typename = void>
		TVec3 & operator <<=(int32 Shift) requires ArithmeticShiftableC<T>
		{
			X <<= Shift;
			Y <<= Shift;
			Z <<= Shift;
			return *this;
		}

		template<typename = void>
		TVec3 operator ~() const requires ArithmeticNotAbleC<T> { return { ~X, ~Y, ~Z }; }

		template<typename = void>
		TVec3 operator |(const TVec3 & Other) const requires ArithmeticOrAbleC<T> { return TVec3 { X | Other.X, Y | Other.Y, Z | Other.Z }; }

		template<typename = void>
		TVec3 operator &(const TVec3 & Other) const requires ArithmeticAndAbleC<T> { return TVec3 { X & Other.X, Y & Other.Y, Z & Other.Z }; }

		template<typename = void>
		TVec3 operator ^(const TVec3 & Other) const requires ArithmeticXorAbleC<T> { return TVec3 { X ^ Other.X, Y ^ Other.Y, Z ^ Other.Z }; }

		template<typename = void>
		TVec3 & operator |=(const TVec3 & Other) requires ArithmeticOrAbleC<T>
		{
			X |= Other.X;
			Y |= Other.Y;
			Z |= Other.Z;
			return *this;
		}

		template<typename = void>
		TVec3 & operator &=(const TVec3 & Other) requires ArithmeticAndAbleC<T>
		{
			X &= Other.X;
			Y &= Other.Y;
			Z &= Other.Z;
			return *this;
		}

		template<typename = void>
		TVec3 & operator ^=(const TVec3 & Other) requires ArithmeticXorAbleC<T>
		{
			X ^= Other.X;
			Y ^= Other.Y;
			Z ^= Other.Z;
			return *this;
		}

	public:
		union
		{
			struct
			{
				T X;
				T Y;
				T Z;
			};

			struct
			{
				T Width;
				T Height;
				T Depth;
			};

			struct
			{
				T R;
				T G;
				T B;
			};

			struct
			{
				TVec2<T> XY;
				T __Z;
			};

			T Values[2];
		};

	public:
		static TVec3<T> Zero;
		static TVec3<T> NaN;
	};

	template<typename T>
	TVec3<T> TVec3<T>::Zero {};

	template<typename T>
	TVec3<T> TVec3<T>::NaN { Xin::NaN<T>, Xin::NaN<T>, Xin::NaN<T> };

	template<typename T>
	forceinline constexpr TVec3<T> Clamp(const TVec3<T> & Value, const TVec3<T> & ValueMin, const TVec3<T> & ValueMax) noexcept
	{
		return TVec3<T> { Clamp(Value.X, ValueMin.X, ValueMax.X), Clamp(Value.Y, ValueMin.Y, ValueMax.Y), Clamp(Value.Z, ValueMin.Z, ValueMax.Z) };
	}


	template<typename Ty, typename T> requires AddableC<Ty, T>
	TVec3<IsAddableT<Ty, T>> operator +(const Ty & Value, const TVec3<T> & Vec) { return { Value + Vec.X, Value + Vec.Y, Value + Vec.Z }; }

	template<typename Ty, typename T> requires SubtractableC<Ty, T>
	TVec3<IsSubtractableT<Ty, T>> operator -(const Ty & Value, const TVec3<T> & Vec) { return { Value - Vec.X, Value - Vec.Y, Value - Vec.Z }; }

	template<typename Ty, typename T> requires MultiplicableC<Ty, T>
	TVec3<IsMultiplicableT<Ty, T>> operator *(const Ty & Value, const TVec3<T> & Vec) { return { Value * Vec.X, Value * Vec.Y, Value * Vec.Z }; }

	template<typename Ty, typename T> requires DivisibleC<Ty, T>
	TVec3<IsDivisibleT<Ty, T>> operator /(const Ty & Value, const TVec3<T> & Vec) { return { Value / Vec.X, Value / Vec.Y, Value / Vec.Z }; }


	using int3 = TVec3<int32>;
	using uint3 = TVec3<uint32>;
	using Int3 = TVec3<int32>;
	using UInt3 = TVec3<uint32>;

	using Vec3SB = TVec3<int8>;
	using Vec3UB = TVec3<uint8>;
	using Vec3SS = TVec3<int16>;
	using Vec3US = TVec3<uint16>;
	using Vec3I = TVec3<int32>;
	using Vec3U = TVec3<uint32>;
	using Vec3SL = TVec3<int64>;
	using Vec3UL = TVec3<uint64>;
	using Vec3F = TVec3<float32>;
	using Vec3D = TVec3<float64>;

	using Size3I = TVec3<int32>;
	using Size3U = TVec3<uint32>;
	using Size3F = TVec3<float32>;

	static_assert(sizeof(Vec3I) == sizeof(int32) * 3);
	static_assert(sizeof(Vec3F) == sizeof(float32) * 3);
}
