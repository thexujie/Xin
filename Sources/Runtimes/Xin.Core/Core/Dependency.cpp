#include "PCH.h"
#include "Dependency.h"

//综合起来依赖属性取值优先级列表如下：
//1.强制：在CoerceValueCallback对依赖属性约束的强制值。
//2.活动动画或具有Hold行为的动画。
//3.本地值：通过CLR包装器调用SetValue设置的值，或者XAML中直接对元素本身设置值（包括binding、StaticResource、DynamicResource）
//4.TemplatedParent模板的触发器
//5.TemplatedParent模板中设置的值
//6.隐式样式
//7.样式触发器
//8.模板触发器
//9.样式
//10.主题样式的触发器
//11.主题样式
//12.继承。这里的继承Inherited是xaml树中的父元素，要区别于面向对象语言子类继承（derived，译为派生更合适）与父类
//13.依赖属性元数据中的默认值

namespace CSharp
{
	enum class EBaseValueSourceInternal : short
	{
		Unkonwn = 0,
		Default,
		Inherited,
		ThemeStyle,
		ThemeStyleTrigger,
		Style,
		TemplateTrigger,
		StyleTrigger,
		ImplicitReference,
		ParentTempalte,
		ParentTempalteTrigger,
		Local
	};
}

namespace Xin
{
	FPropertyBinding::FPropertyBinding(const FBindingSource & Source, const FBindingTarget & Target) : Source(Source), Target(Target)
	{
		if (auto SourceObject = Source.Object.Lock())
			SourceObject->Changed += { this, & FPropertyBinding::OnSourceChanged };

		if (Source.BindingMode == EBindingMode::TwoWay)
		{
			if (auto TargetObject = Target.Object.Lock())
				TargetObject->Changed += { this, &FPropertyBinding::OnTargetChanged };
		}

		ToTarget();
	}

	FPropertyBinding::~FPropertyBinding()
	{
		if (auto SourceObject = Source.Object.Lock())
			SourceObject->Changed -= { this, & FPropertyBinding::OnSourceChanged };

		if (Source.BindingMode == EBindingMode::TwoWay)
		{
			if (auto TargetObject = Target.Object.Lock())
				TargetObject->Changed -= { this, &FPropertyBinding::OnTargetChanged };
		}
	}

	void FPropertyBinding::OnSourceChanged(const FProperty & Property)
	{
		if (Executing)
			return;

		if (Property == Source.Path.Property)
			ToTarget();
	}

	void FPropertyBinding::OnTargetChanged(const FProperty & Property)
	{
		AssertExpr(Source.BindingMode == EBindingMode::TwoWay);
		if (Executing)
			return;

		if (Property == Target.Path.Property)
			ToSource();
	}

	void FPropertyBinding::ToTarget()
	{
		auto SourceObject = Source.Object.Lock();
		auto TargetObject = Target.Object.Lock();
		if (SourceObject && TargetObject)
		{
			Executing = true;
			if (!Source.Path.Property)
			{
				FVariant Value { SourceObject };
				if (Source.Path.Converter)
					Value = Source.Path.Converter->Convert(Value, Target.Path.Property);
				TargetObject->SetPropertyValue(Target.Path.Property, Value);
			}
			else if (Source.Path.Field)
			{
				FVariant Value = SourceObject->GetPropertyValue(Source.Path.Property);
				Value = Source.Path.Field.GetValue(Value);
				if (Source.Path.Converter)
					Value = Source.Path.Converter->Convert(Value, Target.Path.Property);
				TargetObject->SetPropertyValue(Target.Path.Property, Value);
			}
			else
			{
				FVariant Value = SourceObject->GetPropertyValue(Source.Path.Property);
				if (Source.Path.Converter)
					Value = Source.Path.Converter->Convert(Value, Target.Path.Property);
				TargetObject->SetPropertyValue(Target.Path.Property, Value);
			}
			Executing = false;
		}
	}

	void FPropertyBinding::ToSource()
	{
		if (!Source.Path.Property)
			return;

		auto SourceObject = Source.Object.Lock();
		auto TargetObject = Target.Object.Lock();
		if (SourceObject && TargetObject)
		{
			Executing = true;
			if (Source.Path.Field)
			{
				FVariant Value = SourceObject->GetPropertyValue(Source.Path.Property);
				if (Source.Path.Converter)
					Value = Source.Path.Converter->Convert(Value, Source.Path.Property);
				Source.Path.Field.SetValue(Value, TargetObject->GetPropertyValue(Target.Path.Property));
				SourceObject->SetPropertyValue(Source.Path.Property, Value);
			}
			else
			{
				FVariant Value = TargetObject->GetPropertyValue(Target.Path.Property);
				if (Source.Path.Converter)
					Value = Source.Path.Converter->Convert(Value, Source.Path.Property);
				SourceObject->SetPropertyValue(Source.Path.Property, Value);
			}
			Executing = false;
		}
	}

	class FChildChangedCallback
	{
	public:
		FChildChangedCallback(IDependency & This, const FProperty & Property) : This(This), Property(Property)
		{
			
		}

		void operator()(const FProperty &)
		{
			This.OnChanged(Property);
		}

		IDependency & This;
		const FProperty & Property;
	};

	void IDependency::OnPropertyChanging(const FProperty & Property)
	{
		if (Property.Type.IsA<IDependencyRef>())
		{
			IDependencyRef PropertyValue = GetValueT<IDependencyRef>(Property);
			if (PropertyValue)
				PropertyValue->Changed -= { this, FChildChangedCallback { *this, Property } };
				//PropertyValue->Changed -= { this, [&This = *this, &Property = Property](const FProperty &) { This.OnChanged(Property); } };
		}
	}

	void IDependency::OnPropertyChanged(const FProperty & Property)
	{
		if (Property.Type.IsA<IDependencyRef>())
		{
			IDependencyRef PropertyValue = GetValueT<IDependencyRef>(Property);
			if (PropertyValue)
				PropertyValue->Changed += { this, FChildChangedCallback { *this, Property } };
				//PropertyValue->Changed += { this, [&This = *this, &Property = Property](const FProperty &) { This.OnChanged(Property); } };
		}
		PropertyChanged(Property);
		OnChanged(Property);
    }

	void IDependency::OnChanged(const FProperty & Property)
	{
		Changed(Property);
	}

	void IDependency::ResetValues(EValueLayer ValueLayer)
	{
		for (auto Iter = PropertyValues.Begin(); Iter != PropertyValues.End(); ++Iter)
			Iter->Second->ResetToLayer(ValueLayer);
	}

	FVariant IDependency::GetPropertyValue(const FProperty & Property) const
	{
		if (Type().IsA(Property.Instance))
		{
			if (auto Iter = PropertyValues.Find(&Property); Iter != PropertyValues.End())
			{
				FVariant Value = Iter->Second->GetCurrentValue();
				if (Value)
					return Value;
			}
			else
			{
				FVariant Value = Property.GetCurrentValue(this);
				if (Value)
					return Value;
			}
		}
		else
		{
			// search property even if this is not a Property.GetInstanceType().
		}

		if (Property.Metadata.Hierarchical)
		{
			if (IObject * ParentObject = GetParentObject(); ParentObject && ParentObject->IsA<IDependency>())
			{
				IDependency * Parent = StaticCast<IDependency>(ParentObject);
				FVariant Value = Parent->GetPropertyValue(Property); // !!! Property == Parent.Property ?? even thought they have same name.
				if (Value)
					return Value;
			}
		}

		return noval;
	}

	IPropertyValue & IDependency::FindPropertyValue(const FProperty & Property) const
	{
		if (auto Iter = PropertyValues.Find(&Property); Iter != PropertyValues.End())
			return Iter->Second.Ref();

		//if (ObjectParent)
		//{
		//	if (IPropertyValue & PropertyValue = ObjectParent->FindPropertyValue(Property))
		//		return PropertyValue;
		//}
		return IPropertyValue::None;
	}

	IPropertyValue & IDependency::FetchPropertyValue(const FProperty & Property) const
	{
		IPropertyValueRef PropertyValue;
		if (auto Iter = PropertyValues.Find(&Property); Iter != PropertyValues.End())
			PropertyValue = Iter->Second.Get();
		else
		{
			PropertyValue = Property.CreatePropertyValue(ConstCast(*this));
			PropertyValue->Changed = { ConstCast(this), [this, &Property](const FProperty &) { ConstCast(this)->OnChanged(Property); } };
			PropertyValues.Insert({ &Property, PropertyValue }).First->Second.Get();
		}
		return *PropertyValue;
	}

	const FMetadata & IDependency::FetchMetadata(const FProperty & Property)
	{
		for (const FOverridenMetadata & OverridenMetadata : OverridenMetadatas)
		{
			if (OverridenMetadata.Property == Property)
				return OverridenMetadata.Metadata;
		}
		return Property.Metadata;
	}
}
