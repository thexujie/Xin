#include "PCH.h"
#include "Object.h"

namespace Xin::UI
{
	UObject::UObject(FName Name) : IDependency(Name) { }

	UObject::~UObject()
	{
		Bindings.Reset();
		Triggers.Reset();
		PropertyValues.Reset();
	}

	void UObject::Construct()
	{
		if (!Constructed)
		{
			OnConstruct();
			Constructed = true;
			OnConstructed();
		}
	}

	void UObject::OnConstruct()
	{
	}

	void UObject::OnConstructed()
	{
		
	}

	void UObject::Initialize()
	{
		if (!Constructed)
			OnConstruct();

		if (!Initialized)
		{
			Initializing = true;
			OnInitialize();

			Initializing = false;
			OnInitialized();
		}
	}

	void UObject::OnInitialize()
	{
		Initialized = true;
	}

	void UObject::OnInitialized()
	{
	}

	void UObject::Finalize()
	{
		if (Initialized)
		{
			Finalizing = true;
			OnFinalize();

			Finalizing = false;
			OnFinalized();
		}
	}

	void UObject::OnFinalize()
	{
		Initialized = false;
	}

	void UObject::OnFinalized()
	{

	}
}
