#include "PCH.h"
#include "LayoutManager.h"
#include "Controls/Element.h"

#define LAYOUT_DEBUG 1

namespace Xin::UI
{
	void FLayoutManager::InvalidateMeasure(UElement * Element)
	{
		//PenddingMeasureElements.AddUnique(Element);
		PenddingMeasureElements.AddUnique(Element);
#if LAYOUT_DEBUG
		OnDispatch();
#else
		if (!Dispatching)
		{
			Dispatching = true;
			FDispatcher::Current().BeginInvoke([this] { OnDispatch(); });
		}
#endif
	}

	void FLayoutManager::InvalidateArrange(UElement * Element)
	{
		PenddingArrangeElements.AddUnique(Element);
#if LAYOUT_DEBUG
		OnDispatch();
#else
		if (!Dispatching)
		{
			Dispatching = true;
			FDispatcher::Current().BeginInvoke([this] { OnDispatch(); });
		}
#endif
	}

	void FLayoutManager::OnDispatch()
	{
		Dispatching = false;

		if (not PenddingMeasureElements.Size && not PenddingArrangeElements.Size)
			return;

		uintx LayoutDepth = 0;
		uintx NumLayoutElements = 0;
		while (PenddingMeasureElements.Size || PenddingArrangeElements.Size)
		{
			++LayoutDepth;
			AssertExpr(LayoutDepth <= 512);

			NumLayoutElements += PenddingMeasureElements.Size;
			NumLayoutElements += PenddingArrangeElements.Size;

			TList<UElementObj> ArrangeElements = Move(PenddingArrangeElements);
			TList<UElementObj> MeasureElements = Move(PenddingMeasureElements);

			//while (PenddingArrangeElements.Size)
			{
				for (UElementObj ElementObj : ArrangeElements)
				{
					if (UElementRef Element = ElementObj.Lock(); Element/* && !Element->ArrangeDone*/)
					{
						if (UElementRef ParentElement = Element->Parent.Lock())
						{
							if (!Element->MeasureDone)
							{
								AssertExpr(!Element->AvailableSize.AnyNaN());
								Element->Measure(Element->AvailableSize);
							}

							if (!Element->ArrangeDone)
							{
								AssertExpr(!Element->ArrangeRect.AnyNaN());
								Element->Arrange(Element->ArrangeRect);
							}
						}
					}
				}
			}

			//while (PenddingMeasureElements.Size)
			{
				for (UElementObj ElementObj : MeasureElements)
				{
					if (UElementRef Element = ElementObj.Lock(); Element /*&& !Element->MeasureDone*/)
					{
						if (!Element->MeasureDone)
						{
							AssertExpr(!Element->AvailableSize.AnyNaN());
							Element->Measure(Element->AvailableSize);
						}
					}
				}
			}
		}

		//FConsole::WriteLine(u8"Layout finished on {} depth, {} elements."V, LayoutDepth, LayoutElements);

		FEventArgs Args;
		Arranged(Args);

		//if (PenddingMeasureElements.Size || PenddingArrangeElements.Size)
		//{
		//	Dispatching = true;
		//	FDispatcher::MainThread().BeginInvoke([this] { OnDispatch(); });
		//}
		//else
		//{
		//	FEventArgs Args;
		//	Arranged(Args);
		//}
	}
}
