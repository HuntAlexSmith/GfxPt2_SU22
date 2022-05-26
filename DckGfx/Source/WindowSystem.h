#pragma once
//*****************************************************************************
//	File:   WindowSystem.h
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: Window System for managing any SDL window stuff
//*****************************************************************************

#include "System.h"
#include "SDL2/SDL.h"
#include <string>

class WindowSystem : public System {
public:

	WindowSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	void GetWindowSize(int* w, int* h);

	~WindowSystem();

private:

	int width_;
	int height_;
	std::string name_;

	SDL_Window* window_;
	SDL_GLContext glContext_;

};