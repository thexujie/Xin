#include "PCH.h"

#include <iostream>
#include <shlobj.h>
#include <shellapi.h>

#include "00.Test.UI.Types.h"
#include "Element-Paths.h"
#include "Xin.Core/Utils/FileInfo.h"

UElementRef CreateElement_Atlas()
{
	UWindowRef PopupWindow = MakeRefer<UWindow>();
	PopupWindow->Title = u8"Popup Window"T;
	PopupWindow->WindowStyle = EWindowStyle::Normal;

	PopupWindow->BorderStroke = Brushs::IndianRed;

	PopupWindow->Size = { 1280, 720 };
	PopupWindow->FrameThickness = 5;
	PopupWindow->BorderThickness = 2;
	PopupWindow->HitInside = EHitResult::Caption;
	PopupWindow->Background = Brushs::WhiteSmoke;

	PopupWindow->ToolTip = u8"测试一下"T;
	PopupWindow->SetPropertyValue(FToolTip::PlacementProperty(), EPopupPosition::Cursor);

	PopupWindow->MouseDown += [&](const FMouseButtonEventArgs & Args) { /*PopupWindow->Close();*/ };
	//PopupWindow->Content = u8"这是一个弹出窗口"T;

	UButtonRef PopupCloseButton;
	PopupWindow->Content = StackPanel(EOrientation::Vertical,
		{
			TextBlock(u8"可爱的文字 in TextBlock AA"T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { FDimen::Percent90, FDimen::Auto } } }),
			TextBlock(u8"可爱的文字 in TextBlock BB 靐𰻞."T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { FDimen::Percent50, FDimen::Auto } } }),
			Button(u8"HideWindow"T, [PopupWindow = PopupWindow.Get()](const auto &) { PopupWindow->Hide(); }, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near }, { UElement::ToolTipProperty(), u8"Hide This Window."T } }),
			Button(u8"CloseWindow."T, [PopupWindow = PopupWindow.Get()](const auto &) { PopupWindow->Close(); }, 
				{
					{ UElement::HorizontalAlignmentProperty(), EElementAlignment::Near },
					{ UElement::ToolTipProperty(), Image(u8"../../Data/Cat.jpg"V) }
				}),
			Button(u8"Minimum"T, nullptr, { { UElement::HitInsideProperty(), EHitResult::Minimun }, {UControl::ContentHorizontalAlignmentProperty(), EElementAlignment::Near } }),
			Button(u8"Maximum"T, nullptr, { { UElement::HitInsideProperty(), EHitResult::Maximum } }),
			Button(u8"Close"T,nullptr, { { UElement::HitInsideProperty(), EHitResult::Close }, {UControl::ContentHorizontalAlignmentProperty(), EElementAlignment::Far }  }),
		},
		{
			{ UElement::SizeProperty(), FDimen2 { FDimen::Percent100, FDimen::Percent100 } }
		});
	//PopupCloseButton->ChromeComponent = EChromeComponent::Caption;

	UPanelRef MainPanel;
	UPanelRef SecondPanel;
	UButtonRef ButtonA;
	auto OnButtonClick = [PopupWindow = PopupWindow.Get()](const FRoutedEventArgs & Args)
		{
			AssertExpr(Args.Sender.IsA<UButtonControl>());
			UButtonControl & Button = StaticCast<UButtonControl>(Args.Sender);
			static_assert(IsStaticPointerConvertibleV<const UButtonControl &, const IObject &>);
			FConsole::WriteLine(u8"Click Button {}, {}"V, Args.Sender.Name, StaticCast<UButtonControl>(Args.Sender).Content);
			//Button.Storyboards[0]->Begin();

			if (Args.Sender.IsA<UButton>())
			{
				if (PopupWindow->Shown)
					PopupWindow->Hide();
				else
					PopupWindow->Show();
			}
		};

	UTextBlockRef TextBlockAA;
	UTextBlockRef TextBlockBB;
	UTextBlockRef TextBlockCC;
	UButtonRef Button0;
	UListViewRef FileList;

	FString ComplexText = 
		u8"♒强烈❤️❤️❤️推荐♒\n✅运行正常✅\n⚡亭亭玉立⚡\n♈恭喜发财♈\n✨新年快乐✨\n🏅大吉大利🏅\n♒今晚吃鸡♒\n"
		//u8"✋✌️☝️✊✍️⛑️☂️☎♻♿⚠⛔❄️☘️☁️⛅⛈️☂️☔⚡❄️\n"
		//u8"☃️⛄☄️👩🌾🎄🍅🛺🚕🚎🚎🚜💗\n"
		//u8"☕⛷️⛹️⛹️♂️⛹️♀️⚽⚾⛳\n"
		//u8"⛰️⛪⛩⛲⛺⛽⚓⛵⛴️✈️⛱️\n"
		u8"ดีดีดีีีดีดีlālálǎlàوالعدل والسلام靐𰻞𰻞𰻞 في العالم"V;

	UScrollViewerRef Element = ScrollViewer(StackPanel(EOrientation::Vertical,
			{
				TextBlock(FText(ComplexText), { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { FDimen::Percent50, FDimen::Auto } } }),
				TextBlockAA = TextBlock(u8"可爱的文字 in TextBlock AA 靐𰻞"T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { FDimen::Percent90, FDimen::Auto } } }),
				TextBlockBB = TextBlock(u8"可爱的文字 in TextBlock BB"T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { FDimen::Percent50, FDimen::Auto } } }),
				TextBlockCC = TextBlock(u8"可爱的文字 in TextBlock CC"T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { FDimen::Percent50, FDimen::Auto } } }),
#if 1
				Button0 = Button(u8"奇怪的按钮"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center } }),
				//RadioButton(u8"单选按钮 AAAA"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near }, { URadioButton::GroupNameProperty(), u8"AAAA"N } }),
				//RadioButton(u8"单选按钮 BBBB"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near }, { URadioButton::GroupNameProperty(), u8"AAAA"N } }),
				//RadioButton(u8"单选按钮 CCCC"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near }, { URadioButton::GroupNameProperty(), u8"AAAA"N } }),

				//RadioButton(u8"单选按钮 XXXX"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { URadioButton::GroupNameProperty(), u8"XXXX"N } }),
				//RadioButton(u8"单选按钮 YYYY"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { URadioButton::GroupNameProperty(), u8"XXXX"N } }),
				//RadioButton(u8"单选按钮 ZZZZ"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { URadioButton::GroupNameProperty(), u8"XXXX"N } }),
				ListBox(
					{
						u8"ListBox 第一个"T,
						TextBlock(u8"ListBox 中可爱的文字 in TextBlock AA"T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near } }),
						u8"第二个"T,
						RadioButton(u8"ListBox 单选按钮 KKKK"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { URadioButton::GroupNameProperty(), u8"XXXX"N } }),
						u8"第3333333333个"T,
						Image(u8"../../Data/AryaStark_256x256.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { 512, 512 } } }),
						u8"第444444444个"T,
						u8"第5555555个"T,
						u8"第66666666666666个"T,
						u8"第777777777777777777个一二三四五六七八九十"T,
						u8"第8888个"T,
					},
					{ { UElement::SizeProperty(), FDimen2 { FDimen::Percent50, FDimen::Auto } } }
					//{ { UElement::SizeProperty(), FDimen2 { 512, 512 } } }
				),
				FileList = ListView(
					{
					},
					{ { UElement::SizeProperty(), FDimen2 { FDimen::Percent75, 512 } } }
				),
				ScrollViewer(Image(u8"../../Data/AryaStark.jpg"V, { { UImage::StretchProperty(), EStretch::None } }),
					{ { UElement::SizeProperty(), FDimen2 { 256, 256 } } }),
				ListBox(
					{
						u8"第一个"T,
						TextBlock(u8"ListControl 中可爱的文字 in TextBlock AA"T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near } }),
						u8"第二个"T,
						RadioButton(u8"ListBox 单选按钮 KKKK"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { URadioButton::GroupNameProperty(), u8"XXXX"N } }),
						u8"第3333333333个"T,
						Image(u8"../../Data/AryaStark_256x256.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { 512, 512 } } }),
						u8"第444444444个"T,
						u8"第5555555个"T,
						u8"第66666666666666个"T,
						u8"第777777777777777777个"T,
						u8"第8888个"T,
					},
					{
						{ UElement::SizeProperty(), FDimen2 { FDimen::Percent90, 600 } },
					}),
				//ListControl(
				//	{
				//		u8"第一个"T,
				//		TextBlock(u8"ListControl 中可爱的文字 in TextBlock AA"T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near } }),
				//		u8"第二个"T,
				//		RadioButton(u8"单选按钮 KKKK"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { URadioButton::GroupNameProperty(), u8"XXXX"N } }),
				//		u8"第3333333333个"T,
				//		Image(D2DDevice.CreateImage(u8"../../Data/AryaStark_256x256.jpg"V), { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Stretch }, {UElement::SizeProperty(), FDimen2 {512, 512}} }),
				//	},
				//	{
				//		{ UElement::SizeProperty(), FDimen2 { FDimen::Percent50, FDimen::Auto } },
				//	}),
				CheckBox(u8"复选框 AAAA"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near } }),
				CheckBox(u8"复选框 BBBB"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near } }),
				CheckBox(u8"复选框 CCCC"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near } }),
				CheckBox(u8"复选框 XXXX"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UToggleButton::IsThreeStateProperty(), true } }),
				CheckBox(u8"复选框 YYYY"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UToggleButton::IsThreeStateProperty(), true } }),
				CheckBox(u8"复选框 ZZZZ"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UToggleButton::IsThreeStateProperty(), true } }),

				TextBlock(u8"可爱的文字 in TextBlock BB"T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Far } }),
				Path(IconClose2, { 0.5f, {} },
					{
						{ UElement::HorizontalAlignmentProperty(), EElementAlignment::Center },
						{ UPath::FillProperty(), Brushs::AntiqueWhite },
						{ UPath::StrokeProperty(), Brushs::CadetBlue },
						{ UPath::StrokeThicknessProperty(), 4 },
					}),
				TextBlock(u8"可爱的文字 in TextBlock CC"T),
				TextBlock(u8"可爱的文字 in TextBlock DD"T),
				MainPanel = WrapPanel(
					{
						ButtonA = Button(u8"可爱的按钮A"T, OnButtonClick),
						ButtonA = Button(u8"可爱的按钮B"T, OnButtonClick),
						Button(u8"可爱的按钮C"T, OnButtonClick),
						Button(u8"可爱的按钮D"T, OnButtonClick),
						Button(u8"可爱的按钮E"T, OnButtonClick),
						Button(u8"可爱的按钮F"T, OnButtonClick),
						Button(u8"可爱的按钮G"T, OnButtonClick),
						Button(u8"可爱的按钮H"T, OnButtonClick),
						Button(u8"可爱的按钮I"T, OnButtonClick),
						Button(u8"可爱的按钮J"T, OnButtonClick),
						Button(u8"可爱的按钮K"T, OnButtonClick),
						Button(u8"可爱的按钮L"T, OnButtonClick),
					},
					{
						{ UElement::SizeProperty(), FDimen2 { FDimen::Percent100, FDimen::Auto } },
						{ UWrapPanel::OrientationProperty(), EOrientation::Horizontal },
						{ UWrapPanel::ElementAlignmentProperty(), EElementAlignment::Center },
					}
				),
				Image(u8"../../Data/Cat.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center } }),
				TextBlock(u8"可爱的文字 in TextBlock EE"T),
				TextBlock(u8"可爱的文字 in TextBlock FF"T),
				TextBlock(u8"可爱的文字 in TextBlock GG"T),
				TextBlock(u8"可爱的文字 in TextBlock HH"T),
				GridPanel(
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
				)
#endif
			},
			{
				//{ UElement::SizeProperty(), FDimen2 { FDimen::Percent100, FDimen::Auto } },
				//{ UPanel::ElementAlignmentProperty(), EElementAlignment::Center },
			}),
		{
			{ UElement::SizeProperty(), FDimen2 { FDimen::Percent100, FDimen::Percent100 } }
		});

	//ButtonA->ClickMode = EClickMode::Press;
	//MainWindow.GroundingFill = Brushs::Black;
	//MainWindow.Content = u8"大大的笑话"T;

	TextBlockAA->ContextMenu = u8"线程 31840 已退出，返回值为 0 (0x0)。"T;
	TextBlockAA->HitInside = EHitResult::Inside;
	//TextBlockBB->ContextMenu = u8"东西都在哪儿"T;
	TextBlockBB->HitInside = EHitResult::Inside;
	TextBlockBB->ContextMenu = ListBox(
		{
			u8"第一个"T,
			TextBlock(u8"ListControl 中可爱的文字 in TextBlock AA"T, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Near } }),
			u8"第二个"T,
			RadioButton(u8"单选按钮 KKKK"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { URadioButton::GroupNameProperty(), u8"XXXX"N } }),
			u8"第3333333333个"T,
			Image(u8"../../Data/AryaStark_256x256.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { 512, 512 } } }),
			u8"第444444444个"T,
			u8"第5555555个"T,
			u8"第66666666666666个"T,
			u8"第777777777777777777个一二三四五六七八九十"T,
			u8"第8888个"T,
		}
		//{ { UElement::SizeProperty(), FDimen2 { FDimen::Percent50, FDimen::Auto } } }
		//{ { UElement::SizeProperty(), FDimen2 { 512, 512 } } }
	);

	UMenuRef MenuC = Menu(
		{
			MenuItem(u8"C.测试一下"T, [](const auto & Args)
				{
					LogInfo(u8"我要测试一下"V);
				}, Image(u8"../../Data/AryaStark_256x256.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { 64, 64 } } })),
			u8"C.不测试也行"T,
			MenuItem(u8"C.这里有子菜单"T, nullptr,
				Menu({
					u8"D.还可以"T,
					u8"D.测试一下"T,
					Image(u8"../../Data/AryaStark_256x256.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { 128, 128 } } }),
					u8"D.没了"T,
					})),
				RadioButton(u8"C.单选按钮 VVV"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { URadioButton::GroupNameProperty(), u8"VVV"N } }),
				u8"C.这里隔开下"T,
				Image(u8"../../Data/AryaStark_256x256.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { 128, 128} } }),
			});

	TextBlockCC->HitInside = EHitResult::Inside;
	TextBlockCC->ContextMenu = Menu(
	{
		MenuItem(u8"A.测试一下"T, [](const auto & Args)
			{
				LogInfo(u8"我要测试一下"V);
			}, Image(u8"../../Data/AryaStark_256x256.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { 64, 64 } } })),
		u8"A.不测试也行"T,
		MenuItem(u8"A.这里有子菜单"T, nullptr,
			Menu ({
				u8"B.还可以"T,
				u8"B.测试一下"T,
				Image(u8"../../Data/AryaStark_256x256.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { 128, 128} } }),
				MenuItem(u8"B.没了"T, nullptr, MenuC)
			} )),
		RadioButton(u8"A.单选按钮 VVV"T, OnButtonClick, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { URadioButton::GroupNameProperty(), u8"VVV"N } }),
		u8"A.这里隔开下"T,
		Image(u8"../../Data/AryaStark_256x256.jpg"V, { { UElement::HorizontalAlignmentProperty(), EElementAlignment::Center }, { UElement::SizeProperty(), FDimen2 { 64, 64} } }),
	});

	if (FileList)
	{
		FileList->ContextMenu = u8"我看看是什么东西"T;
		FileList->Columns =
		{
			{ FFileInfo::FileIconProperty(), u8"Icon"T },
			{ FFileInfo::FileNameProperty(), u8"File Name"T },
			{
				{
					FFileInfo::FileSizeProperty(), [](uint64 FileSize)
					{
						if (FileSize < 1024)
							return Format(u8"{}B"V, FileSize);
						else if (FileSize < 1024 * 1024)
							return Format(u8"{}KB"V, FileSize >> 10);
						else if (FileSize < 1024 * 1024 * 1024)
							return Format(u8"{}MB"V, FileSize >> 20);
						else
							return Format(u8"{}GB"V, FileSize >> 30);
					},
				},
				u8"File Size"T,
			},
			{ FFileInfo::FilePathProperty(), u8"File Path"T }
		};

		//TList<FFileInfoRef> FileInfos = FFileInfo::EnumPath(u8"C:/Windows/System32/"V);
		//TList<FFileInfoRef> FileInfos = FFileInfo::EnumPath(u8"C:/Windows/System/Speech"V);
		TList<FFileInfoRef> FileInfos = FFileInfo::EnumPath(u8"C:/Windows/System/"V);
		for (FFileInfoRef FileInfo : FileInfos)
		{
			FileInfo->FileIcon = FFileInfo::GetFileIcon(FileInfo->FilePath);
			FileList->Items.AddItem(FileInfo);
		}

		FileList->OnQueryItemContextMenu += [](FListViewQueryItemContextMenuEventArgs & Args)
		{
			FFileInfoRef FileInfo = Args.ListViewItem.Content.Get<FFileInfoRef>();
			if (!FileInfo)
				return;

			Args.Menu = Menu({
				Format(u8"文件 {}"V, FileInfo->FileName),
				MenuItem(u8"在资源管理器中打开(&O)"T, [FileInfo](const FRoutedEventArgs & Args)
					{
						//FWString FilePathW = FEncoding::ToWide(FileInfo->FilePath);
						FWString CommandLineParameters = FEncoding::ToWide(Format(u8"/select,\"{}\""V, FileInfo->FilePath));
						CommandLineParameters.Slice(1).Replace(L'/', L'\\');

						SHELLEXECUTEINFOW ShellExecuteInfoW = { };

						ShellExecuteInfoW.cbSize = sizeof(ShellExecuteInfoW);
						ShellExecuteInfoW.lpFile = L"explorer.exe";
						ShellExecuteInfoW.nShow = SW_SHOW;
						ShellExecuteInfoW.fMask = SEE_MASK_DEFAULT;
						ShellExecuteInfoW.lpParameters = CommandLineParameters.Data;
						ShellExecuteInfoW.lpVerb = L"open";

						ShellExecuteExW(&ShellExecuteInfoW);
					}),
				MenuItem(u8"属性(&O)"T, [F=FileInfo](const FRoutedEventArgs & Args)
					{
						//SHOpenPropSheetW()

						FWString FilePathW = FEncoding::ToWide(F->FilePath);
						SHELLEXECUTEINFOW ShellExecuteInfoW = { };

						ShellExecuteInfoW.cbSize = sizeof(ShellExecuteInfoW);
						ShellExecuteInfoW.lpFile = FilePathW.Data;
						ShellExecuteInfoW.nShow = SW_SHOW;
						ShellExecuteInfoW.fMask = SEE_MASK_INVOKEIDLIST;
						ShellExecuteInfoW.lpVerb = L"properties";

						ShellExecuteExW(&ShellExecuteInfoW);
					})
				});
		};
	}

	return Element;
}
