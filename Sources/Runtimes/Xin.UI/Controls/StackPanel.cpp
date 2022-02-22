#include "PCH.h"
#include "StackPanel.h"

namespace Xin::UI
{
	void UStackPanel::OnInitialize()
	{
		UPanel::OnInitialize();
	}

	FSize UStackPanel::OnMeasure(FSize ConstrainedSize)
	{
		FSize TotalSize = FSize::Zero;
		for (UElementRef Element : Elements)
		{
			if (Orientation == EOrientation::Horizontal)
			{
				FSize ChildSize = Element->Measure(ConstrainedSize);
				if (not Element->Width.IsPercent())
					TotalSize.Width += ChildSize.Width;
				else
					TotalSize.Width += Element->MinWidth;
				if (not Element->Height.IsPercent())
					TotalSize.Height = Max(TotalSize.Height, ChildSize.Height);
				else
					TotalSize.Height = Max(TotalSize.Height, Element->MinHeight);
			}
			else
			{
				FSize ChildSize = Element->Measure(ConstrainedSize);
				if (not Element->Width.IsPercent())
					TotalSize.Width = Max(TotalSize.Width, ChildSize.Width);
				else
					TotalSize.Width = Max(TotalSize.Width, Element->MinWidth);

				if (not Element->Height.IsPercent())
					TotalSize.Height += ChildSize.Height;
				else
					TotalSize.Height += Element->MinHeight;
			}
		}
		return TotalSize;
	}

	void UStackPanel::OnArrange()
	{
		FSize RemainSizePercent = FinalSize;
		float32 TotalPercent = 0;

		for (UElementRef Element : Elements)
		{
			if (Orientation == EOrientation::Horizontal)
			{
				if (Element->Width.IsPercent())
					TotalPercent += Element->Width.Value;
				else
					RemainSizePercent.Width -= Element->DesiredSize.Width;
			}
			else
			{
				if (Element->Height.IsPercent())
					TotalPercent += Element->Height.Value;
				else
					RemainSizePercent.Height -= Element->DesiredSize.Height;
			}
		}

		FSize FullSize = FSize::Zero;
		dimenx LayoutLocation = 0dim;

		for (UElementRef Element : Elements)
		{
			FRect ChildRect;
			if (Orientation == EOrientation::Horizontal)
			{
				ChildRect.X = LayoutLocation;
				ChildRect.Y = 0;

				if (Element->Width.IsPercent())
					ChildRect.Width = RemainSizePercent.Width * Element->Width.Value;
				else
					ChildRect.Width = Element->DesiredSize.Width;

				EElementAlignment ChildAlignment = Element->GetValueT(VerticalAlignmentProperty(), ElementAlignment);
				if (Element->Height.IsPercent())
					ChildRect.Height = FinalSize.Height * Element->Height.Value;
				else if (Element->Height.IsAbsolute())
					ChildRect.Height = Element->DesiredSize.Height;
				else if (ChildAlignment == EElementAlignment::Stretch)
					ChildRect.Height = FinalSize.Height;
				else
					ChildRect.Height = Element->DesiredSize.Height;

				switch (ChildAlignment)
				{
				case EElementAlignment::Center:
					ChildRect.Y = (FinalSize.Height - ChildRect.Height) * 0.5f;
					break;
				case EElementAlignment::Far:
					ChildRect.Y = FinalSize.Height - ChildRect.Height;
					break;
				default:
					break;
				}
				LayoutLocation += ChildRect.Width;
			}
			else
			{
				ChildRect.X = 0;
				ChildRect.Y = LayoutLocation;

				if (Element->Height.IsPercent())
					ChildRect.Height = RemainSizePercent.Height * Element->Height.Value;
				else
					ChildRect.Height = Element->DesiredSize.Height;

				EElementAlignment ChildAlignment = Element->GetValueT(HorizontalAlignmentProperty(), ElementAlignment);
				if (Element->Width.IsPercent())
					ChildRect.Width = FinalSize.Width * Element->Width.Value;
				else if (Element->Width.IsAbsolute())
					ChildRect.Width = Element->DesiredSize.Width;
				else if (ChildAlignment == EElementAlignment::Stretch)
					ChildRect.Width = FinalSize.Width;
				else
					ChildRect.Width = Element->DesiredSize.Width;

				switch (ChildAlignment)
				{
				case EElementAlignment::Center:
					ChildRect.X = (FinalSize.Width - ChildRect.Width) * 0.5f;
					break;
				case EElementAlignment::Far:
					ChildRect.X = FinalSize.Width - ChildRect.Width;
					break;
				default:
					break;
				}
				LayoutLocation += ChildRect.Height;
			}
			Element->Arrange({ ChildRect.Position + ScrollOffset, ChildRect.Size });
		}
	}
}
