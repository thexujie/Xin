#pragma once

#include "Xin.UI.Types.h"
#include "Presenter.h"
#include "Core/ScrollInfo.h"

namespace Xin::UI
{
	class UI_API UScrollAreaInfo : public IScrollInfo
	{
	public:
	};

	class UI_API UScrollArea : public UPresenter
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UScrollArea() = default;
		UScrollArea(FName Name) : UPresenter(Name) {}
		~UScrollArea() = default;

	public:
		void OnConstruct() override;
		void OnInitialize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;
		void OnArranged() override;

	public:
		void OnMouseWhell(FMouseWhellEventArgs & Args) override;
		void OnScrollChanged(FEventArgs & EventArgs);

	public:
		DependencyPropertyRW(FSize, ViewportSize) = FSize::Zero;
		DependencyPropertyRW(FVec2, ScrollExtent) = FVec2::Zero;

		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(FVec2, ScrollOffset) = FVec2::Zero;

		TReferPtr<UScrollAreaInfo> ScrollInfo = MakeRefer<UScrollAreaInfo>();

	public:
		TEvent<void(FEventArgs & Args)> ScrollChanged;
	};
	using UScrollAreaRef = TReferPtr<UScrollArea>;
}
