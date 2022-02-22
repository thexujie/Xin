#pragma once

#include "../Themes.h"

namespace Xin::UI::Themes::Default
{
	struct UI_API FDefaultTheme
	{
		struct
		{
			FColor NormalColor { 0xFF0066B4 };
			FColor HoverColor { 0xFF2286D4 };
			FColor PressedColor { 0xFFDDDDDD };
			FColor DisableColor { 0xFFAAAAAA };
			FColor SelectedColor { 0xFF66A0FF };
		}Primary;

		struct
		{
			FColor NormalColor { 0xFFFDFDFD };
			FColor HoverColor { 0xFFEEEEEE };
			FColor PressedColor { 0xFFDDDDDD };
			FColor DisableColor { 0xFFAAAAAA };
		}Plane;

		struct
		{
			FColor NormalColor { 0xFFFDFDFD };
			FColor HoverColor { 0xFFB0E0FF };
			FColor PressedColor { 0xFFDDDDDD };
			FColor SelectedColor { 0xFF66A0FF };
			FColor DisableColor { 0xFFAAAAAA };
		}Item;

		struct
		{
			FColor NormalColor { 0xFFFEFEFE };
			FColor HoverColor { 0xFFDDDDDD };
			FColor PressedColor { 0xFFCCCCCC };
			FColor DisableColor { 0xFFAAAAAA };
		}Button;

		struct
		{
			FColor NormalColor { 0xFF000000 };
			FColor PressedColor { 0xFF606060 };
			FColor DisableColor { 0xFFA0A0A0 };
		}Text;

		struct
		{
			FColor NormalColor { 0xFFC2C3C3 };
			FColor HoverColor { 0xFF686868 };
			FColor PressedColor { 0xFF5B5B5B };
			FColor DisableColor { 0xFFA0A0A0 };
		}Bar;

		struct
		{
			FColor NormalColor { 0xFFB0B0B0};
		}Shape;

		struct
		{
			FColor NormalColor { 0xFF606060 };
			FColor HoverColor { 0xFF404040 };
			FColor PressedColor { 0xFFA0A0A0 };
			FColor DisableColor { 0xFF808080 };
		}Title;

		struct
		{
			FColor BackgroundColor { 0xFFFAFAFA };
		}Menu;

		struct
		{
			FColor ActiveColor { 0xFF0066B4 };
			FColor InactiveColor { 0xFF808080 };
		}Frame;

		FColor BorderColor { 0xFF808080 };
	};

	extern UI_API FDefaultTheme GDefaultTheme;


	class UI_API FScrollViewerTemplate : public TControlTemplate<UScrollViewer>
	{
	public:
		FScrollViewerTemplate() = default;
		UElementRef LoadTemplateT(UScrollViewer & ScrollViewer) override;
	};

	class UI_API FScrollViewerStyle : public TControlStyle<UScrollViewer>
	{
	public:
		FScrollViewerStyle() = default;
		void LoadStyleT(UScrollViewer & ScrollViewer) override;
	};

	class UI_API FListControlTemplate : public TControlTemplate<UListControl>
	{
	public:
		FListControlTemplate() = default;
		UElementRef LoadTemplateT(UListControl & ListControl) override;
	};

	class UI_API FListControlStyle : public TControlStyle<UListControl>
	{
	public:
		FListControlStyle() = default;
		void LoadStyleT(UListControl & ListControl) override;
	};

	class UI_API FTooltipTemplate : public TControlTemplate<UToolTip>
	{
	public:
		FTooltipTemplate() = default;
		UElementRef LoadTemplateT(UToolTip & Window) override
		{
			if (Window.WindowStyle == EWindowStyle::Frameless)
			{
				Window.BorderThickness = 2;
				Window.BorderStroke = Brushs::Brown;

				UBorderRef Border = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
				//Border->Margin = 30;
				Border->Padding = { 3.0, 3.0 };
				Border->Thickness = Binding(&Window, UControl::BorderThicknessProperty());
				Border->Stroke = Binding(&Window, UControl::BorderStrokeProperty());
				return Border;
			}
			return nullptr;
		}
	};
}

