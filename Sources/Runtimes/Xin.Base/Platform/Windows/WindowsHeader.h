#pragma once

static_assert(XIN_CORE != 0, "You should include Xin.Core.h before include WindowsPrivate.h!");

#ifdef XIN_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef _UNICODE
#undef CreateFile
#undef MoveFile
#undef DeleteFile
#undef CreateFont
#undef UpdateResource
#undef LoadImage
#undef YieldProcessor
#undef MemoryBarrier
#undef ReadForWriteAccess
#undef FindResource
#undef MessageBox
#endif
#endif

namespace Xin::Windows
{
	template<typename ...ArgTs>
	void HRInfo(HRESULT Result, FStringV Format,  ArgTs &&...Args)
	{
		if (FAILED(Result))
		{
			LogError(Format, Forward<ArgTs>(Args)...);
		}
	}

	template<typename ...ArgTs>
	void HRWarning(HRESULT Result, FStringV Format,  ArgTs &&...Args)
	{
		if (FAILED(Result))
		{
			LogWarning(Format, Forward<ArgTs>(Args)...);
		}
	}

	template<typename ...ArgTs>
	void HRError(HRESULT Result, FStringV Format,  ArgTs &&...Args)
	{
		if (FAILED(Result))
		{
			LogError(Format, Forward<ArgTs>(Args)...);
		}
	}

	inline bool Succeeded(HRESULT Result)
	{
		return SUCCEEDED(Result);
	}

	inline bool Failed(HRESULT Result)
	{
		return FAILED(Result);
	}
}