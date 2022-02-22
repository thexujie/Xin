#include "PCH.h"
#include "Image.h"
#include "Window.h"

namespace Xin::UI
{
	UImage::UImage()
	{
		
	}

	void UImage::OnConstruct()
	{
		UElement::OnConstruct();
		HitInside = EHitResult::Ignore;
	}

	void UImage::OnFinalize()
	{
		Bitmap = nullptr;
		UElement::OnFinalize();
	}

	FSize UImage::OnMeasure(FSize ConstrainedSize)
	{
		if (!Bitmap)
		{
			if (Pixmap)
				Bitmap = Graphics.CreateBitmap(*Pixmap);
			else if (ImageSource)
				Bitmap = Graphics.CreateBitmap(ImageSource);
			else {}
		}

		if (!Bitmap)
			return DefaultSize;

		if (Stretch == EStretch::None)
			return Bitmap->Size;

		return Min(Bitmap->Size, ConstrainedSize);
	}

	void UImage::OnPaint(IPainter & Painter)
	{
		if (!Bitmap)
			return;

		switch(Stretch)
		{
		case EStretch::None:
			Painter.DrawBitmap(Bitmap.Ref(), { FPoint::Zero, Bitmap->Size });
			break;
		case EStretch::Fill:
			Painter.DrawBitmap(Bitmap.Ref(), { FPoint::Zero, FinalSize });
			break;
		case EStretch::Uniform:
		{
			FSize StretchScale = FinalSize / Bitmap->Size;
			SizeF StretchSize = Bitmap->Size * Min(StretchScale.X, StretchScale.Y);
			RectF StretchRect { float32(FinalSize.Width - StretchSize.Width) * 0.5f, float32(FinalSize.Height - StretchSize.Height) * 0.5f, StretchSize.Width, StretchSize.Height };
			Painter.DrawBitmap(Bitmap.Ref(), StretchRect);
			break;
		}
		case EStretch::UniformFill:
		{
			FSize StretchScale = FinalSize / Bitmap->Size;
			SizeF StretchSize = Bitmap->Size * Max(StretchScale.X, StretchScale.Y);
			RectF StretchRect { float32(FinalSize.Width - StretchSize.Width) * 0.5f, float32(FinalSize.Height - StretchSize.Height) * 0.5f, StretchSize.Width, StretchSize.Height };
			Painter.DrawBitmap(Bitmap.Ref(), StretchRect);
			break;
		}
		default: ;
		}
	}
}
