#include "PCH.h"
#include "Control.h"

namespace Xin::UI
{
	UControl::UControl() 
	{
		
	}

	void UControl::OnInitialize()
	{
		UElement::OnInitialize();
		Template = FindResourceT<IControlTemplate>(Type());
	}

	void UControl::OnFinalize()
	{
		Template = nullptr;
		UElement::OnFinalize();
	}

	FSize UControl::OnMeasure(FSize ConstrainedSize)
	{
		if (UElementRef Element = TemplateElement)
			return Element->Measure(ConstrainedSize);
		return FSize::Zero;
	}

	void UControl::OnArrange()
	{
		if (UElementRef Element = TemplateElement)
			Element->Arrange({ FPoint::Zero, FinalSize });
	}

	void UControl::OnResourcesChanged()
	{
		Template = FindResourceT<IControlTemplate>(Type());
	}

	void UControl::OnTemplateChanged(IControlTemplateRef TemplateBefore)
	{
		if (__TemplateElement)
		{
			OnRemoveChild(__TemplateElement);
			__TemplateElement = nullptr;
		}

		if (__Template)
			TemplateElement = Template->LoadTemplate(*this);

		if (__TemplateElement)
		{
			OnAddChild(__TemplateElement);
		}

		OnTemplated();
	}
}
