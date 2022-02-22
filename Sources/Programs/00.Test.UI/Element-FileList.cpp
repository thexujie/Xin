#include "PCH.h"

#include <iostream>
#include <shlobj.h>
#include <shellapi.h>

#include "00.Test.UI.Types.h"
#include "Xin.Core/Utils/FileInfo.h"

UElementRef CreateElement_FileList()
{
	FString CompolexText =
		//u8"♒强烈❤️❤️❤️推荐♒\n✅运行正常✅\n⚡亭亭玉立⚡\n♈恭喜发财♈\n✨新年快乐✨\n🏅大吉大利🏅\n♒今晚吃鸡♒\n"
		//u8"✋✌️☝️✊✍️⛑️☂️☎♻♿⚠⛔❄️☘️☁️⛅⛈️☂️☔⚡❄️\n"
		//u8"☃️⛄☄️👩🌾🎄🍅🛺🚕🚎🚎🚜💗\n"
		//u8"☕⛷️⛹️⛹️♂️⛹️♀️⚽⚾⛳\n"
		//u8"⛰️⛪⛩⛲⛺⛽⚓⛵⛴️✈️⛱️\n"
		u8"ดีดีดีีีดีดีlālálǎlàوالعدل والسلام靐𰻞𰻞𰻞 في العالم"V;

	auto OnButtonClick = [&](const FRoutedEventArgs & Args)
	{
		AssertExpr(Args.Sender.IsA<UButtonControl>());
		UButtonControl & Button = StaticCast<UButtonControl>(Args.Sender);
		static_assert(IsStaticPointerConvertibleV<const UButtonControl&, const IObject&>);
		FConsole::WriteLine(u8"Click Button {}, {}"V, Args.Sender.Name, StaticCast<UButtonControl>(Args.Sender).Content);
	};

	UListViewRef FileList = ListView(
			{
			},
			{ { UElement::SizeProperty(), FDimen2 { FDimen::Percent75, 512 } } }
		);

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

			//if (FileList->Items.Size >= 4)
			//	break;
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

					SHELLEXECUTEINFOW ShellExecuteInfoW = {};

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
					SHELLEXECUTEINFOW ShellExecuteInfoW = {};

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

	return FileList;
}
