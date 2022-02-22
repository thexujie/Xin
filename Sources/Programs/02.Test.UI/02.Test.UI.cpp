#include "PCH.h"

#include <iostream>

#include "Xin.Direct2D/Xin.Direct2D.h"
#include "Xin.Skia/Xin.Skia.h"
#include "00.Test.UI/00.Test.UI.h"

enum class ETestContent
{
	FileList = 0,
	Atlas,
	ScrollViewer,
	Chrome,
	Paths,
	Count,
};

ETestContent SelectTestContent()
{
	ETestContent TestContent = ETestContent::Atlas;

	auto PrintItems = [&]()
	{
		FConsole::Clear();

		if (TestContent == ETestContent::FileList) std::cout << ">";
		std::cout << " File List" << std::endl;
		if (TestContent == ETestContent::Atlas) std::cout << ">";
		std::cout << " Controls Atlas" << std::endl;
		if (TestContent == ETestContent::ScrollViewer) std::cout << ">";
		std::cout << " Scroll Viewer" << std::endl;
		if (TestContent == ETestContent::Chrome) std::cout << ">";
		std::cout << " Chrome (Sub Window)" << std::endl;
		if (TestContent == ETestContent::Paths) std::cout << ">";
		std::cout << " Path" << std::endl;
		std::cout << "Press enter to continue, esc to exit." << std::endl;
	};

	PrintItems();
	while (true)
	{
		int32 Ch = FConsole::ReadKey();
		if (Ch == 0 || Ch == 0x1B)
		{
			TestContent = ETestContent::Count;
			break;
		}
		else if (Ch == 0xD)
		{
			break;
		}
		else if (Ch == 0x50)
		{
			if (TestContent < ETestContent::Paths)
			{
				TestContent = ETestContent((int32(TestContent) + 1) % int32(ETestContent::Count));
				PrintItems();
			}                                                                                                                                                                                                                          
		}
		else if (Ch == 0x48)
		{
			if (TestContent > ETestContent::FileList)
			{
				TestContent = ETestContent((int32(TestContent) - 1) % int32(ETestContent::Count));
				PrintItems();
			}
		}
		else {}
	}

	FConsole::Clear();
	return TestContent;
}

int TestUI_Default(UElementRef Element);
int TestUI_Default();

int main(int argc, const char * args[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_WNDW);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	//_CrtSetBreakAlloc(6312);

	FCOMInitializer COMInitializer;

	new char[9] { 't', 'e', 's', 't', ' ', 'l', 'e', 'a', 'k' };

	IGraphicsRef Graphics = D2D::D2DCreateGraphics();
	//IGraphicsRef Graphics = Skia::SkiaCreateGraphics();

	FDesktopGraphicsInitializer DesktopGraphicsInitializer(Graphics);
	//auto WindowPlace = IDesktop::GetDefault().PlaceWindow();
	//return TestUI_Default();

	ETestContent Content = ETestContent::Count;
	for (int ArgIndex = 0; ArgIndex < argc; ++ArgIndex)
	{
		if (strstr(args[ArgIndex], "-atlas"))
		{
			Content = ETestContent::Atlas;
			break;
		}

		if (strstr(args[ArgIndex], "-filelist"))
		{
			Content = ETestContent::FileList;
			break;
		}

		if (strstr(args[ArgIndex], "-chrome"))
		{
			Content = ETestContent::Chrome;
			break;
		}

		if (strstr(args[ArgIndex], "-paths"))
		{
			Content = ETestContent::Paths;
			break;
		}
	}

	//if (Content == ETestContent::Count)
	//	Content = SelectTestContent();

	switch (Content)
	{
	default:
		TestUI_Default();
		break;
	case ETestContent::FileList:
		TestUI_Default(CreateElement_FileList());
		break;
	case ETestContent::Atlas:
		TestUI_Default(CreateElement_Atlas());
		break;
	case ETestContent::Chrome:
		TestUI_Default(CreateElement_Chrome());
		break;
	case ETestContent::ScrollViewer:
		TestUI_Default(CreateElement_ScrollViewer());
		break;
	case ETestContent::Paths:
		TestUI_Default(CreateElement_Paths());
		break;
	}

	return 0;
}
