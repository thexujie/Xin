#pragma once

#include "Xin.UI.Types.h"
#include "Element.h"

namespace Xin::UI
{
	class UI_API UCanvas : public UElement
	{
	public:
		UCanvas(FName Name) : UElement(Name) {}
		~UCanvas() = default;

	};
	using UCanvasRef = TReferPtr<UCanvas>;
}
