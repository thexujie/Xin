#include "PCH.h"
#include "Winversion.h"

#include "Private/InternalNT.h"

namespace Xin::Windows::Internal
{
	static FWinversion LoadWinver()
	{
		RTL_OSVERSIONINFOEXW OsVersionInfoExW = { sizeof(RTL_OSVERSIONINFOEXW) };
		if (!NT_SUCCESS(RtlGetVersion(&OsVersionInfoExW)))
			return { };

		FWinversion Winversion { OsVersionInfoExW.dwMajorVersion, OsVersionInfoExW.dwMinorVersion, OsVersionInfoExW.dwBuildNumber };
		return Winversion;
	}

	FWinversion FWinversion::Current = LoadWinver();
}
