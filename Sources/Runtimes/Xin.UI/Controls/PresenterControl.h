#pragma once

#include "Xin.UI.Types.h"
#include "Control.h"
#include "Presenter.h"

namespace Xin::UI
{
	class UI_API UPresenterControl : public UControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UPresenterControl();
		UPresenterControl(FName Name) : UControl(Name) {}
		~UPresenterControl() = default;

	public:
		void OnChildMeasured(UElement * Element) override;

	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnInitialized() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		void PreKeyDown(FKeyboardEventArgs & Args) override;
		void PreKeyUp(FKeyboardEventArgs & Args) override;

	public:
		virtual void OnChildChanged(UElementRef ChildBefore);
		ProxyPropertyRW(UElementRef, Child) = nullptr;

		[[Meta(AffectsMeasure, AffectsRender)]]
		DependencyPropertyRW(FThickness, Padding) = 0.0;

	public:
		UPresenterRef ContentPresenter;
	};
	using UPresenterControlRef = TReferPtr<UPresenterControl>;
}
