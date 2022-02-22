#include "PCH.h"
#include "System.h"

#include "Strings.h"
#include "Encoding.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsHeader.h"
#endif

namespace Xin
{
	voidp FSystem::Instance()
	{
		return (voidp)GetModuleHandleW(NULL);
	}

	uint32 FSystem::ThreadId()
	{
		return (uint32)GetCurrentThreadId();
	}

	uint32 FSystem::ProcessId()
	{
		return (uint32)GetProcessId(GetCurrentProcess());
	}

	void FSystem::SetThreadDescription(FStringV Description)
	{
		::SetThreadDescription(GetCurrentThread(), FEncoding::ToWide(Description).GetData());
	}
	
	void FSystem::SetThreadDescription(uint32 ThreadId, FStringV Description)
	{
		HANDLE hThread = ::OpenThread(THREAD_SET_LIMITED_INFORMATION, FALSE, ThreadId);
		if (hThread)
		{
			::SetThreadDescription(hThread, FEncoding::ToWide(Description).GetData());
			CloseHandle(hThread);
		}
	}
	
	void FSystem::DebugOutput(FStringV String)
	{
		::OutputDebugStringW(FEncoding::ToWide(String).Data);
		::OutputDebugStringW(L"\r\n");
	}
	
	void FSystem::DebugOutput(FWStringV String)
	{
		::OutputDebugStringW(String.Data);
		::OutputDebugStringW(L"\r\n");
	}

	void FSystem::MessageBox(FStringV Title, FStringV Content)
	{
		FWString TitleW = FEncoding::ToWide(Title);
		FWString ContentW = FEncoding::ToWide(Content);
		MessageBoxW(NULL, ContentW.Data, TitleW.Data, 0);
	}

	FSystemParameterInfo FSystem::ParameterInfo;

	FSystemParameterInfo::FSystemParameterInfo()
	{
		DWORD SPI_KeyboardDelay;
		SystemParametersInfoW(SPI_GETKEYBOARDDELAY, 0, &SPI_KeyboardDelay, 0);
		AssertExpr(SPI_KeyboardDelay <= 3);
		KeyboardDelay = Clamp<uint32>(SPI_KeyboardDelay + 1, 1, 4) * 250;

		SystemParametersInfoW(SPI_GETKEYBOARDSPEED, 0, &KeyboardSpeed, 0);
		AssertExpr(KeyboardSpeed <= 31);
	}
}
