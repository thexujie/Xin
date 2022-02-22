#pragma once

#include "Core.Types.h"

namespace Xin
{
	class IDependency;
	using IDependencyObj = TEntryPtr<IDependency>;

	enum class EBindingMode
	{
		OneWay = 0,
		TwoWay,
	};

	class IValueConverter : public ISharedRefer
	{
	public:
		virtual FVariant Convert(const FVariant & Value, const FProperty & Property = FProperty::None) = 0;
	};

	using IValueConverterRef = TReferPtr<IValueConverter>;

	class FValueConverter : public IValueConverter
	{
	public:
		TFunction<FVariant(const FVariant & Value, const FProperty & Property)> Function;

		template<InvocableC<const FVariant &, const FProperty &> CallableT>
		FValueConverter(CallableT && Callable) : Function(Forward<CallableT>(Callable)) {}

		FVariant Convert(const FVariant & Value, const FProperty & Property) override
		{
			return Function(Value, Property);
		}
	};

	template<typename FromT, typename ToT>
	class TValueConverterOneWay : public IValueConverter
	{
	public:
		TFunction<ToT(const FromT &)> Function;

		TValueConverterOneWay(TFunction<ToT(const FromT &)> Function) : Function(Function) {}

		FVariant Convert(const FVariant & Value, const FProperty & /*Property*/) override
		{
			return Function(Value.Get<FromT>());
		}
	};

	struct FBindingPath
	{
		FBindingPath() = default;

		FBindingPath(const FProperty & Property, const FField & Field)
			: Property(Property), Field(Field) {}

		template<typename T, InvocableC<const T &> CallableT>
		FBindingPath(const TProperty<T> & Property, const FField & Field, CallableT && Callable)
			: Property(Property), Field(Field), Converter(new TValueConverterOneWay<T, InvokeResultT<CallableT, const T &>>(Forward<CallableT>(Callable))) {}

		template<InvocableC<const FVariant&, const FProperty &> CallableT>
		FBindingPath(const FProperty & Property, const FField & Field, CallableT && Callable)
			: Property(Property), Field(Field), Converter(new FValueConverter(Forward<CallableT>(Callable))) {}

		FBindingPath(const FProperty & Property)
			: Property(Property) {}

		template<typename T, InvocableC<const T &> CallableT>
		FBindingPath(const TProperty<T> & Property, CallableT && Callable)
			: Property(Property), Converter(new TValueConverterOneWay<T, InvokeResultT<CallableT, const T &>>(Forward<CallableT>(Callable))) {}

		template<InvocableC<const FVariant &, const FProperty &> CallableT>
		FBindingPath(const FProperty & Property, CallableT && Callable)
			: Property(Property), Converter(new FValueConverter(Forward<CallableT>(Callable))) {}

		const FProperty & Property = FProperty::None;
		const FField & Field = FField::None;

		IValueConverterRef Converter;
	};

	struct FBindingSource
	{
		FBindingSource(IDependency * Object, const FBindingPath & Path, EBindingMode BindingMode)
			: Object(Object), Path(Path), BindingMode(BindingMode) {}

		IDependencyObj Object;
		FBindingPath Path;
		EBindingMode BindingMode = EBindingMode::OneWay;
	};

	struct FBindingTarget
	{
		FBindingTarget(IDependency * Object, const FBindingPath & Path) : Object(Object), Path(Path) {}

		IDependencyObj Object;
		FBindingPath Path;
	};


	inline FBindingSource Binding(IDependency * Object, const FBindingPath & BindingPath, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		return { Object, BindingPath, BindingMode };
	}

	inline FBindingSource Binding(IDependency & Object, const FBindingPath & BindingPath, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		return { &Object, BindingPath, BindingMode };
	}

	inline FBindingSource Binding(IDependency * Object, const FProperty & Property, FName FieldName, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		return { Object, { Property, Property.FindField(FieldName) }, BindingMode };
	}

	inline FBindingSource Binding(IDependency & Object, const FProperty & Property, FName FieldName, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		return { &Object, { Property, Property.FindField(FieldName) }, BindingMode };
	}

	inline FBindingSource Binding(IDependency * Object, const FProperty & Property, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		return { Object, { Property, FField::None }, BindingMode };
	}

	inline FBindingSource Binding(IDependency & Object, const FProperty & Property, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		return { &Object, { Property, FField::None }, BindingMode };
	}

	template<typename T, InvocableC<const T &> CallableT>
	FBindingSource Binding(IDependency * Object, const TProperty<T> & Property, FName FieldName, CallableT && Callable, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		Assert(BindingMode != EBindingMode::TwoWay || IsSameV<T, RemoveCVRefT<InvokeResultT<CallableT, const T &>>>, u8"Invalid TwoWay Binding."V);
		return { Object, { Property, Property.FindField(FieldName), Forward<CallableT>(Callable) }, BindingMode };
	}

	template<typename T, InvocableC<const T &> CallableT>
	FBindingSource Binding(IDependency & Object, const TProperty<T> & Property, FName FieldName, CallableT && Callable, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		Assert(BindingMode != EBindingMode::TwoWay || IsSameV<T, RemoveCVRefT<InvokeResultT<CallableT, const T &>>>, u8"Invalid TwoWay Binding."V);
		return { &Object, { Property, Property.FindField(FieldName), Forward<CallableT>(Callable) }, BindingMode };
	}

	template<typename T, InvocableC<const FVariant &, const FProperty &> CallableT>
	FBindingSource Binding(IDependency * Object, const TProperty<T> & Property, FName FieldName, CallableT && Callable, EBindingMode BindingMode = EBindingMode::TwoWay)
	{
		return { Object, { Property, Property.FindField(FieldName), Forward<CallableT>(Callable) }, BindingMode };
	}

	template<typename T, InvocableC<const FVariant &, const FProperty &> CallableT>
	FBindingSource Binding(IDependency & Object, const TProperty<T> & Property, FName FieldName, CallableT && Callable, EBindingMode BindingMode = EBindingMode::TwoWay)
	{
		return { &Object, { Property, Property.FindField(FieldName), Forward<CallableT>(Callable) }, BindingMode };
	}

	template<typename T, InvocableC<const T &> CallableT>
	FBindingSource Binding(IDependency * Object, const TProperty<T> & Property, CallableT && Callable, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		Assert(BindingMode != EBindingMode::TwoWay || IsSameV<T, RemoveCVRefT<InvokeResultT<CallableT, const T &>>>, u8"Invalid TwoWay Binding."V);
		return { Object, { Property, FField::None, Forward<CallableT>(Callable) }, BindingMode };
	}

	template<typename T, InvocableC<const T &> CallableT>
	FBindingSource Binding(IDependency & Object, const TProperty<T> & Property, CallableT && Callable, EBindingMode BindingMode = EBindingMode::OneWay)
	{
		Assert(BindingMode != EBindingMode::TwoWay || IsSameV<T, RemoveCVRefT<InvokeResultT<CallableT, const T &>>>, u8"Invalid TwoWay Binding."V);
		return { &Object, { Property, FField::None, Forward<CallableT>(Callable) }, BindingMode };
	}

	template<typename T, InvocableC<const FVariant &, const FProperty &> CallableT>
	FBindingSource Binding(IDependency * Object, const TProperty<T> & Property, CallableT && Callable, EBindingMode BindingMode = EBindingMode::TwoWay)
	{
		return { Object, { Property, FField::None, Forward<CallableT>(Callable) }, BindingMode };
	}

	template<typename T, InvocableC<const FVariant &, const FProperty &> CallableT>
	FBindingSource Binding(IDependency & Object, const TProperty<T> & Property, CallableT && Callable, EBindingMode BindingMode = EBindingMode::TwoWay)
	{
		return { &Object, { Property, FField::None, Forward<CallableT>(Callable) }, BindingMode };
	}

	class CORE_API IPropertyBinding : public ISharedRefer {};

	using IBindingRef = TReferPtr<IPropertyBinding>;

	class CORE_API FPropertyBinding : public IPropertyBinding
	{
	public:
		FPropertyBinding(const FBindingSource & Source, const FBindingTarget & Target);
		~FPropertyBinding();

	private:
		void OnSourceChanged(const FProperty & Property);
		void OnTargetChanged(const FProperty & Property);
		void ToTarget();
		void ToSource();

	public:
		FBindingSource Source;
		FBindingTarget Target;

	private:
		bool Executing = false;
	};

	enum class EDependencyValueMode
	{
		None = 0,
		Value,
		ConstLRefValue,
		BindingSource,
	};


	/**
     * \brief Never copy TDependencyValueWrapper !!!
     */
	template<typename T>
	struct TDependencyValueWrapper
	{
		TDependencyValueWrapper() {}

		TDependencyValueWrapper(const TDependencyValueWrapper & That) = delete;

		template<typename Ty> requires(!ReferenceC<RemoveCV<T>> && SameC<Ty, T>)
		TDependencyValueWrapper(const Ty & Value) : ValueMode(EDependencyValueMode::Value), Value(Value) {}

		template<typename Ty> requires(ReferenceC<RemoveCV<T>> && SameC<Ty, T>)
		TDependencyValueWrapper(const Ty & Value) : ValueMode(EDependencyValueMode::ConstLRefValue), ConstLRefValue(Value) {}

		TDependencyValueWrapper(const FBindingSource & BindingSource) : ValueMode(EDependencyValueMode::BindingSource), BindingSource(BindingSource) {}

		template<typename... ParameterTs>
			requires ((sizeof...(ParameterTs) > 1 || !AnyC<RemoveCVRefT<PackIndexingT<0, ParameterTs...>>, T, FBindingSource>) && ConstructibleC<T, ParameterTs...>)
		TDependencyValueWrapper(ParameterTs &&... Arguments) : ValueMode(EDependencyValueMode::Value), Value(Forward<ParameterTs>(Arguments)...) {}

		~TDependencyValueWrapper()
		{
			Reset();
		}

		void Reset()
		{
			switch (ValueMode)
			{
			case EDependencyValueMode::Value:
				Value.~T();
				break;
			case EDependencyValueMode::BindingSource:
				BindingSource.~FBindingSource();
				break;
			default:
				break;
			}
			ValueMode = EDependencyValueMode::None;
		}

		//TDependencyValueWrapper & operator =(IBindingValueRef Value)
		//{
		//    Reset();
		//    Binding = Value;
		//    ValueMode = EDependencyValueMode::Binding;
		//    return *this;
		//}

		//TDependencyValueWrapper & operator =(const T & Value)
		//{
		//    Reset();
		//    ConstLRefValue = Value;
		//    ValueMode = EDependencyValueMode::ConstLRefValue;
		//    return *this;
		//}

		EDependencyValueMode ValueMode = EDependencyValueMode::None;

		union
		{
			TReferenceWrapper<const T> ConstLRefValue {};
			FBindingSource BindingSource;
			T Value;
		};
	};
}

#if XIN_DEBUG
#define DEPENDENCY_DEBUG_DEFAULT_FIELD(Type, Name) \
	TOptional<Type> & __ ## Name ## _Default__ = __ ## Name ## __;
#else
#define DEPENDENCY_DEBUG_DEFAULT_FIELD(Type, Name)
#endif

#define DependencyPropertyRW(Type, Name) \
	__declspec(property(get = __Get ## Name, put = __Set ## Name)) Type Name; \
    static_assert(IsCopyConstructibleV<Type>); \
	static const TProperty<Type> & Name##Property(); \
	Type __Get ## Name() const \
	{ \
		FVariant Value = GetPropertyValue(Name##Property()); \
		if (Value) \
			return Value.Get<Type>();\
		else \
			return { };\
	} \
	void __Set ## Name ## Value(Type const & Value) \
	{\
		ResetValue(Name##Property(), EValueLayer::Local); \
		Type ValueBefore { __Get ## Name() }; \
        if constexpr (IsEqualComparableV<Type>) \
		{ \
			if (ValueBefore == Value) \
            { \
				if (!__ ## Name.HasValue) \
					__ ## Name = Value; \
				return; \
            } \
		} \
		OnPropertyChanging(Name##Property()); \
		__ ## Name = Value; \
		__if_exists (On ## Name ## Changed) \
		{ \
			On ## Name ## Changed(ValueBefore); \
		} \
		OnPropertyChanged(Name##Property()); \
	} \
	void __Set ## Name(const TDependencyValueWrapper<Type> & ValueWrapper) \
	{\
		if (ValueWrapper.ValueMode == EDependencyValueMode::Value) \
		{ \
			__Set ## Name ## Value(ValueWrapper.Value); \
            return; \
		} \
		else if (ValueWrapper.ValueMode == EDependencyValueMode::ConstLRefValue) \
		{ \
			__Set ## Name ## Value(ValueWrapper.ConstLRefValue); \
            return; \
		} \
		else if (ValueWrapper.ValueMode == EDependencyValueMode::BindingSource) \
		{ \
			AddBinding<Type>(Name##Property(), ValueWrapper.BindingSource); \
            return; \
		} \
        else {} \
	} \
	TOptional<Type> __ ## Name; \
	DEPENDENCY_DEBUG_DEFAULT_FIELD(Type, Name) \
	static inline TOptional<Type> __ ## Name ## __


#define AttachedPropertyRW(Type, Name) \
	__declspec(property(get = __Get ## Name, put = __Set ## Name)) Type Name; \
	static const TProperty<Type> & Name##Property(); \
	Type __Get ## Name() const { return GetValueT<Type>(Name##Property()); } \
	void __Set ## Name(const Type & Value) { SetValueT<Type>(Name##Property(), Value);} \
	static inline TOptional<Type> __ ## Name ## __

namespace Xin
{
	template<typename T>
	class TPropertyValue : public IPropertyValue
	{
	public:
		TOptional<T> & DefaultValue;
		TOptional<T> TemplateValue;
		TOptional<T> StyleValue;
		TOptional<T> & LocalValue;
		TOptional<T> ExpressionValue;
		TOptional<T> AnimatedValue;

		explicit TPropertyValue(TOptional<T> & LocalValue, TOptional<T> & DefaultValue)
			: DefaultValue(DefaultValue), LocalValue(LocalValue) {}

		void ResetToLayer(EValueLayer ValueLayer) override
		{
			if (ValueLayer < EValueLayer::Default)
				DefaultValue.Reset();
			if (ValueLayer < EValueLayer::Template)
				TemplateValue.Reset();
			if (ValueLayer < EValueLayer::Style)
				StyleValue.Reset();
			if (ValueLayer < EValueLayer::Local)
				LocalValue.Reset();
			if (ValueLayer < EValueLayer::Expression)
				ExpressionValue.Reset();
			if (ValueLayer < EValueLayer::Animated)
				AnimatedValue.Reset();
		}

		bool Defaulted() const { return DefaultValue.HasValue; }

		T & Default()
		{
			return DefaultValue;
		}

		bool Based() const { return LocalValue.HasValue || StyleValue.HasValue || TemplateValue.HasValue || DefaultValue.HasValue; }

		T & Base()
		{
			if (LocalValue.Valid())
				return LocalValue;
			if (StyleValue.Valid())
				return StyleValue;
			if (TemplateValue.Valid())
				return TemplateValue;
			return DefaultValue;
		}

		bool Effected() const
		{
			return AnimatedValue.HasValue || ExpressionValue.HasValue || LocalValue.HasValue || StyleValue.HasValue || TemplateValue.HasValue;
		}

		bool Defined() const
		{
			return AnimatedValue.HasValue || ExpressionValue.HasValue || LocalValue.HasValue || StyleValue.HasValue || TemplateValue.HasValue || DefaultValue.HasValue;
		}

		T & Current()
		{
			if (AnimatedValue.Valid())
				return AnimatedValue;
			if (ExpressionValue.Valid())
				return ExpressionValue;
			if (LocalValue.Valid())
				return LocalValue;
			if (StyleValue.Valid())
				return StyleValue;
			if (TemplateValue.Valid())
				return TemplateValue;
			AssertExpr(DefaultValue.HasValue);
			return DefaultValue;
		}

		FVariant GetCurrentValue() const override
		{
			if (AnimatedValue.Valid())
				return AnimatedValue;
			if (ExpressionValue.Valid())
				return ExpressionValue;
			if (LocalValue.Valid())
				return LocalValue;
			if (StyleValue.Valid())
				return StyleValue;
			if (TemplateValue.Valid())
				return TemplateValue;
			if (DefaultValue.Valid())
				return DefaultValue;
			return noval;
		}

		void SetValue(EValueLayer ValueLayer, FVariant Value) override
		{
			switch (ValueLayer)
			{
			case EValueLayer::Default:
				DefaultValue = Value.Get<T>();
				break;
			case EValueLayer::Template:
				TemplateValue = Value.Get<T>();
				break;
			case EValueLayer::Style:
				StyleValue = Value.Get<T>();
				break;
			case EValueLayer::Local:
				LocalValue = Value.Get<T>();
				break;
			case EValueLayer::Expression:
				ExpressionValue = Value.Get<T>();
				break;
			case EValueLayer::Animated:
				AnimatedValue = Value.Get<T>();
				break;
			default:
				break;
			}
		}

		void ClearValue(EValueLayer ValueLayer) override
		{
			switch (ValueLayer)
			{
			case EValueLayer::Default:
				DefaultValue.Reset();
				break;
			case EValueLayer::Template:
				TemplateValue.Reset();
				break;
			case EValueLayer::Style:
				StyleValue.Reset();
				break;
			case EValueLayer::Local:
				LocalValue.Reset();
				break;
			case EValueLayer::Expression:
				ExpressionValue.Reset();
				break;
			case EValueLayer::Animated:
				AnimatedValue.Reset();
				break;
			default:
				break;
			}
		}
	};


	template<typename T>
	class TDependencyValue : public TPropertyValue<T>
	{
	public:
		explicit TDependencyValue(TOptional<T> & LocalValue)
			: TPropertyValue<T>(LocalValue, LocalValue) {}

		explicit TDependencyValue(TOptional<T> & LocalValue, TOptional<T> & DefaultValue)
			: TPropertyValue<T>(LocalValue, DefaultValue) {}
	};

	template<typename T>
	class TAttachedValue : public TPropertyValue<T>
	{
	public:
		TAttachedValue(TOptional<T> & DefaultValue) : TPropertyValue<T>(AttachedValue, DefaultValue) {}

	public:
		TOptional<T> AttachedValue;
	};

	class CORE_API IDependency : public IObject
	{
	public:
		static FType & StaticType();

		FType & Type() const override { return StaticType(); }

	public:
		IDependency() = default;

		IDependency(FName Name) : IObject(Name) {}

		virtual void OnPropertyChanging(const FProperty & Property);
		virtual void OnPropertyChanged(const FProperty & Property);
		virtual void OnChanged(const FProperty & Property);

	public:
		mutable TMap<const FProperty *, IPropertyValueRef> PropertyValues;

		void ResetValues(EValueLayer ValueLayer = EValueLayer::Local);

		IPropertyValue & FindPropertyValue(const FProperty & Property) const;
		IPropertyValue & FetchPropertyValue(const FProperty & Property) const;

		FVariant GetPropertyValue(const FProperty & Property) const;

		void SetPropertyValue(const FProperty & Property, const FVariant & Value)
		{
			Property.SetValue(this, Value);
		}

	public:
		template<typename T>
		TPropertyValue<T> & FindPropertyValueT(const FProperty & Property) const
		{
			return StaticCast<TPropertyValue<T>>(FindPropertyValue(Property));
		}

		template<typename T>
		TPropertyValue<T> & FetchPropertyValueT(const FProperty & Property) const
		{
			return StaticCast<TPropertyValue<T>>(FetchPropertyValue(Property));
		}

	public:
		template<typename T>
		T GetValueT(const FProperty & Property, const T & DefaultValue) const
		{
			if (TPropertyValue<T> & PropertyValue = StaticCast<TPropertyValue<T>>(FindPropertyValue(Property)))
			{
				if (PropertyValue.Effected())
					return PropertyValue.Current();
			}
			else
			{
				FVariant LocalValue = Property.GetLocalValue(*this);
				if (LocalValue)
					return LocalValue.Get<T>();
			}

			if (Property.Metadata.Hierarchical)
			{
				if (IObject * ParentObject = GetParentObject(); ParentObject && ParentObject->IsA<IDependency>())
				{
					IDependency * Parent = StaticCast<IDependency>(ParentObject);
					FVariant Value = Parent->GetPropertyValue(Property);
					if (Value)
						return Value.Get<T>();
				}
			}

			return DefaultValue;
		}

		template<typename T>
		T GetValueT(const FProperty & Property) const
		{
			if (TPropertyValue<T> & PropertyValue = StaticCast<TPropertyValue<T>>(FindPropertyValue(Property)))
			{
				if (PropertyValue.Effected())
					return PropertyValue.Current();
			}
			else
			{
				FVariant CurrentValue = Property.GetCurrentValue(*this);
				if (CurrentValue)
					return CurrentValue.Get<T>();
			}

			if (Property.Metadata.Hierarchical)
			{
				if (IObject * ParentObject = GetParentObject(); ParentObject && ParentObject->IsA<IDependency>())
				{
					IDependency * Parent = StaticCast<IDependency>(ParentObject);
					FVariant Value = Parent->GetPropertyValue(Property);
					if (Value)
						return Value.Get<T>();
				}
			}

			return {};
		}

		template<typename T>
		T GetValueT(const TProperty<T> & Property) const
		{
			if (TPropertyValue<T> & PropertyValue = StaticCast<TPropertyValue<T>>(FindPropertyValue(Property)))
			{
				if (PropertyValue.Defined())
					return PropertyValue.Current();
			}
			else
			{
				FVariant CurrentValue = Property.GetCurrentValue(*this);
				if (CurrentValue)
					return CurrentValue.Get<T>();
			}
			return {};
		}

		template<typename T>
		T GetBaseValueT(const FProperty & Property, const T & DefaultValue) const
		{
			if (TPropertyValue<T> & PropertyValue = StaticCast<TPropertyValue<T>>(FindPropertyValue(Property)))
			{
				if (PropertyValue.Based())
					return PropertyValue.Base();
			}
			else
			{
				FVariant LocalValue = Property.GetLocalValue(*this);
				if (LocalValue)
					return LocalValue.Get<T>();
			}
			return DefaultValue;
		}

		template<typename T>
		T GetBaseValueT(const FProperty & Property) const
		{
			if (TPropertyValue<T> & PropertyValue = StaticCast<TPropertyValue<T>>(FindPropertyValue(Property)))
			{
				if (PropertyValue.Based())
					return PropertyValue.Base();
			}
			else
			{
				FVariant LocalValue = Property.GetLocalValue(*this);
				if (LocalValue)
					return LocalValue.Get<T>();
			}
			return {};
		}

		template<typename T>
		void SetValueT(const FProperty & Property, const T & Value)
		{
			TPropertyValue<T> & PropertyValue = FetchPropertyValueT<T>(Property);
			if (PropertyValue)
			{
				PropertyValue.LocalValue = Value;
				OnPropertyChanged(Property);
			}
		}

		void ResetValue(const FProperty & Property, EValueLayer ValueLayer = EValueLayer::Local)
		{
			IPropertyValue & PropertyValue = FindPropertyValue(Property);
			if (PropertyValue)
			{
				PropertyValue.ResetToLayer(ValueLayer);
				//OnPropertyChanged(Property);
			}
		}

	public:
		template<typename T>
		void SetEvaluatedValue(const FProperty & Property, const T & Value)
		{
			if (TPropertyValue<T> & PropertyValue = FetchPropertyValueT<T>(Property))
			{
				PropertyValue.ExpressionValue = Value;
				OnPropertyChanged(Property);
			}
		}

		template<typename T>
		void UnsetEvaluatedValue(const FProperty & Property)
		{
			if (TPropertyValue<T> & PropertyValue = FindPropertyValueT<T>(Property))
			{
				PropertyValue.ExpressionValue = noval;
				OnPropertyChanged(Property);
			}
		}

		template<typename T>
		void SetAnimatedValue(const FProperty & Property, const T & Value)
		{
			if (TPropertyValue<T> & PropertyValue = FetchPropertyValueT<T>(Property))
			{
				PropertyValue.AnimatedValue = Value;
				OnPropertyChanged(Property);
			}
		}

		template<typename T>
		void UnsetAnimatedValue(const FProperty & Property)
		{
			if (TPropertyValue<T> & PropertyValue = FindPropertyValueT<T>(Property))
			{
				PropertyValue.AnimatedValue = noval;
				OnPropertyChanged(Property);
			}
		}

		template<typename T>
		void AddBinding(const TProperty<T> & Property, const FBindingSource & BindingSource)
		{
			Bindings.Add(new FPropertyBinding(BindingSource, FBindingTarget { this, { Property, FField::None } }));
		}

	public:
		template<typename T>
		TOptional<T> & operator [](const TProperty<T> & Property)
		{
			TPropertyValue<T> & PropertyValue = FetchPropertyValueT<T>(Property);
			return PropertyValue.LocalValue;
		}

		template<typename T>
		const TOptional<T> & operator [](const TProperty<T> & Property) const
		{
			TPropertyValue<T> & PropertyValue = FetchPropertyValueT<T>(Property);
			return PropertyValue.LocalValue;
		}

	public:
		virtual const FMetadata & FetchMetadata(const FProperty & Property);

		template<DerivedOfC<FMetadata> MetadataT>
		const MetadataT & FetchMetadataT(const FProperty & Property)
		{
			return StaticCast<MetadataT>(FetchMetadata(Property));
		}

		struct FOverridenMetadata
		{
			FOverridenMetadata() = default;

			FOverridenMetadata(const FProperty & Property, const FMetadata & Metadata) : Property(Property), Metadata(Metadata) {}

			const FProperty & Property = FProperty::None;
			const FMetadata & Metadata = FMetadata::None;
		};

	public:
		TList<FOverridenMetadata> OverridenMetadatas;
		TList<IBindingRef> Bindings;

	public:
		TEvent<void(const FProperty &)> PropertyChanged;
		TEvent<void(const FProperty &)> Changed;
	};

	using IDependencyRef = TReferPtr<IDependency>;


	template<MemberFieldPointerC FieldT, typename GetterT, typename SetterT,
										typename InstanceT = typename IsMemberFieldPointer<FieldT>::InstanceType,
										typename OptinalT = typename IsMemberFieldPointer<FieldT>::ValueType,
										typename ValueT = typename OptinalT::ValueType>
	struct TDependencyProperty : TInstanceProperty<InstanceT, ValueT>
	{
	public:
		using OnChangedT = typename TInstanceProperty<InstanceT, ValueT>::OnChangedT;

	public:
		constexpr TDependencyProperty() = default;

		constexpr TDependencyProperty(FName Name, FieldT Field, GetterT Getter, SetterT Setter, OptinalT & DefaultValue, const FMetadata & Metadata = FMetadata::None, OnChangedT OnChanged = nullptr)
			: TInstanceProperty<InstanceT, ValueT>(Name, Metadata, OnChanged), Field(Field), Getter(Getter), Setter(Setter), DefaultValue(DefaultValue) {}

	public:
		IPropertyValueRef CreatePropertyValue(FVariant Instance) const override
		{
			InstanceT & This = Instance.Get<InstanceT&>();
			OptinalT & LocalValue = This.*Field;
			return new TDependencyValue<ValueT>(LocalValue, DefaultValue);
		}

		ValueT Get(InstanceT & Instance) const override
		{
			return (Instance.*Getter)();
		}

		void Set(InstanceT & Instance, const ValueT & Value) const override
		{
			Instance.*Field = Value;
		}

		FVariant GetValue(FVariant Instance) const override
		{
			InstanceT & This = Instance.Get<InstanceT&>();
			return (This.*Getter)();
		}

		void SetValue(FVariant Instance, FVariant Value) const override
		{
			if constexpr (!IsSameV<SetterT, NullptrT>)
			{
				InstanceT & This = Instance.Get<InstanceT&>();
				return (This.*Setter)(Value.Get<ValueT>());
			}
		}

		FVariant GetLocalValue(FVariant Instance) const override
		{
			InstanceT & This = Instance.Get<InstanceT&>();

			OptinalT & LocalValue = This.*Field;
			if (LocalValue.HasValue)
				return LocalValue;

			return noval;
		}

		FVariant GetDefaultValue(FVariant Instance) const override
		{
			if (DefaultValue.HasValue)
				return DefaultValue;

			return noval;
		}

		FVariant GetCurrentValue(FVariant Instance) const override
		{
			InstanceT & This = Instance.Get<InstanceT&>();

			OptinalT & LocalValue = This.*Field;
			if (LocalValue.HasValue)
				return LocalValue;

			if (DefaultValue.HasValue)
				return DefaultValue;

			return noval;
		}

	public:
		FieldT Field = nullptr;
		GetterT Getter = nullptr;
		SetterT Setter = nullptr;

		OptinalT & DefaultValue = OptinalT::None;
	};


	template<typename InstanceT, typename ValueT>
	struct TAttachedProperty : TInstanceProperty<InstanceT, ValueT>
	{
	public:
		using OptionalT = TOptional<ValueT>;
		using OnChangedT = typename TInstanceProperty<InstanceT, ValueT>::OnChangedT;

	public:
		constexpr TAttachedProperty() = default;

		constexpr TAttachedProperty(FName Name, OptionalT & Field, const FMetadata & Metadata = FMetadata::None, OnChangedT OnChanged = nullptr)
			: TInstanceProperty<InstanceT, ValueT>(Name, Metadata, OnChanged), Field(Field) {}

	public:
		IPropertyValueRef CreatePropertyValue(FVariant /*Instance*/) const override
		{
			OptionalT & DefaultValue = Field;
			return new TAttachedValue<ValueT>(DefaultValue);
		}

		ValueT Get(InstanceT & Instance) const override
		{
			throw EError::NotSupported;
		}

		void Set(InstanceT & Instance, const ValueT & Value) const override
		{
			throw EError::NotSupported;
		}

		FVariant GetValue(FVariant Instance) const override
		{
			IDependency & This = Instance.Get<IDependency&>();
			return This.GetValueT<ValueT>(*this);
		}

		void SetValue(FVariant Instance, FVariant Value) const override
		{
			IDependency & This = Instance.Get<IDependency&>();
			This.SetValueT<ValueT>(*this, Value.Get<ValueT>());
		}

	public:
		OptionalT & Field = OptionalT::None;
	};
}
