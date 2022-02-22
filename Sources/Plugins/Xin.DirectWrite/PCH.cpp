#include "PCH.h"

namespace Xin
{
	void Log(ELogLevel LogLevel, FStringV String)
	{
		Logger().WriteLine(LogLevel, u8"Xin.DWrite"V, String);
	}
}
