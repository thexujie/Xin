#pragma once

#include "Basic.Types.h"

namespace Xin
{
	template<typename T>
	constexpr void ArrayZero(T * Destination, uintx Count)
	{
		if constexpr (IsConstantEvaluated())
		{
			for (uintx Index = 0; Index < Count; ++Index)
				Destination[Index] = {};
		}
		else
		{
			MemoryZero(Destination, sizeof(T) * Count);
		}
	}

	template<typename T>
	constexpr void ArrayCopy(T * Destination, const T * Source, uintx Count)
	{
		if constexpr (IsConstantEvaluated())
		{
			for (uintx Index = 0; Index < Count; ++Index)
				Destination[Index] = Source[Index];
		}
		else
		{
			if constexpr (IsTriviallyConstructibleV<T>)
			{
				MemoryCopy(Destination, Source, sizeof(T) * Count);
			}
			else
			{
				for (uintx Index = 0; Index < Count; ++Index)
					Destination[Index] = Source[Index];
			}
		}
	}

	template<typename T, uintx ArraySize>
	constexpr void ArrayCopy(T (& Destination)[ArraySize], const T (& Source)[ArraySize])
	{
		if constexpr (IsConstantEvaluated())
		{
			for (uintx Index = 0; Index < ArraySize; ++Index)
				Destination[Index] = Source[Index];
		}
		else
		{
			if constexpr (IsTriviallyConstructibleV<T>)
				MemoryCopy(Destination, Source, sizeof(T) * ArraySize);
			else
			{
				for (uintx Index = 0; Index < ArraySize; ++Index)
					Destination[Index] = Source[Index];
			}
		}
	}

	template<typename T>
	constexpr void ArrayFill(T * Destination, uintx Count, const T & Value)
	{
		if constexpr (IsConstantEvaluated())
		{
			for (uintx Index = 0; Index < Count; ++Index)
				Destination[Index] = Value;
		}
		else
		{
			if constexpr (IsTriviallyConstructibleV<T>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					MemoryCopy(Destination + Index, &Value, sizeof(T));
			}
			else
			{
				for (uintx Index = 0; Index < Count; ++Index)
					Destination[Index] = Value;
			}
		}
	}

	template<typename T, sizet ArraySize>
	constexpr void ArrayFill(T (& Destination)[ArraySize], const T & Value)
	{
		if constexpr (IsTriviallyConstructibleV<T>)
		{
			for (uintx Index = 0; Index < ArraySize; ++Index)
				MemoryCopy(Destination + Index, &Value, sizeof(T));
		}
		else
		{
			for (uintx Index = 0; Index < ArraySize; ++Index)
				Destination[Index] = Value;
		}
	}

	enum class EInitializeMode
	{
		Default = 0,
		Construct,
		Zero,
		None,
	};

	template<typename T>
	constexpr bool IsEqualV(T a, T b)
	{
		return a == b;
	}

	enum class EElementFill
	{
		Fill = 0,
	};

	template<typename T, uintx ArraySize>
	class TArray
	{
	public:
		T Data[ArraySize];
		static constexpr uintx Size = ArraySize;

	public:
		//#if 0
		constexpr TArray() = default;
		constexpr ~TArray() = default;
		constexpr TArray(const TArray &) = default;
		constexpr TArray(TArray &&) = default;

		template<uintx ArraySizeY>
		constexpr TArray(const TArray<T, ArraySizeY> & That)
		{
			uintx Count = Min(ArraySize, ArraySizeY);
			for (uintx Index = 0; Index < Count; ++Index)
				Data[Index] = That[Index];

			if constexpr (IsDefaultConstructibleV<T> && IsCopyAssignableV<T>)
			{
				for (uintx Index = Count; Index < ArraySize; ++Index)
					Data[Index] = {};
			}
		}

		constexpr TArray(const T & Value, EElementFill ArrayInitialize)
		{
			if (ArrayInitialize == EElementFill::Fill)
			{
				if constexpr (IsCopyAssignableV<T>)
				{
					for (uintx Index = 0; Index < ArraySize; ++Index)
						Data[Index] = Value;
				}
			}
		}

		constexpr TArray(TInitializerList<T> InitializerList)
		{
			uintx Count = Min(ArraySize, InitializerList.size());
			for (uintx Index = 0; Index < Count; ++Index)
				Data[Index] = InitializerList.begin()[Index];

			if constexpr (IsDefaultConstructibleV<T> && IsCopyAssignableV<T>)
			{
				for (uintx Index = Count; Index < ArraySize; ++Index)
					Data[Index] = {};
			}
		}

		constexpr TArray(TView<T> Elements)
		{
			uintx Count = Min(ArraySize, Elements.Size);
			for (uintx Index = 0; Index < Count; ++Index)
				Data[Index] = Elements[Index];

			if constexpr (IsDefaultConstructibleV<T> && IsCopyAssignableV<T>)
			{
				for (uintx Index = Count; Index < ArraySize; ++Index)
					Data[Index] = {};
			}
		}

		//#endif
		constexpr auto operator <=>(const TArray &) const noexcept = default;
		constexpr TArray & operator =(const TArray &) = default;

		template<uintx AnotherArraySize>
		constexpr TArray & operator =(const TArray<T, AnotherArraySize> & Another)
		{
			for (uintx Index = 0; Index < Min(ArraySize, AnotherArraySize); ++Index)
				Data[Index] = Another[Index];
			return *this;
		}

		constexpr uintx GetSize() const { return ArraySize; }
		constexpr T * GetData() const { return Data; }

		constexpr TArray & operator =(TView<T> Another)
		{
			ArrayCopy(Data, Another.Data, Min(Size, Another.Size));
			for (uintx Index = Another.Size; Index < ArraySize; ++Index)
				Data[Index] = {};
			return *this;
		}

		constexpr TArray & operator =(TInitializerList<T> InitializerList)
		{
			ArrayCopy(Data, InitializerList.begin(), Min(Size, InitializerList.size()));
			for (uintx Index = InitializerList.size(); Index < ArraySize; ++Index)
				Data[Index] = {};
			return *this;
		}

		constexpr void Clear()
		{
			for (uintx Index = 0; Index < ArraySize; ++Index)
				Data[Index] = {};
		}

		constexpr void Repeat(const T & Value)
		{
			ArrayFill(Data, ArraySize, Value);
		}

		constexpr TView<T> View(uintx Index = 0, uintx Count = NullIndex) const
		{
			if (Count == NullIndex)
				return TView<T> { Data + Index, ArraySize - Index };

			return TView<T> { Data + Index, Count - Index };
		}

		constexpr TSlice<T> Slice(uintx Index = 0, uintx Count = NullIndex)
		{
			if (Count == NullIndex)
				return TSlice<T> { Data + Index, ArraySize - Index };

			return TSlice<T> { Data + Index, Count - Index };
		}

		constexpr operator TView<T>() const
		{
			return TView<T>(Data, Size);
		}

		constexpr operator TSlice<T>() const
		{
			return TSlice<T>(ConstCast<T>(Data), Size);
		}

		template<EnumClassC IndexT>
		constexpr T & operator [](IndexT Index) { return Data[static_cast<uintx>(Index)]; }

		template<EnumClassC IndexT>
		constexpr const T & operator [](IndexT Index) const { return Data[static_cast<uintx>(Index)]; }

		constexpr T & operator [](uintx Index) { return Data[Index]; }
		constexpr const T & operator [](uintx Index) const { return Data[Index]; }

		constexpr const T * Begin() const { return Data; }
		constexpr const T * End() const { return Data + ArraySize; }
		constexpr T * Begin() { return Data; }
		constexpr T * End() { return Data + ArraySize; }

		T * Find(const T & Value) requires EqualableC<T>
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Data[Index] == Value)
					return Data + Index;
			}
			return End();
		}

		const T * Find(const T & Value) const requires EqualableC<T>
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Data[Index] == Value)
					return Data + Index;
			}
			return End();
		}

		bool Contains(const T & Value) const requires EqualableC<T>
		{
			return Find(Value) != End();
		}

		template<typename ComparerT>
		auto FindBy(this auto && This, const ComparerT & Comparer)
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Comparer(This.Data[Index]))
					return This.Data + Index;
			}
			return This.End();
		}

		template<typename ComparerT>
		uintx FindIndexBy(this auto && This, const ComparerT & Comparer)
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Comparer(This.Data[Index]))
					return Index;
			}
			return NullIndex;
		}
	};

	template<typename T, size_t ArraySize>
	auto begin(TArray<T, ArraySize> & Array)
	{
		return Array.Begin();
	}

	template<typename T, size_t ArraySize>
	auto end(TArray<T, ArraySize> & Array)
	{
		return Array.End();
	}

	template<typename T, size_t ArraySize>
	auto begin(const TArray<T, ArraySize> & Array)
	{
		return Array.Begin();
	}

	template<typename T, size_t ArraySize>
	auto end(const TArray<T, ArraySize> & Array)
	{
		return Array.End();
	}

	template<HashAbleC T, uintx ArraySize>
	struct THash<TArray<T, ArraySize>>
	{
		uintx operator ()(const TArray<T, ArraySize> & Array) const noexcept
		{
			uintx HashCode = HashCombine(Array.GetSize());
			for (uintx Index = 0; Index < Array.GetSize(); ++Index)
				HashCode = HashAppend(HashCode, Array[Index]);
			return HashCode;
		}
	};
}
