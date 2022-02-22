#pragma once

#include "Basic.Types.h"
#include "Pointer.h"

namespace Xin
{
	template<typename T>
	concept ReferableC = requires(T & Value)
	{
		Value.AddRef();
		Value.Release();
	};

	template<typename T>
	inline constexpr bool IsReferableV = false;

	template<ReferableC T>
	inline constexpr bool IsReferableV<T> = true;

	template<typename T>
	concept ReferableWithReferCountC = requires(T & Value, void ** VV)
	{
		{ T::ReferCount } -> ConvertibleToC<uintx>;
		{ Value.AddRef() } -> ConvertibleToC<uintx>;
		{ Value.Release() } -> ConvertibleToC<uintx>;
	};

	template<typename T>
	inline constexpr bool IsReferableWithReferCountV = false;

	template<ReferableWithReferCountC T>
	inline constexpr bool IsReferableWithReferCountV<T> = true;


	template<typename T>
	concept ReferableWithConstructC = requires(T & Value, void ** VV)
	{
		{ Value.Construct() };
		{ Value.AddRef() } -> ConvertibleToC<uintx>;
		{ Value.Release() } -> ConvertibleToC<uintx>;
	};

	template<typename T>
	concept ReferableWithComC = requires(T & Value, void ** VV)
	{
		__uuidof(T);
		{ Value.QueryInterface(__uuidof(T), VV) } -> ConvertibleToC<long32>;
		{ Value.AddRef() } -> ConvertibleToC<uintx>;
		{ Value.Release() } -> ConvertibleToC<uintx>;
	};

	template<typename T>
	inline constexpr bool IsReferableWithComV = false;

	template<ReferableWithComC T>
	inline constexpr bool IsReferableWithComV<T> = true;

	template<ReferableC T>
	T * ReferAddRef(T * Pointer)
	{
		if (Pointer)
			Pointer->AddRef();
		return Pointer;
	}

	template<ReferableC T>
	void ReferRelease(T * Pointer)
	{
		if (Pointer)
			Pointer->Release();
	}

	template</*ReferAble*/typename T>
	class TReferPtr
	{
	public:
		using PointeeT = T;

	public:
		TReferPtr() noexcept = default;
		TReferPtr(std::nullptr_t) noexcept { }
		TReferPtr(T * Pointer) noexcept : Pointer(ReferAddRef(Pointer))
		{
		}

		TReferPtr(const TReferPtr & That) noexcept
		{
			if (That.Pointer)
			{
				Pointer = That.Pointer;
				Pointer->AddRef();
			}
		}

		TReferPtr(TReferPtr && That) noexcept
		{
			if (That.Pointer)
			{
				Pointer = That.Pointer;
				That.Pointer = nullptr;
			}
		}

		TReferPtr(const TPointerPtr<T> & That) noexcept
		{
			if (That.Pointer)
			{
				Pointer = That.Pointer;
				if (Pointer)
					Pointer->AddRef();
			}
		}

		template<ReferableC Ty> requires (IsConvertibleToV<Ty *, T *> || (IsReferableWithComV<T> && IsReferableWithComV<Ty>))
		TReferPtr(const TReferPtr<Ty> & That) noexcept
		{
			if constexpr (IsConvertibleToV<Ty *, T *>)
			{
				if (static_cast<void *>(Pointer) != static_cast<void *>(That.Get()))
				{
					Pointer = static_cast<T *>(That.Pointer);
					if (Pointer)
						Pointer->AddRef();
				}
			}
			else if constexpr (IsReferableWithComV<T>)
			{
				That.Pointer->QueryInterface(__uuidof(T), GetVV());
			}
			else { }
		}

		~TReferPtr() noexcept
		{
			if (Pointer)
			{
				Pointer->Release();
				Pointer = nullptr;
			}
		}

		bool operator ==(const TReferPtr &) const noexcept = default;
		bool operator ==(const T * Other) const noexcept { return Pointer == Other; }
		friend bool operator ==(const T * Other, const TReferPtr & This) noexcept { return Other == This.Pointer; }
		auto operator <=>(const TReferPtr &) const noexcept = default;

		T * operator ->() const
		{
			if (!Pointer)
				throw std::exception("null com ptr");
			return Pointer;
		}

		T & operator *() { return *Pointer; }
		T ** operator &() { return &Pointer; }

		const T & operator *() const { return *Pointer; }
		operator T *() const { return Pointer; }

		template<typename Ty> requires ConvertibleToC<T *, Ty *>
		operator Ty *() const { return Pointer; }

		template<typename Ty> requires ConvertibleToC<T *, Ty *>
		operator TPointerPtr<Ty> () const { return Pointer; }

		explicit operator bool() const { return !!Pointer; }

		//TReferPtr & operator =(T * That) noexcept
		//{
		//    if (Pointer == That)
		//        return *this;

		//    if (Pointer)
		//    {
		//        Pointer->Release();
		//        Pointer = nullptr;
		//    }

		//    if (That)
		//    {
		//        That->AddRef();
		//        Pointer = That;
		//    }
		//    return *this;
		//}

		TReferPtr & operator =(const TReferPtr & That) noexcept
		{
			if (Pointer == That.Pointer)
				return *this;

			if (Pointer)
			{
				Pointer->Release();
				Pointer = nullptr;
			}

			if (That.Pointer)
			{
				That.Pointer->AddRef();
				Pointer = That.Pointer;
			}
			return *this;
		}

		TReferPtr & operator =(TReferPtr && That) noexcept
		{
			if (Pointer == That.Pointer)
			{
				if (That.Pointer)
				{
					That.Pointer->Release();
					That.Pointer = nullptr;
				}
				return *this;
			}

			if (Pointer)
			{
				Pointer->Release();
				Pointer = nullptr;
			}

			if (That.Pointer)
			{
				Pointer = That.Pointer;
				That.Pointer = nullptr;
			}
			return *this;
		}

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TReferPtr & operator =(const TPointerPtr<Ty> & That) noexcept
		{
			Reset();
			Pointer = StaticCast<T>(That.Pointer);
			if (Pointer)
				Pointer->AddRef();
			return *this;
		}

		void Reset()
		{
			if (Pointer)
			{
				Pointer->Release();
				Pointer = nullptr;
			}
		}

		void Reset(T * Other)
		{
			if (Pointer == Other)
				return;

			if (Pointer)
			{
				Pointer->Release();
				Pointer = nullptr;
			}
			if (Other)
				Other->AddRef();
			Pointer = Other;
		}

		T * Get() const { return Pointer; }
		T & Ref() const { return *Pointer; }

		T ** GetPP()
		{
			AssertExpr(Pointer == nullptr);
			return &Pointer;
		}

		T * const * GetPP() const
		{
			AssertExpr(Pointer == nullptr);
			return &Pointer;
		}

		T ** GetPP_unsafe() const
		{
			return &Pointer;
		}

		void ** GetVV() const
		{
			if (Pointer) throw 1;
			return reinterpret_cast<void **>(const_cast<T **>(&Pointer));
		}

		void ** GetVVSafe() const
		{
			if (Pointer)
			{
				const_cast<TReferPtr *>(this)->Pointer->Release();
				const_cast<TReferPtr *>(this)->Pointer = nullptr;
			}
			return reinterpret_cast<void **>(const_cast<T **>(&Pointer));
		}

		template<typename UT2>
		UT2 ** GetTT() const
		{
			if (Pointer) throw 1;
			return reinterpret_cast<UT2 **>(GetVV());
		}

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		T * Attatch(Ty * That)
		{
			ReferRelease(Pointer);
			Pointer = That;
			return Pointer;
		}

		T * Detatch()
		{
			T * Result = nullptr;
			if (Pointer)
			{
				Result = Pointer;
				Pointer = nullptr;
			}
			return Result;
		}

	public: // ComReferAble
		template<ReferableWithComC T2>
		TReferPtr<T2> QueryInterface() const requires IsReferableWithComV<T>
		{
			if (!Pointer)
				return nullptr;

			TReferPtr<T2> Result;
			Pointer->QueryInterface(__uuidof(T2), Result.GetVV());
			return Result;
		}

		uintx GetReferenceCount() const
		{
			if (!Pointer)
				return 0;

			if (IsReferableWithComV<T>)
			{
				Pointer->AddRef();
				return Pointer->Release();
			}
			else if (IsReferableWithReferCountV<T>)
			{
				return Pointer->ReferCount;
			}
			else
				throw 0;
		}

	public:
		T * Pointer = nullptr;
	};

	template<typename T>
	inline constexpr bool IsReferPointerV = false;

	template<typename T>
	inline constexpr bool IsReferPointerV<TReferPtr<T>> = true;

	template<typename T>
	concept ReferPointerC = IsReferPointerV<T>;

	template<ReferPointerC T>
	using ReferPointeeT = typename T::PointeeT;


	template<ReferableC Ty, ReferPointerC T>
	TReferPtr<Ty> StaticCast(const T & Pointer)
	{
		return TReferPtr<Ty>(static_cast<Ty *>(Pointer.Get()));
	}

	template<ReferableC Ty, ReferPointerC T>
	Ty & StaticCastRef(const T & Pointer)
	{
		return StaticCastRef<Ty>(Pointer.Get());
	}

	template<typename T>
	struct THash<TReferPtr<T>>
	{
		static_assert(IsHashAbleV<T *>);

		uintx operator ()(const TReferPtr<T> & Value) const noexcept
		{
			return Hash(Value.Pointer);
		}
	};

	class BASE_API IRefer
	{
	public:
		IRefer() = default;
		virtual ~IRefer() = default;

	public:
		virtual uintx AddRef() noexcept = 0;
		virtual uintx Release() noexcept = 0;

		virtual FTypeid Typeid() const;
		virtual const FType & Type() const;

		bool Is(const FType & ThatType) const noexcept;
		bool IsA(const FType & ThatType) const noexcept;

		template<typename T>
		bool Is() const noexcept { return Is(Typeof<T>()); }

		template<typename T>
		bool IsA() const noexcept { return IsA(Typeof<T>()); }

	public: // --- operator  new delete
		void * operator new(sizet Size)
		{
			return Malloc(Size, 0);
		}

		void * operator new(sizet Size, alignt Alignment)
		{
			return Malloc(Size, static_cast<sizet>(Alignment));
		}

		void operator delete(void * Address)
		{
			Free(Address, 0);
		}

		void operator delete(void * Address, alignt Alignment)
		{
			Free(Address, static_cast<sizet>(Alignment));
		}

		template<typename... ParametersT>
		void * operator new(sizet Size, ParametersT &&... Parameters)
		{
			return ::operator new(Size, Forward<ParametersT>(Parameters)...);
		}

		void * operator new(sizet Size, int)
		{
			return ::operator new(Size);
		}

		template<typename... ParametersT>
		void * operator new(sizet Size, std::nothrow_t NoThrow, ParametersT &&... Parameters)
		{
			return ::operator new(Size, NoThrow, Forward<ParametersT>(Parameters)...);
		}

		template<typename... ParametersT>
		void * operator new(sizet Size, alignt Alignment, ParametersT &&... Parameters)
		{
			return ::operator new(Size, Alignment, Forward<ParametersT>(Parameters)...);
		}

		template<typename... ParametersT>
		void * operator new(sizet Size, alignt Alignment, void * Pointer, ParametersT &&... Parameters)
		{
			return ::operator new(Size, Alignment, Forward<ParametersT>(Parameters)...);
		}

		template<typename... ParametersT>
		void operator delete(void * Pointer, ParametersT &&... Parameters)
		{
			return ::operator delete(Pointer, Forward<ParametersT>(Parameters)...);
		}

		template<typename... ParametersT>
		void operator delete(void * Pointer, alignt Alignment, ParametersT &&... Parameters)
		{
			return ::operator delete(Pointer, Alignment, Forward<ParametersT>(Parameters)...);
		}
	};

	class BASE_API ISharedRefer : public IRefer
	{
	public:
		ISharedRefer() = default;
		virtual ~ISharedRefer() = default;

	public:
		uintx AddRef() noexcept override
		{
			return Atomics::IncFetch(SharedRefer);
		}

		uintx Release() noexcept override
		{
			AssertExpr(SharedRefer > 0);
			uintx Result = Atomics::DecFetch(SharedRefer);
			if (Result == 0)
				delete this;
			return Result;
		}

	public:
		uintx SharedRefer = 0;
	};

	class IObject;

	class BASE_API FObjectEntry
	{
	public:
		FObjectEntry() = default;
		FObjectEntry(IObject * Pointer) : Pointer(Pointer) {}

		forceinline  uintx AddSharedRefer() noexcept
		{
			return Atomics::IncFetch(SharedRefer);
		}

		forceinline uintx ReleaseSharedRefer() noexcept
		{
			AssertExpr(SharedRefer > 0);
			if (!SharedRefer)
				return 0;

			uintx ModifiedValue = Atomics::DecFetch(SharedRefer);
			if (ModifiedValue == 0)
				ReleaseEntryRefer();
			return ModifiedValue;
		}

		forceinline uintx AddEntryRefer() noexcept
		{
			AssertExpr(EntryRefer > 0);
			return Atomics::IncFetch(EntryRefer);
		}

		forceinline uintx ReleaseEntryRefer() noexcept
		{
			AssertExpr(EntryRefer > 0);
			if (EntryRefer == 0)
				return 0;

			uintx ModifiedValue = Atomics::DecFetch(EntryRefer);
			if (ModifiedValue == 0)
				delete this;
			return ModifiedValue;
		}

		forceinline IObject * Lock()
		{
			uintx CurrentValue = SharedRefer;
			while (true)
			{
				if (CurrentValue == 0)
					return nullptr;

				if (Atomics::CompareExchange(SharedRefer, CurrentValue, CurrentValue + 1))
					break;
			}
			return Pointer;
		}

	public:
		IObject * Pointer = nullptr;
		uintx SharedRefer = 1;
		uintx EntryRefer = 1;
	};


	template<ReferableC T, typename... ArgsT> requires IsConstructibleV<T, ArgsT...>
	TReferPtr<T> MakeRefer(ArgsT &&... Args)
	{
		static_assert(!IsDerivedOfV<T, ISharedRefer>);
		static_assert(!IsAbstractV<T>, "to new an abstract T is not valid.");

		T * Pointer = (T *)Malloc(sizeof(T), alignof(T));
		new (Pointer) T(Forward<ArgsT>(Args)...);

		if constexpr (ReferableWithConstructC<T>)
			Pointer->Construct();
		TReferPtr<T> ReferPtr;
		ReferPtr.Attatch(Pointer);
		return ReferPtr;
	}

	inline FObjectEntry * EntryAddRef(FObjectEntry * Entry)
	{
		if (Entry)
			Entry->AddEntryRefer();
		return Entry;
	}

	inline FObjectEntry * EntryRelease(FObjectEntry * Entry)
	{
		if (Entry)
			Entry->ReleaseEntryRefer();
		return Entry;
	}
}
