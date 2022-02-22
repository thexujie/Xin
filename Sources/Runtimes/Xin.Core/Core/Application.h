#pragma once

#include "Core.Types.h"
#include "ModuleInstance.h"

namespace Xin
{
	class CORE_API IModuleInstance : public ISharedRefer
	{
	public:
		IModuleInstance() = default;
		virtual ~IModuleInstance() = default;

		virtual bool InitializeModule() { return true; }
		virtual void FinalizeModule() {}

	public:
		FString Name;
	};
	using IModuleInstanceRef = TReferPtr<IModuleInstance>;

#define IMPLEMENT_APPLICATION_MODULE(ModuleClass) \
	extern "C" __declspec(dllexport) Xin::IModuleInstance * CreateModuleInstance() { return new ModuleClass(); }

	class CORE_API FApplication
	{
	public:
		FApplication();

		void RegisterService(const FTypeid & ServiceTypeid, TReferPtr<IRefer> Service)
		{
			Services[ServiceTypeid] = Service;
		}

		template<typename ServiceT>
		void RegisterService()
		{
			RegisterService(TTypeid<ServiceT>(), new ServiceT());
		}

		template<typename ServiceT, typename InstanceT>
		void RegisterService()
		{
			RegisterService(TTypeid<ServiceT>(), new InstanceT());
		}

		void UnregisterService(const FTypeid & ServiceTypeid);

		template<typename ServiceT>
		void UnregisterService()
		{
			UnregisterService(TTypeid<ServiceT>());
		}

		IRefer * FindService(const FTypeid & Typeid);
		template<typename ServiceT>
		ServiceT * FindServiceT() { return static_cast<ServiceT *>(FindService(TTypeid<ServiceT>())); }

		IRefer & FetchService(const FTypeid & Typeid)
		{
			IRefer * Service = FindService(Typeid);
			if (!Service)
				throw EError::Nullptr;
			return *Service;
		}
		template<typename ServiceT>
		ServiceT & FetchServiceT() { return static_cast<ServiceT &>(FetchService(TTypeid<ServiceT>())); }

		bool LoadModule(FStringV ModuleName);

	public:
		int32 Loop(TFunction<void()> OnIdle = nullptr);
		void Exit(int32 ExitCode);

	public:
		TMap<FTypeid, FTypeid> ServiceTypes;
		TMap<FTypeid, TReferPtr<IRefer>> Services;

		class CORE_API FModuleHandle : public ISharedRefer
		{
		public:
			FModuleHandle(FString ModuleName);
			~FModuleHandle();

			bool Initialize();

		public:
			FString ModuleName;
			IModuleInstanceRef ModuleInstance;
		public:
			void * ModuleHandle = nullptr;
			FString ModulePath;
		};
		TList<TReferPtr<FModuleHandle>> ModuleHandles;

	public:
		static FApplication & Instance();
	};
}
