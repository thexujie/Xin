#pragma once

#include "Core.Types.h"
#include "Dependency.h"

namespace Xin
{
	class CORE_API IFreezable : public IDependency
	{
	public:
        static FType & StaticType();
        FType & Type() const override { return StaticType(); }

	public:
        TEvent<void(IObject & Object)> Changed;
	};
}
