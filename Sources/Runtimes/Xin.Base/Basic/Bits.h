#pragma once

#include "Basic.Types.h"

namespace Xin
{
	template<typename ToT, typename FromT>
	constexpr ToT BitCast(const FromT & From)
	{
		static_assert(sizeof(FromT) == sizeof(ToT) && IsTriviallyCopyableV<FromT> && IsTriviallyCopyableV<ToT>);
		return std::bit_cast<ToT, FromT>(From);
	}

	template<typename T>
	constexpr inline uint32 BitWidth = sizeof(T) * 8;

	template<typename T>
	bool BitTest(T Value, uint32 Index)
	{
		if constexpr (IsSignedV<T>)
			return BitTest(BitCast<MakeUnsignedT<T>>(Value));
		if constexpr (IsAnyOfV<T, uint8, uint16>)
		{
			return BitTest(uint32(Value), Index);
		}
		else if constexpr (IsSameV<T, uint32>)
		{
			return !!_bittest(reinterpret_cast<const long32 *>(&Value), Index);
		}
		else if constexpr (IsSameV<T, uint64>)
		{
			return !!_bittest64(reinterpret_cast<const int64_t *>(&Value), Index);
		}
		else
			return false;
	}

	template<typename T>
	T BitCeil(const T & Value)
	{
		return std::bit_ceil(Value);
	}

	template<typename T>
	T BitFloor(const T & Value)
	{
		return std::bit_floor(Value);
	}

	//template<typename T>
	//constexpr uint32 BitFindForward(T Value)
	//{
	//	for (int BitIndex = 0; BitIndex < BitWidth<T>; ++BitIndex)
	//	{
	//		if (Value & (1 << BitIndex))
	//			return BitIndex;
	//	}
	//	return 0;
	//}

	// L -> H
	template<typename T>
	uint32 BitFindForward(T Value)
	{
		if constexpr (IsSignedV<T>)
			return BitFindForward(BitCast<MakeUnsignedT<T>>(Value));
		else
		{
			ulong32 Index = 0;
			if constexpr (IsAnyOfV<T, uint8, uint16, uint32>)
			{
				if (_BitScanForward(&Index, Value))
					return Index;
				return 0;
			}
			else if constexpr (IsSameV<T, uint64>)
			{
				if (_BitScanForward64(&Index, Value))
					return Index;
				return 0;
			}
			else
				return 0;
		}
	}

	template<typename T>
	uint32 BitFindReverse(T Value)
	{
		if constexpr (IsSignedV<T>)
			return BitFindReverse(BitCast<MakeUnsignedT<T>>(Value));
		else
		{
			ulong32 Index = 0;
			if constexpr (IsAnyOfV<T, uint8, uint16, uint32>)
			{
				if (_BitScanReverse(&Index, Value))
					return Index;
				return 0;
			}
			else if constexpr (IsSameV<T, uint64>)
			{
				if (_BitScanReverse64(&Index, Value))
					return Index;
				return 0;
			}
			else
				return 0;
		}
	}

	template<typename T>
	constexpr bool BitAll1(T Value)
	{
		if constexpr (IsSignedV<T>)
			return BitAll1(BitCast<MakeUnsignedT<T>>(Value));
		else
			return (Value & static_cast<T>(-1)) == static_cast<T>(-1);
	}

	template<typename T>
	constexpr bool BitAll0(T Value)
	{
		if constexpr (IsSignedV<T>)
			return BitAll0(BitCast<MakeUnsignedT<T>>(Value));
		else
			return !!Value;
	}

	template<typename T>
	constexpr uint32 BitCount1(T Value)
	{
		if constexpr (IsSignedV<T>)
			return BitCount1(BitCast<MakeUnsignedT<T>>(Value));
		else
			return std::popcount(Value);
	}

	template<typename T>
	constexpr uint32 BitCount0(T Value)
	{
		return sizeof(T) * ByteBits - BitCount1(Value);
	}

	template<typename T>
	constexpr uint32 BitCount1Leading(T Value)
	{
		return std::countl_one(Value);
	}

	template<typename T>
	constexpr uint32 BitCount1Trailing(T Value)
	{
		return std::countr_one(Value);
	}

	template<typename T>
	constexpr uint32 BitCount0Leading(T Value)
	{
		return std::countl_zero(Value);
	}

	template<typename T>
	constexpr uint32 BitCount0Trailing(T Value)
	{
		return std::countr_zero(Value);
	}

	class BASE_API FBitSet
	{
	public:
		using UnitT = uintx;
		static constexpr uintx UnitSize = sizeof(UnitT) * 8;
		using AllocatorT = TDefaultAllocator<UnitT>;

	public:
		AllocatorT Allocator;
		UnitT * Data = nullptr;
		uintx Size = 0;
		uintx Capacity = 0;

	public:
		FBitSet() = default;
		FBitSet(const FBitSet & That)
		{
			Data = Allocator.Alloc(RoundUp(That.Capacity, UnitSize));
			Size = That.Size;
			ArrayCopy(Data, That.Data, RoundUp(Size, UnitSize));
		}
		
		~FBitSet()
		{
			if (Data)
			{
				Allocator.Free(Data);
				Data = nullptr;
			}
			Size = 0;
			Capacity = 0;
		}
		
		FBitSet(FBitSet && That) noexcept
		{
			Data = That.Data;
			Size = That.Size;
			Capacity = That.Capacity;
			That.Data = nullptr;
			That.Size = 0;
			That.Capacity = 0;
		}

		FBitSet(uintx NewSize, bool Value)
		{
			Resize(NewSize, Value);
		}
		
		FBitSet(uintx NewSize, uintx NewCapacity, bool Value)
		{
			Resize(NewSize, NewCapacity, Value);
		}

		void Resize(uintx NewSize, bool Value)
		{
			if (NewSize / UnitSize > Capacity / UnitSize)
			{
				uintx NewCapacity = Capacity ? Capacity : 1;
				while (NewCapacity < NewSize)
					NewCapacity *= 2;

				UnitT * NewData = Allocator.Alloc(NewCapacity / UnitSize);
				MemoryCopy(NewData, Data, Size / UnitSize);
				Allocator.Free(Data);
				Data = NewData;
				Capacity = NewCapacity;
			}

			if (NewSize > Size)
			{
				uintx Index = Size;
				uintx Count = NewSize - Size;
				Size = NewSize;
				SetAt(Index, Count, Value);
			}
			else
				Size = NewSize;
		}

		void Resize(uintx NewSize, uintx NewCapacity, bool Value)
		{
			if (NewCapacity && NewCapacity >= NewSize)
				NewCapacity = RoundUp(NewCapacity, UnitSize) * UnitSize;
			if (NewCapacity && NewCapacity >= NewSize)
			{
				Allocator.Free(Data);
				Data = Allocator.Alloc(NewCapacity / UnitSize);
				Capacity = NewCapacity;
			}

			Size = NewSize;
			SetAt(0, Size, Value);
		}
		
		uintx GetSize() const { return Size; }
		bool IsEmpty() const { return Size == 0; }

		void Add(bool Value)
		{
			if (Size + 1 >= Capacity)
			{
				uintx NewCapacity = RoundUp(Max<uintx>(Capacity * 2, UnitSize), UnitSize) * UnitSize;
				while (NewCapacity < Size + 1)
					NewCapacity *= 2;

				UnitT * NewData = Allocator.Alloc(RoundUp(NewCapacity, UnitSize));
				ArrayCopy(NewData, Data, RoundUp(Size, UnitSize));

				Allocator.Free(Data);
				Data = NewData;
				Capacity = NewCapacity;
			}

			uintx Index = Size++;
			SetAt(Index, Value);
		}

		void SetAt(uintx Index, bool Value)
		{
			if (Index >= Size)
				throw EError::OutOfBound;

			UnitT & Unit = Data[Index / UnitSize];
			uintx BitIndex = Index % UnitSize;
			SetBitIndex(Unit, BitIndex, Value);
		}

		void SetAt(uintx Index, uintx Count, bool Value)
		{
			if (Index + Count > Size)
				throw EError::OutOfBound;

			uintx UnitHeadIndex = Index / UnitSize;
			uintx UnitTailIndex = (Index + Count) / UnitSize;

			uintx BitHeadIndex = Index % UnitSize;
			uintx BitTailIndex = (Index + Count) % UnitSize;
			if (UnitHeadIndex == UnitTailIndex)
			{
				UnitT Mask = ~((UnitT(1) << BitHeadIndex) - 1);
				Mask &= ((UnitT(1) << BitTailIndex) - 1);

				if (Value)
					Data[UnitHeadIndex] |= Mask;
				else
					Data[UnitHeadIndex] &= ~Mask;
				return;
			}
			
			if (BitHeadIndex)
			{
				if (Value)
					Data[UnitHeadIndex] |= ~((UnitT(1) << BitHeadIndex) - 1);
				else
					Data[UnitHeadIndex] &= ((UnitT(1) << BitHeadIndex) - 1);
				++UnitHeadIndex;
			}
			
			for (uintx UnitIndex = UnitHeadIndex; UnitIndex < UnitTailIndex; ++UnitIndex)
			{
				Data[UnitIndex] = Value ? UnitT(-1) : 0;
			}

			if (BitTailIndex)
			{
				if (Value)
					Data[UnitTailIndex] |= ((UnitT(1) << BitTailIndex) - 1);
				else
					Data[UnitTailIndex] &= ~((UnitT(1) << BitTailIndex) - 1);
			}
		}

		bool GetAt(uintx Index) const
		{
			UnitT & Unit = Data[Index / UnitSize];
			uintx BitIndex = Index % UnitSize;
			return GetBitAt(Unit, BitIndex);
		}

		bool operator [](uintx Index) const { return GetAt(Index); }
	};

	template<uintx TotalBits>
	class TBitset
	{
	public:
		using WordT = ConditionalT<TotalBits <= sizeof(uint32) * ByteBits, uint32, uint64>;
		static constexpr uintx Size = TotalBits;
		static constexpr uintx WordBits = ByteBits * sizeof(WordT);
		static constexpr uintx NumWords = TotalBits == 0 ? 0 : (TotalBits - 1) / WordBits;

		WordT Data[NumWords + 1];

		auto operator<=> (const TBitset &) const = default;

		class Reference
		{
			friend TBitset<TotalBits>;

		private:
			Reference() noexcept : Container(nullptr), Index(0) {}

			Reference(TBitset<TotalBits> & Container, uintx Index) : Container(&Container), Index(Index) {}

			TBitset<TotalBits> * Container;
			uintx Index;

		public:
			~Reference() noexcept {}

			Reference & operator=(bool Value) noexcept
			{
				Container->InternalSet(Index, Value);
				return *this;
			}

			Reference & operator=(const Reference & Another) noexcept
			{
				Container->InternalSet(Index, static_cast<bool>(Another));
				return *this;
			}

			Reference & Flip() noexcept
			{
				Container->InternalFlip(Index);
				return *this;
			}

			bool operator~() const noexcept
			{
				return !Container->_Subscript(Index);
			}

			operator bool() const noexcept
			{
				return Container->_Subscript(Index);
			}
		};

		constexpr bool _Subscript(uintx Index) const
		{
			return (Data[Index / WordBits] & (WordT{ 1 } << Index % WordBits)) != 0;
		}

		constexpr bool operator[](uintx Index) const
		{
			return _Subscript(Index);
		}

		Reference operator[](uintx Index)
		{
			return Reference(*this, Index);
		}

		constexpr TBitset() noexcept : Data() {}

		static constexpr bool NeedMask = TotalBits < ByteBits * sizeof(uint64);

		static constexpr uint64 Mask = (1ULL << (NeedMask ? TotalBits : 0)) - 1ULL;

		constexpr TBitset(uint64 UI64Value) noexcept : Data{ static_cast<WordT>(NeedMask ? UI64Value & Mask : UI64Value) } {}

		explicit TBitset(FStringV String, char8_t FalseChar = '0', char8_t TrueChar = '1')
		{
			uintx WordIndex = 0;
			if (String.Size != 0)
			{
				uintx BitIndex = 0;
				const char8_t * Current = String.Data + String.Size;
				WordT CurrentWord = 0;
				do
				{
					--Current;
					char8_t Ch = *Current;
					CurrentWord |= static_cast<WordT>(Ch == TrueChar) << BitIndex;

					if (++BitIndex == WordBits)
					{
						Data[WordIndex] = CurrentWord;
						++WordIndex;
						CurrentWord = 0;
						BitIndex = 0;
					}
				}
				while (Current != String.Data);

				if (BitIndex != 0)
				{
					Data[WordIndex] = CurrentWord;
					++WordIndex;
				}
			}

			for (; WordIndex <= NumWords; ++WordIndex)
			{
				Data[WordIndex] = 0;
			}
		}

		TBitset & operator&=(const TBitset & That) noexcept
		{
			for (uintx _Wpos = 0; _Wpos <= NumWords; ++_Wpos)
			{
				Data[_Wpos] &= That.Data[_Wpos];
			}

			return *this;
		}

		TBitset & operator|=(const TBitset & That) noexcept
		{
			for (uintx _Wpos = 0; _Wpos <= NumWords; ++_Wpos)
			{
				Data[_Wpos] |= That.Data[_Wpos];
			}

			return *this;
		}

		TBitset & operator^=(const TBitset & That) noexcept
		{
			for (uintx _Wpos = 0; _Wpos <= NumWords; ++_Wpos)
			{
				Data[_Wpos] ^= That.Data[_Wpos];
			}

			return *this;
		}

		TBitset & operator<<=(uintx Index) noexcept
		{
			const auto WordShift = static_cast<ptrdiff_t>(Index / WordBits);
			if (WordShift != 0)
			{
				for (ptrdiff_t WordIndex = NumWords; 0 <= WordIndex; --WordIndex)
				{
					Data[WordIndex] = WordShift <= WordIndex ? Data[WordIndex - WordShift] : 0;
				}
			}

			if ((Index %= WordBits) != 0)
			{
				for (ptrdiff_t WordIndex = NumWords; 0 < WordIndex; --WordIndex)
				{
					Data[WordIndex] = (Data[WordIndex] << Index) | (Data[WordIndex - 1] >> (WordBits - Index));
				}

				Data[0] <<= Index;
			}
			InternalTrim();
			return *this;
		}

		TBitset & operator>>=(uintx Index) noexcept
		{
			// shift right by _Pos, first by words then by bits
			const auto WordShift = static_cast<ptrdiff_t>(Index / WordBits);
			if (WordShift != 0)
			{
				for (ptrdiff_t WordIndex = 0; WordIndex <= NumWords; ++WordIndex)
				{
					Data[WordIndex] = WordShift <= NumWords - WordIndex ? Data[WordIndex + WordShift] : 0;
				}
			}

			if ((Index %= WordBits) != 0)
			{
				// 0 < _Pos < _Bitsperword, shift by bits
				for (ptrdiff_t WordIndex = 0; WordIndex < NumWords; ++WordIndex)
				{
					Data[WordIndex] = (Data[WordIndex] >> Index) | (Data[WordIndex + 1] << (WordBits - Index));
				}

				Data[NumWords] >>= Index;
			}
			return *this;
		}

		TBitset & SetAll(bool Value) noexcept
		{
			std::memset(&Data, Value ? 0xFF : 0x00, sizeof(Data));
			InternalTrim();
			return *this;
		}

		TBitset & SetAt(uintx Index, bool Value = true)
		{
			return InternalSet(Index, Value);
		}

		TBitset operator~() const noexcept
		{
			TBitset Temp = *this;
			Temp.Flip();
			return Temp;
		}

		TBitset & Flip() noexcept
		{
			// flip all bits
			for (uintx _Wpos = 0; _Wpos <= NumWords; ++_Wpos)
			{
				Data[_Wpos] = ~Data[_Wpos];
			}

			InternalTrim();
			return *this;
		}

		TBitset & FlipAt(uintx Index)
		{
			return InternalFlip(Index);
		}

		uint32 ToUInt32() const
		{
			constexpr bool _Bits_zero = TotalBits == 0;
			constexpr bool _Bits_small = TotalBits <= 32;
			constexpr bool _Bits_large = TotalBits > 64;
			if constexpr (_Bits_zero)
			{
				return 0;
			}
			else if constexpr (_Bits_small)
			{
				return static_cast<unsigned long>(Data[0]);
			}
			else
			{
				if constexpr (_Bits_large)
				{
					for (uintx _Idx = 1; _Idx <= NumWords; ++_Idx)
					{
						if (Data[_Idx] != 0)
						{
							_Xoflo(); // fail if any high-order words are nonzero
						}
					}
				}

				if (Data[0] > ULONG_MAX)
				{
					_Xoflo();
				}

				return static_cast<unsigned long>(Data[0]);
			}
		}

		uint64 ToUInt64() const
		{
			constexpr bool _Bits_zero = TotalBits == 0;
			constexpr bool _Bits_large = TotalBits > 64;
			if constexpr (_Bits_zero)
			{
				return 0;
			}
			else
			{
				if constexpr (_Bits_large)
				{
					for (uintx _Idx = 1; _Idx <= NumWords; ++_Idx)
					{
						if (Data[_Idx] != 0)
						{
							_Xoflo(); // fail if any high-order words are nonzero
						}
					}
				}

				return Data[0];
			}
		}

		uintx CountNoneZero() const noexcept
		{
			// count number of set bits
			const char * const _Bitsperbyte = "\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4"
				"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
				"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
				"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
				"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
				"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
				"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
				"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
				"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
				"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
				"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
				"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
				"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
				"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
				"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
				"\4\5\5\6\5\6\6\7\5\6\6\7\6\7\7\x8";
			const uint8 * Curr = &reinterpret_cast<const uint8 &>(Data);
			const uint8 * const End = Curr + sizeof(Data);
			uintx _Val = 0;
			for (; Curr != End; ++Curr)
			{
				_Val += _Bitsperbyte[*Curr];
			}

			return _Val;
		}

		bool operator==(const TBitset & That) const noexcept
		{
			return std::memcmp(&Data[0], &That.Data[0], sizeof(Data)) == 0;
		}

		bool GetAt(uintx Index) const
		{
			return _Subscript(Index);
		}

		bool Any() const noexcept
		{
			for (uintx WordIndex = 0; WordIndex <= NumWords; ++WordIndex)
			{
				if (Data[WordIndex] != 0)
				{
					return true;
				}
			}

			return false;
		}

		bool None() const noexcept
		{
			return !Any();
		}

		bool All() const noexcept
		{
			if constexpr (TotalBits == 0)
				return true;

			constexpr bool NoPadding = TotalBits % WordBits == 0;
			for (uintx WordIndex = 0; WordIndex < NumWords + NoPadding; ++WordIndex)
			{
				if (Data[WordIndex] != ~static_cast<WordT>(0))
				{
					return false;
				}
			}

			return NoPadding || Data[NumWords] == (static_cast<WordT>(1) << (TotalBits % WordBits)) - 1;
		}

		TBitset operator<<(uintx _Pos) const noexcept
		{
			TBitset _Tmp = *this;
			_Tmp <<= _Pos;
			return _Tmp;
		}

		TBitset operator>>(uintx _Pos) const noexcept
		{
			TBitset _Tmp = *this;
			_Tmp >>= _Pos;
			return _Tmp;
		}

		WordT _Getword(uintx _Wpos) const noexcept
		{
			return Data[_Wpos];
		}

	private:

		void InternalTrim() noexcept
		{
			if constexpr (TotalBits == 0 || TotalBits % WordBits != 0)
			{
				Data[NumWords] &= (WordT{ 1 } << TotalBits % WordBits) - 1;
			}
		}

		TBitset & InternalSet(uintx Index, bool Value) noexcept
		{
			WordT & SelectedWord = Data[Index / WordBits];
			WordT WordMask = WordT{ 1 } << Index % WordBits;
			if (Value)
			{
				SelectedWord |= WordMask;
			}
			else
			{
				SelectedWord &= ~WordMask;
			}

			return *this;
		}

		TBitset & InternalFlip(uintx Index) noexcept
		{
			// flip bit at _Pos, no checking
			Data[Index / WordBits] ^= WordT{ 1 } << Index % WordBits;
			return *this;
		}

		[[noreturn]] void _Xinv() const {
			//_Xinvalid_argument("invalid bitset char");
		}

		[[noreturn]] void _Xoflo() const {
			//_Xoverflow_error("bitset overflow");
		}

		[[noreturn]] void _Xran() const {
			//_Xout_of_range("invalid bitset position");
		}
	};

	template<uintx TotalBits>
	TBitset<TotalBits> operator&(const TBitset<TotalBits> & Left, const TBitset<TotalBits> & Right) noexcept
	{
		TBitset<TotalBits> _Ans = Left;
		_Ans &= Right;
		return _Ans;
	}

	template<uintx TotalBits>
	TBitset<TotalBits> operator|(const TBitset<TotalBits> & Left, const TBitset<TotalBits> & Right) noexcept
	{
		TBitset<TotalBits> _Ans = Left;
		_Ans |= Right;
		return _Ans;
	}

	template<uintx TotalBits>
	TBitset<TotalBits> operator^(const TBitset<TotalBits> & Left, const TBitset<TotalBits> & Right) noexcept
	{
		TBitset<TotalBits> _Ans = Left;
		_Ans ^= Right;
		return _Ans;
	}

	template<uintx TotalBits>
	struct THash<TBitset<TotalBits>>
	{
		uintx operator()(const TBitset<TotalBits> & Value) const noexcept
		{
			return std::_Hash_representation(Value.Data);
		}
	};

}
