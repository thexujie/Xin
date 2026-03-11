#include "PCH.h"
#include "Resource.h"

namespace Xin::UI
{
	FResourceTable::FResourceTable(TInitializerList<TReferPtr<FResourceTable>> ResourceLists)
		: ResourceLists(ResourceLists)
	{
		
	}

	IResourceRef FResourceTable::FindResource(FName Name, const FType & ResourceType) const
	{
		for (const IResourceRef & Resource : Resources)
		{
			if (Resource->Name == Name && (!ResourceType || Resource->Type().IsA(ResourceType)))
				return Resource;
		}

		for (const FResourceTableRef & ResourceList : ResourceLists)
		{
			if (auto Resource = ResourceList->FindResource(Name, ResourceType))
				return Resource;
		}
		return nullptr;
	}
}
