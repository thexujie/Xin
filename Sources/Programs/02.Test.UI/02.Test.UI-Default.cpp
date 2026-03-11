#include "PCH.h"

#include <iostream>
#include <shlobj.h>
#include <shellapi.h>

#include "02.Test.UI.Types.h"
#include "Xin.Core/Utils/FileInfo.h"

int TestUI_Default(UElementRef Element)
{
	UWindow MainWindow;
	MainWindow.Title = u8"File List"T;
	MainWindow.Size = { 1280, 720 };
	MainWindow.WindowShowPosition = EWindowShowPosition::ScreenCenter;
	MainWindow.ExitOnClose = true;
	MainWindow.Content = Element;
	MainWindow.Show();

	FApplication::Instance().Loop();
	return 0;
}

int TestUI_Default()
{
	UWindow MainWindow;
	MainWindow.Title = u8"File List"T;
	MainWindow.ExitOnClose = true;
	MainWindow.WindowShowPosition = EWindowShowPosition::ScreenCenter;
	MainWindow.Size = { 1280, 720 };

	FString CompolexText =
		//u8"♒强烈❤️❤️❤️推荐♒\n✅运行正常✅\n⚡亭亭玉立⚡\n♈恭喜发财♈\n✨新年快乐✨\n🏅大吉大利🏅\n♒今晚吃鸡♒\n"
		//u8"✋✌️☝️✊✍️⛑️☂️☎♻♿⚠⛔❄️☘️☁️⛅⛈️☂️☔⚡❄️\n"
		//u8"☃️⛄☄️👩🌾🎄🍅🛺🚕🚎🚎🚜💗\n"
		//u8"☕⛷️⛹️⛹️♂️⛹️♀️⚽⚾⛳\n"
		//u8"⛰️⛪⛩⛲⛺⛽⚓⛵⛴️✈️⛱️\n"
		u8"ดีดีดีีีดีดีlālálǎlàوالعدل والسلام靐𰻞𰻞𰻞 في العالم"V;

	auto OnButtonClick = [](const FRoutedEventArgs & Args)
		{
			AssertExpr(Args.Sender.IsA<UButtonControl>());
			UButtonControl & Button = StaticCast<UButtonControl>(Args.Sender);
			static_assert(IsStaticPointerConvertibleV<const UButtonControl &, const IObject &>);
			FConsole::WriteLine(u8"Click Button {}, {}"V, Args.Sender.Name, StaticCast<UButtonControl>(Args.Sender).Content);
		};

	auto Panel = GridPanel(
		{
			Button(u8"可爱的按钮M"T, OnButtonClick, { { UGridPanel::GridLocationProperty(), Vec2U { 0, 0 } } }),
			//Button(u8"可爱的按钮N"T, OnButtonClick(UGridPanel::GridLocationProperty(), Vec2U { 1, 0 })),
		Button(u8"可爱的按钮OOOO"T, OnButtonClick, { { UGridPanel::GridLocationProperty(), Vec2U { 2, 0 } }, { UGridPanel::HorizontalAlignmentProperty(), EElementAlignment::Center } }),

		Button(u8"可爱的按钮P"T, OnButtonClick, { { UGridPanel::GridLocationProperty(), Vec2U { 0, 1 } } }),
		Button(u8"可爱的按钮Q"T, OnButtonClick, { { UGridPanel::GridLocationProperty(), Vec2U { 1, 1 } } }),
		Button(u8"可爱的按钮R"T, OnButtonClick, { { UGridPanel::GridLocationProperty(), Vec2U { 2, 1 } }, { UGridPanel::HorizontalAlignmentProperty(), EElementAlignment::Center } }),
		},
		{
			{ UElement::HorizontalAlignmentProperty(), EElementAlignment::Stretch },
			//{ UElement::SizeProperty(), FDimen2 { FDimen::Percent100, FDimen::Auto } }
		},
		{ FDimen::Auto, FDimen::Auto },
		{ FDimen::Percent100, FDimen::Auto, FDimen::Auto }
		);
	MainWindow.Content = StackPanel(EOrientation::Vertical, { Panel });
	//MainWindow.Content = Panel;

	MainWindow.Show();
	FApplication::Instance().Loop();
	return 0;
}
