#include "PCH.h"

#include "ClassFactory.h"
#include "NavigateToOriginalPathCommandVerb.h"

#include <new>

Xin::uintx_t DllRefCount = 0;

template<typename ClassT>
HRESULT TCreateClassFactoryInstance(const IID & riid, void ** ppvObject)
{
    TReferPtr<IClassFactory> pClassFactory(new (std::nothrow) CClassFactory(Xin::TDummy<ClassT>()));
    HRESULT hResult = pClassFactory ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hResult))
        hResult = pClassFactory->QueryInterface(riid, ppvObject);
    return hResult;
}

constexpr wchar_t GProgID_All[] = L"*";
constexpr wchar_t GProgID_Directory[] = L"Directory";
constexpr wchar_t GShellName_NavigateToOriginalPath[] = L"Sample.NavigateToOriginalPath";

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
__inline HINSTANCE GetModuleInstance() { return (HINSTANCE)&__ImageBase; }

HRESULT NTOP_RegisterClass(HKEY RootKey, const GUID & ClassId, FWStringView FriendlyName, FWStringView ThreadingModel)
{
    wchar_t GuidStr[39];
    int GuidStrLength = StringFromGUID2(ClassId, GuidStr, ARRAYSIZE(GuidStr));

    HRESULT Result = FRegkey::SetKeyValue(RootKey, Xin::Format(L"Software\\Classes\\CLSID\\{}", GuidStr), L"", FriendlyName);
    if (SUCCEEDED(Result))
    {
        FWString & Buffer = GetThreadLocalStringBuffer<wchar_t>();
        int ModuleFileNameLength = GetModuleFileNameW(GetModuleInstance(), Buffer.Data, Buffer.GetCapacityWithoutNull());
        while (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            Buffer.Resize(0, Max<size_t>(Buffer.Capacity, 1) * 2);
            ModuleFileNameLength = GetModuleFileNameW(GetModuleInstance(), Buffer.Data, Buffer.GetCapacityWithoutNull());
        }
        Buffer.Resize(ModuleFileNameLength);

        Result = FRegkey::SetKeyValue(RootKey, Xin::Format(L"Software\\Classes\\CLSID\\{}\\InProcServer32", GuidStr), L"", Buffer);
        if (SUCCEEDED(Result))
        {
            Result = FRegkey::SetKeyValue(RootKey, Xin::Format(L"Software\\Classes\\CLSID\\{}\\InProcServer32", GuidStr), L"ThreadingModel", ThreadingModel);
        }
    }
    return S_OK;
}

HRESULT NTOP_UnRegisterClass(HKEY RootKey, const GUID & ClassId)
{
    wchar_t GuidStr[39];
    int GuidStrLength = StringFromGUID2(ClassId, GuidStr, ARRAYSIZE(GuidStr));

    FWString ClassRegkeyPath = Format(L"Software\\Classes\\CLSID\\{}", GuidStr);
    FRegkey::DeleteKey(RootKey, ClassRegkeyPath);
    return S_OK;
}

HRESULT NTOP_RegisterShell(HKEY RootKey, FWStringView ProgID, FWStringView VerbName, FWStringView VerbDisplayName, const GUID & ClassId)
{
    wchar_t GuidStr[39];
    int GuidStrLength = StringFromGUID2(ClassId, GuidStr, ARRAYSIZE(GuidStr));
	
    FWString VerbRegkeyPath = Format(L"Software\\Classes\\{}\\Shell\\{}", ProgID, VerbName);
    FRegkey::DeleteKey(RootKey, VerbRegkeyPath);

    HRESULT Result = FRegkey::SetKeyValue(RootKey, VerbRegkeyPath, L"ExplorerCommandHandler", GuidStr);
    if (SUCCEEDED(Result))
        Result = FRegkey::SetKeyValue(RootKey, VerbRegkeyPath, L"", VerbDisplayName);

    if (SUCCEEDED(Result))
		Result = FRegkey::SetKeyValue(RootKey, VerbRegkeyPath, L"NeverDefault", FWStringView::Empty);
    return Result;
}

HRESULT NTOP_UnRegisterShell(HKEY RootKey, FWStringView ProgID, FWStringView VerbName)
{
    FWString VerbRegkeyPath = Format(L"Software\\Classes\\{}\\Shell\\{}", ProgID, VerbName);
    FRegkey::DeleteKey(RootKey, VerbRegkeyPath);
    return S_OK;
}


#pragma comment(linker, "/export:DllCanUnloadNow,PRIVATE")
#pragma comment(linker, "/export:DllGetClassObject,PRIVATE")
#pragma comment(linker, "/export:DllRegisterServer,PRIVATE")
#pragma comment(linker, "/export:DllUnregisterServer,PRIVATE")

STDAPI DllCanUnloadNow()
{
    return (DllRefCount == 0) ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void ** ppv)
{
    if (clsid == __uuidof(CNavigateToOriginalPathCommandVerb))
        return TCreateClassFactoryInstance<CNavigateToOriginalPathCommandVerb>(riid, ppv);
    return CLASS_E_CLASSNOTAVAILABLE;
}

STDAPI DllRegisterServer()
{
    NTOP_RegisterClass(HKEY_CURRENT_USER, __uuidof(CNavigateToOriginalPathCommandVerb), L"Navigate to original path shell", L"Apartment");
    HRESULT Result = NTOP_RegisterShell(HKEY_CURRENT_USER, GProgID_All, GShellName_NavigateToOriginalPath, L"", __uuidof(CNavigateToOriginalPathCommandVerb));
    if (SUCCEEDED(Result))
        Result = NTOP_RegisterShell(HKEY_CURRENT_USER, GProgID_Directory, GShellName_NavigateToOriginalPath, L"", __uuidof(CNavigateToOriginalPathCommandVerb));
    return Result;
}

STDAPI DllUnregisterServer()
{
    HRESULT Result = NTOP_UnRegisterShell(HKEY_CURRENT_USER, GProgID_All, GShellName_NavigateToOriginalPath);
    Result = NTOP_UnRegisterShell(HKEY_CURRENT_USER, GProgID_Directory, GShellName_NavigateToOriginalPath);
    Result = NTOP_UnRegisterClass(HKEY_CURRENT_USER, __uuidof(CNavigateToOriginalPathCommandVerb));
    return S_OK;
}

int _tmain(int argc, const char * args[])
{
    DllRegisterServer();
    DllUnregisterServer();
    return 0;
}