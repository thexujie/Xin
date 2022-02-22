#include "PCH.h"
#include "ScrollViewer.h"

namespace Xin::UI
{
	FSize UScrollViewer::OnMeasure(FSize ConstrainedSize)
	{
		return UPresenterControl::OnMeasure(ConstrainedSize);
	}

	void UScrollViewer::OnArrange()
	{
		UPresenterControl::OnArrange();
	}
}
