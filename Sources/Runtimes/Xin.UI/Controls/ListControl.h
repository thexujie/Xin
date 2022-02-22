#pragma once

#include "Xin.UI.Types.h"
#include "Control.h"
#include "ContentControl.h"
#include "Panel.h"

namespace Xin::UI
{
	enum class [[Meta]] ESelectMethod
	{
		Press,
		Release,
		Click,
	};

	enum class [[Meta]] ESelectionMode
	{
		Single = 0,
		Multiple,
		Extended,
	};

	class UI_API UListControl : public UControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		static constexpr FName Name_ItemsPanel = u8"PART_ItemsPanel"N;

	public:
		UListControl();
		UListControl(FName Name);
		~UListControl() = default;

	public:
		void OnTemplated() override;
		void OnInitialize() override;
		void OnInitialized() override;
		void OnFinalize() override;
		void OnPaint(IPainter & Painter) override;

	public:
		UPanelRef ItemsPanel;
	};
	using UListControlRef = TReferPtr<UListControl>;
}
