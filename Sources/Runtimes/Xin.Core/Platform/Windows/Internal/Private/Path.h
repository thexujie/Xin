#pragma once

#include "InternalNT.h"

namespace Xin::Windows::Internal
{
	INTERNAL_API FWString NtFilePathToDosFilePath(UNICODE_STRING & NtFilePath);
}
