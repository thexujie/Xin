#pragma once

#include "Basic.Types.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

namespace Xin
{
	template<typename T>
	class [[Meta(SystemType)]] TThickness
	{
	public:
		constexpr TThickness() = default;
		constexpr TThickness(const TThickness &) = default;
		constexpr TThickness & operator =(const TThickness &) = default;
		constexpr bool operator ==(const TThickness & Other) const noexcept = default;
		constexpr auto operator <=>(const TThickness & Other) const noexcept = default;
		constexpr ~TThickness() = default;

		template<ConvertibleToC<T> Ty>
		constexpr TThickness(Ty Value) : Left(static_cast<T>(Value)), Top(static_cast<T>(Value)), Right(static_cast<T>(Value)), Bottom(static_cast<T>(Value)) { }

		constexpr TThickness(T ValueLeftRight, T ValueTopBottom) : Left(ValueLeftRight), Top(ValueTopBottom), Right(ValueLeftRight), Bottom(ValueTopBottom) {}

		template<ConvertibleToC<T> Ty>
		constexpr TThickness(const TVec2<Ty> & Vec2) : Left(static_cast<T>(Vec2.X)), Top(static_cast<T>(Vec2.Y)), Right(static_cast<T>(Vec2.X)), Bottom(static_cast<T>(Vec2.Y)) { }

		template<ConvertibleToC<T> Ty>
		constexpr TThickness(const TVec3<Ty> & Vec3) : Left(static_cast<T>(Vec3.X)), Top(static_cast<T>(Vec3.Y)), Right(static_cast<T>(Vec3.X)), Bottom() { }

		template<ConvertibleToC<T> Ty>
		constexpr TThickness(const TVec4<Ty> & Vec4) : Left(static_cast<T>(Vec4.X)), Top(static_cast<T>(Vec4.Y)), Right(static_cast<T>(Vec4.Z)), Bottom(static_cast<T>(Vec4.W)) { }

		template<ConvertibleToC<T> Ty>
		constexpr TThickness(const TVec3<Ty> & Vec3, Ty Bottom) : Left(static_cast<T>(Vec3.X)), Top(static_cast<T>(Vec3.Y)), Right(static_cast<T>(Vec3.Z)), Bottom(static_cast<T>(Bottom)) { }

		template<ConvertibleToC<T> Ty>
		constexpr TThickness(const TVec2<Ty> & XY, const TVec2<Ty> & ZW) : Left(static_cast<T>(XY.X)), Top(static_cast<T>(XY.Y)), Right(static_cast<T>(ZW.Z)), Bottom(static_cast<T>(ZW.W)) { }
		constexpr TThickness(T Left, T Top, T Right, T Bottom) : Left(Left), Top(Top), Right(Right), Bottom(Bottom) {}

		template<ConvertibleToC<T> Ty>
		constexpr TThickness(const TThickness<Ty> & That) : Left(static_cast<T>(That.Left)), Top(static_cast<T>(That.Top)), Right(static_cast<T>(That.Right)), Bottom(static_cast<T>(That.Bottom)) {}

		template<typename Ty> requires ConvertibleToC<T, Ty>
		operator TVec4<Ty>() const { return TVec4<Ty> { (Ty)Left, (Ty)Top, (Ty)Right, (Ty)Bottom }; }

		void Clear()
		{
			Left = 0;
			Top = 0;
			Right = 0;
			Bottom = 0;
		}

		bool Uniform() const { return Left == Top && Top == Right && Right == Bottom && Bottom == Left; }
		bool AnyZero() const { return Left == 0 || Top == 0 || Right == 0 || Bottom == 0; }
		bool AllZero() const { return Left == 0 && Top == 0 && Right == 0 && Bottom == 0; }
		bool EmptyRect() const { return Size.Empty(); }

		template<typename = void>
		bool AnyNan() const requires FloatingPointC<T> { return IsNaN(Left) || IsNaN(Top) || IsNaN(Right) || IsNaN(Bottom); }

		template<typename = void>
		bool AllNan() const requires FloatingPointC<T> { return IsNaN(Left) && IsNaN(Top) && IsNaN(Right) && IsNaN(Bottom); }

		const T & operator [](int32 Index) const { return reinterpret_cast<const T *>(this)[Index % 4]; }
		T & operator [](int32 Index) { return reinterpret_cast<T *>(this)[Index % 4]; }

		TThickness operator -() const requires IsSignedV<T> { return TThickness(-Left, -Top, -Right, -Bottom); }
		TThickness operator +() const { return *this; }

		template<typename Ty>
		TThickness<IsAddableT<T, Ty>> operator +(const TThickness<Ty> & Another) const requires AddableC<T, Ty> { return { Left + Another.Left, Top + Another.Top, Right + Another.Right, Bottom + Another.Bottom }; }

		template<typename Ty>
		TThickness<IsSubtractableT<T, Ty>> operator -(const TThickness<Ty> & Another) const requires SubtractableC<T, Ty> { return { Left - Another.Left, Top - Another.Top, Right - Another.Right, Bottom - Another.Bottom }; }

		template<typename Ty>
		TThickness<IsMultiplicableT<T, Ty>> operator *(const TThickness<Ty> & Another) const requires MultiplicableC<T, Ty> { return { Left * Another.Left, Top * Another.Top, Right * Another.Right, Bottom * Another.Bottom }; }

		template<typename Ty>
		TThickness<IsDivisibleT<T, Ty>> operator /(const TThickness<Ty> & Another) const requires DivisibleC<T, Ty> { return { Left / Another.Left, Top / Another.Top, Right / Another.Right, Bottom / Another.Bottom }; }

		template<typename Ty>
		TThickness & operator +=(const TThickness<Ty> & Another) requires AddableC<T, Ty>
		{
			Left += Another.Left;
			Top += Another.Top;
			Right += Another.Right;
			Bottom += Another.Bottom;
			return *this;
		}

		template<typename Ty>
		TThickness & operator -=(const TThickness<Ty> & Another) requires SubtractableC<T, Ty>
		{
			Left -= Another.Left;
			Top -= Another.Top;
			Right -= Another.Right;
			Bottom -= Another.Bottom;
			return *this;
		}

		template<typename Ty>
		TThickness & operator *=(const TThickness<Ty> & Another) requires MultiplicableC<T, Ty>
		{
			Left *= Another.Left;
			Top *= Another.Top;
			Right *= Another.Right;
			Bottom *= Another.Bottom;
			return *this;
		}

		template<typename Ty>
		TThickness & operator /=(const TThickness<Ty> & Another) requires DivisibleC<T, Ty>
		{
			Left /= Another.Left;
			Top /= Another.Top;
			Right /= Another.Right;
			Bottom /= Another.Bottom;
			return *this;
		}

		template<typename Ty>
		TThickness<IsAddableT<T, Ty>> operator +(const Ty & Value) const requires AddableC<T, Ty> { return { Left + Value, Top + Value, Right + Value, Bottom + Value }; }

		template<typename Ty>
		TThickness<IsSubtractableT<T, Ty>> operator -(const Ty & Value) const requires SubtractableC<T, Ty> { return { Left - Value, Top - Value, Right - Value, Bottom - Value }; }

		template<typename Ty>
		TThickness<IsMultiplicableT<T, Ty>> operator *(const Ty & Value) const requires MultiplicableC<T, Ty> { return { Left * Value, Top * Value, Right * Value, Bottom * Value }; }

		template<typename Ty>
		TThickness<IsDivisibleT<T, Ty>> operator /(const Ty & Value) const requires DivisibleC<T, Ty> { return { Left / Value, Top / Value, Right / Value, Bottom / Value }; }

		template<typename Ty>
		TThickness & operator +=(const Ty & Value) requires AddableC<T, Ty>
		{
			Left += Value;
			Top += Value;
			Right += Value;
			Bottom += Value;
			return *this;
		}

		template<typename Ty>
		TThickness & operator -=(const Ty & Value) requires SubtractableC<T, Ty>
		{
			Left -= Value;
			Top -= Value;
			Right -= Value;
			Bottom -= Value;
			return *this;
		}

		template<typename Ty>
		TThickness & operator *=(const Ty & Value) requires MultiplicableC<T, Ty>
		{
			Left *= Value;
			Top *= Value;
			Right *= Value;
			Bottom *= Value;
			return *this;
		}

		template<typename Ty>
		TThickness & operator /=(const Ty & Value) requires DivisibleC<T, Ty>
		{
			Left /= Value;
			Top /= Value;
			Right /= Value;
			Bottom /= Value;
			return *this;
		}

		template<typename = void>
		TThickness operator |(T Value) const requires ArithmeticOrAbleC<T> { return TThickness { Left | Value, Top | Value, Right | Value, Bottom | Value }; }

		template<typename = void>
		TThickness operator &(T Value) const requires ArithmeticAndAbleC<T> { return TThickness { Left & Value, Top & Value, Right & Value, Bottom & Value }; }

		template<typename = void>
		TThickness operator ^(T Value) const requires ArithmeticXorAbleC<T> { return TThickness { Left ^ Value, Top ^ Value, Right ^ Value, Bottom ^ Value }; }

		template<typename = void>
		TThickness & operator |=(int32 Shift) requires ArithmeticOrAbleC<T>
		{
			Left |= Shift;
			Top |= Shift;
			Right |= Shift;
			Bottom |= Shift;
			return *this;
		}

		template<typename = void>
		TThickness & operator &=(int32 Shift) requires ArithmeticAndAbleC<T>
		{
			Left &= Shift;
			Top &= Shift;
			Right &= Shift;
			Bottom &= Shift;
			return *this;
		}

		template<typename = void>
		TThickness & operator ^=(int32 Shift) requires ArithmeticXorAbleC<T>
		{
			Left ^= Shift;
			Top ^= Shift;
			Right ^= Shift;
			Bottom ^= Shift;
			return *this;
		}

		template<typename = void>
		TThickness operator >>(int32 Shift) const requires ArithmeticShiftableC<T> { return TThickness { Left >> Shift, Top >> Shift, Right >> Shift, Bottom >> Shift }; }

		template<typename = void>
		TThickness operator <<(int32 Shift) const requires ArithmeticShiftableC<T> { return TThickness { Left << Shift, Top << Shift, Right << Shift, Bottom << Shift }; }

		template<typename = void>
		TThickness & operator >>=(int32 Shift) requires ArithmeticShiftableC<T>
		{
			Left >>= Shift;
			Top >>= Shift;
			Right >>= Shift;
			Bottom >>= Shift;
			return *this;
		}

		template<typename = void>
		TThickness & operator <<=(int32 Shift) requires ArithmeticShiftableC<T>
		{
			Left <<= Shift;
			Top <<= Shift;
			Right <<= Shift;
			Bottom <<= Shift;
			return *this;
		}

		template<typename = void>
		TThickness operator ~() const requires ArithmeticNotAbleC<T> { return { ~Left, ~Top, ~Right, ~Bottom }; }

		template<typename = void>
		TThickness operator |(const TThickness & Another) const requires ArithmeticOrAbleC<T> { return TThickness { Left | Another.Left, Top | Another.Top, Right | Another.Right, Bottom | Another.Bottom }; }

		template<typename = void>
		TThickness operator &(const TThickness & Another) const requires ArithmeticAndAbleC<T> { return TThickness { Left & Another.Left, Top & Another.Top, Right & Another.Right, Bottom & Another.Bottom }; }

		template<typename = void>
		TThickness operator ^(const TThickness & Another) const requires ArithmeticXorAbleC<T> { return TThickness { Left ^ Another.Left, Top ^ Another.Top, Right ^ Another.Right, Bottom ^ Another.Bottom }; }

		template<typename = void>
		TThickness & operator |=(const TThickness & Another) requires ArithmeticOrAbleC<T>
		{
			Left |= Another.Left;
			Top |= Another.Top;
			Right |= Another.Right;
			Bottom |= Another.Bottom;
			return *this;
		}

		template<typename = void>
		TThickness & operator &=(const TThickness & Another) requires ArithmeticAndAbleC<T>
		{
			Left &= Another.Left;
			Top &= Another.Top;
			Right &= Another.Right;
			Bottom &= Another.Bottom;
			return *this;
		}

		template<typename = void>
		TThickness & operator ^=(const TThickness & Another) requires ArithmeticXorAbleC<T>
		{
			Left ^= Another.Left;
			Top ^= Another.Top;
			Right ^= Another.Right;
			Bottom ^= Another.Bottom;
			return *this;
		}

	public:
		T Left;
		T Top;
		T Right;
		T Bottom;

	public:
		constexpr const TVec2<T> & GetLeftTop() const { return reinterpret_cast<const TVec2<T> *>(this)[0]; }
		constexpr void SetLeftTop(const TVec2<T> & Value) { reinterpret_cast<TVec2<T> *>(this)[0] = Value; }
		PropertyRW(TVec2<T>, GetLeftTop, SetLeftTop) LeftTop;

		constexpr const TVec2<T> & GetRightBottom() const { return reinterpret_cast<const TVec2<T> *>(this)[1]; }
		constexpr void SetRightBottom(const TVec2<T> & Value) { reinterpret_cast<TVec2<T> *>(this)[1] = Value; }
		PropertyRW(TVec2<T>, GetRightBottom, SetRightBottom) RightBottom;

	public:
		T GetWidth() const { return Left + Right; }
		PropertyR(T, GetWidth) Width;

		T GetHeight() const { return Top + Bottom; }
		PropertyR(T, GetHeight) Height;

	public:
		TVec2<T> GetSize() const { return { Left + Right, Top + Bottom }; }
		PropertyR(TVec2<T>, GetSize) Size;

	public:
		static TThickness<T> Zero;
		static TThickness<T> Infinity;
		static TThickness<T> NaN;
	};

	template<typename T>
	TThickness<T> TThickness<T>::Zero {};

	template<typename T>
	TThickness<T> TThickness<T>::Infinity { Xin::Infinity<T>, Xin::Infinity<T>, Xin::Infinity<T>, Xin::Infinity<T> };

	template<typename T>
	TThickness<T> TThickness<T>::NaN { Xin::NaN<T>, Xin::NaN<T>, Xin::NaN<T>, Xin::NaN<T> };

	template<typename T>
	forceinline uintx Hash(const TThickness<T> & Vec)
	{
		return HashCombine(Vec.Left, Vec.Top, Vec.Right, Vec.Bottom);
	}

	template<typename T>
	forceinline constexpr TThickness<T> Clamp(const TThickness<T> & Value, const TThickness<T> & ValueMin, const TThickness<T> & ValueMax) noexcept
	{
		return TThickness<T>
		{
			Clamp(Value.Left, ValueMin.Left, ValueMax.Left),
			Clamp(Value.Top, ValueMin.Top, ValueMax.Top),
			Clamp(Value.Right, ValueMin.Right, ValueMax.Right),
			Clamp(Value.Bottom, ValueMin.Bottom, ValueMax.Bottom)
		};
	}


	template<typename Ty, typename T> requires AddableC<Ty, T>
	TThickness<IsAddableT<Ty, T>> operator +(const Ty & Value, const TThickness<T> & Vec) { return { Value + Vec.Left, Value + Vec.Top, Value + Vec.Right, Value + Vec.Bottom }; }

	template<typename Ty, typename T> requires SubtractableC<Ty, T>
	TThickness<IsSubtractableT<Ty, T>> operator -(const Ty & Value, const TThickness<T> & Vec) { return { Value - Vec.Left, Value - Vec.Top, Value - Vec.Right, Value - Vec.Bottom }; }

	template<typename Ty, typename T> requires MultiplicableC<Ty, T>
	TThickness<IsMultiplicableT<Ty, T>> operator *(const Ty & Value, const TThickness<T> & Vec) { return { Value * Vec.Left, Value * Vec.Top, Value * Vec.Right, Value * Vec.Bottom }; }

	template<typename Ty, typename T> requires DivisibleC<Ty, T>
	TThickness<IsDivisibleT<Ty, T>> operator /(const Ty & Value, const TThickness<T> & Vec) { return { Value / Vec.Left, Value / Vec.Top, Value / Vec.Right, Value / Vec.Bottom }; }

	using ThicknessI = TThickness<int32>;
	using ThicknessU = TThickness<uint32>;
	using ThicknessF = TThickness<float32>;
	using ThicknessD = TThickness<float64>;

	static_assert(sizeof(ThicknessI) == 4 * sizeof(float));
	static_assert(sizeof(ThicknessD) == 8 * sizeof(int));
}
