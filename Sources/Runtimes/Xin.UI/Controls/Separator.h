#pragma once

#include "Xin.UI.Types.h"
#include "Element.h"

namespace Xin::UI
{
	class UI_API USeparator : public UElement
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		USeparator() = default;
		~USeparator() = default;
	};
	using USeparatorRef = TReferPtr<USeparator>;
}
