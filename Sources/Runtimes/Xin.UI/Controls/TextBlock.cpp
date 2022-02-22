#include "PCH.h"
#include "TextBlock.h"

#include "Platform/Windows/Xin.UI.Windows.Types.h"

namespace Xin::UI
{
	UTextBlock::UTextBlock(FText Text) : __Text(Text) { }

	FStringV UTextBlock::ToString(FStringV Formal) const
	{
		return u8"\""V + Text.Resolve() + u8"\""V;
	}

	void UTextBlock::OnConstruct()
	{
		UElement::OnConstruct();
		HitInside = EHitResult::Ignore;
	}

	void UTextBlock::OnInitialize()
	{
		UElement::OnInitialize();
		AssertExpr(!TextBlob);

		if (Text)
		{
			FFontFace FontFace;
			FontFace.FaceName = FontFamily;
			FontFace.FontWeight = FontWeight;
			FontFace.FontStyle = FontStyle;
			TextBlob = Graphics.CreateTextBlob(Text.Resolve(), FontFace, FontSize * ScaleRatio, SizeF::Infinity/*ConstrainedSize*/);
		}
	}

	void UTextBlock::OnFinalize()
	{
		TextBlob = nullptr;
		UElement::OnFinalize();
	}

	FSize UTextBlock::OnMeasure(FSize ConstrainedSize)
	{
		if (TextBlob)
		{
			AssertExpr(TextBlob->FinalSize.Width < 0xFFFF * 2.0f);
			AssertExpr(TextBlob->FinalSize.Height < 0xFFFF * 2.0f);
			return TextBlob->FinalSize;
		}
		return FSize::Zero;
	}

	void UTextBlock::OnArrange()
	{
		UElement::OnArrange();
	}

	void UTextBlock::OnPaint(IPainter & Painter)
	{
		if (TextBlob)
			Painter.DrawTextBlob(TextBlob.Ref(), FPoint::Zero, Color);
	}

	void UTextBlock::OnTextChanged(FText TextBefore)
	{
		TextBlob = nullptr;
		if (Initialized && Text)
		{
			FFontFace FontFace;
			FontFace.FaceName = FontFamily;
			FontFace.FontWeight = FontWeight;
			FontFace.FontStyle = FontStyle;
			TextBlob = Graphics.CreateTextBlob(Text.Resolve(), FontFace, FontSize * ScaleRatio, SizeF::Infinity/*ConstrainedSize*/);
		}

		//static bool ShownMSB = false;
		//if (!ShownMSB)
		//	MessageBoxW(NULL, L"UTextBlock::OnTextChanged", L"Caption", 0);
		//ShownMSB = true;
		InvalidateMeasure();
		InvalidateVisual();
	}
}
