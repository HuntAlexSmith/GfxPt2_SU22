#pragma once
//*****************************************************************************
//	File:   Engine.h
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: Engine class for containing the systems that will help manage
//		everything
//*****************************************************************************

#include "System.h"
#include "Mesh.h"

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

	void Render(Mesh* mesh, RenderType type, glm::mat4 modelMat);

	~Engine();

private:

	System* systems_[System::SysType::SystemCount];
	bool isRunning_;

};