#pragma once

#include "Types.h"

namespace Xin
{
	BASE_API uint64 CityHash64(const void * Data, uintx SizeInBytes);

	template<TriviallyCopyableC T>
	forceinline uint64 CityHash64(const T & Value)
	{
		return CityHash64(static_cast<const void *>(&Value), sizeof(T));
	}

	template<TriviallyCopyableC T>
	forceinline uint64 CityHash64(TView<T> View)
	{
		return CityHash64(static_cast<const void *>(View.Data), sizeof(T));
	}

	namespace HashPrivate
	{
#if XIN_X64
		inline constexpr uint64 FNVOffsetBasis = 14695981039346656037ULL;
		inline constexpr uint64 FNVPrime = 1099511628211ULL;
#else // XE_X64
		inline constexpr uint32 FNVOffsetBasis = 2166136261U;
		inline constexpr uint32 FNVPrime = 16777619U;
#endif // XE_X64

		forceinline uintx Fnv1aAppendBytes(uintx Seed, const uint8 * Bytes, uintx Size) noexcept
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				Seed ^= Bytes[Index];
				Seed *= FNVPrime;
			}
			return Seed;
		}

		template <TriviallyCopyAssignableC T>
		forceinline uintx Fnv1aAppendRange(uintx Seed, const T * First, const T * Last) noexcept
		{
			static_assert(IsTriviallyCopyAssignableV<T>, "Only trivial copy assignable types can be directly hashed.");
			const auto FirstByte = reinterpret_cast<const uint8 *>(First);
			const auto LastByte = reinterpret_cast<const uint8 *>(Last);
			return Fnv1aAppendBytes(Seed, FirstByte, static_cast<uintx>(LastByte - FirstByte));
		}

		template <TriviallyCopyAssignableC T>
		forceinline uintx Fnv1aAppendValue(uintx Seed, const T & Value) noexcept
		{
			static_assert(IsTriviallyCopyAssignableV<T>, "Only trivial copy assignable types can be directly hashed.");
			return Fnv1aAppendBytes(Seed, reinterpret_cast<const uint8 *>(&Value), sizeof(T));
		}

		template <TriviallyCopyAssignableC T>
		forceinline uintx HashRepresentation(const T & Value) noexcept
		{
			return Fnv1aAppendValue(FNVOffsetBasis, Value);
		}

		template <TriviallyCopyAssignableC T>
		forceinline uintx HashArrayRepresentation(const T * Value, const uintx Count) noexcept
		{
			static_assert(IsTriviallyCopyAssignableV<T>, "Only trivial copy assignable types can be directly hashed.");
			return Fnv1aAppendBytes(FNVOffsetBasis, reinterpret_cast<const uint8 *>(Value), Count * sizeof(T));
		}
	}

	forceinline constexpr uintx HashCombineValue(uintx HashCodeA, uintx HashCodeB)
	{
		HashCodeA ^= HashCodeB + 0x9e3779b9 + (HashCodeA << 6) + (HashCodeA >> 2);
		return HashCodeA;
	}

	template<typename T>
	struct THash;

	template<typename T>
	uintx Hash(const T & Value) = delete;

	// MemberHashAble  -->>  Value.Hash()
	template<typename T>
	concept MemberHashAbleC = requires(const T & Instance)
	{
		{ Instance.Hash() }->ConvertibleToC<uintx>;
	};
	template<typename T>
	inline constexpr bool IsMemberHashAbleV = false;
	template<MemberHashAbleC T>
	inline constexpr bool IsMemberHashAbleV<T> = true;
	template<typename T>
	struct IsMemberHashAble : BoolConstant<IsMemberHashAbleV<T>> {};

	// TemplateHashAble -->> THash<T>()(Value)
	template<typename T>
	concept TemplateHashAbleC = requires(T Instance)
	{
		{ THash<T>{}(Instance) }->ConvertibleToC<uintx>;
	};
	template<typename T>
	inline constexpr bool IsTemplateHashAbleV = false;
	template<TemplateHashAbleC T>
	inline constexpr bool IsTemplateHashAbleV<T> = true;
	template<typename T>
	struct IsTemplateHashAble : BoolConstant<IsTemplateHashAbleV<T>> {};

	template<TriviallyCopyAssignableC T> requires (!IsTemplateHashAbleV<T> && !IsMemberHashAbleV<T>)
	forceinline uintx Hash(const T & Value) { return HashPrivate::HashRepresentation<T>(Value); }

	// FunctionHashAble -->> Hash(Value)
	template<typename T>
	concept FunctionHashAbleC = requires(T Instance)
	{
		{ Hash(Instance) }->ConvertibleToC<uintx>;
	};
	template<typename T>
	inline constexpr bool IsFunctionHashAbleV = false;
	template<FunctionHashAbleC T>
	inline constexpr bool IsFunctionHashAbleV<T> = true;
	template<typename T>
	struct IsFunctionHashAble : BoolConstant<IsFunctionHashAbleV<T>> {};
	
	// HashAble
	template<typename T>
	concept HashAbleC = DisjunctionV<IsFunctionHashAble<T>, IsTemplateHashAble<T>, IsMemberHashAble<T>>;
	template<typename T>
	inline constexpr bool IsHashAbleV = false;
	template<HashAbleC T>
	inline constexpr bool IsHashAbleV<T> = true;
	template<typename T>
	struct IsHashAble : BoolConstant<IsHashAbleV<T>> {};
	template<typename T>
	inline constexpr bool IsNoneHashAbleV = !IsHashAbleV<T>;
	template<typename T>
	concept NoneHashAbleC = IsNoneHashAbleV<T>;


	template<MemberHashAbleC T>
	forceinline uintx Hash(const T & Value) { return Value.Hash(); }

	template<TemplateHashAbleC T> requires IsDefinedTypeV<THash<T>>
	forceinline uintx Hash(const T & Value) { return THash<T>()(Value); }

	template<HashAbleC T>
	forceinline uintx HashCombine(const T & Value) noexcept
	{
		return Hash(Value);
	}

	template<HashAbleC T1, HashAbleC T2>
	forceinline uintx HashCombine(const T1 & Value1, const T2 & Value2) noexcept
	{
		return HashCombineValue(Hash(Value1), Hash(Value2));
	}

	template<HashAbleC T, HashAbleC ...OthersT>
	forceinline uintx HashCombine(const T & Value, const OthersT & ...OtherValues) noexcept
	{
		return HashCombineValue(HashCombine(Value), HashCombine<OthersT...>(OtherValues...));
	}

	template<HashAbleC ...OthersT>
	forceinline uintx HashAppend(uintx HashCode, const OthersT & ...OtherValues) noexcept
	{
		return HashCombineValue(HashCode, HashCombine<OthersT...>(OtherValues...));
	}

	template<HashAbleC T>
	forceinline uintx HashAppendArray(uintx Seed, const T * Value, uintx Size)
	{
		for (uintx Index = 0; Index < Size; ++Index)
			Seed = HashAppend(Seed, Value[Index]);
		return Seed;
	}

	template<HashAbleC T>
	forceinline uintx HashArray(const T * Value, uintx Size)
	{
		return HashAppendArray(HashPrivate::FNVOffsetBasis, Value, Size);
	}

	template<HashAbleC T>
	struct THasher
	{
		forceinline uintx operator()(const T & Value) const noexcept { return HashCombine(Value); }
	};
}

