#pragma once

#include "Basic.Types.h"
#include "Name.h"
#include "Typeid.h"
#include "Refer.h"
#include "Pointer.h"
#include "Strings.h"

namespace Xin
{
	class BASE_API IObject : public IRefer
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		IObject();
		IObject(FName Name);
		IObject(const IObject &) = delete;
		IObject(IObject &&) noexcept = delete;
		~IObject() override;

	public:
		[[deprecated("error: It is forbidden to Create IObject by new operator, please use MakeRefer<T> instead.")]]
		void * operator new(sizet Size)
		{
			return Malloc(Size, 0);
		}

		[[deprecated("error: It is forbidden to Create IObject by new operator, please use MakeRefer<T> instead.")]]
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

		template<typename... ParametersT>
		void operator delete(void * Pointer, ParametersT &&... Parameters)
		{
			return ::operator delete(Pointer, Forward<ParametersT>(Parameters)...);
		}

	public:
		forceinline uintx AddRef() noexcept final
		{
			AssertExpr(ObjectEntry);
			return ObjectEntry->AddSharedRefer();
		}

		forceinline uintx Release() noexcept final
		{
			AssertExpr(ObjectEntry);
			AssertExpr(ObjectEntry->SharedRefer > 0);

			uintx ModifiedValue = ObjectEntry->ReleaseSharedRefer();
			if (!ModifiedValue)
			{
				ObjectEntry = nullptr;
				delete this;
			}
			return ModifiedValue;
		}

		virtual void Dispose() noexcept {}

	public:
		virtual IObject * GetParentObject() const { return nullptr; }
		virtual IObject * GetChildObject(FName Name) const { return nullptr; }

		template<DerivedOfC<IObject> T>
		T * GetChildObjectT(FName Name) const
		{
			if (IObject * ChildObject = GetChildObject(Name))
			{
				if (ChildObject->IsA<T>())
					return StaticCast<T>(ChildObject);
			}
			return nullptr;
		}

	public:
		virtual void SetName(FName Name) { this->Name = Name; }

		virtual FStringV ToString(FStringV Formal = FStringV::None) const { return FStringV::None; }
		virtual FStringV Describe() const { return FStringV::None; }

	public:
		FName Name;
		FObjectEntry * ObjectEntry = nullptr;
	};

	template<typename T = IObject>
	class TEntryPtr
	{
	public:
		using PointeeT = T;

	public:
		TEntryPtr() noexcept = default;
		TEntryPtr(std::nullptr_t) noexcept { }
		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TEntryPtr(Ty * Object) noexcept : Entry(EntryAddRef(Object ? Object->ObjectEntry : nullptr)) {}

		TEntryPtr(const TPointerPtr<T> & That) noexcept : TEntryPtr(That.Pointer) { }

		~TEntryPtr() noexcept { Reset(); }

		bool operator ==(const TEntryPtr &) const noexcept = default;

		// is this thread safe? you should know about it by yourself.
		bool operator ==(const T * Another) const noexcept { return Entry && Entry->Pointer == Another; }

		bool operator ==(NullptrT) const noexcept { return !Entry || Entry->Pointer == nullptr; }

		auto operator <=>(const TEntryPtr &) const noexcept = default;

		friend bool operator ==(const T * Another, const TEntryPtr & This) noexcept { return (This.Entry ? This.Entry->Pointer : nullptr) == Another; }

		TEntryPtr & operator =(std::nullptr_t)
		{
			Reset();
			return *this;
		}

		operator bool() const { return !!Entry; }

		TEntryPtr(const TEntryPtr & That) noexcept : Entry(EntryAddRef(That.Entry)) {}

		TEntryPtr(TEntryPtr && That) noexcept
		{
			if (That.Entry)
			{
				Entry = That.Entry;
				That.Entry = nullptr;
			}
		}

		TEntryPtr & operator =(const TEntryPtr & That) noexcept
		{
			Reset();
			Entry = EntryAddRef(That.Entry);
			return *this;
		}


		template<typename Ty> requires (SameC<Ty, T> || ConvertibleToC<Ty *, T *>)
		TEntryPtr & operator =(Ty * That) noexcept
		{
			Reset();
			if (That)
				Entry = EntryAddRef(That->ObjectEntry);
			return *this;
		}

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TEntryPtr & operator =(const TPointerPtr<Ty> & That) noexcept
		{
			Reset();
			if (That.Pointer)
				Entry = EntryAddRef(That.Pointer->Entry);
			return *this;
		}
		
		template<typename Ty> requires (SameC<Ty, T> || ConvertibleToC<Ty *, T *>)
		TEntryPtr(const TReferPtr<Ty> & That)
		{
			Reset();
			if (That.Pointer)
				Entry = EntryAddRef(That.Pointer->ObjectEntry);
		}

		template<typename Ty> requires (SameC<Ty, T> || ConvertibleToC<Ty *, T *>)
		TEntryPtr & operator =(const TReferPtr<Ty> & That) noexcept
		{
			Reset();
			if (That.Pointer)
				Entry = EntryAddRef(That.Pointer->ObjectEntry);
			return *this;
		}

		template<typename Ty> requires ConvertibleToC<Ty *, T *>
		TEntryPtr & operator =(const TEntryPtr<Ty> & That) noexcept
		{
			Reset();
			Entry = EntryAddRef(That.Entry);
			return *this;
		}

		void Reset() noexcept
		{
			EntryRelease(Entry);
			Entry = nullptr;
		}

		TReferPtr<T> Lock() const noexcept
		{
			if (!Entry)
				return nullptr;

			if (IObject * Object = Entry->Lock())
			{
				TReferPtr<T> Result { static_cast<T *>(Object) };
				ReferRelease(Object);
				return Result;
			}
			else
				return nullptr;
		}

	public:
		FObjectEntry * Entry = nullptr;
	};

	using FEntryPtr = TEntryPtr<>;

	template<typename T>
	inline constexpr bool IsEntryPointerV = false;

	template<typename T>
	inline constexpr bool IsEntryPointerV<TEntryPtr<T>> = true;

	template<typename T>
	concept EntryPointerC = IsEntryPointerV<T>;

	template<EntryPointerC T>
	using EntryPointeeT = typename T::PointeeT;
}
