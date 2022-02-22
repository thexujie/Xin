#pragma once

#include "ButtonControl.h"

namespace Xin::UI
{
	class UI_API UButton : public UButtonControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		UButton();
		UButton(FName Name) : UButtonControl(Name) {}
		~UButton() = default;

	public:
		void OnPaint(IPainter & Painter) override;
	};
	using UButtonRef = TReferPtr<UButton>;
}
