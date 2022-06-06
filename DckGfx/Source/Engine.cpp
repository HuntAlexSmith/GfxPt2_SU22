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
#include "SceneSystem.h"
#include "GraphicsSystem.h"
#include "RenderSystem.h"

//*****************************************************************************
//  Description:
//		Constructor for the engine class, which created and adds any necessary
//		Systems
//*****************************************************************************
Engine::Engine() : systems_{nullptr}, isRunning_(false)
{
	// Create the new systems
	System* windowSys = new WindowSystem();
	System* inputSys = new InputSystem();
	System* camSys = new CameraSystem();
	System* sceneSys = new SceneSystem();
	System* graphicSys = new GraphicsSystem();
	System* renderSys = new RenderSystem();

	// Add the systems to the engine
	AddSystem(windowSys);
	AddSystem(inputSys);
	AddSystem(camSys);
	AddSystem(sceneSys);
	AddSystem(graphicSys);
	AddSystem(renderSys);
}

//*****************************************************************************
//  Description:
//		Initializes all the systems added in the constructor. Also sets that
//		the engine is running
//*****************************************************************************
void Engine::Initialize()
{
	for (System* sys : systems_)
	{
		if (sys)
			sys->Initialize();
	}
	isRunning_ = true;
}

//*****************************************************************************
//  Description:
//		Updates all the systems in the engine
// 
//	Param dt:
//		Delta time, used for updating the components relative to the framerate
//*****************************************************************************
void Engine::Update(float dt)
{
	for (System* sys : systems_)
	{
		if (sys)
			sys->Update(dt);
	}
}

//*****************************************************************************
//  Description:
//		Shuts all the systems down in the engine
//*****************************************************************************
void Engine::Shutdown()
{
	for (int i = System::SysType::SystemCount - 1; i >= 0; --i)
	{
		if (systems_[i])
			systems_[i]->Shutdown();
	}
}

//*****************************************************************************
//  Description:
//		Gets a certain system from the engine
// 
//	Param sysType:
//		The type of system wanted from the engine
// 
//	Return:
//		Returns a pointer to the system from the engine
//*****************************************************************************
System* Engine::GetSystem(System::SysType sysType)
{
	return systems_[sysType];
}

//*****************************************************************************
//  Description:
//		Adds a given system to the engine
// 
//	Param newSys:
//		Pointer to the system to be added to the engine
//*****************************************************************************
void Engine::AddSystem(System* newSys)
{
	if (newSys)
	{
		systems_[newSys->GetType()] = newSys;
		newSys->SetParent(this);
	}
}

//*****************************************************************************
//  Description:
//		Gets the current state of whether or not the engine is running
// 
//	Return:
//		Returns true if the engine is running, false otherwise
//*****************************************************************************
bool Engine::IsRunning()
{
	return isRunning_;
}

//*****************************************************************************
//  Description:
//		Sets the state of whether or not the engine is running
// 
//	Param running:
//		The state the engine running should go to next
//*****************************************************************************
void Engine::SetIsRunning(bool running)
{
	isRunning_ = running;
}

//*****************************************************************************
//  Description:
//		Renders a given mesh with certain parameters
// 
//	Param mesh:
//		The mesh to be rendered
// 
//	Param type:
//		How the mesh should be rendered (points, lines, triangles)
// 
//	Param modelMat:
//		The object to world transformation matrix
//*****************************************************************************
void Engine::Render(DckMesh* mesh, RenderType type, glm::mat4 modelMat)
{
	RenderSystem* renderSys = dynamic_cast<RenderSystem*>(systems_[System::SysType::RenderSys]);
	if (renderSys)
		renderSys->Render(mesh, type, modelMat);
}

//*****************************************************************************
//  Description:
//		Renders a given mesh with certain parameters as debug (will always be
//		drawn in front of objects)
//	
//	Param mesh:
//		The mesh to be rendered as debug
// 
//	Param type:
//		How the mesh should be rendered (points, lines, triangles)
// 
//	Param modelMat:
//		The Object to World transformation matrix
//*****************************************************************************
void Engine::DebugRender(DckMesh* mesh, RenderType type, glm::mat4 modelMat)
{
	RenderSystem* renderSys = dynamic_cast<RenderSystem*>(systems_[System::SysType::RenderSys]);
	if (renderSys)
		renderSys->RenderDebug(mesh, type, modelMat);
}

//*****************************************************************************
//  Description:
//		Destructor for the engine, deallocating any data allocated to systems
//*****************************************************************************
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
