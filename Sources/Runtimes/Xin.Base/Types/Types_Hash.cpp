#include "PCH.h"
#include "Types_Hash.inl"

#include <cityhash/city.h>

namespace Xin
{
	uint64 CityHash64(const void * Data, uintx SizeInBytes)
	{
		return ::CityHash64(static_cast<const char *>(Data), SizeInBytes);
	}
}
