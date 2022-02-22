#pragma once

#include "Basic.Types.h"

namespace Xin
{
	struct BASE_API FRandom
	{
	public:
		template<IntegerC T>
		static T Rand();

		template<FloatingPointC T>
		static T Rand();

		template<IntegerC T>
		static T Rand(T MinValue, T MaxValue);

		template<FloatingPointC T>
		static T Rand(T MinValue, T MaxValue);
	};
}