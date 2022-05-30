//*****************************************************************************
//	File:   Engine.cpp
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: Engine class for containing the systems that will help manage
//		everything
//*****************************************************************************

#include "Engine.h"
#include "WindowSystem.h"
#include "InputSystem.h"
#include "CameraSystem.h"
#include "GraphicsSystem.h"
#include "RenderSystem.h"

Engine::Engine() : systems_{nullptr}, isRunning_(false)
{
	// Create the new systems
	System* windowSys = new WindowSystem();
	System* inputSys = new InputSystem();
	System* camSys = new CameraSystem();
	System* graphicSys = new GraphicsSystem();
	System* renderSys = new RenderSystem();

	// Add the systems to the engine
	AddSystem(windowSys);
	AddSystem(inputSys);
	AddSystem(camSys);
	AddSystem(graphicSys);
	AddSystem(renderSys);
}

void Engine::Initialize()
{
	for (System* sys : systems_)
	{
		if (sys)
			sys->Initialize();
	}
	isRunning_ = true;
}

void Engine::Update(float dt)
{
	for (System* sys : systems_)
	{
		if (sys)
			sys->Update(dt);
	}
}

void Engine::Shutdown()
{
	for (int i = System::SysType::SystemCount - 1; i >= 0; --i)
	{
		if (systems_[i])
			systems_[i]->Shutdown();
	}
}

System* Engine::GetSystem(System::SysType sysType)
{
	return systems_[sysType];
}

void Engine::AddSystem(System* newSys)
{
	if (newSys)
	{
		systems_[newSys->GetType()] = newSys;
		newSys->SetParent(this);
	}
}

bool Engine::IsRunning()
{
	return isRunning_;
}

void Engine::SetIsRunning(bool running)
{
	isRunning_ = running;
}

void Engine::Render(Mesh* mesh, RenderType type, glm::mat4 modelMat)
{
	RenderSystem* renderSys = dynamic_cast<RenderSystem*>(systems_[System::SysType::RenderSys]);
	if (renderSys)
		renderSys->Render(mesh, type, modelMat);
}

void Engine::DebugRender(Mesh* mesh, RenderType type, glm::mat4 modelMat)
{
	RenderSystem* renderSys = dynamic_cast<RenderSystem*>(systems_[System::SysType::RenderSys]);
	if (renderSys)
		renderSys->RenderDebug(mesh, type, modelMat);
}

Engine::~Engine()
{
	for (int i = System::SysType::SystemCount - 1; i >= 0; --i)
	{
		if (systems_[i])
		{
			delete systems_[i];
			systems_[i] = nullptr;
		}
	}
}
