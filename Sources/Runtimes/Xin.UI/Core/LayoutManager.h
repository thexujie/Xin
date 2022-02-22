#pragma once

#include "Xin.UI.Types.h"

namespace Xin::UI
{
	class UI_API FLayoutManager : public IObject
	{
	public:
		FLayoutManager() = default;

		void InvalidateMeasure(UElement * Element);
		void InvalidateArrange(UElement * Element);
		void OnDispatch();

	public:
		TList<UElementObj> PenddingMeasureElements;
		TList<UElementObj> PenddingArrangeElements;

	private:
		bool Dispatching = false;


	public:
		TEvent<void(FEventArgs &)> Arranged;
	};
	using FLayoutManagerRef = TReferPtr<FLayoutManager>;
}
