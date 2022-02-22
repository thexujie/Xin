#pragma once

#include "Basic.Types.h"

namespace Xin
{
	template<typename T>
	class TFraction
	{
	public:
		T Num = 0;
		T Den = 1;

	public:
		constexpr TFraction() = default;
		constexpr TFraction(const TFraction &) = default;
		TFraction & operator =(const TFraction &) = default;
		auto operator<=>(const TFraction &) const noexcept = default;
		
#pragma warning (disable: 4056)
		constexpr TFraction(float Value) : Num(T(Value * 1000)), Den(1000) {}
#pragma warning (default : 4056)
		operator float() const { return static_cast<float>(Num) / Den; }
	};

	using FFraction = TFraction<int32>;
}
