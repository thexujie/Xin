#pragma once

#include "Types.h"

namespace Xin
{
	inline float32 Floor(float32 Value) { return floorf(Value); }
	inline float32 Ceil(float32 Value) { return ceilf(Value); }
	inline float32 Round(float32 Value) { return roundf(Value); }
	inline int32 FloorI32(float32 Value) { return (int32)floorf(Value); }
	inline uint32 FloorU32(float32 Value) { return (uint32)floorf(Value); }
	inline int64 FloorI64(float32 Value) { return (int64)floorf(Value); }
	inline uint64 FloorU64(float32 Value) { return (uint64)floorf(Value); }

	inline float64 Floor(float64 Value) { return floor(Value); }
	inline float64 Ceil(float64 Value) { return ceil(Value); }
	inline float64 Round(float64 Value) { return round(Value); }
	inline int32 FloorI32(float64 Value) { return (int32)floor(Value); }
	inline uint32 FloorU32(float64 Value) { return (uint32)floor(Value); }
	inline int64 FloorI64(float64 Value) { return (int64)floor(Value); }
	inline uint64 FloorU64(float64 Value) { return (uint64)floor(Value); }

	inline float32 Fmod(float32 Value, float32 Base) { return fmodf(Value, Base); }
	inline float64 Fmod(float64 Value, float64 Base) { return fmod(Value, Base); }

	inline float32 Asin(float32 Value) { return asinf(Value); }
	inline float64 Asin(float64 Value) { return asin(Value); }

	inline float32 Acos(float32 Value) { return acosf(Value); }
	inline float64 Acos(float64 Value) { return acos(Value); }

	inline float32 Atan(float32 Value) { return atanf(Value); }
	inline float64 Atan(float64 Value) { return atan(Value); }
	inline float32 Atan2(float32 ValueA, float32 ValueB) { return atan2f(ValueA, ValueB); }
	inline float64 Atan2(float64 ValueA, float64 ValueB) { return atan2(ValueA, ValueB); }

	inline float32 Sqrt(int32 Value)
	{
		return sqrtf((float32)Value);
	}

	inline float32 Sqrt(uint32 Value)
	{
		return sqrtf((float32)Value);
	}

	inline float32 Sqrt(float32 Value)
	{
		return sqrtf(Value);
	}

	inline float64 Sqrt(float64 Value)
	{
		return sqrt(Value);
	}

	// return (-180, 180]
	inline float32 NormalizeRotateAngle(float32 Angle)
	{
		// returns Angle in the range [0,360)
		Angle = Fmod(Angle, 360);
		if (Angle > 180)
			Angle -= 360;
		return Angle;
	}

	template<IntegerC T>
	constexpr T DivideCeil(T Dividend, T Divisor)
	{
		return (Dividend + Divisor - 1) / Divisor;
	}

	template<IntegerC T>
	constexpr T DivideFloor(T Dividend, T Divisor)
	{
		return Dividend / Divisor;
	}

	template<IntegerC T>
	constexpr T DivideRound(T Dividend, T Divisor)
	{
		return (Dividend + Divisor / 2) / Divisor;
	}

	template<FloatingPointC T>
	bool AreClose(T ValueA, T ValueB, T ClosingEpsilon = Epslion<T>)
	{
		if (ValueA == ValueB)
			return true;
		T TheEpslion = (Abs(ValueA) + Abs(ValueB) + T(10.0)) * ClosingEpsilon;
		T Delta = ValueA - ValueB;
		return (-TheEpslion < Delta) && (TheEpslion > Delta);
	}

	template<FloatingPointC T>
	bool LessThanOrClose(T ValueA, T ValueB, T ClosingEpsilon = Epslion<T>)
	{
		return ValueA < ValueB || AreClose(ValueA, ValueB, ClosingEpsilon);
	}

	template<FloatingPointC T>
	bool LessThanOrCloseZero(T ValueA, T ClosingEpsilon = Epslion<T>)
	{
		return LessThanOrClose<T>(ValueA, 0, ClosingEpsilon);
	}
}
