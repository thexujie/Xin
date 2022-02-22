#include "PCH.h"
#include "Random.h"

namespace Xin
{
	template<IntegerC T>
	T FRandom::Rand()
	{
		std::default_random_engine generator(std::random_device().operator()());
		std::uniform_int_distribution<T> distribution;
		return distribution(generator);
	}

	template<FloatingPointC T>
	T FRandom::Rand()
	{
		std::default_random_engine generator(std::random_device().operator()());
		std::uniform_real_distribution<T> distribution;
		return distribution(generator);
	}

	template<IntegerC T>
	T FRandom::Rand(T MinValue, T MaxValue)
	{
		std::default_random_engine generator(std::random_device().operator()());
		std::uniform_int_distribution<T> distribution(MinValue, MaxValue);
		return distribution(generator);
	}

	template<FloatingPointC T>
	T FRandom::Rand(T MinValue, T MaxValue)
	{
		std::default_random_engine generator(std::random_device().operator()());
		std::uniform_real_distribution<T> distribution(MinValue, MaxValue);
		return distribution(generator);
	}

	template BASE_API int32 FRandom::Rand<int32>();
	template BASE_API uint32 FRandom::Rand<uint32>();
	template BASE_API int64_t FRandom::Rand<int64_t>();
	template BASE_API uint64 FRandom::Rand<uint64>();
	template BASE_API float32 FRandom::Rand<float32>();
	template BASE_API float64 FRandom::Rand<float64>();

	template BASE_API int32 FRandom::Rand<int32>(int32 MinValue, int32 MaxValue);
	template BASE_API uint32 FRandom::Rand<uint32>(uint32 MinValue, uint32 MaxValue);
	template BASE_API int64_t FRandom::Rand<int64_t>(int64_t MinValue, int64_t MaxValue);
	template BASE_API uint64 FRandom::Rand<uint64>(uint64 MinValue, uint64 MaxValue);
	template BASE_API float32 FRandom::Rand<float32>(float32 MinValue, float32 MaxValue);
	template BASE_API float64 FRandom::Rand<float64>(float64 MinValue, float64 MaxValue);
}
