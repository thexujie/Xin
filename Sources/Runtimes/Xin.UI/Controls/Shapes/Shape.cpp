#include "PCH.h"
#include "Shape.h"

namespace Xin::UI
{
	void UShape::OnConstruct()
	{
		UElement::OnConstruct();
		HitInside = EHitResult::Ignore;
	}
}

