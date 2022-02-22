#include "PCH.h"
#include "Rectangle.h"

namespace Xin::UI
{
	void URectangle::OnPaint(IPainter & Painter)
	{
		if (Fill)
			Painter.DrawRect({ FPoint::Zero, FinalSize }, Fill.Ref(), IPen(Stroke.Ref(), StrokeThickness));
	}
}

