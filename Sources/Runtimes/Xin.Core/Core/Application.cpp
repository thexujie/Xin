#include "PCH.h"
#include "Application.h"
#include "Module.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
	FApplication::FModuleHandle::FModuleHandle(FString ModuleName) : ModuleName(ModuleName)
	{
		
	}

	FApplication::FModuleHandle::~FModuleHandle()
	{
		if (ModuleHandle)
		{
			::FreeLibrary(HMODULE(ModuleHandle));
			ModuleHandle = nullptr;
		}
	}

	bool FApplication::FModuleHandle::Initialize()
	{
#if XIN_WINDOWS
		const bool bUserDir = false;
		FWStringV PathW = FEncoding::ToWide(ModuleName);
		if (bUserDir)
			ModuleHandle = ::LoadLibraryExW(PathW.GetData(), NULL, LOAD_LIBRARY_SEARCH_USER_DIRS);
		else
			ModuleHandle = ::LoadLibraryW(PathW);

		if (!ModuleHandle)
		{
			LogWarning(u8"Load module \"{}\" failed."V, ModuleName);
			return false;
		}
		else
		{
			wchar_t ModuleFilePathW[1024] = {};
			uint32 Length = GetModuleFileNameW(HMODULE(ModuleHandle), ModuleFilePathW, 1024);
			ModulePath = FEncoding::ToUtf8(ModuleFilePathW, Length);
		}


		if (auto CreateModuleInstance = (IModuleInstance *(*)())GetProcAddress(HMODULE(ModuleHandle), "CreateModuleInstance"))
			ModuleInstance = CreateModuleInstance();
		return ModuleInstance != nullptr;
#else
		return false;
#endif
	}

	FApplication & FApplication::Instance()
	{
		static FApplication GApplication;
		return GApplication;
	}


	FApplication::FApplication()
	{
	}

	void FApplication::UnregisterService(const FTypeid & ServiceTypeid)
	{
		auto ServiceIter = Services.Find(ServiceTypeid);
		if (ServiceIter != Services.End())
			Services.Erase(ServiceIter);
	}

	IRefer * FApplication::FindService(const FTypeid & Typeid)
	{
		if (!Typeid)
			return nullptr;

		auto ServiceIter = Services.Find(Typeid);
		if (ServiceIter != Services.End())
			return ServiceIter->Second.Get();

		auto ServiceTypeIter = ServiceTypes.Find(Typeid);
		if (ServiceTypeIter == ServiceTypes.End())
			return nullptr;
		return nullptr;

		//IService * Pointer = (IService *)Malloc(InstanceType.GetSize(), InstanceType.GetAlignment());
		//InstanceType.Construct(Pointer);
		//Pointer->Initialize();
		//TSharedPtr<IService> Service = TSharedPtr<IService>(Pointer, [](IService * Service)
		//{
		//	Service->Finalize();
		//	delete Service;
		//});
		//Services[Type] = Service;
		//return Service.Get();
	}

	bool FApplication::LoadModule(FStringV ModuleName)
	{
		TReferPtr<FModuleHandle> ApplicationModule = new FModuleHandle(ModuleName);
		if (ApplicationModule->Initialize())
		{
			ModuleHandles.Add(ApplicationModule);
			return true;
		}
		return true;
	}
}
