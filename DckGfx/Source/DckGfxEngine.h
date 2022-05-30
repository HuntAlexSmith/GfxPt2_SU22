#pragma once
//*****************************************************************************
//	File:   DckGfxEngine.h
//  Author: Hunter Smith
//  Date:   05/27/2022
//  Description: File for containing the static engine object and having
//		funtions for the user to interact with the engine as needed.
//*****************************************************************************

#include "Engine.h"
#include "Mesh.h"
#include "SDL2/SDL.h"

void DckEInitialize();
void DckEUpdate(float dt);
void DckEShutdown();

bool DckEIsRunning();

void DckERender(Mesh* mesh, RenderType type, glm::mat4 modelMat);

bool DckEKeyIsTriggered(SDL_Keycode key);
bool DckEKeyIsDown(SDL_Keycode key);
bool DckEKeyIsReleased(SDL_Keycode key);

bool DckELeftMouseIsTriggered();
bool DckELeftMouseIsDown();
bool DckELeftMouseIsReleased();

bool DckERightMouseIsTriggered();
bool DckERightMouseIsDown();
bool DckERightMouseIsReleased();
