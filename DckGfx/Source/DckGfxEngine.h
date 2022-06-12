#pragma once
//*****************************************************************************
//	File:   DckGfxEngine.h
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: File for containing the static engine object and having
//		funtions for the user to interact with the engine as needed.
//*****************************************************************************

#include "Engine.h"
#include "MeshLib.h"
#include "SDL2/SDL.h"
#include "GfxMath.h"
#include "SceneList.h"

void DckEInitialize();
void DckEUpdate(float dt);
void DckEShutdown();

bool DckEIsRunning();

void DckERender(DckMesh* mesh, RenderType type, glm::mat4 modelMat,
				glm::vec3 tint = glm::vec3(0), glm::vec3 diff = glm::vec3(0), glm::vec3 spec = glm::vec3(0), float sExp = 0.0f);

bool DckEKeyIsTriggered(SDL_Keycode key);
bool DckEKeyIsDown(SDL_Keycode key);
bool DckEKeyIsReleased(SDL_Keycode key);

bool DckELeftMouseIsTriggered();
bool DckELeftMouseIsDown();
bool DckELeftMouseIsReleased();

bool DckERightMouseIsTriggered();
bool DckERightMouseIsDown();
bool DckERightMouseIsReleased();

void DckESetBackColor(glm::vec3 newColor);
glm::vec3 DckEGetBackColor();

void DckESetNextScene(SceneID nextScene);

void DckEAddLight(glm::vec4 pos, glm::vec3 color);
