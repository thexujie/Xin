#pragma once

#include "Xin.UI.Types.h"

namespace Xin::UI
{
	class UObject;

	class UI_API ITriggerAction : public IObject
	{
	public:
		virtual void Play(UObject & Object) = 0;
		virtual void Stop(UObject & Object) = 0;
		virtual void Interrupt(UObject & Object) { Stop(Object); }
	};
	using ITriggerActionRef = TReferPtr<ITriggerAction>;

	class UI_API ITrigger : public IObject
	{
	public:
		ITrigger() = default;

		virtual void Trigger(UObject & Object) = 0;
		virtual void Interrupt(UObject & Object) = 0;
	};
	using ITriggerRef = TReferPtr<ITrigger>;

	class UI_API FTrigger : public ITrigger
	{
	public:
		FTrigger(const FProperty & Property, TView<ITriggerActionRef> Actions, TView<ITriggerActionRef> ExitActions = { });
		FTrigger(const FProperty & Property, TFunction<bool()> Condition, TView<ITriggerActionRef> Actions, TView<ITriggerActionRef> ExitActions = { });

		void Trigger(UObject & Object) override;
		void Interrupt(UObject & Object) override;
	public:
		const FProperty & Property;
		TFunction<bool()> Condition;

		TList<ITriggerActionRef> Actions;
		TList<ITriggerActionRef> ExitActions;
	};
	using FTriggerRef = TReferPtr<FTrigger>;
}
