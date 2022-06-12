//*****************************************************************************
//	File:   DckGfxEngine.cpp
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: File for containing the static engine object and having
//		funtions for the user to interact with the engine as needed.
//*****************************************************************************

#include "DckGfxEngine.h"
#include "InputSystem.h"
#include "LightingSystem.h"
#include "GraphicsSystem.h"
#include "SceneSystem.h"
#include "ShaderLib.h"
#include "MeshLib.h"
#include <iostream>
#include <stdexcept>

// Static declaration, so can't be accessed by other files
static Engine* theEngine;

//*****************************************************************************
//  Description:
//		Initializes the engine, allocating memory to the engine
//		and initializing the engine. Throws exception if error occurs
//*****************************************************************************
void DckEInitialize()
{
	theEngine = new Engine();
	if (theEngine)
	{
		theEngine->Initialize();
		ShaderLibraryInit();
		MeshLibraryInit();
	}
	else
	{
		std::cout << "Dck Engine failed to initialize" << std::endl;
		throw std::runtime_error("Dck Engine failed to initialize");
	}
}

//*****************************************************************************
//  Description:
//		Updates the engine and all of the engine components
// 
//	Param dt:
//		Delta time for updating the components
//*****************************************************************************
void DckEUpdate(float dt)
{
	if (theEngine)
		theEngine->Update(dt);
}

//*****************************************************************************
//  Description:
//		Shuts the engine and all of its systems down, freeing the 
//		allocated eninge afterwards
//*****************************************************************************
void DckEShutdown()
{
	if (theEngine)
	{
		MeshLibraryShutdown();
		ShaderLibraryShutdown();
		theEngine->Shutdown();
		delete theEngine;
	}
}

//*****************************************************************************
//  Description:
//		Returns whether or not the engine is currently running or not
// 
//	Return:
//		Returns a boolean saying whether or not the engine is running
//*****************************************************************************
bool DckEIsRunning()
{
	if (theEngine)
		return theEngine->IsRunning();
}

//*****************************************************************************
//  Description:
//		Renders a mesh with given parameters
//	
//	Param mesh:
//		The mesh that is to be rendered
// 
//	Param type:
//		How the mesh will be rendered (points, lines, or triangles)
// 
//	Param modelMat:
//		The Object to World matrix for the mesh
//*****************************************************************************
void DckERender(DckMesh* mesh, RenderType type, glm::mat4 modelMat,
				glm::vec3 tint, glm::vec3 diff, glm::vec3 spec, float sExp)
{
	if (theEngine)
		theEngine->Render(mesh, type, modelMat, tint, diff, spec, sExp);
}

//*****************************************************************************
//  Description:
//		Checks if the given key is in the triggered state
//	
//	Param key:
//		The key to check the triggered state of
// 
//	Return:
//		Returns true if the key is triggered, false otherwise
//*****************************************************************************
bool DckEKeyIsTriggered(SDL_Keycode key)
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->KeyIsTriggered(key);
	return false;
}

//*****************************************************************************
//  Description:
//		Checks if the given key is in the down state or not
//	
//	Param key:
//		The key to check the down state of
// 
//	Return:
//		Returns true if the key is triggered, false otherwise
//*****************************************************************************
bool DckEKeyIsDown(SDL_Keycode key)
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->KeyIsDown(key);
	return false;
}

//*****************************************************************************
//  Description:
//		Checks if the given key is in the released state or not
//	
//	Param key:
//		The key to check the released state of
// 
//	Return:
//		Returns true if the key is released, false otherwise
//*****************************************************************************
bool DckEKeyIsReleased(SDL_Keycode key)
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->KeyIsReleased(key);
	return false;
}

//*****************************************************************************
//  Description:
//		Checks if the left mouse button is triggered or not
//	
//	Return:
//		Returns true if the left mouse is triggered, false otherwise
//*****************************************************************************
bool DckELeftMouseIsTriggered()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->LMIsTriggered();
	return false;
}

//*****************************************************************************
//  Description:
//		Checks if the left mouse button is down or not
//	
//	Return:
//		Returns true if the left mouse is down, false otherwise
//*****************************************************************************
bool DckELeftMouseIsDown()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->LMIsDown();
	return false;
}

//*****************************************************************************
//  Description:
//		Checks if the left mouse button is released or not
//	
//	Return:
//		Returns true if the left mouse is released, false otherwise
//*****************************************************************************
bool DckELeftMouseIsReleased()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->LMIsReleased();
	return false;
}

//*****************************************************************************
//  Description:
//		Checks if the right mouse button is triggered or not
//	
//	Return:
//		Returns true if the right mouse is triggered, false otherwise
//*****************************************************************************
bool DckERightMouseIsTriggered()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->RMIsTriggered();
	return false;
}

//*****************************************************************************
//  Description:
//		Checks if the right mouse button is down or not
//	
//	Return:
//		Returns true if the right mouse is down, false otherwise
//*****************************************************************************
bool DckERightMouseIsDown()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->RMIsDown();
	return false;
}

//*****************************************************************************
//  Description:
//		Checks if the right mouse button is released or not
//	
//	Return:
//		Returns true if the right mouse is released, false otherwise
//*****************************************************************************
bool DckERightMouseIsReleased()
{
	InputSystem* inputSys = dynamic_cast<InputSystem*>(theEngine->GetSystem(System::SysType::InputSys));
	if (inputSys)
		return inputSys->RMIsReleased();
	return false;
}

//*****************************************************************************
//  Description:
//		Sets the current background color
// 
//	Param newColor:
//		The new color to set the background to (rgb)
//*****************************************************************************
void DckESetBackColor(glm::vec3 newColor)
{
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(theEngine->GetSystem(System::SysType::GraphicsSys));
	if (graphics)
		graphics->SetBackColor(newColor);
}

//*****************************************************************************
//  Description:
//		Gets the current background color
//	
//	Return:
//		Returns the current background color (rgb). Returns black if something
//		went wrong
//*****************************************************************************
glm::vec3 DckEGetBackColor()
{
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(theEngine->GetSystem(System::SysType::GraphicsSys));
	if (graphics)
		return graphics->GetBackColor();
	return glm::vec3(0);
}

//*****************************************************************************
//  Description:
//		Sets the next scene to go to
//	
//	Param nextScene:
//		The scene id of the next scene we want
//*****************************************************************************
void DckESetNextScene(SceneID nextScene)
{
	SceneSystem* sceneSys = dynamic_cast<SceneSystem*>(theEngine->GetSystem(System::SysType::SceneSys));
	if (sceneSys)
		sceneSys->SetNextScene(nextScene);
}

void DckEAddLight(glm::vec4 pos, glm::vec3 color)
{
	LightingSystem* lightSys = dynamic_cast<LightingSystem*>(theEngine->GetSystem(System::SysType::LightingSys));
	if (lightSys)
		lightSys->AddLight(pos, color);
}
