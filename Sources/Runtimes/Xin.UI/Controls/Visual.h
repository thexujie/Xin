#pragma once

#include "Xin.UI.Types.h"
#include "../Core/Object.h"

namespace Xin::UI
{
	class UI_API UVisual : public UObject
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		UVisual();
		UVisual(FName Name);
		~UVisual();
	};

	using UVisualRef = TReferPtr<UVisual>;

}
