#include "PCH.h"

#pragma comment(lib, "windowscodecs.lib")

namespace Xin
{
	void Log(ELogLevel LogLevel, FStringV String)
	{
		Logger().WriteLine(LogLevel, u8"Xin.Direct2D"V, String);
	}
}
