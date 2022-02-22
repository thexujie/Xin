#include "PCH.h"
#include "Ellipse.h"

namespace Xin::UI
{
	void UEllipse::OnPaint(IPainter & Painter)
	{
		if (Fill)
		{
			Painter.DrawEllipse({ FPoint::Zero, FinalSize }, Fill.Ref(), IPen(Stroke.Ref(), StrokeThickness));
		}
	}
}
