#pragma once
//*****************************************************************************
//	File:   Engine.h
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: Engine class for containing the systems that will help manage
//		everything
//*****************************************************************************

#include "System.h"
#include "MeshLib.h"

//*****************************************************************************
//  Description:
//		The engine class, which contains systems that will be allocated,
//		initialized, updated, shutdown, and deallocated by the engine.
//*****************************************************************************
class Engine {
public:

	Engine();

	void Initialize();
	void Update(float dt);
	void Shutdown();

	System* GetSystem(System::SysType sysType);
	void AddSystem(System* newSys);

	bool IsRunning();
	void SetIsRunning(bool running);

	void Render(DckMesh* mesh, RenderType type, glm::mat4 modelMat);
	void DebugRender(DckMesh* mesh, RenderType type, glm::mat4 modelMat);

	~Engine();

private:

	System* systems_[System::SysType::SystemCount]; // The container of systems
	bool isRunning_;                                // Whether or not the engine is running

};