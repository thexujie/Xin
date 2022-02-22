#include "PCH.h"

#include "LLVMIncludes.h"
#pragma comment(lib, R"(version.lib)")

namespace Xin
{
	void Log(ELogLevel LogLevel, FStringV String)
	{
		Logger().WriteLine(LogLevel, u8"Xin.Reflection"V, String);
	}
}
