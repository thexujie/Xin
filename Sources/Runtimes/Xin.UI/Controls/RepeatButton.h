#pragma once

#include "ButtonControl.h"

namespace Xin::UI
{
	class UI_API URepeatButton : public UButtonControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		URepeatButton() = default;
		URepeatButton(FName Name) : UButtonControl(Name) {}
		~URepeatButton() = default;
		void OnConstruct() override;

	public:
		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;
		void OnMouseLeave(FMouseEventArgs & Args) override;

	private:
		FDispatcherTimer DispatcherTimer;
		uintx RepeatIndex = 0;
		void DispatcherTimer_OnTick();

	public:
		DependencyPropertyRW(uint32, Delay) = FSystem::ParameterInfo.KeyboardDelay;
		DependencyPropertyRW(uint32, Interval) = FSystem::ParameterInfo.KeyboardSpeed;
	};
	using URepeatButtonRef = TReferPtr<URepeatButton>;
}
