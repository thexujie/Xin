#include "PCH.h"
#include "WrapPanel.h"

namespace Xin::UI
{
	void UWrapPanel::OnInitialize()
	{
		UPanel::OnInitialize();
	}

	FSize UWrapPanel::OnMeasure(FSize ConstrainedSize)
	{
		FSize TotalSize = FSize::Zero;
		dimenx LineIndex = 0;
		FSize LineSize = FSize::Zero;

		for (UElementRef Element : Elements)
		{
			FSize ChildDesiredSize = Element->Measure(ConstrainedSize);
			if (Orientation == EOrientation::Horizontal)
			{
				if (LineSize.Width == 0)
					LineSize = ChildDesiredSize;
				else if (LineSize.Width + ChildDesiredSize.Width > ConstrainedSize.Width)
				{
					TotalSize.Width = Max(TotalSize.Width, LineSize.Width);
					TotalSize.Height += LineSize.Height;
					++LineIndex;
					LineSize = ChildDesiredSize;
				}
				else
				{
					LineSize.Width += ChildDesiredSize.Width;
					LineSize.Height = Max(LineSize.Height, ChildDesiredSize.Height);
				}
			}
			else
			{
				if (LineSize.Height == 0)
					LineSize = ChildDesiredSize;
				else if (LineSize.Height + ChildDesiredSize.Height > ConstrainedSize.Height)
				{
					TotalSize.Width += LineSize.Width;
					TotalSize.Height = Max(TotalSize.Height, LineSize.Height);
					++LineIndex;
					LineSize = ChildDesiredSize;
				}
				else
				{
					LineSize.Width = Max(LineSize.Width, ChildDesiredSize.Width);
					LineSize.Height += ChildDesiredSize.Height;
				}
			}
		}

		if (Orientation == EOrientation::Horizontal)
		{
			TotalSize.Width = Max(TotalSize.Width, LineSize.Width);
			TotalSize.Height += LineSize.Height;
		}
		else
		{
			TotalSize.Width += LineSize.Width;
			TotalSize.Height = Max(TotalSize.Height, LineSize.Height);
		}
		return TotalSize;
	}

	void UWrapPanel::OnArrange()
	{
		FSize TotalSize = FSize::Zero;
		dimenx LineIndex = 0;
		FSize LineSize = FSize::Zero;

		for (UElementRef Element : Elements)
		{
			FSize ChildDesiredSize = Element->DesiredSize;
			FPoint ChildArrangePosition;

			if (Orientation == EOrientation::Horizontal)
			{
				if (LineSize.Width == 0)
				{
					ChildArrangePosition = {0dim, TotalSize.Height };
					LineSize = ChildDesiredSize;
				}
				else if (LineSize.Width + ChildDesiredSize.Width > FinalSize.Width)
				{
					TotalSize.Width = Max(TotalSize.Width, LineSize.Width);
					TotalSize.Height += LineSize.Height;
					++LineIndex;
					LineSize = ChildDesiredSize;

					ChildArrangePosition = { 0dim, TotalSize.Height };
				}
				else
				{
					ChildArrangePosition = { LineSize.Width, TotalSize.Height };

					LineSize.Width += ChildDesiredSize.Width;
					LineSize.Height = Max(LineSize.Height, ChildDesiredSize.Height);
				}
			}
			else
			{
				if (LineSize.Height == 0)
				{
					ChildArrangePosition = { TotalSize.Width, 0dim };
					LineSize = ChildDesiredSize;
				}
				else if (LineSize.Height + ChildDesiredSize.Height > FinalSize.Height)
				{
					TotalSize.Width += LineSize.Width;
					TotalSize.Height = Max(TotalSize.Height, LineSize.Height);
					++LineIndex;
					LineSize = ChildDesiredSize;

					ChildArrangePosition = { TotalSize.Width, 0dim };
				}
				else
				{
					ChildArrangePosition = { TotalSize.Width, LineSize.Height };

					LineSize.Width = Max(LineSize.Width, ChildDesiredSize.Width);
					LineSize.Height += ChildDesiredSize.Height;
				}
			}
			Element->Arrange({ ChildArrangePosition , ChildDesiredSize });
		}
	}
}
