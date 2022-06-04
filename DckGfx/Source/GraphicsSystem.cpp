//*****************************************************************************
//	File:   GraphicsSystem.cpp
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: System that will manage the graphics API calls, such as init
//		OpenGL
//*****************************************************************************

#include "GraphicsSystem.h"
#include "WindowSystem.h"
#include "Engine.h"
#include "SDL2/SDL.h"
#include "ShaderLib.h"
#include <stdexcept>
#include <iostream>

GraphicsSystem::GraphicsSystem() : System(SysType::GraphicsSys),
	activeShader_(nullptr),
	backColor_(glm::vec3(0.5, 0.5, 0.5))
{
}

void GraphicsSystem::Initialize()
{
	// Load OpenGL
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		std::cout << "OpenGL failed to be loaded" << std::endl;
		throw std::runtime_error("OpenGL failed to be loaded");
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Enable VSync
	int worked = SDL_GL_SetSwapInterval(0);

	// Get the current window size and set the viewport
	WindowSystem* windowSys = dynamic_cast<WindowSystem*>(GetParent()->GetSystem(SysType::WindowSys));
	if (windowSys)
	{
		int w, h;
		windowSys->GetWindowSize(&w, &h);
		glViewport(0, 0, w, h);
	}
}

void GraphicsSystem::Update(float dt)
{
	if (!activeShader_)
	{
		activeShader_ = ShaderLibraryGet("Default Shader");
	}

	// Make sure to clear the screen and the depth buffer
	glClearColor(backColor_.r, backColor_.g, backColor_.b, 1.0f);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Make sure to use the current active shader here, in case it has swapped somewhere else
	activeShader_->Use();
}

void GraphicsSystem::Shutdown()
{
}

void GraphicsSystem::SetActiveShader(Shader* shader)
{
	activeShader_ = shader;
	if (activeShader_)
		activeShader_->Use();
}

Shader* GraphicsSystem::GetActiveShader()
{
	return activeShader_;
}

GraphicsSystem::~GraphicsSystem()
{

}

void GraphicsSystem::SetBackColor(glm::vec3 newColor)
{
	backColor_ = newColor;
}

glm::vec3 GraphicsSystem::GetBackColor()
{
	return backColor_;
}
