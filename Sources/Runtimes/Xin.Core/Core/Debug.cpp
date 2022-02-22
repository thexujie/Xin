#include "PCH.h"
#include "Debug.h"

#ifdef XIN_WINDOWS
#include <Windows.h>
#endif

#include <iostream>

namespace Xin
{
	FDebug & FDebug::Instance()
	{
		static FDebug GInstance;
		return GInstance;
	}

	FDebug::FDebug()
	{

	}

	void FDebug::OnKeyDown(FKey Key)
	{
		for (FDebugValue * DebugValue : DebugValues)
			DebugValue->OnKeyDown(Key);
	}

	FDebugValue::FDebugValue()
	{
		FDebug::Instance().DebugValues.Add(this);
	}

	FDebugValue::~FDebugValue()
	{
		FDebug::Instance().DebugValues.Remove(this);
	}

	void FDebug::Break()
	{
#if XIN_WINDOWS && XIN_DEBUG
		::DebugBreak();
#endif
	}

	void FDebug::WriteLine(FAStringV String)
	{
		std::wstring StdWString = FEncoding::ToStdWString(String);
		::OutputDebugStringW(StdWString.data());
		::OutputDebugStringW(L"\r\n");
		std::wcout << StdWString << std::endl;
	}

	void FDebug::WriteLine(FWStringV String)
	{
		::OutputDebugStringW(String.Data);
		::OutputDebugStringW(L"\r\n");
		std::wcout << FEncoding::AsStdWString(String) << std::endl;
	}

	void FDebug::WriteLine(FUStringV String)
	{
		std::wstring StdWString = FEncoding::ToStdWString(String);
		::OutputDebugStringW(StdWString.data());
		::OutputDebugStringW(L"\r\n");
		std::wcout << StdWString << std::endl;
	}
}
