#include "PCH.h"
#include "ContentControl.h"

namespace Xin::UI
{
	UContentControl::UContentControl()
	{
	}

	UContentControl::~UContentControl()
	{
	}

	void UContentControl::OnInitialize()
	{
		UControl::OnInitialize();

		if (UElementRef Element = TemplateElement)
			ContentPresenter = Element->FindChildT<UPresenter>(PART_ContentPresenter);

		PresentContent();
	}

	void UContentControl::OnInitialized()
	{
		UControl::OnInitialized();
	}

	void UContentControl::OnFinalize()
	{
		UnpresentContent();

		UControl::OnFinalize();
	}

	FSize UContentControl::OnMeasure(FSize ConstrainedSize)
	{
		return UControl::OnMeasure(ConstrainedSize);
	}

	void UContentControl::OnShown()
	{
		UControl::OnShown();
	}

	void UContentControl::OnChildMeasured(UElement * Element)
	{
		UControl::OnChildMeasured(Element);
	}

	void UContentControl::OnTemplated()
	{
		UControl::OnTemplated();

		ContentElement = nullptr;
		if (UElementRef Element = TemplateElement)
			ContentPresenter = Element->FindChildT<UPresenter>(PART_ContentPresenter);
	}

	void UContentControl::PresentContent()
	{
		if (ContentPresented || !ContentPresenter)
			return;

		if (!ContentPresenter || !__Content)
		{
			ContentPresented = true;
			return;
		}

		if (ContentElement)
		{
			//RemoveLogicalChild(ContentElement);
			ContentPresenter->Child = nullptr;
			ContentElement = nullptr;
		}

		ContentElement = LoadContent();

		if (ContentElement)
		{
			//AddLogicalChild(ContentElement);
			ContentPresenter->Child = ContentElement;
		}

		ContentPresented = true;
	}

	void UContentControl::UnpresentContent()
	{
		if (!ContentPresented)
			return;

		AssertExpr(ContentPresenter != nullptr);

		if (ContentPresenter)
		{
			ContentPresenter->Child = nullptr;
			ContentPresenter = nullptr;
		}

		ContentElement = nullptr;
		ContentPresented = false;
	}

	UElementRef UContentControl::LoadContent()
	{
		FVariant & Variant = __Content;

		if (not Variant.Valid())
			return nullptr;

		if (Variant.IsA<UElement>())
		{
			return Variant.Get<UElementRef>();
		}
		else
		{
			const FType & ContentType = Variant.Type();
			if (ContentTemplate)
				return ContentTemplate->Instantiate(*this, Variant);
			else if (IDataTemplateRef DataTemplate = FindResourceT<IDataTemplate>(ContentType))
				return DataTemplate->Instantiate(*this, Variant);
			else
			{
				IDataTemplateRef StringDataTemplate = FindResourceT<IDataTemplate>(Typeof<FString>());
				AssertExpr(StringDataTemplate);
				FString ContentInfo = Format(u8"{} {}"V, ContentType.Name, Variant);
				return StringDataTemplate->Instantiate(*this, ContentInfo);
			}
		}
	}

	void UContentControl::OnContentChanged(FVariant ContentBefore)
	{
		ContentPresented = false;
		PresentContent();
	}
}
