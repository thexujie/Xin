#pragma once

#include "Timeline.h"

namespace Xin::UI
{
	class XIN_EXPORT FPropertyAnimation : public FTimeline
	{
	public:
		FPropertyAnimation(const FProperty & Property) : Property(Property) {}

	public:
		const FProperty & Property;
	};
}
