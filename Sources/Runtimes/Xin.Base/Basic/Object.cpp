#include "PCH.h"
#include "Object.h"
#include "Meta/Type.h"

namespace Xin
{
	BASE_API constexpr FName GGetObjectTypeName_Debug(IObject * Object)
	{
		return Object->Type().Name;
	}

	IObject::IObject() : ObjectEntry(new FObjectEntry(this))
	{
		
	}

	IObject::IObject(FName Name) : Name(Name), ObjectEntry(new FObjectEntry(this))
	{
	}

	IObject::~IObject()
	{
		if (ObjectEntry)
		{
			AssertExpr(ObjectEntry->SharedRefer == 1);
			ObjectEntry->ReleaseSharedRefer();
			ObjectEntry = nullptr;
		}
	}
}
