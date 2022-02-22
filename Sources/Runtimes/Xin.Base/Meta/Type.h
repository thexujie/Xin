#pragma once

#include "Meta.h"

namespace Xin
{
	constexpr FName ReferPointerSubTypeName = u8"ReferPointer"N;

	struct BASE_API FType : FMeta
	{
	public:
		FType() = default;
		FType(FName Name, FTypeid Typeid, const FMetadata & Metadata);
		virtual ~FType() = default;

	public:
		virtual FType & GetPointeeType() const { return None; }
		virtual FType & GetTemplateType() const { return None; }

	public:
		bool Is(const FType & That) const { return operator ==(That); }

		virtual bool IsA(const FType & That) const
		{
			if (!Valid() || !That.Valid())
				return false;

			if (operator ==(That))
				return true;

			if (GetPointeeType().IsA(That.GetPointeeType()))
				return true;

			if (GetTemplateType().IsA(That.GetTemplateType()))
				return true;

			return false;
		}

		template<typename T>
		bool Is() const;

		template<typename T>
		bool IsA() const;

	public:
		FTypeid Typeid;

	public:
		static FType None;
		static TView<FType &> AllTypes();
		static FType & FromName(FName Name);
		static FType & FromTypeid(FTypeid Typeid);
	};

	template<typename T>
	inline constexpr bool IsBuiltinReflectedV = IsAnyOfV<T, void, nullptr_t, bool, chara, char8, char16, char32, int8, int16, int32, int64, uint8, uint16, uint32, uint64, float32, float64>;

	template<typename T>
	concept BuiltinReflectedC = IsBuiltinReflectedV<T>;

	template<typename T>
	concept StaticReflectedC = requires
	{
		{ T::StaticType() } -> SameC<FType &>;
	};
	template<typename T>
	concept TStaticTypeReflectedC = requires
	{
		{ TStaticType<T>::Type() } -> SameC<FType &>;
	};

	struct FTypeHelper
	{
		template<typename T>
		static FType & Type() { return FType::None; }
	};

	template<typename T>
	FType & Typeof()
	{
		// to make sure T is defined.
		static_assert(sizeof(T) > 0);

		if constexpr (TStaticTypeReflectedC<T>)
			return TStaticType<T>::Type();
		else if constexpr (StaticReflectedC<T>)
			return T::StaticType();
		else
			return FType::FromTypeid(typeid(T));
	}

	template<typename T>
	class TBuiltinType : public FType
	{
	public:
		TBuiltinType() = default;
		TBuiltinType(FName Name, const FMetadata & Metadata = FMetadata::None) : FType(Name, typeid(T), Metadata) {}
	};

	extern template BASE_API FType & Typeof<void>();
	extern template BASE_API FType & Typeof<bool>();
	extern template BASE_API FType & Typeof<nullptr_t>();
	extern template BASE_API FType & Typeof<int8>();
	extern template BASE_API FType & Typeof<uint8>();
	extern template BASE_API FType & Typeof<int16>();
	extern template BASE_API FType & Typeof<uint16>();
	extern template BASE_API FType & Typeof<int32>();
	extern template BASE_API FType & Typeof<uint32>();
	extern template BASE_API FType & Typeof<int64>();
	extern template BASE_API FType & Typeof<uint64>();
	extern template BASE_API FType & Typeof<float32>();
	extern template BASE_API FType & Typeof<float64>();

	template<typename T>
	concept ReflectedC = BuiltinReflectedC<T> || StaticReflectedC<T>;

	template<ReflectedC Ty, ReflectedC T>
	const Ty * DynamicCast(const T * Pointer)
	{
		const FType & Type = Typeof<T>();
		const FType & TypeY = Typeof<Ty>();
		if (Type.IsA<Ty>() || TypeY.IsA<T>())
			return static_cast<const Ty *>(Pointer);
		return nullptr;
	}

	template<ReflectedC Ty, ReflectedC T>
	TReferPtr<Ty> DynamicCast(TReferPtr<T> Pointer)
	{
		const FType & Type = Typeof<T>();
		const FType & TypeY = Typeof<Ty>();
		if (Type.IsA<Ty>() || TypeY.IsA<T>())
			return TReferPtr<Ty>(static_cast<Ty *>(Pointer.Get()));
		return nullptr;
	}

	template<typename T>
	bool FType::Is() const { return Is(Typeof<T>()); }

	template<typename T>
	bool FType::IsA() const { return IsA(Typeof<T>()); }

	template<typename T>
	class TTypeRegister
	{
	public:
		TTypeRegister(FType &)
		{
		}
	};

	class BASE_API FPointerType : public FType
	{
	public:
		FPointerType(FName Name, const FType & PointeeType) : FType(Name, FTypeid::None, FMetadata::None), PointeeType(ConstCast(PointeeType)) {}

	public:
		FType & GetPointeeType() const override { return PointeeType; }

	public:
		FType & PointeeType;

	public:
		static FType & GetDecoratedType(FType & Type);
	};

	template<typename T>
	struct TStaticType<T *>
	{
		static FType & Type()
		{
			static_assert(sizeof(T) > 0);
			if constexpr (TStaticTypeReflectedC<T>)
				return FPointerType::GetDecoratedType(TStaticType<T>::Type());
			else if constexpr (StaticReflectedC<T>)
				return FPointerType::GetDecoratedType(T::StaticType());
			else
				return FPointerType::GetDecoratedType(FType::FromTypeid(typeid(T)));
		}
	};
	template<typename T>
	struct TStaticType<const T *>
	{
		static FType & Type()
		{
			static_assert(sizeof(T) > 0);
			if constexpr (TStaticTypeReflectedC<T>)
				return FPointerType::GetDecoratedType(TStaticType<T>::Type());
			else if constexpr (StaticReflectedC<T>)
				return FPointerType::GetDecoratedType(T::StaticType());
			else
				return FPointerType::GetDecoratedType(FType::FromTypeid(typeid(T)));
		}
	};

	class BASE_API FReferPointerType : public FType
	{
	public:
		FReferPointerType(FName Name, const FType & PointeeType) : FType(Name, FTypeid::None, FMetadata::None), PointeeType(ConstCast(PointeeType)) {}

	public:
		FType & GetPointeeType() const override { return PointeeType; }

	public:
		FType & PointeeType;

	public:
		static FType & GetDecoratedType(const FType & Type);
	};

	template<typename T>
	struct TStaticType<TReferPtr<T>>
	{
		static FType & Type()
		{
			static_assert(sizeof(T) > 0);
			if constexpr (TStaticTypeReflectedC<T>)
				return FReferPointerType::GetDecoratedType(TStaticType<T>::Type());
			else if constexpr (StaticReflectedC<T>)
				return FReferPointerType::GetDecoratedType(T::StaticType());
			else
				return FReferPointerType::GetDecoratedType(FType::FromTypeid(typeid(T)));
		}
	};

	class BASE_API FListType : public FType
	{
	public:
		FListType(FName Name, const FType & PointeeType) : FType(Name, FTypeid::None, FMetadata::None), PointeeType(ConstCast(PointeeType)) {}

	public:
		FType & GetPointeeType() const override { return PointeeType; }

	public:
		FType & PointeeType;

	public:
		static FType & GetDecoratedType(const FType & Type);
	};

	template<typename T>
	struct TStaticType<TList<T>>
	{
		static FType & Type()
		{
			static_assert(sizeof(T) > 0);
			if constexpr (TStaticTypeReflectedC<T>)
				return FListType::GetDecoratedType(TStaticType<T>::Type());
			else if constexpr (StaticReflectedC<T>)
				return FListType::GetDecoratedType(T::StaticType());
			else
				return FListType::GetDecoratedType(FType::FromTypeid(typeid(T)));
		}
	};

	inline FTypeid IRefer::Typeid() const
	{
		return FTypeid::None;
	}

	inline const FType & IRefer::Type() const
	{
		return FType::None;
	}

	inline bool IRefer::Is(const FType & ThatType) const noexcept
	{
		return this && Type().Is(ThatType);
	}

	inline bool IRefer::IsA(const FType & ThatType) const noexcept
	{
		return this && Type().IsA(ThatType);
	}

	template<typename T>
	const FType & TVariantInlineValueAdapter<T>::Type() const
	{
		return Typeof<T>();
	}

	template<typename T>
	bool TVariantInlineValueAdapter<T>::EqualWith(const TInlineStorage<7> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<7> & ThatInlineStorage) const
	{
		if constexpr (EqualComparableC<T>)
		{
			const FType & ThisType = Type();
			const FType & ThatType = ThatVariantAdapter->Type();
			if (ThisType != ThatType)
				return false;

			T & This = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(ThisInlineStorage).Ref();
			T & That = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(ThatInlineStorage).Ref();
			return This == That;
		}
		else
			return false;
	}

	template<typename T>
	CompareOrderingT TVariantInlineValueAdapter<T>::CompareWith(const TInlineStorage<7> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<7> & ThatInlineStorage) const
	{
		if constexpr (ComparableC<T>)
		{
			const FType & ThisType = Type();
			const FType & ThatType = ThatVariantAdapter->Type();
			if (ThisType != ThatType)
				return ThisType <=> ThatType;

			T & This = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(ThisInlineStorage).Ref();
			T & That = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(ThatInlineStorage).Ref();
			return This <=> That;
		}
		else
			return this <=> ThatVariantAdapter;
	}

	template<typename T>
	const FType & TVariantAdapter<T>::Type() const
	{
		return Typeof<T>();
	}

	template<typename T>
	bool TVariantAdapter<T>::EqualWith(const TInlineStorage<7> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<7> & ThatInlineStorage) const
	{
		if (!ThatVariantAdapter)
			return false;

		TVariantAdapter & OtherAdapter = StaticCastRef<TVariantAdapter>(ThatVariantAdapter);
		if constexpr (ComparableC<T>)
			return Value == OtherAdapter.Value;
		else
			return false;
	}

	template<typename T>
	CompareOrderingT TVariantAdapter<T>::CompareWith(const TInlineStorage<7> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<7> & ThatInlineStorage) const
	{
		if (!ThatVariantAdapter)
			return ConstCast(this) <=> ThatVariantAdapter;

		TVariantAdapter & OtherAdapter = StaticCastRef<TVariantAdapter>(ThatVariantAdapter);
		if constexpr (ComparableC<T>)
			return Value <=> OtherAdapter.Value;
		else
			return ConstCast(this) <=> ThatVariantAdapter;
	}
}
