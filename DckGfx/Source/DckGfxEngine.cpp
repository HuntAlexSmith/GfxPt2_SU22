//*****************************************************************************
//	File:   DckGfxEngine.cpp
//  Author: Hunter Smith
//  Date:   05/27/2022
//  Description: File for containing the static engine object and having
//		funtions for the user to interact with the engine as needed.
//*****************************************************************************

#include "DckGfxEngine.h"
#include "InputSystem.h"
#include <iostream>
#include <stdexcept>

static Engine* theEngine;

void DckEInitialize()
{
	theEngine = new Engine();
	if (theEngine)
		theEngine->Initialize();
	else
	{
		std::cout << "Dck Engine failed to initialize" << std::endl;
		throw std::runtime_error("Dck Engine failed to initialize");
	}
}

void DckEUpdate(float dt)
{
	if (theEngine)
		theEngine->Update(dt);
}

void DckEShutdown()
{
	if (theEngine)
	{
		theEngine->Shutdown();
		delete theEngine;
	}
}

bool DckEIsRunning()
{
	if (theEngine)
		return theEngine->IsRunning();
}

void DckERender(Mesh* mesh, RenderType type, glm::mat4 modelMat)
{
	if (theEngine)
		theEngine->Render(mesh, type, modelMat);
}

//
//	KEYBOARD INPUT STUFF
//
bool DckEKeyIsTriggered(SDL_Keycode key)
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->KeyIsTriggered(key);
	return false;
}

bool DckEKeyIsDown(SDL_Keycode key)
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->KeyIsDown(key);
	return false;
}

bool DckEKeyIsReleased(SDL_Keycode key)
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->KeyIsReleased(key);
	return false;
}

//
//	MOUSE INPUT STUFF
//
bool DckELeftMouseIsTriggered()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->LMIsTriggered();
	return false;
}

bool DckELeftMouseIsDown()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->LMIsDown();
	return false;
}

bool DckELeftMouseIsReleased()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->LMIsReleased();
	return false;
}

bool DckERightMouseIsTriggered()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->RMIsTriggered();
	return false;
}

bool DckERightMouseIsDown()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->RMIsDown();
	return false;
}

bool DckERightMouseIsReleased()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->RMIsReleased();
	return false;
}
