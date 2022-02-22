#pragma once

#include "Basic.Types.h"

namespace Xin
{
	class IPointerReferencer
	{
	public:
		IPointerReferencer() = default;
		virtual ~IPointerReferencer() = default;
	};

	template<typename T>
	class TPointerReferencer : public IPointerReferencer
	{
	public:
		TPointerReferencer(T * Pointer) : Pointer(Pointer) { }
		virtual void DestroyPointer() = 0;

		void AddWeakRefer()
		{
			AssertExpr(WeakRefer > 0);
			Atomics::IncFetch(WeakRefer);
		}

		void ReleaseWeakReference()
		{
			AssertExpr(WeakRefer > 0);
			if (Atomics::DecFetch(WeakRefer) == 0)
			{
				DestroyPointer();
				delete this;
			}
		}

		void AddSharedRefer()
		{
			AssertExpr(SharedRefer > 0);
			Atomics::IncFetch(SharedRefer);
		}

		void ReleaseSharedReference()
		{
			AssertExpr(SharedRefer > 0);
			if (Atomics::DecFetch(SharedRefer) == 0)
				ReleaseWeakReference();
		}

	public:
		T * Pointer = nullptr;
		uintx SharedRefer = 1;
		uintx WeakRefer = 1;
	};

	template<typename T, typename DeleterT>
	class TDefaultPointerReferencer : public TPointerReferencer<T>
	{
	public:
		TDefaultPointerReferencer(const TDefaultPointerReferencer &) = delete;
		TDefaultPointerReferencer & operator =(const TDefaultPointerReferencer &) = delete;

		TDefaultPointerReferencer(T * Pointer, DeleterT && Deleter) : TPointerReferencer<T>(Pointer), Deleter(Move(Deleter)) { }

		void DestroyPointer() override
		{
			Deleter(TPointerReferencer<T>::Pointer);
		}

	private:
		DeleterT Deleter;
	};

	template<typename T>
	class TSharedPtr
	{
	public:
		template<typename ThatT>
		friend class TSharedPtr;

	public:
		TSharedPtr() = default;
		TSharedPtr(std::nullptr_t) {}
		TSharedPtr(const TSharedPtr & That) : Referencer(That.Referencer) { if (Referencer) Referencer->AddSharedRefer(); }
		TSharedPtr(TSharedPtr && That) noexcept : Referencer(That.Referencer) { That.Referencer = nullptr; }

		TSharedPtr & operator =(const TSharedPtr & That)
		{
			Referencer = That.Referencer;
			if (Referencer) Referencer->AddSharedRefer();
			return *this;
		}

		~TSharedPtr() { Reset(); }

		bool operator ==(const TSharedPtr &) const noexcept = default;
		auto operator <=>(const TSharedPtr &) const noexcept = default;

		explicit TSharedPtr(T * Pointer) : Referencer(new TDefaultPointerReferencer(Pointer, TDeleterDefault<T>())) {}

		template<typename DeleterT>
		TSharedPtr(T * Pointer, DeleterT && Deleter) : Referencer(new TDefaultPointerReferencer(Pointer, Deleter)) {}

		template<typename Ty> requires (SameC<Ty *, T *> || ConvertibleToC<Ty *, T *>)
		TSharedPtr(TPointerReferencer<Ty> * Referencer) : Referencer(reinterpret_cast<TPointerReferencer<T> *>(Referencer))
		{
			Referencer->AddSharedRefer();
		}

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TSharedPtr(const TSharedPtr<Ty> & That)
			: Referencer(reinterpret_cast<TPointerReferencer<T> *>(const_cast<TPointerReferencer<Ty> *>(That.Referencer)))
		{
			if (Referencer)
				Referencer->AddSharedRefer();
		}

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TSharedPtr(TSharedPtr<Ty> && That) noexcept
			: Referencer(reinterpret_cast<TPointerReferencer<T> *>(const_cast<TPointerReferencer<Ty> *>(That.Referencer)))
		{
			That.Referencer = nullptr;
		}

		TSharedPtr & operator =(std::nullptr_t)
		{
			Reset();
			return *this;
		}

		int32 GetSharedReferenceCount() const { return Referencer ? Referencer->GetSharedReferenceCount() : 0; }
		int32 GetWeakReferenceCount() const { return Referencer ? Referencer->GetWeakReferenceCount() : 0; }

		void Reset()
		{
			if (Referencer)
			{
				Referencer->ReleaseSharedReference();
				Referencer = nullptr;
			}
		}

		bool Valid() const { return Referencer != nullptr && Referencer->SharedRefer > 0; }

		T * Get() const { return Valid() ? Referencer->Pointer : nullptr; }
		T & Ref() const { return *Get(); }

		T & operator *() const { return Ref(); }
		T * operator ->() const { return Get(); }

		T & operator [](uintx Index) { return Referencer->Pointer[Index]; }
		const T & operator [](uintx Index) const { return Referencer->Pointer[Index]; }
		explicit operator bool() const { return Valid(); }

	public:
		TPointerReferencer<T> * Referencer = nullptr;
	};

	template<typename T, typename... ArgsT>
	TSharedPtr<T> MakeShared(ArgsT &&... Args) requires (!IsArrayV<T>)
	{
		return TSharedPtr<T>(new T(Forward<ArgsT>(Args)...));
	}

	template<typename Ty, typename T> requires ConvertibleToC<T *, Ty *>
	TSharedPtr<Ty> StaticPointerCast(const TSharedPtr<T> & Pointer)
	{
		return TSharedPtr<Ty>(reinterpret_cast<TPointerReferencer<Ty> *>(const_cast<TPointerReferencer<T> *>(Pointer.Referencer)));
	}

	template<typename Ty, typename T>
	TSharedPtr<Ty> ReinterpretPointerCast(const TSharedPtr<T> & Pointer)
	{
		return TSharedPtr<Ty>(reinterpret_cast<TPointerReferencer<Ty> *>(const_cast<TPointerReferencer<T> *>(Pointer.Referencer)));
	}

	template<typename T>
	class TWeakPtr
	{
	public:
		using PointerReferencerT = TPointerReferencer<T>;

	public:
		TWeakPtr() noexcept = default;
		~TWeakPtr() noexcept { Reset(); }

		TWeakPtr & operator =(std::nullptr_t)
		{
			Reset();
			return *this;
		}

		template<typename Ty> requires (SameC<Ty *, T *> || ConvertibleToC<Ty *, T *>)
		TWeakPtr(const TWeakPtr<Ty> & That) noexcept : Referencer(reinterpret_cast<PointerReferencerT *>(That.Referencer))
		{
			if (Referencer)
				Referencer->AddWeakRefer();
		}

		template<typename Ty> requires (SameC<Ty *, T *> || ConvertibleToC<Ty *, T *>)
		TWeakPtr(TWeakPtr<Ty> && That) noexcept : Referencer(reinterpret_cast<PointerReferencerT *>(That.Referencer))
		{
			That.Referencer = nullptr;
		}

		void Reset()
		{
			if (Referencer)
			{
				Referencer->ReleaseWeakReference();
				Referencer = nullptr;
			}
		}

		TSharedPtr<T> Lock() const
		{
			if (!Referencer)
				return nullptr;

			uintx ReferCount = Referencer->SharedRefer;
			while (true)
			{
				if (ReferCount == 0)
					return nullptr;

				if (Atomics::CompareExchange(Referencer->SharedRefer, ReferCount, ReferCount + 1))
					break;
			}

			Referencer->ReleaseSharedReference();
			return TSharedPtr<T>(Referencer);
		}


		template<typename Ty> requires (SameC<Ty *, T *> || ConvertibleToC<Ty *, T *>)
		TWeakPtr & operator =(const TWeakPtr<Ty> & That)
		{
			return operator =(That.Lock());
		}

		template<typename Ty> requires (SameC<Ty *, T *> || ConvertibleToC<Ty *, T *>)
		TWeakPtr & operator =(const TSharedPtr<Ty> & That)
		{
			Reset();
			if (That.Referencer && That.Referencer->SharedRefer > 0)
			{
				Referencer = reinterpret_cast<PointerReferencerT *>(That.Referencer);
				Referencer->AddWeakRefer();
			}
			return *this;
		}

	public:
		TPointerReferencer<T> * Referencer = nullptr;
	};
}
