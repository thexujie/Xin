#include "PCH.h"
#include "Resource.h"

namespace Xin::UI
{
	IResourceRef FResourceTable::FindResource(FName Name, const FType & ResourceType)
	{
		for (IResourceRef & Resource : Resources)
		{
			if (Resource->Name == Name && (!ResourceType || Resource->Type().IsA(ResourceType)))
				return Resource;
		}

		for (FResourceTableRef ResourceSheet : Tables)
		{
			if (auto Resource = ResourceSheet->FindResource(Name, ResourceType))
				return Resource;
		}
		return nullptr;
	}
}
