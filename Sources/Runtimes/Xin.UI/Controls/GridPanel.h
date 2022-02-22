#pragma once

#include "Panel.h"

namespace Xin::UI
{
	struct UI_API FGridDimen
	{
		FGridDimen() = default;
		FGridDimen(const FGridDimen &) = default;
		bool operator==(const FGridDimen &) const = default;

		FGridDimen(FDimen Size) : Size(Size) {}

		FDimen Size = FDimen::Auto;
	};


	class UI_API UGridPanel : public UPanel
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UGridPanel() = default;
		UGridPanel(FName Name) : UPanel(Name) {}
		~UGridPanel() = default;

	public:
		void OnInitialize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		void OnSizeChanged(const FDimen2 & SizeBefore) override;

	public:
		FGridDimen GetRow(uintx RowIndex) const { return RowIndex < Rows.Size ? Rows[RowIndex] : FGridDimen { }; }
		FGridDimen GetCol(uintx ColIndex) const { return ColIndex < Cols.Size ? Cols[ColIndex] : FGridDimen { }; }

	public:
		TList<FGridDimen> Rows;
		TList<FGridDimen> Cols;

	private:
		TList<UElementRef> CellElements;
		TList<dimenx> RowHeights;
		TList<dimenx> ColWidths;

	public:
		SizeU GetGridSize() const { return { (uint32)Cols.Size, (uint32)Rows.Size }; }
		PropertyR(SizeU, GetGridSize) GridSize;
		AttachedPropertyRW(Vec2U, GridLocation) = Vec2U::Zero;
	};
	using UGridPanelRef = TReferPtr<UGridPanel>;
}
