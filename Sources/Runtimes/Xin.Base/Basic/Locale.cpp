#include "PCH.h"
#include "Locale.h"

#include "Platform/Platform.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsHeader.h"
#endif

namespace Xin
{
	FLocale FLocale::Current()
	{
		return std::locale::classic();
	}
}
