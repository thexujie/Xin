#include "PCH.h"
#include "Border.h"

namespace Xin::UI
{
	UBorder::UBorder()
	{
	}

	void UBorder::OnConstruct()
	{
		UPresenter::OnConstruct();
		HitInside = EHitResult::Ignore;
	}

	FSize UBorder::OnMeasure(FSize ConstrainedSize)
	{
		if (!Child)
			return DefaultSize;
		
		FThickness ContentPadding = Thickness + Padding;
		FSize ChildConstrainedSize = Child->Measure(ConstrainedSize - ContentPadding);
		return ChildConstrainedSize + ContentPadding;
	}

	void UBorder::OnArrange()
	{
		if (!Child)
			return;

		FThickness BorderExtent = Thickness + Padding;
		ArrangeChild(*Child, { BorderExtent.LeftTop, FinalRect.Size - BorderExtent.Size });
	}

	void UBorder::OnPaint(IPainter & Painter)
	{
		FThickness Thickness = this->Thickness;
		if (Thickness.Uniform())
		{
			if (Fill && Stroke && Thickness > 0dim)
				Painter.DrawRect({ FPoint::Zero, FinalSize }, Fill.Ref(), { Stroke.Ref(), (float32)Thickness.Left });
			else if (Fill)
				Painter.DrawRect({ FPoint::Zero, FinalSize }, Fill.Ref(), IPen::None);
			else if (Stroke && Thickness > 0dim)
				Painter.DrawRect({ FPoint::Zero, FinalSize }, IBrush::None, { Stroke.Ref(), (float32)Thickness.Left });
			else { }
		}
		else
		{
			if (Fill)
				Painter.FillRect({ FPoint::Zero, FinalSize }, Fill.Ref());

			if (Stroke && Thickness > 0dim)
			{
				if (Thickness.Left > 0)
					Painter.DrawLine({ (float32)(Thickness.Left * 0.5), (float32)FinalSize.Height }, { (float32)(Thickness.Left * 0.5), 0 }, { Stroke.Ref(), (float32)Thickness.Left });
				if (Thickness.Top > 0)
					Painter.DrawLine({ 0, (float32)(Thickness.Top * 0.5) }, { (float32)FinalSize.Width, (float32)(Thickness.Top * 0.5) }, { Stroke.Ref(), (float32)Thickness.Top });
				if (Thickness.Right > 0)
					Painter.DrawLine({ (float32)(FinalSize.Width - Thickness.Right * 0.5), 0 }, { (float32)(FinalSize.Width - Thickness.Right * 0.5), (float32)FinalSize.Height }, { Stroke.Ref(), (float32)Thickness.Right });
				if (Thickness.Bottom > 0)
					Painter.DrawLine({ (float32)FinalSize.Width, (float32)(FinalSize.Height - Thickness.Bottom * 0.5) }, { 0, (float32)(FinalSize.Height - Thickness.Bottom * 0.5) }, { Stroke.Ref(), (float32)Thickness.Bottom });
			}
		}
	}
}
