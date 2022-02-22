#pragma once

#include "Xin.UI.Types.h"
#include "Element.h"

namespace Xin::UI
{
	class UI_API UPresenter : public UElement
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UPresenter();
		UPresenter(FName Name) : UElement(Name) {}
		~UPresenter();

	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnChildMeasured(UElement * Element) override;

	public:
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
	};
	using UPresenterRef = TReferPtr<UPresenter>;
}
