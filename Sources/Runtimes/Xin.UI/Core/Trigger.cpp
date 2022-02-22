#include "PCH.h"
#include "Trigger.h"
#include "Object.h"

namespace Xin::UI
{
	FTrigger::FTrigger(const FProperty & Property, TView<ITriggerActionRef> Actions, TView<ITriggerActionRef> ExitActions)
		: Property(Property), Actions(Actions), ExitActions(ExitActions)
	{
	}

	FTrigger::FTrigger(const FProperty & Property, TFunction<bool()> Condition, TView<ITriggerActionRef> Actions, TView<ITriggerActionRef> ExitActions)
		: Property(Property) , Condition(Condition) , Actions(Actions) , ExitActions(ExitActions)
	{
	}

	void FTrigger::Trigger(UObject & Object)
	{
		bool IsEntering = Condition ? Condition() : Property.GetValue(Object).Get<bool>();
		if (IsEntering)
		{
			for (ITriggerActionRef & Action : ExitActions)
				Action->Stop(Object);
			for (ITriggerActionRef & Action : Actions)
				Action->Play(Object);
		}
		else
		{
			for (ITriggerActionRef & Action : Actions)
				Action->Stop(Object);
			for (ITriggerActionRef & Action : ExitActions)
				Action->Play(Object);
		}
	}

	void FTrigger::Interrupt(UObject & Object)
	{
		for (ITriggerActionRef & Action : ExitActions)
			Action->Interrupt(Object);
		for (ITriggerActionRef & Action : Actions)
			Action->Interrupt(Object);
	}
}
