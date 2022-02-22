#pragma once

#include "Xin.UI.Types.h"
#include "Control.h"
#include "Presenter.h"

namespace Xin::UI
{
	class UI_API UContentControl : public UControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UContentControl();
		UContentControl(FName Name) : UControl(Name) {}
		~UContentControl();
		FStringV Describe() const override { return Format(u8"{}"V, Content); }

	public:
		void OnInitialize() override;
		void OnInitialized() override;
		void OnFinalize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnShown() override;

		void OnChildMeasured(UElement * Element) override;

	public:
		void OnTemplated() override;

	protected:
		void PresentContent();
		void UnpresentContent();
		virtual UElementRef LoadContent();

	public:
		bool ContentPresented = false;
		UPresenterRef ContentPresenter;
		UElementRef ContentElement;

	public:
		virtual void OnContentChanged(FVariant ContentBefore);
		DependencyPropertyRW(FVariant, Content) = FVariant::None;

		ProxyPropertyRW(IDataTemplateRef, ContentTemplate);
	};
	using UContentControlRef = TReferPtr<UContentControl>;
}
