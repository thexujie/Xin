#include "PCH.h"

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "pathcch.lib")

namespace Xin
{
	void Log(ELogLevel LogLevel, FStringV String)
	{
		Logger().WriteLine(LogLevel, u8"Xin.Windows.Internal"V, String);
	}
}
