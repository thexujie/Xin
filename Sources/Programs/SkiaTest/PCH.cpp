#include "PCH.h"

namespace Xin
{
	void Log(ELogLevel LogLevel, FStringView String)
	{
		Logger().WriteLine(LogLevel, Str("CoreTest"), String);
	}
}
