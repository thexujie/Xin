#pragma once

#include "Xin.UI.Types.h"

namespace Xin::UI
{
	class UI_API [[Meta]] IScrollInfo : public IObject
	{
	public:
		static constexpr FName InterfaceName = u8"IScrollInfo"N;

	public:
		IScrollInfo() = default;
		virtual ~IScrollInfo() = default;

	public:
		void SetViewport(FSize ViewportSize, FSize ExtentSize);

		void Scroll(FVec2 Offset);

		void LineUp();
		void LineDown();
		void PageUp();
		void PageDown();

		void PageLeft();
		void PageRight();

	public:
		FSize ViewportSize;
		FSize ExtentSize;
		FPoint ScrollOffset;

	public:
		TEvent<void(FEventArgs & Args)> ScrollChanged;
	};
	using IScrollInfoRef = TReferPtr<IScrollInfo>;
}
