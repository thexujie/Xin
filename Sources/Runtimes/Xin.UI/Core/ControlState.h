#pragma once

#include "../Xin.UI.Types.h"
#include "../Animations/Storyboard.h"

namespace Xin::UI
{
	struct UI_API FVisualStateName
	{
		static constexpr inline FName CommonStatesGroupName = u8"CommonStates";

		FName GroupName;
		FName StateName;

		FVisualStateName() = default;
		FVisualStateName(FName StateName) : GroupName(CommonStatesGroupName), StateName(StateName) {  }
		FVisualStateName(FName GroupName, FName StateName) : GroupName(GroupName), StateName(StateName) {  }
	};

	class UI_API FControlState : public IDependency
	{
	public:
		FControlState(FName Name, TView<FStoryboardRef> Storyboards) : IDependency(Name), Storyboards(Storyboards) {}

	public:
		void PlayStoryboards();
		void StopStoryboards();

	public:
		TList<FStoryboardRef> Storyboards;
	};
	using FControlStateRef = TReferPtr<FControlState>;

	class UI_API FControlStateGroup : public IDependency
	{
	public:
		FControlStateGroup(FName Name) : IDependency(Name) {}

	public:
		void AddVisualState(FControlStateRef ControlState)
		{
			ControlStates.Add(ControlState);
		}

		void AddVisualState(FName Name, TView<FStoryboardRef> Storyboards)
		{
			AssertExpr(FindIf(ControlStates, [Name](const FControlStateRef & ControlState) { return ControlState->Name == Name; }) == ControlStates.End());
			ControlStates.Add(MakeRefer<FControlState>(Name, Storyboards));
		}
	public:
		void SetCurrentState(FName StateName);
		void SetCurrentState(FControlStateRef CurrentState);
		void ClearState();

	public:
		FControlStateRef CurrentState = nullptr;
		TList<FControlStateRef> ControlStates;
	};
	using FVisualStateGroupRef = TReferPtr<FControlStateGroup>;

	class FControlStateManager : public IDependency
	{
	public:
		FControlStateManager() = default;

		void ClearStates();

		void ClearStateGroups()
		{
			ClearStates();
			StateGroups.Clear();
		}

	public:
		TList<FVisualStateGroupRef> StateGroups;
	};
	using FControlStateManagerRef = TReferPtr<FControlStateManager>;
}
