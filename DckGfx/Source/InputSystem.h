#pragma once
//*****************************************************************************
//	File:   InputSystem.h
//  Author: Hunter Smith
//  Date:   05/27/2022
//  Description: System that will manage the states of input from the user
//*****************************************************************************

#include "System.h"
#include "SDL2/SDL.h"
#include <map>

class InputSystem : public System {
public:

	enum KeyState {
		None,
		Triggered,
		Down,
		Released
	};

	InputSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	bool KeyIsTriggered(SDL_Keycode key);
	bool KeyIsDown(SDL_Keycode key);
	bool KeyIsReleased(SDL_Keycode key);

	~InputSystem();

private:

	SDL_Event inputEvent_;
	std::map<SDL_Keycode, KeyState> inputMap_;
};