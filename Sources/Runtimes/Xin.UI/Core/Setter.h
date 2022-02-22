#pragma once

#include "Xin.UI.Types.h"
#include "Trigger.h"
#include "Object.h"

namespace Xin::UI
{
	class UI_API FSetterAction : public ITriggerAction
	{
	public:
		FSetterAction(IDependencyObj Object, const FProperty & Property, FVariant Value) : Object(Object), Property(Property), Value(Value) {}

		void Play(UObject & Owner) override
		{
			if (auto Instance = Object.Lock())
				Property.SetValue(Instance, Value);
		}

		void Stop(UObject & Owner) override
		{
		}

	public:
		IDependencyObj Object;

		const FProperty & Property;
		FVariant Value;
	};

	template<typename ValueT>
	class UI_API TSetterAction : public FSetterAction
	{
	public:
		TSetterAction(IDependencyObj Object, const TProperty<ValueT> & Property, const ValueT & Value) : FSetterAction(Object, Property, Value) {}
	};
}
