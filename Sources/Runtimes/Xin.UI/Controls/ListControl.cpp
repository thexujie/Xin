#include "PCH.h"
#include "ListControl.h"

#include "ContentControl.h"
#include "StackPanel.h"

namespace Xin::UI
{
	UListControl::UListControl() 
	{
		
	}

	UListControl::UListControl(FName Name) : UControl(Name)
	{

	}

	void UListControl::OnTemplated()
	{
		UControl::OnTemplated();
	}

	void UListControl::OnInitialize()
	{
		UControl::OnInitialize();

		if (UElementRef Element = TemplateElement)
		{
			ItemsPanel = Element->FindChildT<UPanel>(Name_ItemsPanel);
			AssertExpr(ItemsPanel);
		}
	}

	void UListControl::OnInitialized()
	{
		UControl::OnInitialized();
	}

	void UListControl::OnFinalize()
	{
		UControl::OnFinalize();
	}

	void UListControl::OnPaint(IPainter & Painter)
	{
		UControl::OnPaint(Painter);
	}
}
