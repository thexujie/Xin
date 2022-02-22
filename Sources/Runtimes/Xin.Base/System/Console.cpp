#include "PCH.h"
#include "Console.h"
#include <conio.h>
#include <iostream>

#include "Platform/Platform.h"

namespace Xin
{
	static FEncoding ConsoleEncoding = FEncoding::ANSI;

	FEncoding FConsole::GetEncoding()
	{
#if XIN_WINDOWS
		uint32 CodePage = GetConsoleOutputCP();
		AssertExpr(CodePage == ConsoleEncoding.CodePage);
#endif
		return ConsoleEncoding;
	}

	FEncoding FConsole::SetEncoding(FEncoding Encoding)
	{
		FEncoding EncodingOld = ConsoleEncoding;
		ConsoleEncoding = Encoding;
#if XIN_WINDOWS
		SetConsoleOutputCP(Encoding.CodePage);
#endif
		return EncodingOld;
	}

	int32 FConsole::ReadKey(FStringV TipText)
	{
		Write(TipText);
		return _getch();
	}

	void FConsole::Clear()
	{
#if XIN_WINDOWS
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		//DWORD ConsoleMode = 0;
		//if (!GetConsoleMode(hConsole, &ConsoleMode))
		//	return;

		//if (!SetConsoleMode(hConsole, ConsoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
		//	return;

		COORD ZeroCoord = { 0, 0 };

		CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo {};
		GetConsoleScreenBufferInfo(hConsole, &ConsoleScreenBufferInfo);

		SMALL_RECT ScrollRect = { 0, 0, ConsoleScreenBufferInfo.dwSize.X, ConsoleScreenBufferInfo.dwSize.Y };
		COORD ScrollTarget = { 0, (SHORT)(0 - ConsoleScreenBufferInfo.dwSize.Y) };

		CHAR_INFO CharInfo = { { TEXT(' ') }, ConsoleScreenBufferInfo.wAttributes };
		ScrollConsoleScreenBufferW(hConsole, &ScrollRect, NULL, ScrollTarget, &CharInfo);

		SetConsoleCursorPosition(hConsole, ZeroCoord);
#endif
	}

	void FConsole::Write(FAStringV String)
	{
		if (ConsoleEncoding == FEncoding::UTF8)
			std::cout << FEncoding::ToStdUString(String);
		else
			std::cout << FEncoding::AsStdString(String);
	}

	void FConsole::Write(FWStringV String)
	{
		if (ConsoleEncoding == FEncoding::UTF8)
			std::cout << FEncoding::ToStdUString(String);
		else
			std::cout << FEncoding::ToStdString(String);
	}

	void FConsole::Write(FUStringV String)
	{
		if (ConsoleEncoding == FEncoding::UTF8)
			std::cout << FEncoding::AsStdString(String);
		else
			std::cout << FEncoding::ToStdString(String);
	}

	void FConsole::WriteLine()
	{
		std::cout << std::endl;
	}

	void FConsole::WriteLine(FAStringV String)
	{
		if (ConsoleEncoding == FEncoding::UTF8)
			std::cout << FEncoding::ToStdUString(String) << std::endl;
		else
			std::cout << FEncoding::AsStdString(String) << std::endl;
	}

	void FConsole::WriteLine(FWStringV String)
	{
		if (ConsoleEncoding == FEncoding::UTF8)
			std::cout << FEncoding::ToStdUString(String) << std::endl;
		else
			std::cout << FEncoding::ToStdString(String) << std::endl;
	}

	void FConsole::WriteLine(FUStringV String)
	{
		if (ConsoleEncoding == FEncoding::UTF8)
			std::cout << FEncoding::AsStdString(String) << std::endl;
		else
			std::cout << FEncoding::ToStdString(String) << std::endl;
	}
}
