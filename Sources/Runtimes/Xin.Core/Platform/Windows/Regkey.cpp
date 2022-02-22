#include "PCH.h"
#include "Regkey.h"

namespace Xin::Windows
{
	FRegkey::FRegkey(HKEY RootKey, FStringV Path)
	{
		
	}


	HRESULT FRegkey::SetKeyValue(HKEY RootKey, FWStringV Path, FWStringV KeyName, FWStringV KeyValue)
	{
		return HRESULT_FROM_WIN32(RegSetKeyValueW(RootKey, Path.Data, KeyName.Data, REG_SZ, KeyValue.Data, DWORD(KeyValue.Size * sizeof(wchar_t))));
	}
	
	HRESULT FRegkey::DeleteKey(HKEY RootKey, FWStringV KeyPath)
	{
		return HRESULT_FROM_WIN32(RegDeleteTreeW(RootKey, KeyPath.Data));
	}
}