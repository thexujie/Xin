#pragma once

#include "Basic.Types.h"

namespace Xin
{
	template<typename T>
	struct ConcreteT
	{
		using Type = T;
	};

	template<typename T>
	typename ConcreteT<T>::Type * ConcreteCast(T * Resource)
	{
		return static_cast<typename ConcreteT<T>::Type *>(Resource);
	}

	template<typename T>
	const typename ConcreteT<T>::Type * ConcreteCast(const T * Resource)
	{
		return static_cast<const typename ConcreteT<T>::Type *>(Resource);
	}

	template<typename T>
	class TPointerPtr
	{
	public:
		using PointeeT = T;

	public:
		TPointerPtr() = default;
		TPointerPtr(const TPointerPtr & That) noexcept = default;
		TPointerPtr(TPointerPtr && That) noexcept : Pointer(That.Pointer) { That.Pointer = nullptr; }

		TPointerPtr & operator =(const TPointerPtr & That) noexcept
		{
			Pointer = That.Pointer;
			return *this;
		}

		TPointerPtr & operator =(TPointerPtr && That) noexcept
		{
			Pointer = That.Pointer;
			That.Pointer = nullptr;
			return *this;
		}

		bool operator ==(const TPointerPtr &) const noexcept = default;
		auto operator <=>(const TPointerPtr &) const noexcept = default;
		~TPointerPtr() { if (Pointer) { Pointer = nullptr; } }

		TPointerPtr(std::nullptr_t) { }

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TPointerPtr(const TPointerPtr<Ty> & That) : Pointer(That.Pointer) {}

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TPointerPtr(TPointerPtr<Ty> && That) noexcept : Pointer(That.Pointer) { That.Pointer = nullptr; }

		TPointerPtr(T * Pointer) : Pointer(Pointer) { }

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TPointerPtr(Ty * Pointer) : Pointer(Pointer) { }

		T * operator ->() const
		{
			if (!Pointer)
				throw std::exception("null com ptr");
			return Pointer;
		}

		T & operator *()
		{
			if (!Pointer)
				throw EError::Nullptr;
			return *Pointer;
		}

		const T & operator *() const
		{
			if (!Pointer)
				throw EError::Nullptr;
			return *Pointer;
		}


		operator T *() { return Pointer; }
		operator const T *() const { return Pointer; }
		explicit operator bool() const { return !!Pointer; }

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TPointerPtr & operator =(const TPointerPtr<Ty> & That)
		{
			Pointer = That.Pointer;
			return *this;
		}

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TPointerPtr & operator =(Ty * That)
		{
			Pointer = That;
			return *this;
		}

		void Reset() { if (Pointer) { Pointer = nullptr; } }

		T * Get() const { return Pointer; }

		T ** GetPP() const
		{
			if (Pointer) throw EError::State;
			return const_cast<T **>(&Pointer);
		}

		void ** GetVV() const
		{
			if (Pointer) throw EError::State;
			return reinterpret_cast<void **>(const_cast<T **>(&Pointer));
		}

		template<typename Ty>
		Ty ** GetTT() const
		{
			if (Pointer) throw EError::State;
			return reinterpret_cast<Ty **>(GetVV());
		}

		const T & Ref() const { return *Pointer; }
		T & Ref() { return *Pointer; }

	public:
		T * Pointer = nullptr;
	};

	template<typename T>
	inline constexpr bool IsPointerPointerV = false;

	template<typename T>
	inline constexpr bool IsPointerPointerV<TPointerPtr<T>> = true;

	template<typename T>
	concept PointerPointerC = IsPointerPointerV<T>;

	template<PointerPointerC T>
	using PointerPointeeT = typename T::PointeeT;

	template<typename Ty, PointerPointerC T>
	TPointerPtr<Ty> StaticCast(const T & Pointer)
	{
		return TPointerPtr<Ty>(static_cast<Ty *>(Pointer.Get()));
	}

	template<typename Ty, PointerPointerC T>
	Ty & StaticCastRef(const T & Pointer)
	{
		return *(static_cast<Ty *>(Pointer.Get()));
	}

	template<typename T, typename... ArgsT>
	TPointerPtr<T> MakePointer(ArgsT &&... Args)
	{
		return TPointerPtr<T>(new T(Forward<ArgsT>(Args)...));
	}
}
