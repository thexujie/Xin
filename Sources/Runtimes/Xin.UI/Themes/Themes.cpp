#include "PCH.h"
#include "Themes.h"

namespace Xin::UI::Themes
{
	UElementRef FDefaultInt64DataTemplate::Instantiate(UControl & Control, const FVariant & Variant)
	{
		FStringV Text = Format(u8"{}"V, Variant.Get<int64>());
		UTextBlockRef TextBlock = MakeRefer<UTextBlock>(Text);
		TextBlock->Color = Control.Foreground;
		TextBlock->FontFamily = Binding(Control, UControl::FontFamilyProperty());
		TextBlock->FontSize = Binding(Control, UControl::FontSizeProperty());
		TextBlock->FontWeight = Binding(Control, UControl::FontWeightProperty());
		TextBlock->FontStyle = Binding(Control, UControl::FontStyleProperty());
		TextBlock->HorizontalAlignment = Binding(&Control, UControl::ContentHorizontalAlignmentProperty());
		TextBlock->VerticalAlignment = Binding(&Control, UControl::ContentVerticalAlignmentProperty());
		return TextBlock;
	}

	UElementRef FDefaultUInt64DataTemplate::Instantiate(UControl & Control, const FVariant & Variant)
	{
		FStringV Text = Format(u8"{}"V, Variant.Get<uint64>());
		UTextBlockRef TextBlock = MakeRefer<UTextBlock>(Text);
		TextBlock->Color = Control.Foreground;
		TextBlock->FontFamily = Binding(Control, UControl::FontFamilyProperty());
		TextBlock->FontSize = Binding(Control, UControl::FontSizeProperty());
		TextBlock->FontWeight = Binding(Control, UControl::FontWeightProperty());
		TextBlock->FontStyle = Binding(Control, UControl::FontStyleProperty());
		TextBlock->HorizontalAlignment = Binding(&Control, UControl::ContentHorizontalAlignmentProperty());
		TextBlock->VerticalAlignment = Binding(&Control, UControl::ContentVerticalAlignmentProperty());
		return TextBlock;
	}

	UElementRef FDefaultUInt32DataTemplate::Instantiate(UControl & Control, const FVariant & Variant)
	{
		FStringV Text = Format(u8"{}"V, Variant.Get<uint32>());
		UTextBlockRef TextBlock = MakeRefer<UTextBlock>(Text);
		TextBlock->Color = Control.Foreground;
		TextBlock->FontFamily = Binding(Control, UControl::FontFamilyProperty());
		TextBlock->FontSize = Binding(Control, UControl::FontSizeProperty());
		TextBlock->FontWeight = Binding(Control, UControl::FontWeightProperty());
		TextBlock->FontStyle = Binding(Control, UControl::FontStyleProperty());
		TextBlock->HorizontalAlignment = Binding(&Control, UControl::ContentHorizontalAlignmentProperty());
		TextBlock->VerticalAlignment = Binding(&Control, UControl::ContentVerticalAlignmentProperty());
		return TextBlock;
	}

	UElementRef FDefaultStringDataTemplate::Instantiate(UControl & Control, const FVariant & Variant) 
	{
		UTextBlockRef TextBlock = MakeRefer<UTextBlock>(Variant.Get<FStringV>());
		TextBlock->Color = Control.Foreground;
		TextBlock->FontFamily = Binding(Control, UControl::FontFamilyProperty());
		TextBlock->FontSize = Binding(Control, UControl::FontSizeProperty());
		TextBlock->FontWeight = Binding(Control, UControl::FontWeightProperty());
		TextBlock->FontStyle = Binding(Control, UControl::FontStyleProperty());
		TextBlock->HorizontalAlignment = Binding(&Control, UControl::ContentHorizontalAlignmentProperty());
		TextBlock->VerticalAlignment = Binding(&Control, UControl::ContentVerticalAlignmentProperty());
		return TextBlock;
	}

	UElementRef FDefaultTextDataTemplate::Instantiate(UControl & Control, const FVariant & Variant)
	{
		UTextBlockRef TextBlock = MakeRefer<UTextBlock>(Variant.Get<FText>());
		TextBlock->Color = Control.Foreground;
		TextBlock->FontFamily = Binding(Control, UControl::FontFamilyProperty());
		TextBlock->FontSize = Binding(Control, UControl::FontSizeProperty());
		TextBlock->FontWeight = Binding(Control, UControl::FontWeightProperty());
		TextBlock->FontStyle = Binding(Control, UControl::FontStyleProperty());
		TextBlock->HorizontalAlignment = Binding(&Control, UControl::ContentHorizontalAlignmentProperty());
		TextBlock->VerticalAlignment = Binding(&Control, UControl::ContentVerticalAlignmentProperty());
		return TextBlock;
	}

	UElementRef FDefaultPixmapTemplate::Instantiate(UControl & Control, const FVariant & Variant)
	{
		UImageRef Image = MakeRefer<UImage>();
		Image->Pixmap = Variant.Get<IPixmapRef>();
		Image->HorizontalAlignment = Binding(&Control, UControl::ContentHorizontalAlignmentProperty());
		Image->VerticalAlignment = Binding(&Control, UControl::ContentVerticalAlignmentProperty());
		return Image;
	}

	UElementRef FDefaultBitmapTemplate::Instantiate(UControl & Control, const FVariant & Variant)
	{
		UImageRef Image = MakeRefer<UImage>();
		Image->Bitmap = Variant.Get<IBitmapRef>();
		Image->HorizontalAlignment = Binding(&Control, UControl::ContentHorizontalAlignmentProperty());
		Image->VerticalAlignment = Binding(&Control, UControl::ContentVerticalAlignmentProperty());
		return Image;
	}

	UElementRef FDefaultWindowTemplate::LoadTemplateT(UWindow & Window)
	{
		UBorderRef Border = MakeRefer<UBorder>();
		{
			Border->Name = UControl::PART_ContentPresenter;
			Border->Fill = Binding(&Window, UControl::BackgroundProperty());
		}
		return Border;
	}
}