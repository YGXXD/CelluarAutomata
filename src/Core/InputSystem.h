#pragma once
#include "../Template/Delegate.h"
#include <SDL.h>
#include <memory>
#include <unordered_map>

DECLARE_FUNCTION_DELEGATE_NO_PARAMETER(ActionInputDelegate, void);;
DECLARE_FUNCTION_DELEGATE(AxisInputDelegate, void, float);

enum class InputType : uint8_t 
{
	OnPressed,
	OnReleased,
	MaxValue
};

enum class InputState : uint8_t
{
	InReleasing,
	InPressing,
	MaxValue
};

struct KeyInput
{
	InputState LastState;
	InputState CurrState;

	ActionInputDelegate OnKeyDown;
	ActionInputDelegate OnKeyUp;
	AxisInputDelegate OnKeying;
};

class Actor;
class InputSystem
{
public:
	InputSystem();
	~InputSystem() = default;

	void Update(float DeltaSeconds);

	template<class T>
	void AxisBind(uint8_t Id, T* pObj, void(T::*Func)(float))	
	{
		if(!KeyInputMap.count(Id))
		{
			KeyInput& NewInput = KeyInputMap[Id];
			NewInput.CurrState = KeyBoardState[Id] ? InputState::InPressing : InputState::InReleasing;
			NewInput.LastState = NewInput.CurrState;
		}
		KeyInputMap[Id].OnKeying.Bind(pObj, Func);
	}

	void AxisBind(uint8_t Id, void(*Func)(float))	
	{
		if(!KeyInputMap.count(Id))
		{
			SDL_PumpEvents();
			KeyInput& NewInput = KeyInputMap[Id];
			NewInput.CurrState = KeyBoardState[Id] ? InputState::InPressing : InputState::InReleasing;
			NewInput.LastState = NewInput.CurrState;
		}
		KeyInputMap[Id].OnKeying.Bind(Func);
	}

	template<class T>
	void ActionBind(uint8_t Id, InputType Type, T* pObj, void(T::*Func)())
	{
		if(!KeyInputMap.count(Id))
		{
			KeyInput& NewInput = KeyInputMap[Id];
			NewInput.CurrState = KeyBoardState[Id] ? InputState::InPressing : InputState::InReleasing;
			NewInput.LastState = NewInput.CurrState;
		}
		switch (Type)
	   	{
			case InputType::OnPressed:
				KeyInputMap[Id].OnKeyDown.Bind(pObj, Func);
				break;
			case InputType::OnReleased:
				KeyInputMap[Id].OnKeyUp.Bind(pObj, Func);
				break;
			default:
				break;
		}
	}

	void ActionBind(uint8_t Id, InputType Type, void(*Func)())
	{
		if(!KeyInputMap.count(Id))
		{
			KeyInput& NewInput = KeyInputMap[Id];
			NewInput.CurrState = KeyBoardState[Id] ? InputState::InPressing : InputState::InReleasing;
			NewInput.LastState = NewInput.CurrState;
		}
		switch (Type)
	   	{
			case InputType::OnPressed:
				KeyInputMap[Id].OnKeyDown.Bind(Func);
				break;
			case InputType::OnReleased:
				KeyInputMap[Id].OnKeyUp.Bind(Func);
				break;
			default:
				break;
		}
	}

	void PossessActor(std::weak_ptr<Actor> Actor);
private:
	const uint8_t* KeyBoardState;
	std::weak_ptr<Actor> ActorWeak;
	std::unordered_map<uint8_t, KeyInput> KeyInputMap;
};
