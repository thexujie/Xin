#pragma once

#include "WindowsPrivate.h"

namespace Xin::Windows
{
	class CORE_API FRegkey
	{
	public:
		FRegkey() = default;
		FRegkey(HKEY RootKey, FStringV Path);

	public:
		static HRESULT SetKeyValue(HKEY RootKey, FWStringV Path, FWStringV KeyName, FWStringV KeyValue);
		static HRESULT DeleteKey(HKEY RootKey, FWStringV KeyPath);
	};
}
