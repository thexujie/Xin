#pragma once

#include "Xin.UI.Types.h"
#include "PresenterControl.h"

namespace Xin::UI
{
	class UI_API UScrollViewer : public UPresenterControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UScrollViewer() = default;
		UScrollViewer(FName Name): UPresenterControl(Name) {}
		~UScrollViewer() = default;

	public:
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;
	};
	using UScrollViewerRef = TReferPtr<UScrollViewer>;
}
