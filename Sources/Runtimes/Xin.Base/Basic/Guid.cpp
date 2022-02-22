#include "PCH.h"
#include "Guid.h"
#include "Formatter.h"
#include "Platform/Platform.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsHeader.h"
#include <bcrypt.h>
#pragma comment(lib, "Bcrypt.lib")
#endif

namespace Xin
{
	FGuid FGuid::Empty = {};

	FStringV FGuid::Format(FStringV Formal) const
	{
		return Xin::Format(u8"{{{:X}-{:X}-{:X}-{:X}-{:X}{:X}{:X}}}"V, Data1, Data2, Data3,
			uint16(((Data4[0] << 8) | Data4[1])),
			uint16(((Data4[2] << 8) | Data4[3])),
			uint16(((Data4[4] << 8) | Data4[5])),
			uint16(((Data4[6] << 8) | Data4[7])));
	}
	
#ifdef XIN_WINDOWS
	FGuid::FGuid(const struct _GUID & GUID)
	{
		Data1 = GUID.Data1;
		Data2 = GUID.Data2;
		Data3 = GUID.Data3;
		ArrayCopy(Data4, GUID.Data4);
	}
	
	FGuid FGuid::Generate()
	{
		FGuid Guid;
		BCRYPT_ALG_HANDLE hAlgorithm = NULL;
		NTSTATUS Status =
			::BCryptOpenAlgorithmProvider(
				&hAlgorithm,
				BCRYPT_RNG_ALGORITHM,
				NULL,
				0);
		if (Status < 0)
			return Empty;
		
		::BCryptGenRandom(hAlgorithm, reinterpret_cast<PUCHAR>(&Guid), sizeof(FGuid), 0);
		::BCryptCloseAlgorithmProvider(hAlgorithm, 0);
		return Guid;
	}
#endif
}
