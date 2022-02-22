#include "PCH.h"
#include "GridPanel.h"

namespace Xin::UI
{
	void UGridPanel::OnInitialize()
	{
		CellElements.Resize(GridSize.Width * GridSize.Height);
		for (UElementRef Element : Elements)
		{
			Vec2U GridLocation = Element->GetValueT<Vec2U>(GridLocationProperty(), Vec2U::Zero);
			CellElements[GridLocation.Y * GridSize.Width + GridLocation.X] = Element;
		}
		UPanel::OnInitialize();
	}

	FSize UGridPanel::OnMeasure(FSize ConstrainedSize)
	{
		RowHeights.Reset(GridSize.Height, 0);
		ColWidths.Reset(GridSize.Width, 0);

		for (uintx RowIndex = 0; RowIndex < GridSize.Height; ++RowIndex)
		{
			FGridDimen RowDimen = RowIndex < Rows.Size ? Rows[RowIndex] : FGridDimen { };
			dimenx RowHeight = RowDimen.Size.IsAbsolute() ? RowDimen.Size.Value : ConstrainedSize.Height;

			for (uintx ColIndex = 0; ColIndex < GridSize.Width; ++ColIndex)
			{
				FGridDimen ColDimen = ColIndex < Cols.Size ? Cols[ColIndex] : FGridDimen { };
				dimenx ColWidth = ColDimen.Size.IsAbsolute() ? ColDimen.Size.Value : ConstrainedSize.Width;

				UElementRef & Element = CellElements[RowIndex * GridSize.Width + ColIndex];
				if (Element)
				{
					Element->Measure({ ColWidth , RowHeight });
				}
			}
		}

		// adjust heights
		for (uintx RowIndex = 0; RowIndex < GridSize.Height; ++RowIndex)
		{
			FGridDimen RowDimen = RowIndex < Rows.Size ? Rows[RowIndex] : FGridDimen { };
			if (RowDimen.Size.IsAbsolute())
				RowHeights[RowIndex] = RowDimen.Size.Value;
			else if (RowDimen.Size.IsAuto())
			{
				for (uintx ColIndex = 0; ColIndex < GridSize.Width; ++ColIndex)
				{
					UElementRef & Element = CellElements[RowIndex * GridSize.Width + ColIndex];
					if (Element)
						RowHeights[RowIndex] = Max(RowHeights[RowIndex], Element->DesiredSize.Height);
				}
			}
			else { }
		}

		// adjust width
		for (uintx ColIndex = 0; ColIndex < GridSize.Width; ++ColIndex)
		{
			FGridDimen ColDimen = ColIndex < Cols.Size ? Cols[ColIndex] : FGridDimen { };
			if (ColDimen.Size.IsAbsolute())
				ColWidths[ColIndex] = ColDimen.Size.Value;
			else if (ColDimen.Size.IsAuto())
			{
				for (uintx RowIndex = 0; RowIndex < GridSize.Height; ++RowIndex)
				{
					UElementRef & Element = CellElements[RowIndex * GridSize.Width + ColIndex];
					if (Element)
						ColWidths[ColIndex] = Max(ColWidths[ColIndex], Element->DesiredSize.Width);
				}
			}
			else { }
		}

		FSize TotalSize = FSize::Zero;
		for (uintx ColIndex = 0; ColIndex < GridSize.Width; ++ColIndex)
			TotalSize.Width += ColWidths[ColIndex];
		for (uintx RowIndex = 0; RowIndex < GridSize.Height; ++RowIndex)
			TotalSize.Height += RowHeights[RowIndex];
		return TotalSize;
	}

	void UGridPanel::OnArrange()
	{
		// We calculate row(s)/col(s) with unit percent here.
		FSize FixedSize = FSize::Zero;
		for (uintx ColIndex = 0; ColIndex < GridSize.Width; ++ColIndex)
		{
			if (!Cols[ColIndex].Size.IsPercent())
				FixedSize.Width += ColWidths[ColIndex];
		}
		for (uintx RowIndex = 0; RowIndex < GridSize.Height; ++RowIndex)
		{
			if (!Rows[RowIndex].Size.IsPercent())
				FixedSize.Height += RowHeights[RowIndex];
		}

		// --- Percent
		FSize RemainingSizePercent = Max(FSize::Zero, { FinalSize.Width - FixedSize.Width, FinalSize.Height - FixedSize.Height });
		for (uintx RowIndex = 0; RowIndex < GridSize.Height; ++RowIndex)
		{
			FGridDimen RowDimen = Rows[RowIndex];
			if (RowDimen.Size.IsPercent())
			{
				RowHeights[RowIndex] = RemainingSizePercent.Height * RowDimen.Size.Value;
				FixedSize.Height += RowHeights[RowIndex];
			}
		}
		for (uintx ColIndex = 0; ColIndex < GridSize.Width; ++ColIndex)
		{
			FGridDimen ColDimen = Cols[ColIndex];
			if (ColDimen.Size.IsPercent())
			{
				ColWidths[ColIndex] = RemainingSizePercent.Width * ColDimen.Size.Value;
				FixedSize.Width += ColWidths[ColIndex];
			}
		}

		FPoint Position = FPoint::Zero;
		for (uintx RowIndex = 0; RowIndex < GridSize.Height; ++RowIndex)
		{
			Position.X = 0;
			for (uintx ColIndex = 0; ColIndex < GridSize.Width; ++ColIndex)
			{
				UElementRef & Element = CellElements[RowIndex * GridSize.Width + ColIndex];
				if (Element)
				{
					const FSize & ChildSize = Element->DesiredSize;
					EElementAlignment HorizontalAlignment = Element->GetValueT(HorizontalAlignmentProperty());
					EElementAlignment VerticalAlignment = Element->GetValueT(VerticalAlignmentProperty());

					FRect FinalRect = { };
					switch(HorizontalAlignment)
					{
					case EElementAlignment::Near:
						FinalRect.X = Position.X;
						FinalRect.Width = ChildSize.Width;
						break;
					case EElementAlignment::Center:
						FinalRect.X = Position.X + (ColWidths[ColIndex] - ChildSize.Width) * 0.5;
						FinalRect.Width = ChildSize.Width;
						break;
					case EElementAlignment::Far:
						FinalRect.X = Position.X + ColWidths[ColIndex] - ChildSize.Width;
						FinalRect.Width = ChildSize.Width;
						break;
					default:
					case EElementAlignment::Stretch:
						FinalRect.X = Position.X;
						FinalRect.Width = ColWidths[ColIndex];
						break;
					}

					switch(VerticalAlignment)
					{
					case EElementAlignment::Near:
						FinalRect.Y = Position.Y;
						FinalRect.Height = ChildSize.Height;
						break;
					case EElementAlignment::Center:
						FinalRect.Y = Position.Y + (RowHeights[RowIndex] - ChildSize.Height) * 0.5;
						FinalRect.Height = ChildSize.Height;
						break;
					case EElementAlignment::Far:
						FinalRect.Y = Position.Y + RowHeights[RowIndex] - ChildSize.Height;
						FinalRect.Height = ChildSize.Height;
						break;
					default:
					case EElementAlignment::Stretch:
						FinalRect.Y = Position.Y;
						FinalRect.Height = RowHeights[RowIndex];
						break;
					}

					Element->Arrange(FinalRect);
				}
				Position.X += ColWidths[ColIndex];
			}
			Position.Y += RowHeights[RowIndex];
		}
	}

	void UGridPanel::OnSizeChanged(const FDimen2 & SizeBefore)
	{
		UPanel::OnSizeChanged(SizeBefore);
	}
}
