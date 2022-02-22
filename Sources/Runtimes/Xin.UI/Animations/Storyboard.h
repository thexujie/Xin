#pragma once

#include "Timeline.h"
#include "Core/Trigger.h"

namespace Xin::UI
{
	class XIN_EXPORT FStoryboard : public FTimeline
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		FStoryboard(IDependency & Object) : Object(Object) {}
		~FStoryboard();

		void Play();
		void Stop();

	public:
		void OnTimer();
		void ApplyTime(IDependency & Object, const FTimelineTime & Time) override;

	private:
		void StopTimer();

	public:
		IDependency & Object;

	public:
		TList<FTimelineRef> Timelines;
		uint32 StartTime = 0;

	public:
		TEvent<void()> Completed;

	private:
		bool Reversing = false;
		uint32 AvialiableDuration = 0;

	private:
		FDispatcherTimer Timer;
	};
	using FStoryboardRef = TReferPtr<FStoryboard>;

	class XIN_EXPORT FBeginStoryboardAction : public UI::ITriggerAction
	{
	public:
		FBeginStoryboardAction(FStoryboardRef Storyboard) : Storyboard(Storyboard) {}

		void Play(UObject & Object) override
		{
			Storyboard->Play();
		}

		void Stop(UObject & Object) override
		{
			Storyboard->Stop();
		}

	public:
		FStoryboardRef Storyboard;
	};
}
