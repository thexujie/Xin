#include "PCH.h"
#include "ScrollBar.h"

namespace Xin::UI
{
	ICommandRef UScrollBar::LineUpCommand = MakeRefer<ICommand>(u8"ScrollBar.LineUp"N);;
	ICommandRef UScrollBar::LineDownCommand = MakeRefer<ICommand>(u8"ScrollBar.LineDown"N);;
	ICommandRef UScrollBar::PageUpCommand = MakeRefer<ICommand>(u8"ScrollBar.PageUp"N);;
	ICommandRef UScrollBar::PageDownCommand = MakeRefer<ICommand>(u8"ScrollBar.PageDown"N);;

	UScrollBar::UScrollBar()
	{
	}

	void UScrollBar::OnConstruct()
	{
		CommandBindings.Add({ LineUpCommand, { this, &UScrollBar::OnLineUpCommand } });
		CommandBindings.Add({ LineDownCommand, { this, &UScrollBar::OnLineDownCommand } });
		CommandBindings.Add({ PageUpCommand, { this, &UScrollBar::OnPageUpCommand } });
		CommandBindings.Add({ PageDownCommand, { this, &UScrollBar::OnPageDownCommand } });
		UControl::OnConstruct();
	}

	void UScrollBar::OnInitialize()
	{
		UControl::OnInitialize();
	}

	FSize UScrollBar::OnMeasure(FSize ConstrainedSize)
	{
		return UControl::OnMeasure(ConstrainedSize);
	}

	void UScrollBar::OnArrange()
	{
		UControl::OnArrange();
	}

	void UScrollBar::OnPaint(IPainter & Painter)
	{
		//Painter.FillRect({ { }, ActualSize }, Colors::Red);
	}

	void UScrollBar::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UControl::OnMouseDown(Args);
	}

	void UScrollBar::OnMouseUp(FMouseButtonEventArgs & Args)
	{
		UControl::OnMouseDown(Args);
	}

	void UScrollBar::OnMouseEnter(FMouseEventArgs & Args)
	{
		UControl::OnMouseEnter(Args);
	}

	void UScrollBar::OnMouseLeave(FMouseEventArgs & Args)
	{
		UControl::OnMouseLeave(Args);
	}

	void UScrollBar::OnLineUpCommand(FRoutedEventArgs & RoutedEventArgs)
	{
		LineUp(RoutedEventArgs);
		RoutedEventArgs.Handled = true;
	}

	void UScrollBar::OnLineDownCommand(FRoutedEventArgs & RoutedEventArgs)
	{
		LineDown(RoutedEventArgs);
		RoutedEventArgs.Handled = true;
	}

	void UScrollBar::OnPageUpCommand(FRoutedEventArgs & RoutedEventArgs)
	{
		PageUp(RoutedEventArgs);
		RoutedEventArgs.Handled = true;
	}

	void UScrollBar::OnPageDownCommand(FRoutedEventArgs & RoutedEventArgs)
	{
		PageDown(RoutedEventArgs);
		RoutedEventArgs.Handled = true;
	}
}
