#pragma once

#include "Xin.UI.Types.h"

namespace Xin::UI
{
	enum class EUnit : uint32
	{
		Auto = 0,
		Percent,
		Absolute,// in pixels .eg
	};

	struct UI_API FDimen
	{
	public:
		constexpr FDimen() noexcept = default;
		constexpr FDimen(const FDimen &) noexcept = default;
		constexpr FDimen(FDimen &&) noexcept = default;
		constexpr FDimen & operator =(const FDimen &) noexcept = default;
		bool operator==(const FDimen & Other) const noexcept { return Equal(Unit, Other.Unit, Value, Other.Value); };
		auto operator<=>(const FDimen & Other) const noexcept { return Compare(Unit, Other.Unit, Value, Other.Value); };

		constexpr FDimen(dimenx Value) : Value(Value), Unit(EUnit::Absolute) {}
		constexpr FDimen(dimenx Value, EUnit Unit) : Value(Value), Unit(Unit) {}

		constexpr FDimen operator +(const FDimen & Other) const { return { Value + Other.Value, Unit }; }
		constexpr FDimen & operator +=(const FDimen & Other) { Value += Other.Value; return *this; }
		constexpr FDimen operator -(const FDimen & Other) const { return { Value - Other.Value, Unit }; }
		constexpr FDimen & operator -=(const FDimen & Other) { Value -= Other.Value; return *this; }
		constexpr FDimen operator *(const FDimen & Other) const { return { Value * Other.Value, Unit }; }
		constexpr FDimen & operator *=(const FDimen & Other) { Value *= Other.Value; return *this; }
		constexpr FDimen operator /(const FDimen & Other) const { return { Value / Other.Value, Unit }; }
		constexpr FDimen & operator /=(const FDimen & Other) { Value /= Other.Value; return *this; }

		constexpr FDimen operator +(dimenx Other) const { return { Value + Other, Unit }; }
		constexpr FDimen & operator +=(dimenx Other) { Value += Other; return *this; }
		constexpr FDimen operator -(dimenx Other) const { return { Value - Other, Unit }; }
		constexpr FDimen & operator -=(dimenx Other) { Value -= Other; return *this; }
		constexpr FDimen operator *(dimenx Other) const { return { Value * Other, Unit }; }
		constexpr FDimen & operator *=(dimenx Other) { Value *= Other; return *this; }
		constexpr FDimen operator /(dimenx Other) const { return { Value / Other, Unit }; }
		constexpr FDimen & operator /=(dimenx Other) { Value /= Other; return *this; }

		forceinline bool IsAuto() const { return IsNaN(Value) || Unit == EUnit::Auto; }
		forceinline bool IsPercent() const { return !IsNaN(Value) && Unit == EUnit::Percent; }
		forceinline bool IsAbsolute() const { return !IsNaN(Value) && Unit == EUnit::Absolute; }

		dimenx GetActualValue(dimenx Reference) const
		{
			if (Unit == EUnit::Percent)
				return Reference * Value;
			return Value;
		}

	public:
		dimenx Value;
		EUnit Unit;

	public:
		static FDimen Auto;
		static FDimen Percent10;
		static FDimen Percent20;
		static FDimen Percent25;
		static FDimen Percent30;
		static FDimen Percent40;
		static FDimen Percent50;
		static FDimen Percent60;
		static FDimen Percent70;
		static FDimen Percent75;
		static FDimen Percent80;
		static FDimen Percent90;
		static FDimen Percent100;

		static FDimen Percent(dimenx Perentage) { return { Perentage, EUnit::Percent }; }
	};


	constexpr FDimen operator""pct(long double Value)
	{
		return FDimen(dimenx(Value * 0.01), EUnit::Percent);
	}

	constexpr FDimen operator""pct(uint64 Value) noexcept
	{
		return FDimen(dimenx(Value * 0.01), EUnit::Percent);
	}

	constexpr FDimen atsz = { NaN<dimenx>, EUnit::Auto };
}

namespace Xin
{
	forceinline bool IsNaN(UI::FDimen Value) { return IsNaN(Value.Value); }

	inline UI::FDimen Max(const UI::FDimen & ValueA, const UI::FDimen & ValueB)
	{
		return UI::FDimen { Max(ValueA.Value, ValueB.Value), ValueA.Unit };
	}

	inline UI::FDimen Min(const UI::FDimen & ValueA, const UI::FDimen & ValueB)
	{
		return UI::FDimen { Min(ValueA.Value, ValueB.Value), ValueA.Unit };
	}

	inline UI::FDimen Sqrt(const UI::FDimen & Value)
	{
		return UI::FDimen { Sqrt(Value.Value), Value.Unit };
	}

	template<>
	constexpr UI::FDimen Infinity<UI::FDimen> = UI::FDimen(Infinity<dimenx>, UI::EUnit::Absolute);

	template<>
	constexpr UI::FDimen NaN<UI::FDimen> = UI::FDimen(NaN<dimenx>, UI::EUnit::Auto);

	template<>
	constexpr UI::FDimen SignalingNaN<UI::FDimen> = UI::FDimen(SignalingNaN<dimenx>, UI::EUnit::Auto);
}

namespace Xin::UI
{
	using FDimen2 = TVec2<FDimen>;
	using FDimen3 = TVec3<FDimen>;
	using FDimen4 = TVec4<FDimen>;

	forceinline bool AnyNaN(FDimen2 Value) { return IsNaN(Value.X.Value) || IsNaN(Value.Y.Value); }
	forceinline bool AllNaN(FDimen2 Value) { return IsNaN(Value.X.Value) && IsNaN(Value.Y.Value); }

	inline TVec2<dimenx> GetActualValue(const FDimen2 & Dimen, TVec2<dimenx> Reference)
	{
		return { Dimen.X.GetActualValue(Reference.X), Dimen.Y.GetActualValue(Reference.Y) };
	}
}
