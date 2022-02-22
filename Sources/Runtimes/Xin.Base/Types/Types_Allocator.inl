#pragma once

#include "Types.h"

namespace Xin
{
	forceinline constexpr byte * ByteCopy(byte * Destination, const byte * Source, uintx Count)
	{
		if constexpr (IsConstantEvaluated())
		{
			for (uintx Index = 0; Index < Count; ++Index)
				Destination[Index] = Source[Index];
			return Destination;
		}
		else
			return (byte *)memcpy(Destination, Source, Count);
	}

	forceinline constexpr void * ByteSet(byte * Destination, uint8 Value, uintx Count)
	{
		if constexpr (IsConstantEvaluated())
		{
			for (uintx Index = 0; Index < Count; ++Index)
				Destination[Index] = Value;
			return Destination;
		}
		else
			return memset(Destination, Value, Count);
	}

	forceinline constexpr void * ByteZero(byte * Destination, uintx Count)
	{
		if constexpr (IsConstantEvaluated())
		{
			for (uintx Index = 0; Index < Count; ++Index)
				Destination[Index] = 0;
			return Destination;
		}
		else
			return memset(Destination, 0, Count);
	}

	forceinline void * MemoryCopy(void * Destination, const void * Source, uintx Count)
	{
		return std::memcpy(Destination, Source, Count);
	}

	template<typename T>
	forceinline void * MemoryCopy(void * Destination, const T & Object)
	{
		return std::memcpy(Destination, &Object, sizeof(Object));
	}

	forceinline void * MemoryFill(void * Destination, uint8 Value, uintx Count)
	{
		return std::memset(Destination, Value, Count);
	}

	forceinline void * MemoryZero(void * Destination, uintx Count)
	{
		return std::memset(Destination, 0, Count);
	}

	forceinline int MemoryCompare(const void * Lhs, const void * Rhs, uintx Count)
	{
		return memcmp(Lhs, Rhs, Count);
	}

	forceinline void * MemoryMove(void * Destination, void * Source, uintx Size)
	{
		return memmove(Destination, Source, Size);
	}

	template<typename T, uintx Size>
	forceinline constexpr T * MemoryZero(T (&Destination)[Size])
	{
		MemoryZero(Destination, sizeof(T) * Size);
		return Destination;
	}

	template<typename T>
	forceinline void SafeDelete(T *& Address)
	{
		delete Address;
		Address = nullptr;
	}

#if XIN_PUBLISH
	forceinline void * Malloc(uintx Size, uintx Alignment = 0, uint32 StackOffset = 0)
	{
		assert(Size > 0);
		void * Block;
		if (Alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
			Block = ::_aligned_malloc(Size, Alignment);
		else
			Block = ::malloc(Size);
		assert(Block != nullptr);
		return Block;
	}

	forceinline void Free(void * Block, uintx Alignment = 0)
	{
		if (Alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
			::_aligned_free(Block);
		else
			::free(Block);
	}
#else
	BASE_API void * Malloc(uintx Size, uintx Alignment = 0);
	BASE_API void Free(void * Block, uintx Alignment = 0);
#endif

	class FAllocator
	{
	public:
		FAllocator() = delete;

		template<typename T, typename ...ArgsT>
		forceinline static void DirectCounstruct(T * Destination, ArgsT &&... Args)
		{
			new (Destination) T(Forward<ArgsT>(Args)...);
		}

		template<typename T>
		forceinline static void DefaultConstruct(T * Destination, uintx Count)
		{
			if constexpr (IsTriviallyConstructibleV<T>)
				MemoryZero(Destination, sizeof(T) * Count);
			else if constexpr (IsDefaultConstructibleV<T>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					new (Destination + Index) T();
			}
			else
				throw EError::NotSupported;
		}

		template<typename T>
		forceinline static constexpr void CopyConstruct(T * Destination, const T * Source, uintx Count)
		{
			if constexpr (IsConstantEvaluated())
			{
				for (uintx Index = 0; Index < Count; ++Index)
					new (Destination + Index) T(Source[Index]);
			}
			else if constexpr (IsTriviallyConstructibleV<T>)
				MemoryCopy(Destination, Source, sizeof(T) * Count);
			else if constexpr (IsCopyConstructibleV<T>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					new (Destination + Index) T(Source[Index]);
			}
			else
				throw EError::NotSupported;
		}

		template<typename T>
		forceinline static constexpr void CopyConstructRepeat(T * Destination, const T & Value, uintx Count)
		{
			if constexpr (IsConstantEvaluated())
			{
				for (uintx Index = 0; Index < Count; ++Index)
					new (Destination + Index) T(Value);
			}
			else if constexpr (IsTriviallyConstructibleV<T>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					MemoryCopy(Destination + Index, &Value, sizeof(T));
			}
			else if constexpr (IsCopyConstructibleV<T>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					new (Destination + Index) T(Value);
			}
			else
				throw EError::NotSupported;
		}

		template<typename T, bool ReverseOrder = false>
		forceinline static constexpr void MoveConstruct(T * Destination, T * Source, uintx Count)
		{
			if constexpr (IsConstantEvaluated())
			{
				if constexpr (ReverseOrder)
				{
					for (uintx Index = Count; Index > 0; --Index)
						new (Destination + (Index - 1)) T(Move(Source[Index - 1]));
				}
				else
				{
					for (uintx Index = 0; Index < Count; ++Index)
						new (Destination + Index) T(Move(Source[Index]));
				}
			}
			else if constexpr (IsTriviallyMoveConstructibleV<T>)
			{
				//if (static_cast<uintx>(Abs(Destination - Source)) > Count)
				//	MemoryMove(Destination, Source, sizeof(T) * Count);
				//else
				MemoryCopy(Destination, Source, sizeof(T) * Count);
			}
			else if constexpr (IsMoveConstructibleV<T>)
			{
				if constexpr (ReverseOrder)
				{
					for (uintx Index = Count; Index > 0; --Index)
						new (Destination + Index - 1) T(Move(Source[Index - 1]));
				}
				else
				{
					for (uintx Index = 0; Index < Count; ++Index)
						new (Destination + Index) T(Move(Source[Index]));
				}
			}
			else
				throw EError::NotSupported;
		}

		template<typename T, typename... ArgumentTs>
		forceinline static constexpr void Construct(T * Source, ArgumentTs && ... Arguments)
		{
			new (Source) T(Forward<ArgumentTs>(Arguments)...);
		}

		template<typename T>
		forceinline static constexpr void Destruct(const T * Source, uintx Count)
		{
			if constexpr (IsDestructibleV<T>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					(Source + Index)->~T();
			}
		}

		template<typename T>
		forceinline static void CopyAssign(T * Destination, const T * Source, uintx Count)
		{
			if constexpr (IsTriviallyAssignableV<T &, const T &>)
			{
				MemoryCopy(Destination, Source, sizeof(T) * Count);
			}
			else if constexpr (IsAssignableV<T &, const T &>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					Destination[Index] = Source[Index];
			}
			else
				throw EError::NotSupported;
		}

		template<typename T>
		forceinline static void CopyAssignRepeat(T * Destination, const T & Value, uintx Count)
		{
			if constexpr (IsTriviallyAssignableV<T, T>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					MemoryCopy(Destination + Index, &Value, sizeof(T));
			}
			else if constexpr (IsAssignableV<T &, const T &>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					Destination[Index] = Value;
			}
			else
				throw EError::NotSupported;
		}

		template<typename T>
		forceinline static void MoveAssign(T * Destination, const T * Source, uintx Count)
		{
			if constexpr (IsTriviallyAssignableV<T &, const T &>)
			{
				MemoryCopy(Destination, Source, sizeof(T) * Count);
			}
			else if constexpr (IsAssignableV<T &, T &&>)
			{
				for (uintx Index = 0; Index < Count; ++Index)
					Destination[Index] = Move(Source[Index]);
			}
			else
				throw EError::NotSupported;
		}
	};

	template<typename T>
	class TDefaultAllocator
	{
	public:
		static constexpr bool NeedInstance = false;

		using PointerT = T *;

		forceinline constexpr static T * Alloc(uintx Count)
		{
			return static_cast<PointerT>(Malloc(sizeof(T) * Count, alignof(T)));
		}

		forceinline constexpr static void Free(PointerT Address)
		{
			Xin::Free(static_cast<void *>(Address), alignof(T));
		}

		forceinline constexpr static void Free(PointerT Address, uintx Count)
		{
			Xin::Free(static_cast<void *>(Address), alignof(T));
		}
	};

	template<typename T, typename AllocatorT, bool NeedStorageInstance>
	class TStorage;

	template<typename T, typename AllocatorT>
	class TStorage<T, AllocatorT, false>
	{
	public:
		T * Data = nullptr;


	protected:
		constexpr T * Alloc(uintx Count)
		{
			return AllocatorT::Alloc(Count);
		}

		constexpr void Free(T * Address, uintx Count)
		{
			AllocatorT::Free(Address, Count);
		}
	};

	//template<typename T, typename AllocatorT>
	//class TStorage<T, AllocatorT, true>
	//{
	//public:
	//	AllocatorT Allocator;
	//	T * Data = nullptr;

	//	constexpr T * Alloc(uintx Count)
	//	{
	//		return Allocator.Alloc(Count);
	//	}

	//	constexpr void Free(T * Address)
	//	{
	//		Allocator.Free(Address);
	//	}

	//	constexpr void Free(T * Address, uintx Count)
	//	{
	//		Allocator.Free(Address, Count);
	//	}
	//};
}
