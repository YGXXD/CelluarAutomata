#include "InputSystem.h"
#include "Actor.h"

InputSystem::InputSystem()
{
	KeyBoardState = SDL_GetKeyboardState(nullptr);
}

void InputSystem::Update(float DeltaSeconds)
{
	if(ActorWeak.expired())
		return;

	for(auto& InputInfo : KeyInputMap)
	{
		auto& Input = InputInfo.second;
		Input.CurrState = KeyBoardState[InputInfo.first] ? InputState::InPressing : InputState::InReleasing;
		if(Input.CurrState != Input.LastState)
		{
			switch (Input.CurrState)
		   	{
				case InputState::InPressing:
					Input.OnKeyDown();
					break;
				case InputState::InReleasing:
					Input.OnKeyUp();
					break;
				default:
					break;
			}
			Input.LastState = Input.CurrState;
		}
		else if(Input.CurrState == InputState::InPressing && Input.LastState == InputState::InPressing)
		{
			Input.OnKeying(DeltaSeconds);
		}
	}
}

void InputSystem::PossessActor(std::weak_ptr<Actor> Actor)
{
	KeyInputMap.clear();
	ActorWeak = Actor;
	if(!ActorWeak.expired())
	{
		ActorWeak.lock()->SetUpInput(this);
	}
}
