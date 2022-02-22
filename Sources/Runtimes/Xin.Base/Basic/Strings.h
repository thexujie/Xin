#pragma once

#include "Basic.Types.h"
#include "Array.h"

namespace Xin
{
	template class BASE_API TDefaultAllocator<char8>;

	template<typename T, typename AllocatorT = TDefaultAllocator<T>>
	struct [[Meta(SystemType)]] TString : public TStorage<T, AllocatorT, AllocatorT::NeedInstance>
	{
	public:
		using ViewType = TView<T>;
		using SliceType = TSlice<T>;
		using CharType = T;
		using AllocatorType = AllocatorT;

		using ParentType = TStorage<T, AllocatorT, AllocatorT::NeedInstance>;
		using ParentType::Data;

	public:
		uintx Length = 0;
		uintx Capacity = 0;

	public:
		constexpr TString() noexcept = default;

		constexpr explicit TString(uintx Size) : Length(Size), Capacity(Size + 1)
		{
			Data = ParentType::Alloc(Capacity);
			ArrayZero(Data, Size);
			FinishNull();
		}

		constexpr TString(TString && That) noexcept
		{
			Length = That.Length;
			Capacity = That.Capacity;
			Data = That.Data;
			That.Length = 0;
			That.Capacity = 0;
			That.Data = nullptr;
		}

		constexpr TString(const TString & That)
		{
			Length = That.Length;
			Capacity = Length > 0 ? Length + 1 : 0;
			Data = Length > 0 ? ParentType::Alloc(That.Capacity) : nullptr;
			ArrayCopy(Data, That.Data, That.Length);
			FinishNull();
		}

		constexpr TString(ViewType String)
		{
			Length = String.Size;
			Capacity = Length > 0 ? Length + 1 : 0;
			Data = Length > 0 ? ParentType::Alloc(Capacity) : nullptr;
			ArrayCopy(Data, String.Data, String.Size);
			FinishNull();
		}

		constexpr TString(const T * String, uintx StringLength = NullIndex) : TString(ViewType(String, StringLength == NullIndex ? Xin::Length(String) : StringLength)) { }

		//TString(const class FVariant & Variant);
		//TString(FVariant && Variant);

		constexpr ~TString()
		{
			if (Data)
			{
				ParentType::Free(Data, Capacity);

				Length = 0;
				Capacity = 0;
				Data = nullptr;
			}
		}

		constexpr TString & operator =(TString && That) noexcept
		{
			ParentType::Free(Data, Capacity);

			Length = That.Length;
			Capacity = That.Capacity;
			Data = That.Data;
			That.Length = 0;
			That.Capacity = 0;
			That.Data = nullptr;
			return *this;
		}

		constexpr TString & operator =(const TString & That)
		{
			return operator =(ViewType(That));
		}

		constexpr TString & operator =(ViewType That)
		{
			if (That.Size == 0)
			{
				Length = 0;
				FinishNull();
				return *this;
			}

			if (Capacity <= That.Size)
			{
				ParentType::Free(Data, Capacity);

				uintx NewCapacity = Capacity > 0 ? Capacity : 1;
				while (NewCapacity <= That.Size)
					NewCapacity *= 2;
				Capacity = NewCapacity;
				Data = ParentType::Alloc(Capacity);
			}

			ArrayCopy(Data, That.Data, That.Size);
			Length = That.Size;
			FinishNull();
			return *this;
		}

		constexpr TString & operator =(const std::basic_string<T> & String)
		{
			operator =(ViewType(String.data(), String.size()));
			return *this;
		}

		constexpr TString & operator =(std::basic_string_view<T> String)
		{
			operator =(ViewType(String.data(), String.size()));
			return *this;
		}

		constexpr TString & operator =(const T * String)
		{
			operator =(ViewType(String, Xin::Length(String)));
			return *this;
		}

		constexpr TString & operator +=(const T * String)
		{
			Append(ViewType(String, Xin::Length(String)));
			return *this;
		}

		constexpr TString & operator +=(ViewType String)
		{
			Append(String);
			return *this;
		}

		constexpr uintx GetSize() const noexcept { return Length; }
		constexpr uintx GetCapacity() const noexcept { return Capacity; }
		constexpr uintx GetCapacityWithoutNull() const noexcept { return Capacity > 0 ? Capacity - 1 : 0; }
		constexpr T * GetData() const noexcept { return Data; }

		constexpr void Append(T Ch)
		{
			Insert(Length, ViewType(&Ch, 1));
		}

		constexpr void Append(const T * String, uintx StringLength = NullIndex)
		{
			Insert(Length, ViewType(String, StringLength));
		}

		constexpr void Append(ViewType String)
		{
			Insert(Length, String);
		}

		constexpr void Insert(uintx Index, ViewType String)
		{
			if (String.Empty() || Index > Length)
				return;

			if (Capacity <= Length + String.Size)
			{
				uintx NewCapacity = Capacity > 0 ? Capacity * 2 : 1;
				while (NewCapacity <= Length + String.Size)
					NewCapacity *= 2;
				T * NewData = ParentType::Alloc(NewCapacity);
				ArrayCopy(NewData, Data, Length);
				ArrayCopy(NewData + Index + String.Size, Data + Index, Length - Index);
				ParentType::Free(Data, Capacity);
				Data = NewData;
				Capacity = NewCapacity;
			}
			else
			{
				if (Index + String.Size > Length)
				{
					ArrayCopy(Data + Index + String.Size, Data + Index, Length - Index);
				}
				else if (Index + String.Size < Length)
				{
					ArrayCopy(Data + Length, Data + Length - String.Size, String.Size);
					ArrayCopy(Data + Index + String.Size, Data + Index, Length - Index - String.Size);
				}
				else {}
			}
			ArrayCopy(Data + Index, String.Data, String.Size);
			Length += String.Size;
			FinishNull();
		}

		constexpr void Erase(uintx Index, uintx Count) noexcept
		{
			if (Index + Count >= Length)
				return;

			ArrayCopy(Data + Index, Data + Index + Count, Length - Index - Count);
			Length -= Count;
			FinishNull();
		}

		constexpr bool Empty() const noexcept { return Length == 0; }
		constexpr bool Valid() const noexcept { return Length != 0; }
		constexpr bool ValidIndex(uintx Index) const noexcept { return Index < Length; }
		constexpr T & operator [](uintx Index) const noexcept { return Data[Index]; }

		constexpr ViewType View(uintx Index, uintx Count = NullIndex) const noexcept
		{
			if (Count == NullIndex)
			{
				if (Index >= Length)
					return ViewType {};
				Count = Length - Index;
			}
			else if (Index + Count > Length)
				return ViewType {};
			else {}

			return ViewType { Data + Index, Count };
		}

		constexpr SliceType Slice(uintx Index, uintx Count = NullIndex) const noexcept
		{
			if (Count == NullIndex)
			{
				if (Index >= Length)
					return SliceType {};
				Count = Length - Index;
			}
			else if (Index + Count > Length)
				return SliceType {};
			else {}

			return SliceType { Data + Index, Count };
		}

		constexpr explicit operator bool() const noexcept { return !!Length; }

		constexpr operator TView<T>() const noexcept { return ViewType { Data, Length }; }

		constexpr operator TSlice<T>() const noexcept { return SliceType { Data, Length }; }

		constexpr void AddN(uintx Count)
		{
			if (!Count)
				return;

			if (Capacity <= Length + Count)
			{
				uintx NewCapacity = Capacity > 0 ? Capacity : 1;
				while (NewCapacity <= Length + Count)
					NewCapacity *= 2;
				T * NewData = ParentType::Alloc(NewCapacity);
				ArrayCopy(NewData, Data, Length);
				ParentType::Free(Data, Capacity);
				Data = NewData;
				Capacity = NewCapacity;
			}
			Length += Count;
			FinishNull();
		}

		constexpr void AddN(uintx Count, const T & C)
		{
			if (!Count)
				return;

			if (Capacity <= Length + Count)
			{
				uintx NewCapacity = Capacity > 0 ? Capacity : 1;
				while (NewCapacity <= Length + Count)
					NewCapacity *= 2;
				T * NewData = ParentType::Alloc(NewCapacity);
				ArrayCopy(NewData, Data, Length);
				ParentType::Free(Data, Capacity);
				Data = NewData;
				Capacity = NewCapacity;
			}
			ArrayFill(Data + Length, Count, C);
			Length += Count;
			FinishNull();
		}

		constexpr void Resize(uintx Length2, uintx Capacity2 = 0)
		{
			if (Capacity2 == 0)
				Capacity2 = Capacity;

			if (Capacity2 != Capacity || Capacity2 <= Length2)
			{
				if (Capacity2 <= Length2)
					Capacity2 = Length2 + 1;

				T * NewData = ParentType::Alloc(Capacity2);
				ArrayCopy(NewData, Data, Min(Length, Length2));
				ParentType::Free(Data, Capacity);
				Data = NewData;
				Length = Length2;
				Capacity = Capacity2;
				FinishNull();
			}
			else
			{
				Length = Length2;
			}
		}

		constexpr void Reserve(uintx NewCapacity)
		{
			if (NewCapacity > Capacity)
			{
				T * NewData = ParentType::Alloc(NewCapacity);
				ArrayCopy(NewData, Data, Length);
				ParentType::Free(Data, Capacity);
				Data = NewData;
				Capacity = NewCapacity;
			}
		}

		constexpr void Clear()
		{
			Resize(0);
		}

		constexpr void Reset()
		{
			if (Data)
			{
				ParentType::Free(Data, Capacity);
				Length = 0;
				Capacity = 0;
				Data = nullptr;
			}
		}

		constexpr TString & Remove(ViewType Pattern)
		{
			if (Length < Pattern.Size || Pattern.Size == 0)
				return *this;

			for (uintx Index = 0; Index <= Length - Pattern.Size; ++Index)
			{
				if (StringCompare<T>(TView<T>(Data + Index, Pattern.Size), Pattern) == 0)
				{
					ArrayCopy(Data + Index, Data + Index + Pattern.Size, Length - Index - Pattern.Size);
					Length -= Pattern.Size;
				}
			}
			FinishNull();
			return *this;
		}

		/*constexpr*/
		TString & Replace(ViewType Pattern, ViewType Substitute)
		{
			if (Length < Pattern.Size || Pattern.Size == 0 || Substitute.Size == 0)
				return *this;

			for (uintx Index = 0; Index <= Length - Pattern.Size;/**/)
			{
				uintx Step = 1;
				if (StringCompare<T>(TView<T>(Data + Index, Pattern.Size), Pattern) == 0)
				{
					uintx NewSize = Length + Substitute.Size - Pattern.Size;
					if (NewSize >= Capacity)
					{
						uintx NewCapacity = Capacity > 0 ? Capacity : 1;
						while (NewCapacity <= NewSize)
							NewCapacity *= 2;

						T * NewData = ParentType::Alloc(NewCapacity);
						ArrayCopy(NewData, Data, Index);
						ArrayCopy(NewData + Index + Substitute.Size, Data + Index + Pattern.Size, Length - Index - Pattern.Size);
						ParentType::Free(Data, Capacity);
						Data = NewData;
						Capacity = NewCapacity;
					}
					else
						ArrayCopy(Data + Index + Substitute.Size, Data + Index + Pattern.Size, Length - Index - Pattern.Size);
					ArrayCopy(Data + Index, Substitute.Data, Substitute.Size);
					Length = NewSize;
					Step = Substitute.Size;
				}
				Index += Step;
			}
			FinishNull();
			return *this;
		}

		/*constexpr*/
		TString & ReplaceAt(uintx Index, uintx Count, ViewType Substitute)
		{
			if (Length < Count || Count == 0 || Substitute.Size == 0)
				return *this;

			uintx NewSize = Length + Substitute.Size - Count;
			if (Capacity <= NewSize)
			{
				uintx NewCapacity = Capacity > 0 ? Capacity : 1;
				while (NewCapacity < NewSize + 1)
					NewCapacity *= 2;

				T * NewData = ParentType::Alloc(NewCapacity);
				ArrayCopy(NewData, Data, Index);
				ArrayCopy(NewData + Index + Substitute.Size, Data + Index + Count, Length - Index - Count);
				ParentType::Free(Data, Capacity);
				Data = NewData;
				Capacity = NewCapacity;
			}
			else
				ArrayCopy(Data + Index + Substitute.Size, Data + Index + Count, Length - Index - Count);
			ArrayCopy(Data + Index, Substitute.Data, Substitute.Size);
			Length = NewSize;

			FinishNull();
			return *this;
		}

		constexpr TString & SetAt(uintx Index, ViewType Substitute)
		{
			if (Index + Substitute.Size >= Length)
				throw EError::OutOfBound;

			ArrayCopy(Data + Index, Substitute.Data, Substitute.Size);
			return *this;
		}

		constexpr TString & RemoveAt(uintx Index, uintx Count = 1)
		{
			if (Index + Count >= Length)
				throw EError::OutOfBound;

			ArrayCopy(Data + Index, Data + Index + Count, Length - Index - Count);
			Length -= Count;
			FinishNull();
			return *this;
		}

		constexpr TString & TrimStart(TView<T> TrimmedChars = WhiteSpaces<T>)
		{
			uintx Index = 0;
			for (; Index < Length; ++Index)
			{
				if (TrimmedChars.FindFirst(Data[Index]) == NullIndex)
					break;
			}

			if (Index > 0)
			{
				ArrayCopy(Data, Data + Index, Length - Index);
				Length -= Index;
				FinishNull();
			}

			return *this;
		}

		constexpr TString & TrimEnd(TView<T> TrimmedChars = WhiteSpaces<T>)
		{
			uintx Index = Length;
			for (; Index > 0; --Index)
			{
				if (TrimmedChars.FindFirst(Data[Index - 1]) == NullIndex)
					break;
			}

			if (Index < Length)
			{
				Length = Index;
				FinishNull();
			}
			return *this;
		}

		constexpr TString & Trim(TView<T> TrimmedChars = WhiteSpaces<T>)
		{
			uintx IndexFrom = 0;
			for (; IndexFrom < Length; ++IndexFrom)
			{
				if (TrimmedChars.FindFirst(Data[IndexFrom]) == NullIndex)
					break;
			}

			uintx IndeTo = Length;
			for (; IndeTo > IndexFrom; --IndeTo)
			{
				if (TrimmedChars.FindFirst(Data[IndeTo - 1]) == NullIndex)
					break;
			}

			if (Length != IndeTo - IndexFrom)
			{
				ArrayCopy(Data, Data + IndexFrom, IndeTo - IndexFrom);
				Length = IndeTo - IndexFrom;
				FinishNull();
			}
			return *this;
		}

		constexpr TString & ToUpper()
		{
			for (uintx Index = 0; Index < Length; ++Index)
				Data[Index] = Xin::ToUpper(Data[Index]);
			return *this;
		}

		constexpr TString & ToLower()
		{
			for (uintx Index = 0; Index < Length; ++Index)
				Data[Index] = Xin::ToLower(Data[Index]);
			return *this;
		}

		constexpr uintx FindFirst(const T & Ch) const
		{
			return Array::FindFirst<T>(Data, Length, Ch);
		}

		constexpr uintx FindFirst(ViewType Pattern) const
		{
			return Array::FindFirst<T>(Data, Length, Pattern.Data, Pattern.Size);
		}

		constexpr uintx FindFirstOf(ViewType Chs) const
		{
			return Array::FindFirstOf<T>(Data, Length, Chs.Data, Chs.Size);
		}

		constexpr uintx FindLast(const T & Ch) const
		{
			return Array::FindLast<T>(Data, Length, Ch);
		}

		constexpr uintx FindLast(ViewType Pattern) const
		{
			return Array::FindLast<T>(Data, Length, Pattern.Data, Pattern.Size);
		}

		constexpr uintx FindLastOf(ViewType Chs) const
		{
			return Array::FindLastOf<T>(Data, Length, Chs.Data, Chs.Size);
		}

		constexpr bool Contains(ViewType Pattern) const
		{
			return FindFirst(Pattern) != NullIndex;
		}

		constexpr bool StartsWith(ViewType Pattern) const
		{
			return Array::StartsWith(Data, Length, Pattern.Data, Pattern.Size);
		}

		constexpr bool EndsWith(ViewType Pattern) const
		{
			return Array::EndsWith(Data, Length, Pattern.Data, Pattern.Size);
		}

		template<typename... ArgsT>
		TString & Format(ArgsT... Args)
		{
			return *this;
		}

		constexpr TString operator +(const TString & Other) const
		{
			if (Other.Length == 0)
				return *this;

			TString Result;
			Result.Reserve(Length + Other.Length);
			Result.Resize(Length + Other.Length);
			ArrayCopy(Result.Data, Data, Length);
			ArrayCopy(Result.Data + Length, Other.Data, Other.Length);
			return Result;
		}

		constexpr auto operator <=>(const TString & That) const noexcept
		{
			return StringCompare<T>(Data, Length, That.Data, That.Length) <=> 0;
		}

		constexpr auto operator ==(const TString & That) const
		{
			return StringCompare<T>(Data, Length, That.Data, That.Length) == 0;
		}

		constexpr auto operator <=>(const ViewType & That) const
		{
			return StringCompare<T>(Data, Length, That.Data, That.Size) <=> 0;
		}

		constexpr auto operator ==(const ViewType & That) const
		{
			return StringCompare<T>(Data, Length, That.Data, That.Size) == 0;
		}

		constexpr void FinishNull()
		{
			if (Length)
				Data[Length] = T(0);
		}

		constexpr ViewType ToString() const { return ViewType(Data, Length); }

		constexpr void SetSize(uintx NewSize)
		{
			if (NewSize >= Capacity)
				throw EError::OutOfBound;
			Length = NewSize;
			FinishNull();
		}

	public:
		static TString None;
	};

	inline FStringV NullString {};

	template<typename T, typename AllocatorT>
	TString<T, AllocatorT> TString<T, AllocatorT>::None = {};

	template<typename CharT>
	TString<CharT> & GetThreadLocalStringBuffer();

	template<CharacterC CharT = char8>
	bool StringEqual(TString<CharT> Left, TView<CharT> Right)
	{
		return StringEqual<CharT>(Left.View(0), Right);
	}

	template<CharacterC CharT = char8>
	bool StringEqual(TView<CharT> Left, TString<CharT> Right)
	{
		return StringEqual<CharT>(Left, Right.View(0));
	}

	template<CharacterC CharT = char8>
	bool StringEqualIC(TString<CharT> Left, TView<CharT> Right)
	{
		return StringEqualIC<CharT>(Left.View(0), Right);
	}

	template<CharacterC CharT = char8>
	bool StringEqualIC(TView<CharT> Left, TString<CharT> Right)
	{
		return StringEqualIC<CharT>(Left, Right.View(0));
	}

	namespace Strings
	{
		BASE_API FAStringV Printf(const char * Format, ...);
		BASE_API FUStringV Printf(const char8_t * Format, ...);
		BASE_API FWStringV Printf(const wchar_t * Format, ...);

		template<typename... ArgsT>
		FAStringV PrintArgs(const char * Format, ArgsT... Args) { return Printf(Format, Forward<ArgsT>(Args)...); }

		template<typename... ArgsT>
		FAStringV PrintArgs(const char8_t * Format, ArgsT... Args) { return Printf(Format, Forward<ArgsT>(Args)...); }

		template<typename... ArgsT>
		FAStringV PrintArgs(const wchar_t * Format, ArgsT... Args) { return Printf(Format, Forward<ArgsT>(Args)...); }

		BASE_API int32 ToBool(FStringV String);
		BASE_API int32 ToInt32(FStringV String, uint32 Radix = 10);
		BASE_API uint32 ToUInt32(FStringV String, uint32 Radix = 10);
		BASE_API int64_t ToInt64(FStringV String, uint32 Radix = 10);
		BASE_API uint64 ToUInt64(FStringV String, uint32 Radix = 10);
		BASE_API intx ToIntX(FStringV String, uint32 Radix = 10);
		BASE_API uintx ToUIntX(FStringV String, uint32 Radix = 10);

		BASE_API float32 ToFloat32(FStringV String);
		BASE_API float64 ToFloat64(FStringV String);
	}

	template<typename CharT>
	TView<CharT> GetLine(TView<CharT> String, uintx & Offset, CharT Delimiter)
	{
		uintx Position = Offset;
		uintx Count = 0;
		while (Offset < String.Size)
		{
			if (String[Offset++] == Delimiter)
				break;

			++Count;
		}
		return TView<CharT>(String.Data + Position, Count);
	}

	template<typename CharT>
	TView<CharT> GetLine(TView<CharT> String, uintx & Offset, TView<CharT> Delimiters)
	{
		uintx Begin = Offset;
		uintx Count = 0;
		while (Offset < String.Size)
		{
			if (Delimiters.FindFirst(String[Offset++]) != NullIndex)
				break;

			++Count;
		}
		return TView<CharT>(String.Data + Begin, Count);
	}

	template<typename CharT>
	TView<CharT> operator +(TView<CharT> Lhs, TView<CharT> Rhs)
	{
		TString<CharT> & Buffer = Xin::GetThreadLocalStringBuffer<CharT>();
		Buffer.Resize(Lhs.Size + Rhs.Size);
		Xin::MemoryCopy(Buffer.Data, Lhs.Data, sizeof(CharT) * Lhs.Size);
		Xin::MemoryCopy(Buffer.Data + Lhs.Size, Rhs.Data, sizeof(CharT) * Rhs.Size);
		return Buffer;
	}

	template<typename CharT>
	TView<CharT> operator +(const CharT * Lhs, TView<CharT> Rhs)
	{
		return Xin::TView<CharT>(Lhs) + Rhs;
	}

	template<typename CharT>
	TView<CharT> operator +(TView<CharT> Lhs, const CharT * Rhs)
	{
		return Lhs + Xin::TView<CharT>(Rhs);
	}

	template<typename CharT>
	TView<CharT> operator +(TView<CharT> Lhs, const TString<CharT> & Rhs)
	{
		return Lhs + Xin::TView<CharT>(Rhs);
	}

	template<typename CharT>
	TView<CharT> operator +(const TString<CharT> & Lhs, TView<CharT> Rhs)
	{
		return Xin::TView<CharT>(Lhs) + Rhs;
	}

	// TString + const Char *
	template<typename CharT>
	TView<CharT> operator +(const TString<CharT> & Lhs, const CharT * Rhs)
	{
		return Lhs + Xin::TView<CharT>(Rhs);
	}

	// const Char * + TString
	template<typename CharT>
	TView<CharT> operator +(const CharT * Lhs, const TString<CharT> & Rhs)
	{
		return Xin::TView<CharT>(Lhs) + Xin::TView<CharT>(Rhs);
	}

	using FAString = TString<chara>;
	using FWString = TString<charw>;
	using FU8String = TString<char8>;
	using FU16String = TString<char16>;
	using FU32String = TString<char32>;

	using FString = TString<char8>;

	template<TriviallyCopyableC T>
	forceinline uint64 CityHash64(const TString<T> & String)
	{
		return CityHash64(static_cast<const void *>(String.Data), sizeof(T) * String.Length);
	}

	template<typename CharT>
	struct THash<TString<CharT>>
	{
		uintx operator ()(const Xin::TString<CharT> & String) const noexcept
		{
			return HashPrivate::HashArrayRepresentation<CharT>(String.Data, String.Length);
		}
	};
}

namespace std
{
	template<typename CharT>
	struct hash<Xin::TString<CharT>>
	{
		size_t operator ()(const Xin::TString<CharT> & String) const noexcept
		{
			return _Hash_array_representation<CharT>(String.Data, String.Length);
		}
	};
}
