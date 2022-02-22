#include "PCH.h"
#include "ScrollInfo.h"

namespace Xin::UI
{
	void IScrollInfo::SetViewport(FSize ViewportSize_, FSize ExtentSize_)
	{
		if (ViewportSize != ViewportSize_ || ExtentSize != ExtentSize_)
		{
			ViewportSize = ViewportSize_;
			ExtentSize = ExtentSize_;
			ScrollOffset = Clamp(ScrollOffset, FVec2::Zero, ExtentSize);
			//if (ClampedScrollOffset != ScrollOffset)
			{
				FEventArgs EventArgs;
				ScrollChanged(EventArgs);
			}
		}
	}

	void IScrollInfo::Scroll(FVec2 Offset)
	{
		FPoint ClampedScrollOffset = Clamp(Offset, FVec2::Zero, ExtentSize);

		if (ClampedScrollOffset == ScrollOffset)
			return;

		ScrollOffset = ClampedScrollOffset;

		FEventArgs EventArgs;
		ScrollChanged(EventArgs);
	}

	void IScrollInfo::LineUp()
	{

	}

	void IScrollInfo::LineDown()
	{

	}

	void IScrollInfo::PageUp()
	{
		Scroll({ ScrollOffset.X, ScrollOffset.Y - ViewportSize.Height });
	}

	void IScrollInfo::PageDown()
	{
		Scroll({ ScrollOffset.X, ScrollOffset.Y + ViewportSize.Height });
	}


	void IScrollInfo::PageLeft()
	{
		Scroll({ ScrollOffset.X - ViewportSize.Width, ScrollOffset.Y });
	}

	void IScrollInfo::PageRight()
	{
		Scroll({ ScrollOffset.X + ViewportSize.Width, ScrollOffset.Y });
	}
}

