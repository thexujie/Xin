#include "PCH.h"
#include "Module.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
    FModule::FModule() :
        Handle(NULL)
    {

    }

    FModule::FModule(const FString & ModulePath)
    {
        Load(ModulePath);
    }

    FModule::~FModule()
    {
        Free();
    }

    FModule::FModule(const FModule & That) : ModulePath(That.ModulePath)
    {
	    if (ModulePath)
	    {
            FWStringV ModulePathW = FEncoding::ToWide(ModulePath);
            Handle = ::LoadLibraryW(ModulePathW);
	    }
    }

    bool FModule::Load(FStringV ModulePath, bool bUserDir)
    {
        Free();
        if (!ModulePath.Empty())
        {
            FWStringV ModulePathW = FEncoding::ToWide(ModulePath);
            if (bUserDir)
                Handle = ::LoadLibraryExW(ModulePathW, NULL, LOAD_LIBRARY_SEARCH_USER_DIRS);
            else
                Handle = ::LoadLibraryW(ModulePathW);
            //if (!Handle)
            //    core::war() << __FUNCTION__" LoadLibrary [" << pathw << "] failed " << win32::winerr_str(GetLastError());
            //else
            {
                wchar_t ModuleFilePathW[1024] = {};
                uint32 Length = GetModuleFileNameW(static_cast<HMODULE>(Handle), ModuleFilePathW, 1024);
                this->ModulePath = FEncoding::ToUtf8(ModuleFilePathW, Length);
            }
        }
        else
            this->ModulePath.Reset();
        return !!Handle;
    }

    void FModule::Free()
    {
        if (Handle)
        {
            ::FreeLibrary((HMODULE)Handle);
            Handle = NULL;
        }
    }

    void * FModule::GetProc(const char * name)
    {
        if (Handle)
            return static_cast<void *>(::GetProcAddress((HMODULE)Handle, name));
        else
            return nullptr;
    }

    FModule FModule::Current()
    {
        HMODULE hModule = NULL;
        if (::GetModuleHandleExW(/*GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS*/0, L"Xin.Core.dll", &hModule))
        {
			FModule Library;
            Library.Handle = hModule;

            FWString ModulePathW { 511 };
            DWORD dwLength = ::GetModuleFileNameW(hModule, ModulePathW.Data, DWORD(ModulePathW.Length));
            if (dwLength > ModulePathW.Length)
            {
                ModulePathW.Resize(dwLength);
                dwLength = ::GetModuleFileNameW(hModule, ModulePathW.Data, dwLength);
                AssertExpr(dwLength == ModulePathW.Length);
            }
            else
                ModulePathW.Resize(dwLength);

            ModulePathW.Replace(L"\\"V, L"/"V);
            Library.ModulePath = FEncoding::ToUtf8(ModulePathW);
            return Library;
        }
        return FModule {};
    }

    FModule FModule::Application()
    {
        HMODULE hModule = GetModuleHandleW(NULL);
        if (hModule)
        {
            FModule Library;
            Library.Handle = hModule;

            FWString ModulePathW { 511 };
            DWORD dwLength = ::GetModuleFileNameW(hModule, ModulePathW.Data, DWORD(ModulePathW.Length));
            if (dwLength > ModulePathW.Length)
            {
                ModulePathW.Resize(dwLength);
                dwLength = ::GetModuleFileNameW(hModule, ModulePathW.Data, dwLength);
                AssertExpr(dwLength == ModulePathW.Length);
            }
            else
                ModulePathW.Resize(dwLength);

            ModulePathW.Replace(L"\\"V, L"/"V);
            Library.ModulePath = FEncoding::ToUtf8(ModulePathW);
            return Library;
        }
        return FModule {};
    }
}
