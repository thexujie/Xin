#pragma once

#include "Basic.Types.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Thickness.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Name.h"
#include "Text.h"
#include "Strings.h"
#include "Refer.h"
#include "Object.h"
#include "Pointer.h"

namespace Xin
{
	enum class EVariantKind : uintx
	{
		None = 0,

		Integer,

		Float32,
		Float64,

		Vec2SB,
		Vec3SB,
		Vec4SB,
		Vec2UB,
		Vec3UB,
		Vec4UB,
		Vec2SS,
		Vec3SS,
		Vec4SS,
		Vec2US,
		Vec3US,
		Vec4US,
		Vec2I,
		Vec3I,
		Vec4I,
		Vec2U,
		Vec3U,
		Vec4U,
		Vec2SL,
		Vec3SL,
		Vec4SL,
		Vec2UL,
		Vec3UL,
		Vec4UL,
		Vec2F,
		Vec3F,
		Vec4F,
		Vec2D,
		Vec3D,
		Vec4D,

		Vector2,
		Vector3,
		Vector4,

		Name,
		Text,
		String,

		ReferPointer,
		ObjectPointer,

		Adapter,
	};

#define ITERATE_VARIANT_KIND \
		ITERATE_VARIANT_KIND_LOOP_BODY(int64,			Integer) \
		ITERATE_VARIANT_KIND_LOOP_BODY(float32,			Float32) \
		ITERATE_VARIANT_KIND_LOOP_BODY(float64,			Float64) \
		\
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2SB,			Vec2SB) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3SB,			Vec3SB) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4SB,			Vec4SB) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2UB,			Vec2UB) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3UB,			Vec3UB) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4UB,			Vec4UB) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2SS,			Vec2SS) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3SS,			Vec3SS) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4SS,			Vec4SS) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2US,			Vec2US) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3US,			Vec3US) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4US,			Vec4US) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2I,			Vec2I) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3I,			Vec3I) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4I,			Vec4I) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2U,			Vec2U) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3U,			Vec3U) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4U,			Vec4U) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2SL,			Vec2SL) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3SL,			Vec3SL) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4SL,			Vec4SL) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2UL,			Vec2UL) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3UL,			Vec3UL) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4UL,			Vec4UL) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2F,			Vec2F) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3F,			Vec3F) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4F,			Vec4F) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec2D,			Vec2D) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec3D,			Vec3D) \
		ITERATE_VARIANT_KIND_LOOP_BODY(Vec4D,			Vec4D) \
		\
		ITERATE_VARIANT_KIND_LOOP_BODY(FVector2,		Vector2) \
		ITERATE_VARIANT_KIND_LOOP_BODY(FVector3,		Vector3) \
		ITERATE_VARIANT_KIND_LOOP_BODY(FVector4,		Vector4) 


	constexpr uintx VariantAdapterInlineBytes = 64 - sizeof(uintx);
	constexpr uintx VariantAdapterInlinePointers = VariantAdapterInlineBytes / sizeof(voidp);
	static_assert(VariantAdapterInlineBytes % sizeof(voidp) == 0);

	template<typename T>
	constexpr inline bool IsFundamentalVariantV = IsFundamentalV<T>
		|| IsAnyOfV<T, Vec2I, Vec3I, Vec4I, Vec2U, Vec3U, Vec4U, Vec2F, Vec3F, Vec4F, Vec2D, Vec3D, Vec4D, FVector2, FVector3, FVector4, FName, FText, FStringV, FString>;

	template<typename T>
	concept FundamentalVariantC = IsFundamentalVariantV<T> && !IsReferenceV<T>;

	template<typename T>
	concept NoneFundamentalVariantC = !IsFundamentalVariantV<T> && !IsReferenceV<T>;

	template<typename T>
	constexpr inline bool IsTriviallyVariantV = IsStandardLayoutV<T> && sizeof(T) <= sizeof(FVector4);
	template<typename T>
	concept TriviallyVariantV = IsTriviallyVariantV<T>;

	template<typename T>
	using VariantValueT = ConditionalT<IsFundamentalVariantV<T>, T, T &>;

	template<uintx NumInlinePointers>
	struct TInlineStorage
	{
		static constexpr uintx NumInlineBytes = NumInlinePointers * sizeof(voidp);
		voidp Pointers[NumInlinePointers];
	};

	template<typename T, uintx NumInlinePointers>
	struct TInlineValue : TInlineStorage<NumInlinePointers>
	{
		T * Get() const
		{
			if constexpr (sizeof(T) <= TInlineStorage<NumInlinePointers>::NumInlineBytes)
				return const_cast<T *>((const T *)TInlineStorage<NumInlinePointers>::Pointers);
			else
				return const_cast<T *>((const T *)(TInlineStorage<NumInlinePointers>::Pointers[0]));
		}

		T & Ref() const
		{
			if constexpr (sizeof(T) <= TInlineStorage<NumInlinePointers>::NumInlineBytes)
				return const_cast<T &>(*(const T *)TInlineStorage<NumInlinePointers>::Pointers);
			else
				return const_cast<T &>(*(const T *)(TInlineStorage<NumInlinePointers>::Pointers[0]));
		}

		T * CopyConstruct(const T & Value)
		{
			MemoryZero(TInlineStorage<NumInlinePointers>::Pointers);
			if constexpr (sizeof(T) <= TInlineStorage<NumInlinePointers>::NumInlineBytes)
				return new(TInlineStorage<NumInlinePointers>::Pointers) T(Value);
			else
			{
				T * Pointer = new T(Value);
				TInlineStorage<NumInlinePointers>::Pointers[0] = Pointer;
				return Pointer;
			}
		}

		template<typename = void> requires MoveConstructibleC<T>
		T * MoveConstruct(T && Value)
		{
			MemoryZero(TInlineStorage<NumInlinePointers>::Pointers);
			if constexpr (sizeof(T) <= TInlineStorage<NumInlinePointers>::NumInlineBytes)
				return new(TInlineStorage<NumInlinePointers>::Pointers) T(Value);
			else
			{
				T * Pointer = new T(Move(Value));
				TInlineStorage<NumInlinePointers>::Pointers[0] = Pointer;
				return Pointer;
			}
		}

		void Dispose()
		{
			T * Pointer = Get();
			if constexpr (sizeof(T) <= TInlineStorage<NumInlinePointers>::NumInlineBytes)
				Pointer->~T();
			else
				delete Pointer;
			MemoryZero(TInlineStorage<NumInlinePointers>::Pointers);
		}
	};

	class BASE_API IVariantAdapter
	{
	public:
		virtual ~IVariantAdapter() = default;
		virtual const FType & Type() const = 0;
		virtual void Dispose(TInlineStorage<VariantAdapterInlinePointers> & InlineValue) = 0;

		virtual bool EqualWith(const TInlineStorage<VariantAdapterInlinePointers> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<VariantAdapterInlinePointers> & ThatInlineStorage) const = 0;
		virtual CompareOrderingT CompareWith(const TInlineStorage<VariantAdapterInlinePointers> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<VariantAdapterInlinePointers> & ThatInlineStorage) const = 0;

		virtual IVariantAdapter * CopyConstruct(TInlineStorage<VariantAdapterInlinePointers> & ThisInlineStorage, const TInlineStorage<VariantAdapterInlinePointers> & ThatInlineStorage) const = 0;
		virtual IVariantAdapter * MoveConstruct(TInlineStorage<VariantAdapterInlinePointers> & ThisInlineStorage, TInlineStorage<VariantAdapterInlinePointers> & ThatInlineStorage) = 0;
	};

	template<typename T>
	class TVariantInlineValueAdapter final : public IVariantAdapter
	{
		static_assert(!IsReferenceV<T>);
		static_assert(!IsDerivedOfV<T, IRefer>);
		static_assert(!IsPointerV<T> || !IsDerivedOfV<RemoveCVRefT<RemovePointerT<T>>, IRefer>);

	public:
		TVariantInlineValueAdapter() = default;

		void Dispose(TInlineStorage<VariantAdapterInlinePointers> & InlineValue) override
		{
			StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(InlineValue).Dispose();
		}

		const FType & Type() const override;

		bool EqualWith(const TInlineStorage<7> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<7> & ThatInlineStorage) const override;

		CompareOrderingT CompareWith(const TInlineStorage<7> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<7> & ThatInlineStorage) const override;

		IVariantAdapter * CopyConstruct(TInlineStorage<VariantAdapterInlinePointers> & ThisInlineStorage, const TInlineStorage<7> & ThatInlineStorage) const override
		{
			if constexpr (IsCopyConstructibleV<T>)
			{
				TInlineValue<T, VariantAdapterInlinePointers> & ThisInlineValue = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(ThisInlineStorage);
				const TInlineValue<T, VariantAdapterInlinePointers> & ThatInlineValue = StaticCast<const TInlineValue<T, VariantAdapterInlinePointers>>(ThatInlineStorage);
				ThisInlineValue.CopyConstruct(ThatInlineValue.Ref());
				return Instance();
			}
			else
			{
				static_assert(DependentFalse<T>, "Type T is not CopyConstructible.");
				return nullptr;
			}
		}

		IVariantAdapter * MoveConstruct(TInlineStorage<VariantAdapterInlinePointers> & ThisInlineStorage, TInlineStorage<VariantAdapterInlinePointers> & ThatInlineStorage) override
		{
			if constexpr (IsMoveConstructibleV<T>)
			{
				TInlineValue<T, VariantAdapterInlinePointers> & ThisInlineValue = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(ThisInlineStorage);
				TInlineValue<T, VariantAdapterInlinePointers> & ThatInlineValue = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(ThatInlineStorage);
				ThisInlineValue.MoveConstruct(Move(ThatInlineValue.Ref()));
				return Instance();
			}
			else if constexpr (IsCopyConstructibleV<T>)
			{
				TInlineValue<T, VariantAdapterInlinePointers> & ThisInlineValue = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(ThisInlineStorage);
				TInlineValue<T, VariantAdapterInlinePointers> & ThatInlineValue = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(ThatInlineStorage);
				ThisInlineValue.CopyConstruct(ThatInlineValue.Ref());
				return Instance();
			}
			else
			{
				static_assert(DependentFalse<T>, "Type T is not MoveConstructible or CopyConstructible.");
				return nullptr;
			}
		}

		T & Resolve(const TInlineStorage<VariantAdapterInlinePointers> & InlineValue)
		{
			const TInlineValue<T, VariantAdapterInlinePointers> & ThisInlineValue = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(InlineValue);
			return ThisInlineValue.Ref();
		}

	public:
		static IVariantAdapter * Create(TInlineStorage<VariantAdapterInlinePointers> & InlineValue, const T & InitValue)
		{
			if constexpr (IsCopyConstructibleV<T>)
			{
				TInlineValue<T, VariantAdapterInlinePointers> & TheInlineValue = StaticCast<TInlineValue<T, VariantAdapterInlinePointers>>(InlineValue);
				TheInlineValue.CopyConstruct(InitValue);
				return Instance();
			}
			else
			{
				static_assert(DependentFalse<T>, "Type T is not CopyConstructible.");
				return nullptr;
			}
		}

	private:
		static IVariantAdapter * Instance()
		{
			static TVariantInlineValueAdapter VariantAdapter;
			return &VariantAdapter;
		}
	};

	template<typename T>
	class TVariantAdapter final : public IVariantAdapter
	{
		static_assert(!IsReferenceV<T>);
		static_assert(!IsDerivedOfV<T, IRefer>);
		static_assert(!IsPointerV<T> || !IsDerivedOfV<RemoveCVRefT<RemovePointerT<T>>, IRefer>);

	public:
		TVariantAdapter(const T & Value) : Value(Value) { }
		TVariantAdapter(T && Value) : Value(Move(Value)) { }

		void Dispose(TInlineStorage<VariantAdapterInlinePointers> &) override
		{
			if constexpr (ObjectC<T>)
				Value.~T();
			if constexpr (sizeof(TVariantAdapter) > VariantAdapterInlineBytes)
				delete this;
		}

		const FType & Type() const override;

		bool EqualWith(const TInlineStorage<7> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<7> & ThatInlineStorage) const override;

		CompareOrderingT CompareWith(const TInlineStorage<7> & ThisInlineStorage, IVariantAdapter * ThatVariantAdapter, const TInlineStorage<7> & ThatInlineStorage) const override;

		IVariantAdapter * CopyConstruct(TInlineStorage<VariantAdapterInlinePointers> & ThisInlineStorage, const TInlineStorage<7> &ThatInlineStorage) const override
		{
			if constexpr (IsCopyConstructibleV<T>)
			{
				if constexpr (sizeof(TVariantAdapter) <= VariantAdapterInlineBytes)
					return new (ThisInlineStorage.Pointers) TVariantAdapter(Value);
				else
					return new TVariantAdapter(Value);
			}
			else
			{
				static_assert(DependentFalse<T>, "Type T is not CopyConstructible.");
				return nullptr;
			}
		}

		IVariantAdapter * MoveConstruct(TInlineStorage<VariantAdapterInlinePointers> & ThisInlineStorage, TInlineStorage<VariantAdapterInlinePointers> & ThatInlineStorage) override
		{
			if constexpr (IsMoveConstructibleV<T>)
			{
				if constexpr (sizeof(TVariantAdapter) <= VariantAdapterInlineBytes)
					return new (ThisInlineStorage.Pointers) TVariantAdapter(Move(Value));
				else
					return new TVariantAdapter(Move(Value));
			}
			else if constexpr (IsCopyConstructibleV<T>)
			{
				if constexpr (sizeof(TVariantAdapter) <= VariantAdapterInlineBytes)
					return new (ThisInlineStorage.Pointers) TVariantAdapter(Value);
				else
					return new TVariantAdapter(Value);
			}
			else
			{
				static_assert(DependentFalse<T>, "Type T is not MoveConstructible or CopyConstructible.");
				return nullptr;
			}
		}

		T & Resolve(const TInlineStorage<VariantAdapterInlinePointers> &)
		{
			return Value;
		}

	public:
		T Value;

	public:
		static IVariantAdapter * Create(TInlineStorage<VariantAdapterInlinePointers> & InlineValue, const T & InitValue)
		{
			if constexpr (IsCopyConstructibleV<T>)
			{
				if constexpr (sizeof(TVariantAdapter) <= VariantAdapterInlineBytes)
					return new (InlineValue.Pointers) TVariantAdapter(InitValue);
				else
					return new TVariantAdapter(InitValue);
			}
			else
			{
				static_assert(DependentFalse<T>, "Type T is not CopyConstructible.");
				return nullptr;
			}
		}
	};

	struct FVariantKind
	{
		uintx Value = 0;

		FVariantKind & operator =(EVariantKind VariantKind)
		{
			SetVariantKind(VariantKind);
			return *this;
		}

		operator EVariantKind() const
		{
			return GetVariantKind();
		}

	public:
		EVariantKind GetVariantKind() const
		{
			if (Value & 1)
				return EVariantKind::Adapter;
			else
				return EVariantKind(Value >> 1);
		}

		void SetVariantKind(EVariantKind VariantKind)
		{
			Value = (uintx(VariantKind) << 1) & ~1;
		}

		IVariantAdapter * GetAdapterValue() const
		{
			if (Value & 1)
				return (IVariantAdapter *)(void *)(Value & ~1);
			else
				return nullptr;
		}

		void SetAdapterValue(IVariantAdapter * VariantAdapter)
		{
			Value = uintx((void *)VariantAdapter) | 1;
		}

	public:
		PropertyRW(EVariantKind, GetVariantKind, SetVariantKind) VariantKind;
		PropertyRW(IVariantAdapter *, GetAdapterValue, SetAdapterValue) AdapterValue;
	};
	static_assert(sizeof(FVariantKind) == sizeof(uintx));


	struct BASE_API FVariant
	{
	public:
		FVariant() {}
		FVariant(NovalT) {}

		FVariant(const FVariant & That)
		{
			operator =(That);
		}

		FVariant(FVariant && That) noexcept
		{
			operator =(Move(That));
		}

		template<typename T>
		FVariant(const T & Value)
		{
			Set(Value);
		}

		~FVariant()
		{
			Reset();
		}

		bool operator ==(const FVariant & Other) const
		{
			if (Kind != Other.Kind) return false;

			switch (Kind)
			{
			case EVariantKind::None: return true;

			case EVariantKind::Integer: return IntegerValue == Other.IntegerValue;
			case EVariantKind::Float32: return Float32Value == Other.Float32Value;
			case EVariantKind::Float64: return Float64Value == Other.Float64Value;

			case EVariantKind::Vec2SB: return Vec2SBValue == Other.Vec2SBValue;
			case EVariantKind::Vec3SB: return Vec3SBValue == Other.Vec3SBValue;
			case EVariantKind::Vec4SB: return Vec4SBValue == Other.Vec4SBValue;
			case EVariantKind::Vec2UB: return Vec2UBValue == Other.Vec2UBValue;
			case EVariantKind::Vec3UB: return Vec3UBValue == Other.Vec3UBValue;
			case EVariantKind::Vec4UB: return Vec4UBValue == Other.Vec4UBValue;
			case EVariantKind::Vec2SS: return Vec2SSValue == Other.Vec2SSValue;
			case EVariantKind::Vec3SS: return Vec3SSValue == Other.Vec3SSValue;
			case EVariantKind::Vec4SS: return Vec4SSValue == Other.Vec4SSValue;
			case EVariantKind::Vec2US: return Vec2USValue == Other.Vec2USValue;
			case EVariantKind::Vec3US: return Vec3USValue == Other.Vec3USValue;
			case EVariantKind::Vec4US: return Vec4USValue == Other.Vec4USValue;
			case EVariantKind::Vec2I: return Vec2IValue == Other.Vec2IValue;
			case EVariantKind::Vec3I: return Vec3IValue == Other.Vec3IValue;
			case EVariantKind::Vec4I: return Vec4IValue == Other.Vec4IValue;
			case EVariantKind::Vec2U: return Vec2UValue == Other.Vec2UValue;
			case EVariantKind::Vec3U: return Vec3UValue == Other.Vec3UValue;
			case EVariantKind::Vec4U: return Vec4UValue == Other.Vec4UValue;
			case EVariantKind::Vec2SL: return Vec2SLValue == Other.Vec2SLValue;
			case EVariantKind::Vec3SL: return Vec3SLValue == Other.Vec3SLValue;
			case EVariantKind::Vec4SL: return Vec4SLValue == Other.Vec4SLValue;
			case EVariantKind::Vec2UL: return Vec2ULValue == Other.Vec2ULValue;
			case EVariantKind::Vec3UL: return Vec3ULValue == Other.Vec3ULValue;
			case EVariantKind::Vec4UL: return Vec4ULValue == Other.Vec4ULValue;
			case EVariantKind::Vec2F: return Vec2FValue == Other.Vec2FValue;
			case EVariantKind::Vec3F: return Vec3FValue == Other.Vec3FValue;
			case EVariantKind::Vec4F: return Vec4FValue == Other.Vec4FValue;
			case EVariantKind::Vec2D: return Vec2DValue == Other.Vec2DValue;
			case EVariantKind::Vec3D: return Vec3DValue == Other.Vec3DValue;
			case EVariantKind::Vec4D: return Vec4DValue == Other.Vec4DValue;

			case EVariantKind::Vector2: return Vector2Value == Other.Vector2Value;
			case EVariantKind::Vector3: return Vector3Value == Other.Vector3Value;
			case EVariantKind::Vector4: return Vector4Value == Other.Vector4Value;

			case EVariantKind::Name: return NameValue == Other.NameValue;
			case EVariantKind::Text: return TextValue == Other.TextValue;
			case EVariantKind::String: return StringValue == Other.StringValue;

			case EVariantKind::ReferPointer: return ReferPointerValue == Other.ReferPointerValue;
			case EVariantKind::ObjectPointer: return ObjectEntryValue == Other.ObjectEntryValue;

			case EVariantKind::Adapter: return AdapterValue == Other.AdapterValue && (!AdapterValue || AdapterValue->EqualWith(InlineValue, Other.AdapterValue, Other.InlineValue));

			default:
				return false;
			}
		}

		CompareOrderingT operator <=>(const FVariant & Other) const
		{
			if (Kind != Other.Kind)
				return Kind <=> Other.Kind;

			switch (Kind)
			{
			case EVariantKind::Integer: return IntegerValue <=> Other.IntegerValue;
			case EVariantKind::Float32: return Compare(Float32Value, Other.Float32Value);
			case EVariantKind::Float64: return Compare(Float64Value, Other.Float64Value);

			case EVariantKind::Vec2SB: return Vec2SBValue <=> Other.Vec2SBValue;
			case EVariantKind::Vec3SB: return Vec3SBValue <=> Other.Vec3SBValue;
			case EVariantKind::Vec4SB: return Vec4SBValue <=> Other.Vec4SBValue;
			case EVariantKind::Vec2UB: return Vec2UBValue <=> Other.Vec2UBValue;
			case EVariantKind::Vec3UB: return Vec3UBValue <=> Other.Vec3UBValue;
			case EVariantKind::Vec4UB: return Vec4UBValue <=> Other.Vec4UBValue;

			case EVariantKind::Vec2SS: return Vec2SSValue <=> Other.Vec2SSValue;
			case EVariantKind::Vec3SS: return Vec3SSValue <=> Other.Vec3SSValue;
			case EVariantKind::Vec4SS: return Vec4SSValue <=> Other.Vec4SSValue;
			case EVariantKind::Vec2US: return Vec2USValue <=> Other.Vec2USValue;
			case EVariantKind::Vec3US: return Vec3USValue <=> Other.Vec3USValue;
			case EVariantKind::Vec4US: return Vec4USValue <=> Other.Vec4USValue;

			case EVariantKind::Vec2I: return Vec2IValue <=> Other.Vec2IValue;
			case EVariantKind::Vec3I: return Vec3IValue <=> Other.Vec3IValue;
			case EVariantKind::Vec4I: return Vec4IValue <=> Other.Vec4IValue;
			case EVariantKind::Vec2U: return Vec2UValue <=> Other.Vec2UValue;
			case EVariantKind::Vec3U: return Vec3UValue <=> Other.Vec3UValue;
			case EVariantKind::Vec4U: return Vec4UValue <=> Other.Vec4UValue;

			case EVariantKind::Vec2SL: return Vec2SLValue <=> Other.Vec2SLValue;
			case EVariantKind::Vec3SL: return Vec3SLValue <=> Other.Vec3SLValue;
			case EVariantKind::Vec4SL: return Vec4SLValue <=> Other.Vec4SLValue;
			case EVariantKind::Vec2UL: return Vec2ULValue <=> Other.Vec2ULValue;
			case EVariantKind::Vec3UL: return Vec3ULValue <=> Other.Vec3ULValue;
			case EVariantKind::Vec4UL: return Vec4ULValue <=> Other.Vec4ULValue;

			case EVariantKind::Vec2F: return Vec2FValue <=> Other.Vec2FValue;
			case EVariantKind::Vec3F: return Vec3FValue <=> Other.Vec3FValue;
			case EVariantKind::Vec4F: return Vec4FValue <=> Other.Vec4FValue;
			case EVariantKind::Vec2D: return Vec2DValue <=> Other.Vec2DValue;
			case EVariantKind::Vec3D: return Vec3DValue <=> Other.Vec3DValue;
			case EVariantKind::Vec4D: return Vec4DValue <=> Other.Vec4DValue;

			case EVariantKind::Vector2: return Vector2Value <=> Other.Vector2Value;
			case EVariantKind::Vector3: return Vector3Value <=> Other.Vector3Value;
			case EVariantKind::Vector4: return Vector4Value <=> Other.Vector4Value;

			case EVariantKind::Name: return NameValue <=> Other.NameValue;
			case EVariantKind::Text: return TextValue <=> Other.TextValue;
			case EVariantKind::String: return StringValue <=> Other.StringValue;

			case EVariantKind::ReferPointer: return ReferPointerValue <=> Other.ReferPointerValue;
			case EVariantKind::ObjectPointer: return ObjectEntryValue <=> Other.ObjectEntryValue;

			case EVariantKind::Adapter:
				if (AdapterValue == Other.AdapterValue || !AdapterValue || !Other.AdapterValue)
					return AdapterValue <=> Other.AdapterValue;
				return AdapterValue->CompareWith(InlineValue, Other.AdapterValue, Other.InlineValue);

			default: 
				return std::strong_ordering::equal;
			}
		}

		forceinline void Reset()
		{
			if (Kind == EVariantKind::String)
				StringValue.Reset();
			else if (Kind == EVariantKind::ReferPointer)
			{
				if (ReferPointerValue)
				{
					ReferRelease(ReferPointerValue);
					ReferPointerValue = nullptr;
				}
			}
			else if (Kind == EVariantKind::ObjectPointer)
			{
				if (ObjectEntryValue)
				{
					EntryRelease(ObjectEntryValue);
					ObjectEntryValue = nullptr;
				}
			}
			else if (Kind == EVariantKind::Adapter)
			{
				if (AdapterValue)
				{
					AdapterValue->Dispose(InlineValue);
					AdapterValue = nullptr;
				}
			}
			else { }
			MemoryZero(this, sizeof(*this));
		}

		FVariant & operator =(const FVariant & That)
		{
			Reset();

			Kind = That.Kind;

			switch (Kind)
			{
			case EVariantKind::None: break;

			case EVariantKind::Integer: IntegerValue = That.IntegerValue; break;
			case EVariantKind::Float32: Float32Value = That.Float32Value; break;
			case EVariantKind::Float64: Float64Value = That.Float64Value; break;

			case EVariantKind::Vec2SB: Vec2SBValue = That.Vec2SBValue; break;
			case EVariantKind::Vec3SB: Vec3SBValue = That.Vec3SBValue; break;
			case EVariantKind::Vec4SB: Vec4SBValue = That.Vec4SBValue; break;
			case EVariantKind::Vec2UB: Vec2UBValue = That.Vec2UBValue; break;
			case EVariantKind::Vec3UB: Vec3UBValue = That.Vec3UBValue; break;
			case EVariantKind::Vec4UB: Vec4UBValue = That.Vec4UBValue; break;

			case EVariantKind::Vec2SS: Vec2SSValue = That.Vec2SSValue; break;
			case EVariantKind::Vec3SS: Vec3SSValue = That.Vec3SSValue; break;
			case EVariantKind::Vec4SS: Vec4SSValue = That.Vec4SSValue; break;
			case EVariantKind::Vec2US: Vec2USValue = That.Vec2USValue; break;
			case EVariantKind::Vec3US: Vec3USValue = That.Vec3USValue; break;
			case EVariantKind::Vec4US: Vec4USValue = That.Vec4USValue; break;

			case EVariantKind::Vec2I: Vec2IValue = That.Vec2IValue; break;
			case EVariantKind::Vec3I: Vec3IValue = That.Vec3IValue; break;
			case EVariantKind::Vec4I: Vec4IValue = That.Vec4IValue; break;
			case EVariantKind::Vec2U: Vec2UValue = That.Vec2UValue; break;
			case EVariantKind::Vec3U: Vec3UValue = That.Vec3UValue; break;
			case EVariantKind::Vec4U: Vec4UValue = That.Vec4UValue; break;

			case EVariantKind::Vec2SL: Vec2SLValue = That.Vec2SLValue; break;
			case EVariantKind::Vec3SL: Vec3SLValue = That.Vec3SLValue; break;
			case EVariantKind::Vec4SL: Vec4SLValue = That.Vec4SLValue; break;
			case EVariantKind::Vec2UL: Vec2ULValue = That.Vec2ULValue; break;
			case EVariantKind::Vec3UL: Vec3ULValue = That.Vec3ULValue; break;
			case EVariantKind::Vec4UL: Vec4ULValue = That.Vec4ULValue; break;

			case EVariantKind::Vec2F: Vec2FValue = That.Vec2FValue; break;
			case EVariantKind::Vec3F: Vec3FValue = That.Vec3FValue; break;
			case EVariantKind::Vec4F: Vec4FValue = That.Vec4FValue; break;
			case EVariantKind::Vec2D: Vec2DValue = That.Vec2DValue; break;
			case EVariantKind::Vec3D: Vec3DValue = That.Vec3DValue; break;
			case EVariantKind::Vec4D: Vec4DValue = That.Vec4DValue; break;

			case EVariantKind::Vector2: Vector2Value = That.Vector2Value; break;
			case EVariantKind::Vector3: Vector3Value = That.Vector3Value; break;
			case EVariantKind::Vector4: Vector4Value = That.Vector4Value; break;

			case EVariantKind::Name: NameValue = That.NameValue; break;
			case EVariantKind::Text: TextValue = That.TextValue; break;

			case EVariantKind::String: StringValue = That.StringValue; break;

			case EVariantKind::ReferPointer:
				ReferPointerValue = That.ReferPointerValue;
				if (ReferPointerValue)
					ReferPointerValue->AddRef();
				break;
			case EVariantKind::ObjectPointer:
				ObjectEntryValue = That.ObjectEntryValue;
				EntryAddRef(ObjectEntryValue);
				break;

			case EVariantKind::Adapter:
				if (That.AdapterValue)
					AdapterValue = That.AdapterValue->CopyConstruct(InlineValue, That.InlineValue);
				break;

			default:
				break;
			}
			return *this;
		}

		FVariant & operator =(FVariant && That) noexcept
		{
			Reset();
			Kind = That.Kind;
			switch (Kind)
			{
			case EVariantKind::Integer: IntegerValue = That.IntegerValue; break;
			case EVariantKind::Float32: Float32Value = That.Float32Value; break;
			case EVariantKind::Float64: Float64Value = That.Float64Value; break;

			case EVariantKind::Vec2SB: Vec2SBValue = That.Vec2SBValue; break;
			case EVariantKind::Vec3SB: Vec3SBValue = That.Vec3SBValue; break;
			case EVariantKind::Vec4SB: Vec4SBValue = That.Vec4SBValue; break;
			case EVariantKind::Vec2UB: Vec2UBValue = That.Vec2UBValue; break;
			case EVariantKind::Vec3UB: Vec3UBValue = That.Vec3UBValue; break;
			case EVariantKind::Vec4UB: Vec4UBValue = That.Vec4UBValue; break;

			case EVariantKind::Vec2SS: Vec2SSValue = That.Vec2SSValue; break;
			case EVariantKind::Vec3SS: Vec3SSValue = That.Vec3SSValue; break;
			case EVariantKind::Vec4SS: Vec4SSValue = That.Vec4SSValue; break;
			case EVariantKind::Vec2US: Vec2USValue = That.Vec2USValue; break;
			case EVariantKind::Vec3US: Vec3USValue = That.Vec3USValue; break;
			case EVariantKind::Vec4US: Vec4USValue = That.Vec4USValue; break;

			case EVariantKind::Vec2I: Vec2IValue = That.Vec2IValue; break;
			case EVariantKind::Vec3I: Vec3IValue = That.Vec3IValue; break;
			case EVariantKind::Vec4I: Vec4IValue = That.Vec4IValue; break;
			case EVariantKind::Vec2U: Vec2UValue = That.Vec2UValue; break;
			case EVariantKind::Vec3U: Vec3UValue = That.Vec3UValue; break;
			case EVariantKind::Vec4U: Vec4UValue = That.Vec4UValue; break;

			case EVariantKind::Vec2F: Vec2FValue = That.Vec2FValue; break;
			case EVariantKind::Vec3F: Vec3FValue = That.Vec3FValue; break;
			case EVariantKind::Vec4F: Vec4FValue = That.Vec4FValue; break;
			case EVariantKind::Vec2D: Vec2DValue = That.Vec2DValue; break;
			case EVariantKind::Vec3D: Vec3DValue = That.Vec3DValue; break;
			case EVariantKind::Vec4D: Vec4DValue = That.Vec4DValue; break;

			case EVariantKind::Vector2: Vector2Value = That.Vector2Value; break;
			case EVariantKind::Vector3: Vector3Value = That.Vector3Value; break;
			case EVariantKind::Vector4: Vector4Value = That.Vector4Value; break;

			case EVariantKind::Name: NameValue = Move(That.NameValue); break;
			case EVariantKind::Text: TextValue = Move(That.TextValue); break;
			case EVariantKind::String: StringValue = Move(That.StringValue); break;

			case EVariantKind::ReferPointer:
				ReferPointerValue = That.ReferPointerValue;
				That.ReferPointerValue = nullptr;
				break;
			case EVariantKind::ObjectPointer:
				ObjectEntryValue = That.ObjectEntryValue;
				That.ObjectEntryValue = nullptr;
				break;

			case EVariantKind::Adapter:
				if (That.AdapterValue)
					AdapterValue = That.AdapterValue->MoveConstruct(InlineValue, That.InlineValue);
				break;

			default:
				Kind = EVariantKind::None;
				break;
			}
			That.Reset();
			return *this;
		}

		template<FundamentalVariantC T>
		FVariant & Set(const T & Value)
		{
			Reset();

			if constexpr (IsSameV<T, VoidT<T>>) { Kind = EVariantKind::None; }
			if constexpr (IsSameV<T, NovalT>) { Kind = EVariantKind::None; }
			if constexpr (IsSameV<T, NullptrT>) { Kind = EVariantKind::None; }

			else if constexpr (IsSameV<T, bool>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }

			else if constexpr (IsSameV<T, chara>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, charw>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, char8>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, char16>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, char32>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }

			else if constexpr (IsSameV<T, int8>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, uint8>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, int16>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, uint16>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, int32>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, uint32>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, int64>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }
			else if constexpr (IsSameV<T, uint64>) { Kind = EVariantKind::Integer; IntegerValue = int64(Value); }

			else if constexpr (IsSameV<T, float32>) { Kind = EVariantKind::Float32; Float32Value = Value; }
			else if constexpr (IsSameV<T, float64>) { Kind = EVariantKind::Float64; Float64Value = Value; }

			else if constexpr (IsSameV<T, Vec2SB>) { Kind = EVariantKind::Vec2SB; Vec2SBValue = Value; }
			else if constexpr (IsSameV<T, Vec3SB>) { Kind = EVariantKind::Vec3SB; Vec3SBValue = Value; }
			else if constexpr (IsSameV<T, Vec4SB>) { Kind = EVariantKind::Vec4SB; Vec4SBValue = Value; }
			else if constexpr (IsSameV<T, Vec2UB>) { Kind = EVariantKind::Vec2UB; Vec2UBValue = Value; }
			else if constexpr (IsSameV<T, Vec3UB>) { Kind = EVariantKind::Vec3UB; Vec3UBValue = Value; }
			else if constexpr (IsSameV<T, Vec4UB>) { Kind = EVariantKind::Vec4UB; Vec4UBValue = Value; }

			else if constexpr (IsSameV<T, Vec2SS>) { Kind = EVariantKind::Vec2SS; Vec2SSValue = Value; }
			else if constexpr (IsSameV<T, Vec3SS>) { Kind = EVariantKind::Vec3SS; Vec3SSValue = Value; }
			else if constexpr (IsSameV<T, Vec4SS>) { Kind = EVariantKind::Vec4SS; Vec4SSValue = Value; }
			else if constexpr (IsSameV<T, Vec2US>) { Kind = EVariantKind::Vec2US; Vec2USValue = Value; }
			else if constexpr (IsSameV<T, Vec3US>) { Kind = EVariantKind::Vec3US; Vec3USValue = Value; }
			else if constexpr (IsSameV<T, Vec4US>) { Kind = EVariantKind::Vec4US; Vec4USValue = Value; }

			else if constexpr (IsSameV<T, Vec2I>) { Kind = EVariantKind::Vec2I; Vec2IValue = Value; }
			else if constexpr (IsSameV<T, Vec3I>) { Kind = EVariantKind::Vec3I; Vec3IValue = Value; }
			else if constexpr (IsSameV<T, Vec4I>) { Kind = EVariantKind::Vec4I; Vec4IValue = Value; }
			else if constexpr (IsSameV<T, Vec2U>) { Kind = EVariantKind::Vec2U; Vec2UValue = Value; }
			else if constexpr (IsSameV<T, Vec3U>) { Kind = EVariantKind::Vec3U; Vec3UValue = Value; }
			else if constexpr (IsSameV<T, Vec4U>) { Kind = EVariantKind::Vec4U; Vec4UValue = Value; }

			else if constexpr (IsSameV<T, Vec2SL>) { Kind = EVariantKind::Vec2SL; Vec2SLValue = Value; }
			else if constexpr (IsSameV<T, Vec3SL>) { Kind = EVariantKind::Vec3SL; Vec3SLValue = Value; }
			else if constexpr (IsSameV<T, Vec4SL>) { Kind = EVariantKind::Vec4SL; Vec4SLValue = Value; }
			else if constexpr (IsSameV<T, Vec2UL>) { Kind = EVariantKind::Vec2UL; Vec2ULValue = Value; }
			else if constexpr (IsSameV<T, Vec3UL>) { Kind = EVariantKind::Vec3UL; Vec3ULValue = Value; }
			else if constexpr (IsSameV<T, Vec4UL>) { Kind = EVariantKind::Vec4UL; Vec4ULValue = Value; }

			else if constexpr (IsSameV<T, Vec2F>) { Kind = EVariantKind::Vec2F; Vec2FValue = Value; }
			else if constexpr (IsSameV<T, Vec3F>) { Kind = EVariantKind::Vec3F; Vec3FValue = Value; }
			else if constexpr (IsSameV<T, Vec4F>) { Kind = EVariantKind::Vec4F; Vec4FValue = Value; }
			else if constexpr (IsSameV<T, Vec2D>) { Kind = EVariantKind::Vec2D; Vec2DValue = Value; }
			else if constexpr (IsSameV<T, Vec3D>) { Kind = EVariantKind::Vec3D; Vec3DValue = Value; }
			else if constexpr (IsSameV<T, Vec4D>) { Kind = EVariantKind::Vec4D; Vec4DValue = Value; }

			else if constexpr (IsSameV<T, FVector2>) { Kind = EVariantKind::Vector2; Vector2Value = Value; }
			else if constexpr (IsSameV<T, FVector3>) { Kind = EVariantKind::Vector3; Vector3Value = Value; }
			else if constexpr (IsSameV<T, FVector4>) { Kind = EVariantKind::Vector4; Vector4Value = Value; }

			else if constexpr (IsSameV<T, FName>) { Kind = EVariantKind::Name; NameValue = Value; }
			else if constexpr (IsSameV<T, FText>) { Kind = EVariantKind::Text; TextValue = Value; }
			else if constexpr (IsSameV<T, FString>) { Kind = EVariantKind::String; StringValue = Value; }
			else if constexpr (IsSameV<T, FStringV>) { Kind = EVariantKind::String; StringValue = Value; }

			else if constexpr (IsReferPointerV<T>)
			{
				Kind = EVariantKind::ReferPointer;
				ReferPointerValue = ReferAddRef(Value.Pointer);
			}
			else if constexpr (IsEntryPointerV<T>)
			{
				Kind = EVariantKind::ObjectPointer;
				ObjectEntryValue = EntryAddRef(Value.Entry);
			}
			else
			{
				static_assert(DependentFalse<T>, "Invalid T type.");
			}
			return *this;
		}

		template<typename T> requires (!FundamentalVariantC<T>)
		FVariant & Set(const T & Value)
		{
			Reset();
			if constexpr (IsPointerV<T>)
			{
				if (!Value)
					return *this;

				using PointeeT = RemoveCVRefT<RemovePointerT<T>>;
				PointeeT * Pointee = const_cast<PointeeT *>(Value);
				if constexpr (IsDerivedOfV<PointeeT, IRefer>)
				{
					Kind = EVariantKind::ReferPointer;
					ReferPointerValue = ReferAddRef(Pointee);
				}
				else if constexpr (IsDerivedOfV<PointeeT, IObject>)
				{
					Kind = EVariantKind::ObjectPointer;
					ObjectEntryValue = EntryAddRef(Pointee->ObjectEntry);
				}
				else if constexpr (IsSameV<PointeeT, FObjectEntry>)
				{
					Kind = EVariantKind::ObjectPointer;
					ObjectEntryValue = Value;
				}
				else
				{
					//Kind = EVariantKind::Adapter;
					AdapterValue = TVariantAdapter<PointeeT *>::Create(InlineValue, Pointee);
				}
			}
			else if constexpr (IsDerivedOfV<T, IRefer>)
			{
				Kind = EVariantKind::ReferPointer;
				ReferPointerValue = ReferAddRef(const_cast<T *>(&Value));
			}
			else if constexpr (IsDerivedOfV<T, IObject>)
			{
				Kind = EVariantKind::ObjectPointer;
				ObjectEntryValue = EntryAddRef(Value.ObjectEntry);
			}
			else if constexpr (IsReferPointerV<T>)
			{
				Set(Value.Pointer);
			}
			else if constexpr (IsEntryPointerV<T>)
			{
				Set(Value.Entry);
			}
			else if constexpr (IsOperatorCastableToV<T, FVariant>)
			{
				operator =(Value.operator FVariant());
			}
			else
			{
				//Kind = EVariantKind::Adapter;
				if constexpr (IsCopyConstructibleV<T>)
					AdapterValue = TVariantAdapter<T>::Create(InlineValue, Value);
				else
					AdapterValue = TVariantAdapter<T *>::Create(InlineValue, ConstCast(&Value));
			}
			return *this;
		}

		template<FundamentalVariantC T>
		T Get() const
		{
			if constexpr (IsSameV<T, NullptrT>)
				return nullptr;
			else if constexpr (IsSameV<T, bool>)
			{
				if (Kind == EVariantKind::Integer)
					return !!IntegerValue;
				else
					return false;
			}
			else if constexpr (IsIntegerV<T>)
			{
				if (Kind == EVariantKind::Integer)
					return static_cast<T>(IntegerValue);
				else
					return 0;
			}
			else if constexpr (IsSameV<T, float32>)
			{
				switch(Kind)
				{
				case EVariantKind::Integer: return (float32)IntegerValue;
				case EVariantKind::Float32: return Float32Value;
				case EVariantKind::Float64: return (float32)Float64Value;
				default: return 0;
				}
			}
			else if constexpr (IsSameV<T, float64>)
			{
				switch (Kind)
				{
				case EVariantKind::Integer: return (float64)IntegerValue;
				case EVariantKind::Float32: return (float64)Float32Value;
				case EVariantKind::Float64: return Float64Value;
				default: return 0;
				}
			}
			else if constexpr (IsSameV<T, Vec2SB>) { return Kind == EVariantKind::Vec2SB ? Vec2SBValue : Vec2SB { }; }
			else if constexpr (IsSameV<T, Vec3SB>) { return Kind == EVariantKind::Vec3SB ? Vec3SBValue : Vec3SB { }; }
			else if constexpr (IsSameV<T, Vec4SB>) { return Kind == EVariantKind::Vec4SB ? Vec4SBValue : Vec4SB { }; }
			else if constexpr (IsSameV<T, Vec2UB>) { return Kind == EVariantKind::Vec2UB ? Vec2UBValue : Vec2UB { }; }
			else if constexpr (IsSameV<T, Vec3UB>) { return Kind == EVariantKind::Vec3UB ? Vec3UBValue : Vec3UB { }; }
			else if constexpr (IsSameV<T, Vec4UB>) { return Kind == EVariantKind::Vec4UB ? Vec4UBValue : Vec4UB { }; }

			else if constexpr (IsSameV<T, Vec2SS>) { return Kind == EVariantKind::Vec2SS ? Vec2SSValue : Vec2SS { }; }
			else if constexpr (IsSameV<T, Vec3SS>) { return Kind == EVariantKind::Vec3SS ? Vec3SSValue : Vec3SS { }; }
			else if constexpr (IsSameV<T, Vec4SS>) { return Kind == EVariantKind::Vec4SS ? Vec4SSValue : Vec4SS { }; }
			else if constexpr (IsSameV<T, Vec2US>) { return Kind == EVariantKind::Vec2US ? Vec2USValue : Vec2US { }; }
			else if constexpr (IsSameV<T, Vec3US>) { return Kind == EVariantKind::Vec3US ? Vec3USValue : Vec3US { }; }
			else if constexpr (IsSameV<T, Vec4US>) { return Kind == EVariantKind::Vec4US ? Vec4USValue : Vec4US { }; }

			else if constexpr (IsSameV<T, Vec2I>) { return Kind == EVariantKind::Vec2I ? Vec2IValue : Vec2I { }; }
			else if constexpr (IsSameV<T, Vec3I>) { return Kind == EVariantKind::Vec3I ? Vec3IValue : Vec3I { }; }
			else if constexpr (IsSameV<T, Vec4I>) { return Kind == EVariantKind::Vec4I ? Vec4IValue : Vec4I { }; }

			else if constexpr (IsSameV<T, Vec2U>) { return Kind == EVariantKind::Vec2U ? Vec2UValue : Vec2U { }; }
			else if constexpr (IsSameV<T, Vec3U>) { return Kind == EVariantKind::Vec3U ? Vec3UValue : Vec3U { }; }
			else if constexpr (IsSameV<T, Vec4U>) { return Kind == EVariantKind::Vec4U ? Vec4UValue : Vec4U { }; }

			else if constexpr (IsSameV<T, Vec2SL>) { return Kind == EVariantKind::Vec2SL ? Vec2SLValue : Vec2SL { }; }
			else if constexpr (IsSameV<T, Vec3SL>) { return Kind == EVariantKind::Vec3SL ? Vec3SLValue : Vec3SL { }; }
			else if constexpr (IsSameV<T, Vec4SL>) { return Kind == EVariantKind::Vec4SL ? Vec4SLValue : Vec4SL { }; }
			else if constexpr (IsSameV<T, Vec2UL>) { return Kind == EVariantKind::Vec2UL ? Vec2ULValue : Vec2UL { }; }
			else if constexpr (IsSameV<T, Vec3UL>) { return Kind == EVariantKind::Vec3UL ? Vec3ULValue : Vec3UL { }; }
			else if constexpr (IsSameV<T, Vec4UL>) { return Kind == EVariantKind::Vec4UL ? Vec4ULValue : Vec4UL { }; }

			else if constexpr (IsSameV<T, Vec2F>) { return Kind == EVariantKind::Vec2F ? Vec2FValue : Vec2F { }; }
			else if constexpr (IsSameV<T, Vec3F>) { return Kind == EVariantKind::Vec3F ? Vec3FValue : Vec3F { }; }
			else if constexpr (IsSameV<T, Vec4F>) { return Kind == EVariantKind::Vec4F ? Vec4FValue : Vec4F { }; }
			else if constexpr (IsSameV<T, Vec2D>) { return Kind == EVariantKind::Vec2D ? Vec2DValue : Vec2D { }; }
			else if constexpr (IsSameV<T, Vec3D>) { return Kind == EVariantKind::Vec3D ? Vec3DValue : Vec3D { }; }
			else if constexpr (IsSameV<T, Vec4D>) { return Kind == EVariantKind::Vec4D ? Vec4DValue : Vec4D { }; }

			else if constexpr (IsSameV<T, FVector2>) { return Kind == EVariantKind::Vector2 ? Vector2Value : FVector2 { }; }
			else if constexpr (IsSameV<T, FVector3>) { return Kind == EVariantKind::Vector3 ? Vector3Value : FVector3 { }; }
			else if constexpr (IsSameV<T, FVector4>) { return Kind == EVariantKind::Vector4 ? Vector4Value : FVector4 { }; }

			else if constexpr (IsSameV<T, FName>) { return Kind == EVariantKind::Name ? NameValue : FName { }; }
			else if constexpr (IsSameV<T, FText>) { return Kind == EVariantKind::Text ? TextValue : FText { }; }
			else if constexpr (IsSameV<T, FString>) { return Kind == EVariantKind::String ? StringValue : FString { }; }
			else if constexpr (IsSameV<T, FStringV>) { return Kind == EVariantKind::String ? FStringV(StringValue) : FStringV { }; }

			else
			{
				static_assert(DependentFalse<T>, "Invalid T type.");
				throw EError::NotSupported;
			}
		}

		template<typename T> requires (!FundamentalVariantC<T> && !ReferenceC<T> && !PointerC<T>)
		T Get() const
		{
			if constexpr (SameC<T, FVariant>)
				return *this;
			else if constexpr (ConstructibleC<T, FVariant>)
				return T(*this);
			else if constexpr (IsReferPointerV<T>)
			{
				using PointeeT = ReferPointeeT<T>;
				if constexpr (IsDerivedOfV<PointeeT, IRefer>)
				{
					if (Kind == EVariantKind::ReferPointer)
						return TReferPtr<PointeeT> { static_cast<PointeeT *>(ReferPointerValue) };
					else if (Kind == EVariantKind::ObjectPointer)
					{
						if constexpr (IsDerivedOfV<PointeeT, IObject>)
						{
							if (ObjectEntryValue)
							{
								if (IObject * Object = ObjectEntryValue->Lock())
								{
									TReferPtr<PointeeT> Result { static_cast<PointeeT *>(Object) };
									ReferRelease(Object);
									return Result;
								}
							}
						}
					}
					else { }
				}
				else
				{
					if (Kind == EVariantKind::Adapter)
					{
						AssertExpr(AdapterValue != nullptr);
						//if (AdapterValue->Type() == Typeof<T>())
						{
							TVariantAdapter<T> & Adapter = StaticCastRef<TVariantAdapter<T>>(AdapterValue);
							return Adapter.Resolve(InlineValue);
						}
					}
				}
				return nullptr;
			}
			else if constexpr (IsEntryPointerV<T>)
			{
				if (Kind == EVariantKind::ReferPointer)
					return T((EntryPointeeT<T> *)ReferPointerValue);
				else if (Kind == EVariantKind::ObjectPointer)
				{
					if (ObjectEntryValue)
					{
						if (IObject * Object = ObjectEntryValue->Lock())
						{
							T Result { static_cast<EntryPointeeT<T> *>(Object) };
							ReferRelease(Object);
							return Result;
						}
					}
					return nullptr;
				}
				else
					return nullptr;
			}
			else
			{
				if constexpr (IsCopyConstructibleV<T>)
				{
					// https://gist.github.com/JoelFilho/f86eb49569e0a961c26a5d3b7c5e4f61
					// We need explicit parameter to prevent this !!!:
					// TList<int> List (12.0f);
//#define ITERATE_VARIANT_KIND_LOOP_BODY(Type, VariantKind) \
//					if constexpr (IsExplicitlyConvertibleFromV<T, Type>) { \
//						if (Kind == EVariantKind::VariantKind) { \
//							return static_cast<T>(VariantKind ## Value); \
//						} \
//					};
//					ITERATE_VARIANT_KIND
//#undef ITERATE_VARIANT_KIND_LOOP_BODY

					if (Kind == EVariantKind::Adapter)
					{
						AssertExpr(AdapterValue != nullptr);
						//if (AdapterValue->Type() == Typeof<T>())
						{
							TVariantAdapter<T> & Adapter = StaticCastRef<TVariantAdapter<T>>(AdapterValue);
							return Adapter.Resolve(InlineValue);
						}
					}

					if constexpr (IsDefaultConstructibleV<T>)
						return T { };
					else
						throw EError::NotSupported;
				}
				else
					throw EError::NotSupported;
			}
		}
		
		template<LValueReferenceC T>
		T Get() const
		{
			using UnderlyingT = RemoveCVRefT<T>;

			FVariant & This = ConstCast(*this);

			if constexpr (IsSameV<UnderlyingT, FVariant>) { return This; }

			else if constexpr (IsAnyOfV<UnderlyingT,
				bool,
				chara, charw, char8, char16, char32,
				int8, uint8, int16, uint16, int32, uint32, int64, uint64,
				float32, float64>)
			{
				if (Kind == EVariantKind::Integer)
					return *reinterpret_cast<UnderlyingT *>(&This.IntegerValue);
				else if (Kind == EVariantKind::Float32)
					return *reinterpret_cast<UnderlyingT *>(&This.Float32Value);
				else if (Kind == EVariantKind::Float64)
					return *reinterpret_cast<UnderlyingT *>(&This.Float64Value);
				else
					return *(UnderlyingT *)nullptr;
			}

			else if constexpr (IsSameV<UnderlyingT, Vec2SB>) { if (Kind == EVariantKind::Vec2SB) return This.Vec2SBValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3SB>) { if (Kind == EVariantKind::Vec3SB) return This.Vec3SBValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4SB>) { if (Kind == EVariantKind::Vec4SB) return This.Vec4SBValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec2UB>) { if (Kind == EVariantKind::Vec2UB) return This.Vec2UBValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3UB>) { if (Kind == EVariantKind::Vec3UB) return This.Vec3UBValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4UB>) { if (Kind == EVariantKind::Vec4UB) return This.Vec4UBValue; }

			else if constexpr (IsSameV<UnderlyingT, Vec2SS>) { if (Kind == EVariantKind::Vec2SS) return This.Vec2SSValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3SS>) { if (Kind == EVariantKind::Vec3SS) return This.Vec3SSValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4SS>) { if (Kind == EVariantKind::Vec4SS) return This.Vec4SSValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec2US>) { if (Kind == EVariantKind::Vec2US) return This.Vec2USValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3US>) { if (Kind == EVariantKind::Vec3US) return This.Vec3USValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4US>) { if (Kind == EVariantKind::Vec4US) return This.Vec4USValue; }

			else if constexpr (IsSameV<UnderlyingT, Vec2I>) { if (Kind == EVariantKind::Vec2I) return This.Vec2IValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3I>) { if (Kind == EVariantKind::Vec3I) return This.Vec3IValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4I>) { if (Kind == EVariantKind::Vec4I) return This.Vec4IValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec2U>) { if (Kind == EVariantKind::Vec2U) return This.Vec2UValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3U>) { if (Kind == EVariantKind::Vec3U) return This.Vec3UValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4U>) { if (Kind == EVariantKind::Vec4U) return This.Vec4UValue; }

			else if constexpr (IsSameV<UnderlyingT, Vec2SL>) { if (Kind == EVariantKind::Vec2SL) return This.Vec2SLValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3SL>) { if (Kind == EVariantKind::Vec3SL) return This.Vec3SLValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4SL>) { if (Kind == EVariantKind::Vec4SL) return This.Vec4SLValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec2UL>) { if (Kind == EVariantKind::Vec2UL) return This.Vec2ULValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3UL>) { if (Kind == EVariantKind::Vec3UL) return This.Vec3ULValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4UL>) { if (Kind == EVariantKind::Vec4UL) return This.Vec4ULValue; }

			else if constexpr (IsSameV<UnderlyingT, Vec2F>) { if (Kind == EVariantKind::Vec2F) return This.Vec2FValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3F>) { if (Kind == EVariantKind::Vec3F) return This.Vec3FValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4F>) { if (Kind == EVariantKind::Vec4F) return This.Vec4FValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec2D>) { if (Kind == EVariantKind::Vec2D) return This.Vec2DValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec3D>) { if (Kind == EVariantKind::Vec3D) return This.Vec3DValue; }
			else if constexpr (IsSameV<UnderlyingT, Vec4D>) { if (Kind == EVariantKind::Vec4D) return This.Vec4DValue; }

			else if constexpr (IsSameV<UnderlyingT, FVector2>) { if (Kind == EVariantKind::Vector2) return This.Vector2Value; }
			else if constexpr (IsSameV<UnderlyingT, FVector3>) { if (Kind == EVariantKind::Vector3) return This.Vector3Value; }
			else if constexpr (IsSameV<UnderlyingT, FVector4>) { if (Kind == EVariantKind::Vector4) return This.Vector4Value; }

			else if constexpr (IsSameV<UnderlyingT, FName>) { if (Kind == EVariantKind::Name) return This.NameValue; }
			else if constexpr (IsSameV<UnderlyingT, FText>) { if (Kind == EVariantKind::Text) return This.TextValue; }

			else if constexpr (IsSameV<UnderlyingT, FString>)
			{
				if (Kind == EVariantKind::String)
					return This.StringValue;
			}

			else if constexpr (IsDerivedOfV<UnderlyingT, IRefer>)
			{
				if (Kind == EVariantKind::ReferPointer)
					return *(UnderlyingT *)This.ReferPointerValue;
				else
					return *(UnderlyingT *)nullptr;
			}
			else if constexpr (IsSameV<UnderlyingT, FObjectEntry>)
			{
				if (Kind == EVariantKind::ObjectPointer)
					return This.ObjectEntryValue;
				else
					return nullptr;
			}
			else if constexpr (FundamentalVariantC<UnderlyingT>)
			{
				This.Reset();
				This = UnderlyingT { };
				return Get<UnderlyingT &>();
			}
			else
			{
				if (Kind == EVariantKind::Adapter)
				{
					AssertExpr(AdapterValue != nullptr);
					if constexpr (IsCopyConstructibleV<UnderlyingT>)
					{
						if (AdapterValue->Type() == Typeof<UnderlyingT>())
						{
							TVariantAdapter<UnderlyingT> & Adapter = StaticCastRef<TVariantAdapter<UnderlyingT>>(This.AdapterValue);
							return Adapter.Resolve(InlineValue);
						}
					}
					else
					{
						//const FType & Type = AdapterValue->Type();
						//if (Type.GetPointeeType() == Typeof<UnderlyingT>())
						{
							TVariantAdapter<UnderlyingT *> & Adapter = StaticCastRef<TVariantAdapter<UnderlyingT *>>(This.AdapterValue);
							return *Adapter.Resolve(InlineValue);
						}
					}
				}
				else { }
			}

			return *(UnderlyingT *)nullptr;
		}

		template<RValueReferenceC T>
		T Get() const
		{
			using UnderlyingT = RemoveCVRefT<T>;
			return Move(Get<UnderlyingT &>());
		}

		template<PointerC T>
		T Get() const
		{
			using PointeeT = RemoveCVRefT<RemovePointerT<T>>;
			if constexpr (IsDerivedOfV<PointeeT, IRefer>)
			{
				//if (AdapterValue->Type() == Typeof<T>())
				if (ReferPointerValue && ReferPointerValue->IsA<PointeeT>())
					return static_cast<T>(ReferPointerValue);
				else
					return nullptr;
			}
			else if constexpr (IsSameV<PointeeT, FObjectEntry>)
			{
				if (Kind == EVariantKind::ObjectPointer)
					return ObjectEntryValue;
				else
					return nullptr;
			}
			else
			{
				if (Kind == EVariantKind::Adapter)
				{
					AssertExpr(AdapterValue != nullptr);
					if (AdapterValue->Type() == Typeof<T>())
					{
						TVariantAdapter<PointeeT *> & Adapter = StaticCastRef<TVariantAdapter<PointeeT *>>(AdapterValue);
						return Adapter.Resolve(InlineValue);
					}
				}
				return nullptr;
			}
		}

	public:
		// We must explicit operator for ConvertibleToC<FVariant, T> to work fine.
		//template<FundamentalVariantC T>
		//explicit operator T() const { return Get<T>(); }

		template<typename T>
		FVariant & operator =(const T & Value)
		{
			return Set(Value);
		}

		bool Empty() const { return Kind == EVariantKind::None; }
		bool Valid() const { return Kind != EVariantKind::None; }
		explicit operator bool() const { return Kind != EVariantKind::None; }

		void Abandon()
		{
			Kind = EVariantKind::None;
		}

		const FType & Type() const;

		bool IsA(const FType & Type_) const;

		template<typename T>
		bool IsA() const { return IsA(Typeof<T>()); }

		FStringV Format(FStringV Formal = u8"{}"V) const;

	public:
		union
		{
			int64 IntegerValue;

			float32 Float32Value;
			float64 Float64Value;

			Vec2SB Vec2SBValue;
			Vec3SB Vec3SBValue;
			Vec4SB Vec4SBValue;

			Vec2UB Vec2UBValue;
			Vec3UB Vec3UBValue;
			Vec4UB Vec4UBValue;

			Vec2SS Vec2SSValue;
			Vec3SS Vec3SSValue;
			Vec4SS Vec4SSValue;

			Vec2US Vec2USValue;
			Vec3US Vec3USValue;
			Vec4US Vec4USValue;

			Vec2I Vec2IValue;
			Vec3I Vec3IValue;
			Vec4I Vec4IValue;

			Vec2U Vec2UValue;
			Vec3U Vec3UValue;
			Vec4U Vec4UValue;

			Vec2SL Vec2SLValue;
			Vec3SL Vec3SLValue;
			Vec4SL Vec4SLValue;

			Vec2UL Vec2ULValue;
			Vec3UL Vec3ULValue;
			Vec4UL Vec4ULValue;

			Vec2F Vec2FValue;
			Vec3F Vec3FValue;
			Vec4F Vec4FValue;

			Vec2D Vec2DValue;
			Vec3D Vec3DValue;
			Vec4D Vec4DValue;

			FVector2 Vector2Value;
			FVector3 Vector3Value;
			FVector4 Vector4Value;

			FName NameValue;
			FText TextValue;
			FString StringValue;

			IRefer * ReferPointerValue;
			FObjectEntry * ObjectEntryValue;

			TInlineStorage<VariantAdapterInlinePointers> InlineValue = {};
		};

	public:
		FVariantKind Kind { };

	public:
		EVariantKind GetVariantKind() const { return Kind.GetVariantKind(); }
		void SetVariantKind(EVariantKind VariantKind) { Kind.SetVariantKind(VariantKind); }

		IVariantAdapter * GetAdapterValue() const { return Kind.GetAdapterValue(); }
		void SetAdapterValue(IVariantAdapter * VariantAdapter) { Kind.SetAdapterValue(VariantAdapter); }

	public:
		PropertyRW(EVariantKind, GetVariantKind, SetVariantKind) VariantKind;
		PropertyRW(IVariantAdapter *, GetAdapterValue, SetAdapterValue) AdapterValue;

	public:
		static FVariant None;
	};

	static_assert(sizeof(FVariant) == 64);

	template<>
	struct BASE_API TFormatter<FVariant>;

	template<typename T>
	class TOptional
	{
	public:
		using ElementT = ConditionalT<IsLValueReferenceV<T>, TReferenceWrapper<RemoveRefT<T>>, T>;
		static_assert(IsDestructibleV<ElementT>);
		using ValueType = ElementT;

	public:
		constexpr TOptional() : Dummy() {}
		constexpr TOptional(NovalT) : Dummy() {}

		constexpr TOptional(const TOptional & That)
		{
			if (That.HasValue)
			{
				new(&Value) T(That.Value);
				HasValue = true;
			}
		}

		explicit constexpr TOptional(TOptional && That) noexcept
		{
			if (That.HasValue)
			{
				new(&Value) T(Move(That.Value));
				HasValue = true;
			}
		}

		template<typename = void> requires (!SameC<ElementT, FVariant>)
		constexpr TOptional(const ElementT & That) : Value(That), HasValue(true) { }

		template<typename = void> requires (!SameC<ElementT, FVariant>)
		explicit constexpr TOptional(ElementT && That) : Value(That), HasValue(true) {}

		constexpr TOptional(const FVariant & Variant)
		{
			if (Variant.Valid())
			{
				new(&Value) T(Variant.Get<T>());
				HasValue = true;
			}
		}

		explicit constexpr TOptional(FVariant && Variant)
		{
			if (Variant.Valid())
			{
				new(&Value) T(Move(Variant.Get<ElementT>()));
				HasValue = true;
			}
		}

		TOptional(const FNullIndex &) : Value(T(NullIndex)), HasValue(true) {}

		template<typename Ty>
			requires (!AnyC<RemoveCVRefT<Ty>, ElementT, FVariant, TOptional> && ConstructibleC<ElementT, Ty>)
			explicit(!ConvertibleToC<Ty, ElementT>)
		constexpr TOptional(Ty && Value) : Value(Forward<Ty>(Value)), HasValue(true) {}

		template<typename... ParameterTs>
			requires ((!AnyC<RemoveCVRefT<PackIndexingT<0, ParameterTs...>>, T, FVariant, TOptional> || sizeof...(ParameterTs) > 1) && ConstructibleC<T, ParameterTs...>)
			explicit(sizeof...(ParameterTs) == 1 && !ConvertibleToC<RemoveCVRefT<PackIndexingT<0, ParameterTs...>>, ElementT>)
		constexpr TOptional(ParameterTs &&... Arguments) : Value(Forward<ParameterTs>(Arguments)...), HasValue(true) {}

		template<typename Ty>
			requires (!AnyC<RemoveCVRefT<Ty>, ElementT, TOptional> && ConstructibleC<ElementT, const Ty&>)
			explicit(!ConvertibleToC<Ty, ElementT>)
		constexpr TOptional(const TOptional<Ty> & That)
		{
			if (That.HasValue)
			{
				new(&Value) T(That.Value);
				HasValue = true;
			}
		}

		template<typename Ty>
			requires (!AnyC<RemoveCVRefT<Ty>, ElementT, TOptional> && ConstructibleC<ElementT, Ty&&>)
			explicit(!ConvertibleToC<Ty, ElementT>)
		constexpr TOptional(TOptional<Ty> && That)
		{
			if (That.HasValue)
			{
				new(&Value) T(Move(That.Value));
				HasValue = true;
			}
		}

		constexpr ~TOptional()
		{
			Reset();
		}

		constexpr bool operator ==(const T & Other) const noexcept
		{
			if (!HasValue)
				return false;
			return Value == Other;
		}

		constexpr bool operator ==(const TOptional & Other) const noexcept
		{
			if (HasValue != Other.HasValue)
				return false;
			return Value == Other.Value;
		}

		constexpr CompareOrderingT operator <=>(const TOptional & Other) const noexcept
		{
			if (HasValue != Other.HasValue)
				return HasValue <=> Other.HasValue;
			if (!HasValue)
				return HasValue <=> Other.HasValue;
			return Value <=> Other.Value;
		}

		constexpr TOptional & operator =(NovalT)
		{
			Reset();
			return *this;
		}

		template<typename = void> requires (!SameC<ElementT, FVariant>)
		constexpr TOptional & operator =(const T & That)
		{
			Reset();
			new(&Value) T(That);
			HasValue = true;
			return *this;
		}

		template<typename = void> requires (!SameC<ElementT, FVariant>)
		constexpr TOptional & operator =(T && That) noexcept
		{
			Reset();
			new(&Value) T(Move(That));
			HasValue = true;
			return *this;
		}

		constexpr TOptional & operator =(const TOptional & That)
		{
			Reset();
			if (That.HasValue)
			{
				new(&Value) T(That.Value);
				HasValue = true;
			}
			return *this;
		}

		constexpr TOptional & operator =(TOptional && That) noexcept
		{
			Reset();
			if (That.HasValue)
			{
				new(&Value) T(Move(That.Value));
				HasValue = true;
			}
			return *this;
		}

		constexpr TOptional & operator =(const FVariant & Variant)
		{
			if (!Variant.Valid())
				Reset();
			else if (HasValue)
				Value = Variant.Get<ElementT>();
			else
			{
				new(&Value) T(Variant.Get<ElementT>());
				HasValue = true;
			}
			return *this;
		}

		constexpr TOptional & operator =(FVariant && Variant)
		{
			if (!Variant.Valid())
				Reset();
			else if (HasValue)
				Value = Move(Variant.Get<ElementT>());
			else
			{
				new(&Value) T(Move(Variant.Get<ElementT>()));
				HasValue = true;
			}
			return *this;
		}

		template<typename Ty>
			requires (!AnyC<RemoveCVRefT<Ty>, ElementT, FVariant> && ConstructibleC<ElementT, const Ty&> && AssignableC<ElementT &, const Ty&>)
		constexpr TOptional & operator =(const Ty & That)
		{
			if (HasValue)
				Value = That;
			else
			{
				new(&Value) T(That);
				HasValue = true;
			}
			return *this;
		}

		template<typename Ty>
			requires (!AnyC<RemoveCVRefT<Ty>, ElementT, FVariant, TOptional> && ConstructibleC<ElementT, Ty> && AssignableC<T&, Ty>)
		constexpr TOptional & operator =(Ty && That)
		{
			if (HasValue)
				Value = Move(That);
			else
			{
				new(&Value) T(Move(That));
				HasValue = true;
			}
			return *this;
		}

		template<typename Ty>
			requires (!AnyC<Ty, ElementT, TOptional> && ConstructibleC<ElementT, const Ty&> && AssignableC<ElementT &, const Ty &>)
		constexpr TOptional & operator =(const TOptional<Ty> & That) noexcept
		{
			if (HasValue)
				Value = That.Value;
			else
			{
				new(&Value) T(That.Value);
				HasValue = true;
			}
			return *this;
		}

		template<typename Ty>
			requires (!AnyC<Ty, ElementT, TOptional> && ConstructibleC<ElementT, Ty&&> && AssignableC<T&, Ty&&>)
		constexpr TOptional & operator =(TOptional<Ty> && That) noexcept
		{
			if (HasValue)
				Value = Move(That.Value);
			else
			{
				new(&Value) T(Move(That.Value));
				HasValue = true;
			}
			return *this;
		}

		template<typename... ParameterTs> requires ConstructibleC<ElementT, ParameterTs...>
		constexpr TOptional & Emplace(ParameterTs &&... Arguments)
		{
			Reset();
			new(&Value) T(Forward<ParameterTs>(Arguments)...);
			return *this;
		}

		constexpr void Reset()
		{
			if (HasValue)
			{
				Value.~ElementT();
				HasValue = false;
			}
		}

		ElementT & Get() const
		{
			if (HasValue)
				return Value;
			throw EError::NotSupported;
		}

		ElementT & Get(ElementT & DefaultValue) noexcept
		{
			if (HasValue)
				return Value;
			return DefaultValue;
		}

		const ElementT & Get(const ElementT & DefaultValue) const noexcept
		{
			if (HasValue)
				return Value;
			return DefaultValue;
		}

		constexpr bool Valid() const noexcept { return HasValue; }
		constexpr explicit operator bool() const noexcept { return HasValue; }

		constexpr operator T &()
		{
			if (HasValue)
				return Value;
			throw EError::NotSupported;
		}

		constexpr operator const T &() const
		{
			if (HasValue)
				return Value;
			throw EError::NotSupported;
		}

		template<typename = void> requires (!IsLValueReferenceV<T>)
		constexpr operator T &&()
		{
			if (HasValue)
				return Value;
			throw EError::NotSupported;
		}

		explicit operator FVariant() const noexcept
		{
			if (!HasValue)
				return FVariant();
			return FVariant(Value);
		}

	public:
		union
		{
			TDummy<ElementT> Dummy;
			ElementT Value;
		};

		bool HasValue = false;

	public:
		static TOptional None;
	};

	template<typename T>
	inline TOptional<T> TOptional<T>::None;
}
