#pragma once

#include "Array.h"
#include "Enumerable.h"
#include "Set.h"

namespace Xin
{
	template<typename T, typename ElementT = ConditionalT<IsLValueReferenceV<T>, TReferenceWrapper<RemoveRefT<T>>, T>, typename AllocatorT = TDefaultAllocator<ElementT>, bool NeedInstance = AllocatorT::NeedInstance>
	class TList : public TStorage<ElementT, AllocatorT, NeedInstance>
	{
	public:
		using ParentType = TStorage<ElementT, AllocatorT, NeedInstance>;
		using ParentType::Data;
		//using ElementT = ConditionalT<LValueReferenceC<T>, TReferenceWrapper<RemoveRefT<T>>, T>;

		using Iterator = TPointerIterator<T, false>;
		using ConstIterator = TPointerIterator<T, true>;

	public:
		uint64 Size = 0;
		uint64 Capacity = 0;

	public:
		TList() = default;

		TList(IEnumerable<T> && Enumerable)
		{
			for (const T & Element : Enumerable)
				Emplace(Element);
		}

		template<typename Ty>
		TList(IEnumerable<Ty> && Enumerable)
		{
			for (const Ty & Element : Enumerable)
				Emplace(Element);
		}

		explicit TList(uintx Size, EInitializeMode InitializeMode = EInitializeMode::Default) : Size(Size), Capacity(Size)
		{
			if (Size > 0)
			{
				Data = Capacity > 0 ? ParentType::Alloc(Capacity) : nullptr;
				switch (InitializeMode)
				{
				case EInitializeMode::Default:
				case EInitializeMode::Construct:
					FAllocator::DefaultConstruct<ElementT>(Data, Size);
					break;
				case EInitializeMode::Zero:
				{
					MemoryZero(Data, sizeof(T) * Size);
					break;
				}
				default:
					break;
				}
			}
		}

		explicit TList(uintx Size, const T & Value, EElementFill) : Size(Size), Capacity(Size)
		{
			Data = Capacity > 0 ? ParentType::Alloc(Capacity) : nullptr;
			FAllocator::CopyConstructRepeat<ElementT>(Data, Value, Size);
		}

		explicit TList(TSet<T> Set) : TList(Set.Size(), EInitializeMode::None)
		{
			uintx Index = 0;
			auto Iter = Set.Begin();
			while (Iter != Set.End())
			{
				FAllocator::CopyConstruct<ElementT>(Data + Index, &(*Iter), 1);
				++Index;
				++Iter;
			}
		}

		TList(const TList & That) requires IsCopyConstructibleV<T>
		{
			Size = That.Size;
			Capacity = That.Capacity;
			Data = Capacity > 0 ? ParentType::Alloc(Capacity) : nullptr;
			FAllocator::CopyConstruct<ElementT>(Data, That.Data, Size);
		}

		template<typename Ty> requires ConstructibleC<T, const Ty &>
		TList(const TList<Ty> & That)
		{
			Size = That.Size;
			Capacity = That.Capacity;
			Data = Capacity > 0 ? ParentType::Alloc(That.Capacity) : nullptr;
			for (uintx Index = 0; Index < That.Size; ++Index)
				FAllocator::Construct<ElementT>(Data + Index, *(That.Data + Index));
		}

		TList(TList && That) noexcept
		{
			Size = That.Size;
			Capacity = That.Capacity;
			Data = That.Data;
			That.Size = 0;
			That.Capacity = 0;
			That.Data = nullptr;
		}

		TList(TView<T> Array) requires IsCopyConstructibleV<T>
		{
			Size = Array.Size;
			Capacity = Array.Size;
			Data = Capacity > 0 ? ParentType::Alloc(Capacity) : nullptr;
			FAllocator::CopyConstruct<ElementT>(Data, Array.Data, Size);
		}

		TList(TInitializerList<T> InitList) requires IsCopyConstructibleV<T>
		{
			Size = static_cast<uintx>(InitList.size());
			Capacity = Size;
			Data = Capacity > 0 ? ParentType::Alloc(Capacity) : nullptr;
			FAllocator::CopyConstruct<ElementT>(Data, InitList.begin(), Size);
		}

		~TList()
		{
			if (Data)
			{
				FAllocator::Destruct<ElementT>(Data, Size);
				ParentType::Free(Data, Capacity);
				Size = 0;
				Capacity = 0;
				Data = nullptr;
			}
		}

		template<typename = void> requires ComparableC<T>
		auto operator <=>(const TList & Another) const
		{
			if (auto Result = Compare(Size, Another.Size); Result != 0)
				return Result;

			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (auto Result = Compare(Data[Index], Another.Data[Index]); Result != 0)
					return Result;
			}
			return CompareOrderingT::equivalent;
		}

		bool operator ==(const TList & Another) const
		{
			if (Size != Another.Size)
				return false;

			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Data[Index] != Another.Data[Index])
					return false;
			}
			return true;
		}

		bool Empty() const { return Size == 0; }
		uintx GetSize() const { return Size; }
		ElementT * GetData() const { return Data; }

		T & operator [](uintx Index)
		{
			if (Index >= Size)
				throw EError::OutOfBound;
			return Data[Index];
		}

		const T & operator [](uintx Index) const
		{
			if (Index >= Size)
				throw EError::OutOfBound;
			return Data[Index];
		}

		const T & GetAt(uintx Index) const
		{
			if (Index >= Size)
				throw EError::OutOfBound;
			return Data[Index];
		}

		T & GetAt(uintx Index)
		{
			if (Index >= Size)
				throw EError::OutOfBound;
			return Data[Index];
		}

		const T & Front() const { return GetAt(0); }
		T & Front() { return GetAt(0); }
		const T & Back() const { return GetAt(Size - 1); }
		T & Back() { return GetAt(Size - 1); }

		uintx AddInitialized(uintx Count)
		{
			if (!Count)
				return Size;

			uintx InitSize = Size;
			Size = Size + Count;
			if (Size > Capacity)
			{
				uintx NewCapacity = Capacity > 0 ? Capacity : 1;
				while (NewCapacity < Size)
					NewCapacity *= 2;
				ElementT * NewData = ParentType::Alloc(NewCapacity);
				FAllocator::MoveConstruct<ElementT>(NewData, Data, InitSize);
				FAllocator::Destruct<ElementT>(Data, InitSize);
				ParentType::Free(Data, Capacity);
				FAllocator::DefaultConstruct<ElementT>(NewData + InitSize, Count);
				Data = NewData;
				Capacity = NewCapacity;
			}
			else
			{
				FAllocator::DefaultConstruct<ElementT>(Data + InitSize, Count);
			}
			return InitSize;
		}

		void Reduce(uintx Count)
		{
			Count = Min(Size, Count);
			if (!Count)
				return;
			FAllocator::Destruct<ElementT>(Data + Size - Count, Count);
			Size = Size - Count;
		}

		template<typename... ArgsT>
		T & Emplace(ArgsT &&... Args)
		{
			if (Size + 1 > Capacity)
			{
				uintx NewCapacity = Capacity > 0 ? Capacity : 1;
				while (NewCapacity < Size + 1)
					NewCapacity *= 2;
				ElementT * NewData = ParentType::Alloc(NewCapacity);
				FAllocator::MoveConstruct<ElementT>(NewData, Data, Size);
				FAllocator::Destruct<ElementT>(Data, Size);
				ParentType::Free(Data, Capacity);
				Data = NewData;
				Capacity = NewCapacity;
			}
			else { }

			FAllocator::DirectCounstruct<ElementT>(Data + Size, Forward<ArgsT>(Args)...);
			++Size;
			return Data[Size - 1];
		}

		T & Add(const T & Element)
		{
			Emplace(Element);
			return Data[Size - 1];
		}

		T & Add(T && Element) requires (!LValueReferenceC<T>)
		{
			Emplace(Forward<T>(Element));
			return Data[Size - 1];
		}

		T & AddDefault() requires IsDefaultConstructibleV<T>
		{
			Emplace(T());
			return Data[Size - 1];
		}

		T & AddZeroed()
		{
			Reserve(Size + 1);
			MemoryZero(Data + Size, sizeof(T));
			++Size;
			return Data[Size - 1];
		}

		T & AddNone()
		{
			Reserve(Size + 1);
			++Size;
			return Data[Size - 1];
		}

		void AddN(const T & Element, uintx Count)
		{
			if (!Count)
				return;

			Reserve(Size + Count);
			FAllocator::CopyConstructRepeat<ElementT>(Data + Size, Element, Count);
			Size += Count;
		}

		void AddRange(TView<T> Another)
		{
			Reserve(Size + Another.Size);
			FAllocator::CopyConstruct<ElementT>(Data + Size, Another.Data, Another.Size);
			Size += Another.Size;
		}

		void AddRange(TInitializerList<T> InitList)
		{
			Reserve(Size + InitList.size());
			FAllocator::CopyConstruct<ElementT>(Data + Size, InitList.begin(), InitList.size());
			Size += InitList.size();
		}

		T & AddUnique(const T & Element, bool * Added = nullptr)
		{
			if (auto Iter = Find(Element); Iter != End())
			{
				if (Added) 
					*Added = false;
				return *Iter;
			}
			if (Added) 
				*Added = true;
			return Add(Element);
		}

		bool Remove(const T & Element)
		{
			Iterator Where = Find(Element);
			if (Where == End())
				return false;
			Erase(Where);
			return true;
		}

		bool Contains(const T & Element) const
		{
			Iterator Where = Find(Element);
			return Where != End();
		}

		void Insert(uintx Index, const T & Value, uintx Count = 1)
		{
			if (Index == NullIndex)
				Index = Size;

			Reserve(Size + Count);
			FAllocator::MoveConstruct<T, true>(Data + Index + Count, Data + Index, Size - Index);
			FAllocator::Destruct<ElementT>(Data + Index, Count);
			FAllocator::CopyConstructRepeat<ElementT>(Data + Index, Value, Count);
			Size += Count;
		}

		void SetAt(uintx Index, const T & Value, uintx Count = 1)
		{
			if (Index + Count > Size)
				throw EError::OutOfBound;

			FAllocator::CopyAssignRepeat<ElementT>(Data + Index, Value, Count);
		}

		TList & operator =(const TList & That)
		{
			if (That.Data == Data)
				return *this;

			FAllocator::Destruct<ElementT>(Data, Size);
			ParentType::Free(Data, Capacity);

			Size = That.Size;
			Capacity = That.Capacity;
			Data = Capacity > 0 ? ParentType::Alloc(That.Capacity) : nullptr;
			FAllocator::CopyConstruct<ElementT>(Data, That.Data, That.Size);
			return *this;
		}

		template<typename Ty> requires ConstructibleC<T, const Ty &>
		TList & operator =(const TList<Ty> & That)
		{
			static_assert(!SameC<T, Ty>);

			FAllocator::Destruct<ElementT>(Data, Size);
			ParentType::Free(Data, Capacity);

			Size = That.Size;
			Capacity = That.Capacity;
			Data = Capacity > 0 ? ParentType::Alloc(That.Capacity) : nullptr;
			for (uintx Index = 0; Index < That.Size; ++Index)
				FAllocator::Construct<ElementT>(Data + Index, *(That.Data + Index));
			return *this;
		}

		TList & operator =(TList && That) noexcept
		{
			if (That.Data == Data)
				return *this;

			FAllocator::Destruct<ElementT>(Data, Size);
			ParentType::Free(Data, Capacity);

			Size = That.Size;
			Capacity = That.Capacity;
			Data = That.Data;
			That.Size = 0;
			That.Capacity = 0;
			That.Data = nullptr;
			return *this;
		}

		TList & operator =(TInitializerList<T> InitList)
		{
			FAllocator::Destruct<ElementT>(Data, Size);
			ParentType::Free(Data, Capacity);

			Size = InitList.size();
			Capacity = Size;
			Data = Capacity > 0 ? ParentType::Alloc(Capacity) : nullptr;
			FAllocator::CopyConstruct<ElementT>(Data, InitList.begin(), Size);
			return *this;
		}

		TList & operator =(TView<T> That)
		{
			FAllocator::Destruct<ElementT>(Data, Size);
			ParentType::Free(Data, Capacity);

			Size = That.Size;
			Capacity = That.Size;
			Data = Capacity > 0 ? ParentType::Alloc(Capacity) : nullptr;
			FAllocator::CopyConstruct<ElementT>(Data, That.Data, That.Size);
			return *this;
		}

		template<typename Ty> requires ConstructibleC<T, const Ty &>
		TList & operator =(const TView<Ty> & That)
		{
			static_assert(!SameC<T, Ty>);

			FAllocator::Destruct<ElementT>(Data, Size);
			ParentType::Free(Data, Capacity);

			Size = That.Size;
			Capacity = That.Size;
			Data = Capacity > 0 ? ParentType::Alloc(That.Size) : nullptr;
			for (uintx Index = 0; Index < That.Size; ++Index)
				FAllocator::Construct<ElementT>(Data + Index, *(That.Data + Index));
			return *this;
		}

		TList & operator =(IEnumerable<T> && Enumerable)
		{
			FAllocator::Destruct<ElementT>(Data, Size);
			ParentType::Free(Data, Capacity);

			for (const T & Element : Enumerable)
				Emplace(Move(Element));

			return *this;
		}


		void Clear()
		{
			Resize(0);
		}

		void Reset()
		{
			if (Data)
			{
				FAllocator::Destruct<ElementT>(Data, Size);
				ParentType::Free(Data, Capacity);
				Size = 0;
				Capacity = 0;
				Data = nullptr;
			}
		}

		void Reset(uintx NewSize, const T & Value)
		{
			FAllocator::Destruct<ElementT>(Data, Size);
			Reserve(NewSize);
			FAllocator::CopyConstructRepeat<ElementT>(Data, Value, NewSize);
			Size = NewSize;
		}

		TView<T> View(uintx Index = 0, uintx Count = NullIndex) const
		{
			if (Count == NullIndex)
			{
				if (Index >= Size)
					return TView<T>();
				Count = Size - Index;
			}
			else if (Index + Count > Size)
				return TView<T>();
			else {}

			return TView<T>(Data + Index, Count);
		}

		TSlice<T> Slice(uintx Index = 0, uintx Count = NullIndex) const
		{
			if (Count == NullIndex)
			{
				if (Index >= Size)
					return TSlice<T>();
				Count = Size - Index;
			}
			else if (Index + Count > Size)
				return TSlice<T>();
			else {}

			return TSlice<T>(Data + Index, Count);
		}

	public:
		void ReserveTo(uintx NewCapacity)
		{
			if (NewCapacity > Capacity)
			{
				ElementT * NewData = ParentType::Alloc(NewCapacity);
				FAllocator::MoveConstruct<ElementT>(NewData, Data, Size);
				FAllocator::Destruct<ElementT>(Data, Size);
				ParentType::Free(Data, Capacity);
				Data = NewData;
				Capacity = NewCapacity;
			}
		}

		void Reserve(uintx NewCapacity)
		{
			if (NewCapacity > Capacity)
			{
				uintx TheCapacity = Max(Capacity, 1);
				while (TheCapacity < NewCapacity)
					TheCapacity *= 2;
				ReserveTo(TheCapacity);
			}
		}

	public: // Resize
		void Resize(uintx NewSize, EInitializeMode InitializeMode = EInitializeMode::Default)
		{
			Reserve(NewSize);
			AdjustSize(NewSize, InitializeMode);
		}

		void ResizeTo(uintx NewSize, EInitializeMode InitializeMode = EInitializeMode::Default)
		{
			ReserveTo(NewSize);
			AdjustSize(NewSize, InitializeMode);
		}

		void ResizeTo(uintx NewSize, const T & Value)
		{
			ReserveTo(NewSize);
			AdjustSize(NewSize, Value);
		}

		void SetSize(uintx NewSize)
		{
			Size = NewSize;
		}

		void AdjustSize(uintx NewSize, EInitializeMode InitializeMode = EInitializeMode::Default)
		{
			if (NewSize > Size)
			{
				switch (InitializeMode)
				{
				case EInitializeMode::Default:
				case EInitializeMode::Construct:
					FAllocator::DefaultConstruct<ElementT>(Data + Size, NewSize - Size);
					break;
				case EInitializeMode::Zero:
					MemoryZero(Data + Size, sizeof(T) * (NewSize - Size));
					break;
				default:
					break;
				}
				Size = NewSize;
			}
			else if (NewSize < Size)
			{
				FAllocator::Destruct<ElementT>(Data + NewSize, Size - NewSize);
				Size = NewSize;
			}
			else {}
		}

		void AdjustSize(uintx NewSize, const T & Value)
		{
			if (NewSize > Size)
			{
				FAllocator::CopyConstructRepeat<ElementT>(Data + Size, Value, NewSize - Size);
				Size = NewSize;
			}
			else if (NewSize < Size)
			{
				FAllocator::Destruct<ElementT>(Data + NewSize, Size - NewSize);
				Size = NewSize;
			}
			else {}
		}

	public: // Erase
		Iterator Erase(Iterator Where, uintx Count = 1)
		{
			return EraseAt(Where - Begin(), Count);
		}

		Iterator EraseAt(uintx Index, uintx Count = 1)
		{
			if (Index + Count > Size)
				throw EError::OutOfBound;

			FAllocator::Destruct<ElementT>(Data + Index, Count);
			FAllocator::MoveConstruct<ElementT>(Data + Index, Data + Index + Count, Size - Count - Index);
			Size = Size - Count;
			return Data + Index;
		}

		T & PushFront(const T & Element)
		{
			Insert(0, Element);
			return Data[0];
		}

		T & PushBack(const T & Element)
		{
			return Add(Element);
		}

		T PopFront() requires IsCopyConstructibleV<T>
		{
			if (Size == 0)
				throw EError::OutOfBound;

			T Value = Front();
			EraseAt(0);
			return Value;
		}

		T PopBack() requires IsCopyConstructibleV<T>
		{
			if (Size == 0)
				throw EError::OutOfBound;

			T Value = Back();
			EraseAt(Size - 1);
			return Value;
		}

		Iterator Find(const T & Value) requires EqualableC<T>
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Data[Index] == Value)
					return Data + Index;
			}
			return End();
		}

		ConstIterator Find(const T & Value) const requires EqualableC<T>
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Data[Index] == Value)
					return Data + Index;
			}
			return End();
		}

		template<typename ComparerT>
		Iterator FindBy(const ComparerT & Comparer)
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Comparer(Data[Index]))
					return Data + Index;
			}
			return End();
		}

		template<typename ComparerT>
		ConstIterator FindBy(const ComparerT & Comparer) const
		{
			for (uintx Index = 0; Index < Size; ++Index)
			{
				if (Comparer(Data[Index]))
					return Data + Index;
			}
			return End();
		}

		bool Contains(const T & Value) const requires EqualableC<T>
		{
			return Find(Value) != End();
		}

		operator TView<T>() const
		{
			return TView<T>(Data, Size);
		}

		operator TSlice<T>() const
		{
			return TSlice<T>(Data, Size);
		}

		ConstIterator Begin() const { return Data; }
		ConstIterator End() const { return Data + Size; }
		Iterator Begin() { return Data; }
		Iterator End() { return Data + Size; }

		TList & Swap(TList & Another)
		{
			auto Data_ = Data;
			auto Size_ = Size;
			auto Capacity_ = Capacity;
			Data = Another.Data;
			Size = Another.Size;
			Capacity = Another.Capacity;
			Another.Data = Data_;
			Another.Size = Size_;
			Another.Capacity = Capacity_;
			return *this;
		}

	public: // for std::*
		using value_type = T;

		Iterator erase(Iterator Where)
		{
			Erase(Where, 1);
			return Where;
		}

		void push_back(const T & Element)
		{
			Add(Element);
		}

		void push_back(T && Element)  requires (!LValueReferenceC<T>)
		{
			Add(Move(Element));
		}
	};

	template<typename T>
	struct IsVector : FalseType {};

	template<typename T>
	struct IsVector<TList<T>> : TrueType
	{
		using Type = T;
	};

	template<typename T>
	inline constexpr bool IsVectorV = IsVector<T>::Value;

	template<TriviallyCopyableC T>
	forceinline uint64 CityHash64(const TList<T> List)
	{
		return CityHash64(static_cast<const void *>(List.Data), sizeof(T) * List.Size);
	}

	template<HashAbleC T>
	struct THash<TList<T>>
	{
		uintx operator ()(const TList<T> & Vector) const noexcept
		{
			uintx HashCode = HashCombine(Vector.GetSize());
			for (uintx Index = 0; Index < Vector.GetSize(); ++Index)
				HashCode = HashAppend(HashCode, Vector[Index]);
			return HashCode;
		}
	};

	template<typename T>
	struct THash<TView<T>>
	{
		uintx operator ()(const TView<T> & View) const noexcept
		{
			uintx HashCode = HashCombine(View.Size);
			for (uintx Index = 0; Index < View.Size; ++Index)
				HashCode = HashAppend(HashCode, View[Index]);
			return HashCode;
		}
	};

	template<typename T>
	auto begin(Xin::TList<T> & Vector)
	{
		return Vector.Begin();
	}

	template<typename T>
	auto end(Xin::TList<T> & Vector)
	{
		return Vector.End();
	}

	template<typename T>
	auto begin(const Xin::TList<T> & Vector)
	{
		return Vector.Begin();
	}

	template<typename T>
	auto end(const Xin::TList<T> & Vector)
	{
		return Vector.End();
	}

	template<typename OutT, typename InT>
	TView<OutT> ReinterpretView(const TList<InT> & View)
	{
		return TView<OutT> { reinterpret_cast<const OutT *>(View.Data), View.Size * sizeof(InT) / sizeof(OutT) };
	}

	template<typename T>
	void Swap(TList<T> & Left, TList<T> & Right) noexcept
	{
		Left.Swap(Right);
	}
}

namespace std
{
	// std::sort ...
	template<typename T, bool Const>
	struct iterator_traits<Xin::TPointerIterator<T, Const>>
	{
		using iterator_concept = contiguous_iterator_tag;
		using iterator_category = random_access_iterator_tag;
		using value_type = remove_cv_t<T>;
		using difference_type = ptrdiff_t;
		using pointer = T *;
		using reference = T &;
	};
}
