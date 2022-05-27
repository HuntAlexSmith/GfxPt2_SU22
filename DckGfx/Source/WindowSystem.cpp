//*****************************************************************************
//	File:   WindowSystem.cpp
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: Window System for managing any SDL window stuff
//*****************************************************************************

#include "Engine.h"
#include "WindowSystem.h"
#include <stdexcept>
#include <iostream>

WindowSystem::WindowSystem() : System(SysType::WindowSys),
	width_(1280),
	height_(720),
	name_("DckGfx"),
	window_(nullptr),
	glContext_(nullptr)
{
}

void WindowSystem::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL Failed to Initialize" << std::endl;
		throw std::runtime_error("SDL Failed to Initialize");
	}
	SDL_GL_LoadLibrary(nullptr);

	// Set Window Attributes:
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	// Depth Buffer:
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Now create the window
	window_ = SDL_CreateWindow("DckGfx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_, height_, SDL_WINDOW_OPENGL);
	if (!window_)
	{
		std::cout << "Window failed to be created" << std::endl;
		throw std::runtime_error("Window failed to be created");
	}

	glContext_ = SDL_GL_CreateContext(window_);
	if (!glContext_)
	{
		std::cout << "OpenGL Context failed to be created" << std::endl;
		throw std::runtime_error("OpenGL Context failed to be created");
	}
}

void WindowSystem::Update(float dt)
{
	SDL_GL_SwapWindow(window_);
}

void WindowSystem::Shutdown()
{
	SDL_GL_DeleteContext(glContext_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void WindowSystem::GetWindowSize(int* w, int* h)
{
	SDL_GetWindowSize(window_, w, h);
}

WindowSystem::~WindowSystem()
{

}