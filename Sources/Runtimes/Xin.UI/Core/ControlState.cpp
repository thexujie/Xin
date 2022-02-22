#include "PCH.h"
#include "ControlState.h"

namespace Xin::UI
{
	void FControlState::PlayStoryboards()
	{
		for (FStoryboardRef & Storyboard : Storyboards)
			Storyboard->Play();
	}

	void FControlState::StopStoryboards()
	{
		for (FStoryboardRef & Storyboard : Storyboards)
			Storyboard->Stop();
	}

	void FControlStateGroup::SetCurrentState(FName StateName)
	{
		for (FControlStateRef & ControlState : ControlStates)
		{
			if (ControlState->Name == StateName)
			{
				SetCurrentState(ControlState);
				return;
			}
		}
		SetCurrentState(FControlStateRef { nullptr });
	}

	void FControlStateGroup::SetCurrentState(FControlStateRef VisualState)
	{
		if (VisualState == CurrentState)
			return;

		if (CurrentState)
			CurrentState->StopStoryboards();
		CurrentState = VisualState;
		if (CurrentState)
			CurrentState->PlayStoryboards();
	}

	void FControlStateGroup::ClearState()
	{
		if (CurrentState)
		{
			CurrentState->StopStoryboards();
			CurrentState = nullptr;
		}
	}

	void FControlStateManager::ClearStates()
	{
		for (FVisualStateGroupRef & VisualStateGroup : StateGroups)
			VisualStateGroup->ClearState();
	}
}
