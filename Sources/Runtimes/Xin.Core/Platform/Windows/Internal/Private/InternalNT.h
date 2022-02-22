#pragma once

#include "../Internal.Types.h"

#include "phnt/include/phnt_windows.h"
#include "phnt/include/phnt.h"

#define NT_FAILED(Status)  (((NTSTATUS)(Status)) < 0)

#define PHLIBAPI

namespace Xin::Windows::Internal
{
	typedef struct _LANGANDCODEPAGE
	{
		USHORT Language;
		USHORT CodePage;
	} LANGANDCODEPAGE, * PLANGANDCODEPAGE;

	struct NTHandle
	{
		NTHandle() = default;
		NTHandle(HANDLE Handle) : Handle(Handle) {}
		~NTHandle()
		{
			if (Handle != INVALID_HANDLE_VALUE)
			{
				::NtClose(Handle);
				Handle = INVALID_HANDLE_VALUE;
			}
		}

		HANDLE * operator &() { return &Handle; }
		operator HANDLE() const { return Handle; }
		HANDLE Handle = INVALID_HANDLE_VALUE;
	};
}
