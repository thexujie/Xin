#pragma once

#include "Xin.UI.Types.h"

#include "Border.h"

#include "Shapes/Shape.h"
#include "Shapes/Line.h"
#include "Shapes/Rectangle.h"
#include "Shapes/Ellipse.h"
#include "Shapes/Polyline.h"
#include "Shapes/Path.h"

#include "Separator.h"
#include "Image.h"
#include "TextBlock.h"
#include "Border.h"

#include "ContentControl.h"
#include "Button.h"
#include "RadioButton.h"
#include "CheckBox.h"

#include "ButtonControl.h"
#include "Thumb.h"
#include "RepeatButton.h"
#include "ToggleButton.h"
#include "Track.h"

#include "ScrollBar.h"
#include "StackPanel.h"
#include "WrapPanel.h"
#include "OverlapPanel.h"
#include "GridPanel.h"
#include "ScrollArea.h"
#include "ScrollPanel.h"
#include "ScrollViewer.h"

#include "ListControl.h"
#include "ListBox.h"
#include "ListView.h"
#include "Menu.h"

#include "Chrome.h"
#include "ChromePanel.h"

#include "Window.h"
#include "ToolTip.h"
#include "ContextMenu.h"

namespace Xin::UI
{
	inline USeparatorRef Separator(FDimen2 Size = { FDimen::Auto, FDimen::Auto })
	{
		USeparatorRef Separator = MakeRefer<USeparator>();
		Separator->Size = Size;
		return Separator;
	}

	inline UButtonRef Button(FVariant Content, FRoutedEventHandler ClickHandler = nullptr, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UButtonRef Button = MakeRefer<UButton>();
		Button->Content = Content;
		for (auto & Pair : Properties)
			Button->SetPropertyValue(Pair.First, Pair.Second);

		if (ClickHandler)
			Button->Click += ClickHandler;

		return Button;
	}

	inline URadioButtonRef RadioButton(FVariant Content, FRoutedEventHandler ClickHandler = nullptr, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		URadioButtonRef RadioButton = MakeRefer<URadioButton>();
		RadioButton->Content = Content;
		for (auto & Pair : Properties)
			RadioButton->SetPropertyValue(Pair.First, Pair.Second);
		if (ClickHandler)
			RadioButton->Click += ClickHandler;
		return RadioButton;
	}

	inline UCheckBoxRef CheckBox(FVariant Content, FRoutedEventHandler ClickHandler = nullptr, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UCheckBoxRef CheckBox = MakeRefer<UCheckBox>();
		CheckBox->Content = Content;
		for (auto & Pair : Properties)
			CheckBox->SetPropertyValue(Pair.First, Pair.Second);
		if (ClickHandler)
			CheckBox->Click += ClickHandler;
		return CheckBox;
	}

	inline UHLineRef LineH(FDimen LineHeight)
	{
		UHLineRef HLine = MakeRefer<UHLine>();
		HLine->Height = LineHeight;
		return HLine;
	}

	inline UVLineRef LineV(FDimen LineWidth)
	{
		UVLineRef VLine = MakeRefer<UVLine>();
		VLine->Width = LineWidth;
		return VLine;
	}

	inline UPolylineRef Polyline(TView<FPoint> Points, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UPolylineRef Polyline = MakeRefer<UPolyline>();
		Polyline->Points = Points;
		for (auto & Pair : Properties)
			Polyline->SetPropertyValue(Pair.First, Pair.Second);
		return Polyline;
	}

	inline UPathRef Path(FStringV PathCommand, const FTransform3x2F & PathTransform, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UPathRef Path = MakeRefer<UPath>();
		Path->Command = PathCommand;
		Path->PathTransform = PathTransform;
		for (auto & Pair : Properties)
			Path->SetPropertyValue(Pair.First, Pair.Second);
		return Path;
	}

	inline UStackPanelRef StackPanel(EOrientation Orientation, TView<UElementRef> Elements, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UStackPanelRef StackPanel = MakeRefer<UStackPanel>();
		StackPanel->Orientation = Orientation;

		StackPanel->Elements = Elements;
		for (auto & Pair : Properties)
			StackPanel->SetPropertyValue(Pair.First, Pair.Second);
		return StackPanel;
	}

	inline UWrapPanelRef WrapPanel(TView<UElementRef> Elements, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		TReferPtr<UWrapPanel> WrapPanel = MakeRefer<UWrapPanel>();
		WrapPanel->Elements = Elements;
		for (auto & Pair : Properties)
			WrapPanel->SetPropertyValue(Pair.First, Pair.Second);
		return WrapPanel;
	}

	inline UListBoxRef ListBox(TView<FVariant> Items, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UListBoxRef ListBox = MakeRefer<UListBox>();

		TList<UListBoxItemRef> ListBoxItems;
		for (const FVariant & Item : Items)
		{
			if (Item.IsA<UListBoxItem>())
				ListBoxItems.Add(Item.Get<UListBoxItemRef>());
			else
			{
				UListBoxItemRef ListBoxItem = MakeRefer<UListBoxItem>();
				ListBoxItem->Content = Item;
				ListBoxItems.Add(ListBoxItem);
			}
		}

		ListBox->Items = ListBoxItems;

		for (auto & Pair : Properties)
			ListBox->SetPropertyValue(Pair.First, Pair.Second);
		return ListBox;
	}

	inline UListViewRef ListView(TView<FVariant> Items, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UListViewRef ListView = MakeRefer<UListView>();
		ListView->Items = Items;

		for (auto & Pair : Properties)
			ListView->SetPropertyValue(Pair.First, Pair.Second);
		return ListView;
	}

	inline UScrollPanelRef ScrollPanel(UElementRef Child, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UScrollPanelRef ScrollPanel = MakeRefer<UScrollPanel>();
		ScrollPanel->Child = Child;
		for (auto & Pair : Properties)
			ScrollPanel->SetPropertyValue(Pair.First, Pair.Second);
		return ScrollPanel;
	}

	inline UScrollViewerRef ScrollViewer(UElementRef Child, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UScrollViewerRef ScrollViewer = MakeRefer<UScrollViewer>();
		ScrollViewer->Child = Child;
		for (auto & Pair : Properties)
			ScrollViewer->SetPropertyValue(Pair.First, Pair.Second);
		return ScrollViewer;
	}

	inline UPanelRef GridPanel(TView<UElementRef> Elements, TView<TPair<const FProperty &, FVariant>> Properties = { }, TView<FGridDimen> Rows = { }, TView<FGridDimen> Cols = { })
	{
		TReferPtr<UGridPanel> Element = MakeRefer<UGridPanel>();
		Element->Rows = Rows;
		Element->Cols = Cols;
		Element->Elements = Elements;
		for (auto & Pair : Properties)
			Element->SetPropertyValue(Pair.First, Pair.Second);
		return Element;
	}

	inline UTextBlockRef TextBlock(FText Text, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		TReferPtr<UTextBlock> Element = MakeRefer<UTextBlock>(Text);
		for (auto & Pair : Properties)
			Element->SetPropertyValue(Pair.First, Pair.Second);
		return Element;
	}

	inline UImageRef Image(FStringV ImageSource, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		TReferPtr<UImage> Image = MakeRefer<UImage>();
		Image->ImageSource = ImageSource;
		for (auto & Pair : Properties)
			Image->SetPropertyValue(Pair.First, Pair.Second);
		return Image;
	}
}
