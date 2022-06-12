//*****************************************************************************
//	File:   InputSystem.cpp
//  Author: Hunter Smith
//  Date:   05/27/2022
//  Description: System that will manage the states of input from the user
//*****************************************************************************

#include "InputSystem.h"
#include "WindowSystem.h"
#include "Engine.h"

typedef std::pair<SDL_Keycode, InputSystem::KeyState> Key;

InputSystem::InputSystem() : System(SysType::InputSys),
inputEvent_(),
inputMap_(),
mouseX_(0),
mouseY_(0),
mouseMask_(0),
leftMouse_(KeyState::None),
rightMouse_(KeyState::None)
{
}

void InputSystem::Initialize()
{
	// Create the keys we want to be read
	Key wKey(SDLK_w, KeyState::None);
	Key aKey(SDLK_a, KeyState::None);
	Key sKey(SDLK_s, KeyState::None);
	Key dKey(SDLK_d, KeyState::None);

	Key upKey(SDLK_UP, KeyState::None);
	Key leftKey(SDLK_LEFT, KeyState::None);
	Key rightKey(SDLK_RIGHT, KeyState::None);
	Key downKey(SDLK_DOWN, KeyState::None);

	Key f1Key(SDLK_F1, KeyState::None);
	Key f2Key(SDLK_F2, KeyState::None);
	Key f3Key(SDLK_F3, KeyState::None);
	Key f4Key(SDLK_F4, KeyState::None);

	// Add the keys to the input map
	inputMap_.insert(wKey);
	inputMap_.insert(aKey);
	inputMap_.insert(sKey);
	inputMap_.insert(dKey);

	inputMap_.insert(upKey);
	inputMap_.insert(leftKey);
	inputMap_.insert(rightKey);
	inputMap_.insert(downKey);

	inputMap_.insert(f1Key);
	inputMap_.insert(f2Key);
	inputMap_.insert(f3Key);
	inputMap_.insert(f4Key);
}

void InputSystem::Update(float dt)
{
	// Update any keys that are triggered or released
	for (auto& itr : inputMap_)
	{
		switch (itr.second)
		{
			// If the key was triggered, it is now down
		case KeyState::Triggered:
			itr.second = KeyState::Down;
			break;

			// If the key was released, it is now none
		case KeyState::Released:
			itr.second = KeyState::None;
			break;

			// Default case
		default:
			break;
		}
	}

	switch (leftMouse_)
	{
		case KeyState::Triggered:
			leftMouse_ = KeyState::Down;
			break;
		case KeyState::Released:
			leftMouse_ = KeyState::None;
			break;
		default:
			break;
	}

	switch (rightMouse_)
	{
		case KeyState::Triggered:
			rightMouse_ = KeyState::Down;
			break;
		case KeyState::Released:
			rightMouse_ = KeyState::None;
			break;
		default:
			break;
	}

	// Loop for processing all input events
	while (SDL_PollEvent(&inputEvent_))
	{
		// Check for the quit event
		if (inputEvent_.type == SDL_QUIT)
		{
			GetParent()->SetIsRunning(false);
			return;
		}

		switch (inputEvent_.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				if (inputEvent_.button.button == SDL_BUTTON_LEFT)
					leftMouse_ = KeyState::Triggered;
				if (inputEvent_.button.button == SDL_BUTTON_RIGHT)
					rightMouse_ = KeyState::Triggered;
				break;
			case SDL_MOUSEBUTTONUP:
				if (inputEvent_.button.button == SDL_BUTTON_LEFT)
					leftMouse_ = KeyState::Released;
				if (inputEvent_.button.button == SDL_BUTTON_RIGHT)
					rightMouse_ = KeyState::Released;
				break;
			case SDL_WINDOWEVENT:
				if (inputEvent_.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					WindowSystem* windowSys = dynamic_cast<WindowSystem*>(GetParent()->GetSystem(WindowSys));
					if (windowSys)
						windowSys->UpdateWindowSize(inputEvent_.window.data1, inputEvent_.window.data2);
				}
				break;
		}

		// Otherwise, check that the current key is being checked for input
		SDL_Keycode currentKey = inputEvent_.key.keysym.sym;
		auto result = inputMap_.find(currentKey);
		if (result != inputMap_.end())
		{
			switch (inputEvent_.type)
			{
				// If the event is keydown, the key is now triggered
				case SDL_KEYDOWN:
					result->second = KeyState::Triggered;
					break;

				// If the event is keyup, the key is now released
				case SDL_KEYUP:
					result->second = KeyState::Released;
					break;

				// Default case
				default:
					break;
			}
		}
	}
}

void InputSystem::Shutdown()
{
	inputMap_.clear();
}

bool InputSystem::KeyIsTriggered(SDL_Keycode key)
{
	auto keyItr = inputMap_.find(key);
	if (keyItr != inputMap_.end())
		if (keyItr->second == KeyState::Triggered)
			return true;
	return false;
}

bool InputSystem::KeyIsDown(SDL_Keycode key)
{
	auto keyItr = inputMap_.find(key);
	if (keyItr != inputMap_.end())
		if (keyItr->second == KeyState::Down)
			return true;
	return false;
}

bool InputSystem::KeyIsReleased(SDL_Keycode key)
{
	auto keyItr = inputMap_.find(key);
	if (keyItr != inputMap_.end())
		if (keyItr->second == KeyState::Released)
			return true;
	return false;
}

bool InputSystem::LMIsTriggered()
{
	return leftMouse_ == KeyState::Triggered;
}

bool InputSystem::LMIsDown()
{
	return leftMouse_ == KeyState::Down;
}

bool InputSystem::LMIsReleased()
{
	return leftMouse_ == KeyState::Released;
}

bool InputSystem::RMIsTriggered()
{
	return rightMouse_ == KeyState::Triggered;
}

bool InputSystem::RMIsDown()
{
	return rightMouse_ == KeyState::Down;
}

bool InputSystem::RMIsReleased()
{
	return rightMouse_ == KeyState::Released;
}

void InputSystem::GetMouseScreenPos(int* x, int* y)
{
	SDL_GetMouseState(x, y);
}

InputSystem::~InputSystem()
{

}
