#pragma once

#include "Xin.UI.Types.h"

namespace Xin::UI
{
	class UObject;
	struct FTimelineTime
	{
		uint32 CurrentTime;
		uint32 Duration;
	};

	class XIN_EXPORT FTimeline : public IAnimatable
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		FTimeline() = default;
		~FTimeline() {}

		virtual void Activate(IDependency & Object) { Activated = true; }
		virtual void Begin(IDependency & Object) { Begun = true; }
		virtual void End(IDependency & Object) { Begun = false; }
		virtual void Inactivate(IDependency & Object) { Activated = false; }

		virtual void ApplyTime(IDependency & Object, const FTimelineTime & Time) = 0;

	public:
		bool Activated = false;
		bool Begun = false;

	public:
		ProxyPropertyRW(uint32, Start) = 0;
		ProxyPropertyRW(uint32, Duration) = 0;
		ProxyPropertyRW(bool, AutoReverse) = false;
	};

	using FTimelineRef = TReferPtr<FTimeline>;
}
