#pragma once

#include "Basic.Types.h"

namespace Xin
{
	template<typename T, typename DeleterT = TDeleterDefault<T>>
	class TUniquePtr
	{
		template<typename ThatT, typename ThatDeleterT>
		friend class TUniquePtr;

	public:
		T * Pointer = nullptr;
		DeleterT Deleter;

	public:
		TUniquePtr() = default;
		TUniquePtr(const TUniquePtr &) = delete;
		TUniquePtr & operator =(const TUniquePtr &) = delete;

		TUniquePtr(TUniquePtr && That) noexcept : Pointer(That.Pointer), Deleter(Move(That.Deleter))
		{
			That.Pointer = nullptr;
		}

		TUniquePtr(T * PointerIn, DeleterT DeleterIn = DeleterT()) : Pointer(PointerIn), Deleter(DeleterIn) { }
		TUniquePtr(NullptrT) : Pointer(nullptr) { }

		TUniquePtr & operator =(NullptrT) noexcept
		{
			T * OldPointer = Pointer;
			Pointer = nullptr;
			Deleter(OldPointer);
			return *this;
		}

		template<typename ThatT, typename ThatDeleterT>
		TUniquePtr(TUniquePtr<ThatT, ThatDeleterT> && That) noexcept
			: Pointer(That.Data), Deleter(Move(That.Deleter))
		{
			That.Data = nullptr;
		}

		~TUniquePtr()
		{
			Deleter(Pointer);
		}

		T * Get() const { return Pointer; }
		T & Ref() { return *Pointer; }
		const T & Ref() const { return *Pointer; }

		DeleterT & GetDeleter() { return Deleter; }
		const DeleterT & GetDeleter() const { return Deleter; }
		bool Valid() const { return Pointer != nullptr; }

		T * Release()
		{
			T * Result = Pointer;
			Pointer = nullptr;
			return Result;
		}

		void Reset(T * PointerIn = nullptr)
		{
			T * OldPointer = Pointer;
			Pointer = PointerIn;
			Deleter(OldPointer);
		}

		explicit operator bool() const { return !!Pointer; }
		T & operator *() const { return *Pointer; }
		T * operator ->() const { return Pointer; }

		operator const T * () const { return Pointer; }
		operator T * () { return Pointer; }

		TUniquePtr & operator =(TUniquePtr && That) noexcept
		{
			if (this != &That)
			{
				T * OldPointer = Pointer;
				Pointer = That.Pointer;
				That.Pointer = nullptr;
				Deleter(OldPointer);
				Deleter = Move(That.Deleter);
			}
			return *this;
		}

		template<typename ThatT, typename ThatDeleterT>
		TUniquePtr operator =(TUniquePtr<ThatT, ThatDeleterT> && That)
		{
			T * OldPointer = Pointer;
			Pointer = That.Data;
			That.Data = nullptr;
			Deleter(OldPointer);
			Deleter = Move(That.Deleter);
			return *this;
		}
	};


	template<typename T, typename DeleterT>
	class TUniquePtr<T[], DeleterT>
	{
		template<typename ThatT, typename ThatDeleterT>
		friend class TUniquePtr;

	private:
		T * Pointer = nullptr;
		DeleterT Deleter;

	public:
		TUniquePtr(const TUniquePtr &) = delete;
		TUniquePtr & operator =(const TUniquePtr &) = delete;

	public:
		TUniquePtr() : Pointer(nullptr) { }

		TUniquePtr(std::nullptr_t) : Pointer(nullptr) { }

		template<typename ThatT, typename = EnableIfT<IsConvertibleToV<ThatT, T>>>
		TUniquePtr(ThatT * PointerIn) : Pointer(PointerIn) { }

		TUniquePtr(TUniquePtr && That) noexcept
			: Pointer(That.Pointer), Deleter(Move(That.Deleter))
		{
			That.Pointer = nullptr;
		}

		template<typename ThatT, typename ThatDeleterT>
		TUniquePtr(TUniquePtr<ThatT, ThatDeleterT> && That) noexcept
			: Pointer(That.Data), Deleter(Move(That.Deleter))
		{
			That.Data = nullptr;
		}

		~TUniquePtr()
		{
			Deleter(Pointer);
		}

		T * Get() const { return Pointer; }
		DeleterT & GetDeleter() { return Deleter; }
		const DeleterT & GetDeleter() const { return Deleter; }
		bool IsValid() const { return Pointer != nullptr; }

		T * Release()
		{
			T * Result = Pointer;
			Pointer = nullptr;
			return Result;
		}

		void Reset(T * PointerIn = nullptr)
		{
			T * OldPointer = Pointer;
			Pointer = PointerIn;
			Deleter(OldPointer);
		}

		T & operator *() const { return *Pointer; }
		T * operator ->() const { return Pointer; }

		TUniquePtr & operator =(std::nullptr_t)
		{
			T * OldPointer = Pointer;
			Pointer = nullptr;
			Deleter(OldPointer);
			return *this;
		}

		TUniquePtr & operator =(TUniquePtr && That) noexcept
		{
			if (this != &That)
			{
				T * OldPointer = Pointer;
				Pointer = That.Pointer;
				That.Pointer = nullptr;
				Deleter(OldPointer);
				Deleter = Move(That.Deleter);
			}
			return *this;
		}

		template<typename ThatT, typename ThatDeleterT>
		TUniquePtr operator =(TUniquePtr<ThatT, ThatDeleterT> && That)
		{
			T * OldPointer = Pointer;
			Pointer = That.Data;
			That.Data = nullptr;
			Deleter(OldPointer);
			Deleter = Move(That.Deleter);
			return *this;
		}

		T & operator [](uintx Index) const
		{
			return Pointer[Index];
		}
	};

	template<typename LhsT, typename RhsT>
	bool operator ==(const TUniquePtr<LhsT> & Lhs, const TUniquePtr<RhsT> & Rhs)
	{
		return Lhs.Get() == Rhs.Get();
	}

	template<typename T>
	bool operator ==(const TUniquePtr<T> & Lhs, const TUniquePtr<T> & Rhs)
	{
		return Lhs.Get() == Rhs.Get();
	}

	template<typename LhsT, typename RhsT>
	bool operator !=(const TUniquePtr<LhsT> & Lhs, const TUniquePtr<RhsT> & Rhs)
	{
		return Lhs.Get() != Rhs.Get();
	}

	template<typename T>
	bool operator !=(const TUniquePtr<T> & Lhs, const TUniquePtr<T> & Rhs)
	{
		return Lhs.Get() != Rhs.Get();
	}

	template<typename T>
	bool operator ==(const TUniquePtr<T> & Lhs, std::nullptr_t)
	{
		return !Lhs.Valid();
	}

	template<typename T>
	bool operator ==(std::nullptr_t, const TUniquePtr<T> & Rhs)
	{
		return !Rhs.Valid();
	}

	template<typename T>
	bool operator !=(const TUniquePtr<T> & Lhs, std::nullptr_t)
	{
		return Lhs.Valid();
	}

	template<typename T>
	bool operator !=(std::nullptr_t, const TUniquePtr<T> & Rhs)
	{
		return Rhs.Valid();
	}

	template<typename T, typename... ArgsT> requires (!IsArrayV<T>)
	TUniquePtr<T> MakeUnique(ArgsT &&... Args)
	{
		return TUniquePtr<T, TDeleterDefault<T>>(new T(Forward<ArgsT>(Args)...), TDeleterDefault<T>());
	}

	template<typename T> requires IsArrayV<T>
	TUniquePtr<T> MakeUnique(uintx Size)
	{
		using ThisT = std::remove_all_extents_t<T>;
		return TUniquePtr<T>(new ThisT[Size]());
	}

	template<typename T, auto(*Deleter)(T *)>
	class TScopedPtr
	{
	public:
		T * Pointer = nullptr;

	public:
		TScopedPtr() = default;
		TScopedPtr(const TScopedPtr &) = delete;
		TScopedPtr & operator =(const TScopedPtr &) = delete;

		TScopedPtr(TScopedPtr && That) noexcept : Pointer(That.Pointer)
		{
			That.Pointer = nullptr;
		}

		TScopedPtr(NullptrT) : Pointer(nullptr) { }

		TScopedPtr(T * Pointer) : Pointer(Pointer) { }

		TScopedPtr & operator =(NullptrT) noexcept
		{
			if (Pointer)
			{
				Deleter(Pointer);
				Pointer = nullptr;
			}
			return *this;
		}

		TScopedPtr & operator =(T * ThatPointer) noexcept
		{
			if (Pointer)
				Deleter(Pointer);
			Pointer = ThatPointer;
			return *this;
		}

		template<typename ThatT>
		TScopedPtr(TScopedPtr<ThatT, Deleter> && That) noexcept
			: Pointer(That.Data)
		{
			That.Data = nullptr;
		}

		~TScopedPtr()
		{
			if (Pointer)
			{
				Deleter(Pointer);
				Pointer = nullptr;
			}
		}

		T * Get() const { return Pointer; }
		T & Ref() { return *Pointer; }
		const T & Ref() const { return *Pointer; }

		decltype(Deleter) GetDeleter() const { return Deleter; }
		bool Valid() const { return Pointer != nullptr; }

		T * Detatch()
		{
			T * Result = Pointer;
			Pointer = nullptr;
			return Result;
		}

		void Reset(T * ThatPointer = nullptr)
		{
			if (Pointer)
				Deleter(Pointer);
			Pointer = ThatPointer;
		}

		explicit operator bool() const { return !!Pointer; }
		T & operator *() const { return *Pointer; }
		T * operator ->() const { return Pointer; }

		operator const T * () const { return Pointer; }
		operator T * () { return Pointer; }

		TScopedPtr & operator =(TScopedPtr && That) noexcept
		{
			if (this != &That)
			{
				T * OldPointer = Pointer;
				Pointer = That.Pointer;
				That.Pointer = nullptr;
				Deleter(OldPointer);
			}
			return *this;
		}

		template<typename ThatT>
		TScopedPtr operator =(TScopedPtr<ThatT, Deleter> && That)
		{
			if (Pointer)
				Deleter(Pointer);
			Pointer = That.Pointer;
			That.Pointer = nullptr;
			return *this;
		}
	};
}
