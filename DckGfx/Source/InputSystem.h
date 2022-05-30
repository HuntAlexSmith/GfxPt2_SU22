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

	bool LMIsTriggered();
	bool LMIsDown();
	bool LMIsReleased();

	bool RMIsTriggered();
	bool RMIsDown();
	bool RMIsReleased();

	void GetMouseScreenPos(int* x, int* y);

	~InputSystem();

private:

	SDL_Event inputEvent_;
	std::map<SDL_Keycode, KeyState> inputMap_;

	int mouseX_, mouseY_;
	Uint8 mouseMask_;

	KeyState leftMouse_;
	KeyState rightMouse_;
};