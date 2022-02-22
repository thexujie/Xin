#include "PCH.h"
#include "FileLinePeriod.h"

namespace Xin
{
	bool FFileLinePeriod::Check() const
	{
        static thread_local std::map<TTuple<const char *, int>, uint64> __LastCheckMilliseconds;
		uint64 Milliseconds = FDateTime::SteadyMilliseconds();

        uint64 LastCheckMilliseconds = __LastCheckMilliseconds[Id];
        if (Milliseconds < LastCheckMilliseconds)
            return false;

		if ((Milliseconds - LastCheckMilliseconds) * Count < static_cast<uint64>(Period))
			return false;

        __LastCheckMilliseconds[Id] = Milliseconds;
        return true;
	}
}
