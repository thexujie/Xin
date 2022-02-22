#include "PCH.h"
#include "Basic/Log.h"

namespace Xin
{
	void Log(ELogLevel LogLevel, FStringV String)
	{
		Logger().WriteLine(LogLevel, u8"Core.Foundation"V, String);
	}
}
