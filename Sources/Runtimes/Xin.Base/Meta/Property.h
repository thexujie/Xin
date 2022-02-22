#pragma once

#include "Meta.h"

#define ProxyPropertyRW4(Type, Getter, Setter, Name) \
	__declspec(property(get = Getter, put = Setter)) Type Name; \
	static const TProperty<Type> & Name##Property(); \
	Type __ ## Name

#define ProxyPropertyRW(Type, Name) \
	__declspec(property(get = __Get ## Name, put = __Set ## Name)) Type Name; \
	static const TProperty<Type> & Name##Property(); \
	VariantValueT<Type> & __Get ## Name() const { return ConstCast(__ ## Name); } \
	void __Set ## Name(const Type & Value) \
	{\
		if (__ ## Name == Value) \
			return; \
		__if_exists(OnPropertyChanging) \
		{ \
			OnPropertyChanging(Name##Property()); \
		} \
		__if_exists(On ## Name ## Changing) \
		{ \
			On ## Name ## Changing(Value); \
		} \
		__if_exists(On ## Name ## Changed) \
		{ \
			Type ValueBefore { __ ## Name }; \
			__ ## Name = Value; \
			On ## Name ## Changed(ValueBefore); \
		} \
		__if_not_exists(On ## Name ## Changed) \
		{ \
			__ ## Name = Value; \
		} \
		__if_exists(OnPropertyChanged) \
		{ \
			OnPropertyChanged(Name##Property()); \
		} \
	} \
	Type __ ## Name


namespace Xin
{
	struct FField;

	enum class EValueLayer
	{
		None = 0,
		Default,
		Template,
		Style,
		Local,
		Expression,
		Animated,
		Current,
	};


	struct FProperty;
	class BASE_API IPropertyValue : public ISharedRefer
	{
	public:
		IPropertyValue() = default;
		IPropertyValue(const IPropertyValue &) = delete;

		explicit operator bool() const { return this != &None; }
		virtual void ResetToLayer(EValueLayer ValueLayer) {}
		//virtual FVariant EffectedValue() const { return noval; }
		//virtual FVariant LocalValue() const { return noval; }
		virtual FVariant GetCurrentValue() const { return noval; }

		virtual void ClearValue(EValueLayer ValueLayer) { }
		virtual void SetValue(EValueLayer ValueLayer, FVariant Value) { }

	public:
		TDelegate<void(const FProperty &)> Changed;

	public:
		static IPropertyValue None;
	};

	using IPropertyValueRef = TReferPtr<IPropertyValue>;

	struct BASE_API FProperty : FMeta
	{
	public:
		FProperty();
		FProperty(const FType & Instance, const FType & Type);
		FProperty(const FType & Instance, const FType & Type, FName Name, const FMetadata & Metadata);

		virtual IPropertyValueRef CreatePropertyValue(FVariant Instance) const { return nullptr; }

		virtual FVariant GetValue(FVariant Instance) const { return nullptr; }
		virtual void SetValue(FVariant Instance, FVariant Value) const { }

		virtual FVariant GetLocalValue(FVariant Instance) const { return nullptr; }
		virtual FVariant GetDefaultValue(FVariant Instance) const { return GetLocalValue(Move(Instance)); }
		virtual FVariant GetCurrentValue(FVariant Instance) const { return GetLocalValue(Move(Instance)); }

		virtual void AfterChagned(FVariant Instance) const {}

		const FField & FindField(FName FieldName) const;
	public:
		const FType & Instance;
		const FType & Type;

	public:
		static FProperty None;
	};

	template<typename ValueT>
	struct TProperty : FProperty
	{
	public:
		TProperty() = default;
		TProperty(const FType & Instance, FName Name, const FMetadata & Metadata) : FProperty(Instance, Typeof<ValueT>(), Name, Metadata) {}

	public:
		static TProperty None;
	};

	template<typename ValueT>
	TProperty<ValueT> TProperty<ValueT>::None;

	template<typename InstanceT, typename ValueT>
	struct TInstanceProperty : TProperty<ValueT>
	{
	public:
		using OnChangedT = void (InstanceT::*)();

		TInstanceProperty() = default;
		TInstanceProperty(FName Name, const FMetadata & Metadata = FMetadata::None, OnChangedT OnChanged= nullptr)
			: TProperty<ValueT>(Typeof<InstanceT>(), Name, Metadata), OnChanged(OnChanged) {}

	public:
		void AfterChagned(FVariant Instance) const final
		{
			if (OnChanged)
			{
				InstanceT & This = Instance.Get<InstanceT &>();
				(This.*OnChanged)();
			}
		}

	public:
		virtual ValueT Get(InstanceT & Instance) const { throw EError::NotImplemented; }
		virtual void Set(InstanceT & Instance, const ValueT & Value) const { throw EError::NotImplemented; }

	public:
		OnChangedT OnChanged = nullptr;

	public:
		static TInstanceProperty None;
	};

	template<typename InstanceT, typename ValueT>
	TInstanceProperty<InstanceT, ValueT> TInstanceProperty<InstanceT, ValueT>::None;


	template<MemberFieldPointerC FieldT,
		typename InstanceT = typename IsMemberFieldPointer<FieldT>::InstanceType,
		typename ValueT = typename IsMemberFieldPointer<FieldT>::ValueType>
	struct TFieldProperty : TInstanceProperty<InstanceT, ValueT>
	{
	public:
		using ParentT = TInstanceProperty<InstanceT, ValueT>;
		using OnChangedT = typename TInstanceProperty<InstanceT, ValueT>::OnChangedT;

	public:
		TFieldProperty() : Field(nullptr) {}
		TFieldProperty(FName Name, FieldT Field, const FMetadata & Metadata = FMetadata::None, OnChangedT OnChanged = nullptr) : ParentT(Name, Metadata, OnChanged), Field(Field) { }

	public:
		FVariant GetValue(FVariant Instance) const override
		{
			InstanceT & This = Instance.Get<InstanceT &>();
			return This.*Field;
		}

		void SetValue(FVariant Instance, FVariant Value) const override
		{
			InstanceT & This = Instance.Get<InstanceT &>();
			This.*Field = Value.Get<ValueT>();
		}

		FVariant GetLocalValue(FVariant Instance) const override
		{
			InstanceT & This = Instance.Get<InstanceT &>();
			return This.*Field;
		}

	public:
		FieldT Field;
	};

	enum class EValueCompoundType
	{
		None = 0,
		XValue,
		LRefValue,
		RRefValue,
		ConstLRefValue,
		ConstRRefValue,
	};

	template<typename T, EValueCompoundType ValueCompoundType>
	using CompoundTypeT = T;

	inline namespace Private
	{
		template<typename GetterT, typename>
		struct TypeMemberTraits
		{
			using InstanceType = typename IsMemberFunctionPointer<GetterT>::InstanceType;
			using ValueType = RemoveCVRefT<typename IsMemberFunctionPointer<GetterT>::ValueType>;
		};

		template<typename GetterT>
		struct TypeMemberTraits<GetterT, NullptrT>
		{
			using InstanceType = typename IsMemberFunctionPointer<GetterT>::InstanceType;
			using ValueType = typename IsMemberFunctionPointer<GetterT>::ValueType;
		};

		template<typename SetterT>
		struct TypeMemberTraits<NullptrT, SetterT>
		{
			using InstanceType = typename IsMemberFunctionPointer<SetterT>::InstanceType;
			using ValueType = RemoveCVRefT<typename IsMemberFunctionPointer<SetterT>::template ParameterType<0>>;
		};
	}

	template<typename GetterT, typename SetterT,
			typename InstanceT = typename TypeMemberTraits<GetterT, SetterT>::InstanceType,
			typename ValueT = typename TypeMemberTraits<GetterT, SetterT>::ValueType>
	struct TFunctorProperty : TInstanceProperty<InstanceT, ValueT>
	{
	public:
		using OnChangedT = typename TInstanceProperty<InstanceT, ValueT>::OnChangedT;

	public:
		TFunctorProperty() = default;
		TFunctorProperty(FName Name, GetterT Getter, SetterT Setter, const FMetadata & Metadata = FMetadata::None, OnChangedT OnChanged = nullptr)
			: TInstanceProperty<InstanceT, ValueT>(Name, Metadata, OnChanged), Getter(Getter), Setter(Setter) { }

	public:
		ValueT Get(InstanceT & Instance) const override
		{
			if constexpr (!IsSameV<GetterT, NullptrT>)
			{
				return (Instance.*Getter)();
			}
			else
				throw EError::NotSupported;
		}

		void Set(InstanceT & Instance, const ValueT & Value) const override
		{
			if constexpr (!IsSameV<SetterT, NullptrT>)
			{
				return (Instance.*Setter)(Value);
			}
			else
				throw EError::NotSupported;
		}

		FVariant GetValue(FVariant Instance) const override
		{
			if constexpr (!IsSameV<GetterT, NullptrT>)
			{
				InstanceT & This = Instance.Get<InstanceT &>();
				return (This.*Getter)();
			}
			else
				return FVariant::None;
		}

		void SetValue(FVariant Instance, FVariant Value) const override
		{
			if constexpr (!IsSameV<SetterT, NullptrT>)
			{
				InstanceT & This = Instance.Get<InstanceT &>();
				return (This.*Setter)(Value.Get<ValueT>());
			}
		}

		FVariant GetLocalValue(FVariant Instance) const override
		{
			if constexpr (!IsSameV<GetterT, NullptrT>)
			{
				InstanceT & This = Instance.Get<InstanceT &>();
				return (This.*Getter)();
			}
			else
				return FVariant::None;
		}

	public:
		GetterT Getter = nullptr;
		SetterT Setter = nullptr;
	};

	template<MemberFieldPointerC FieldT, typename GetterT, typename SetterT,
										typename InstanceT = typename IsMemberFieldPointer<FieldT>::InstanceType,
										typename ValueT = typename IsMemberFieldPointer<FieldT>::ValueType>
	struct TProxyProperty : TInstanceProperty<InstanceT, ValueT>
	{
	public:
		using OnChangedT = typename TInstanceProperty<InstanceT, ValueT>::OnChangedT;

	public:
		TProxyProperty() = default;
		TProxyProperty(FName Name, FieldT Field, GetterT Getter, SetterT Setter, const FMetadata & Metadata = FMetadata::None, OnChangedT OnChanged = nullptr)
			: TInstanceProperty<InstanceT, ValueT>(Name, Metadata, OnChanged), Field(Field), Getter(Getter), Setter(Setter) { }

	public:
		IPropertyValueRef CreatePropertyValue(FVariant /*Instance*/) const override
		{
			throw EError::NotSupported;
		}

		ValueT Get(InstanceT & Instance) const override
		{
			return Instance.*Field;
		}

		void Set(InstanceT & Instance, const ValueT & Value) const override
		{
			if constexpr (!IsSameV<SetterT, NullptrT>)
				return (Instance.*Setter)(Value);
			else
				throw EError::NotSupported;
		}

		FVariant GetValue(FVariant Instance) const override
		{
			InstanceT & This = Instance.Get<InstanceT &>();
			return This.*Field;
		}

		void SetValue(FVariant Instance, FVariant Value) const override
		{
			if constexpr (!IsSameV<SetterT, NullptrT>)
			{
				InstanceT & This = Instance.Get<InstanceT &>();
				return (This.*Setter)(Value.Get<ValueT>());
			}
		}

		FVariant GetLocalValue(FVariant Instance) const override
		{
			InstanceT & This = Instance.Get<InstanceT &>();
			return This.*Field;
		}

	public:
		FieldT Field = nullptr;
		GetterT Getter = nullptr;
		SetterT Setter = nullptr;
	};
}
