//*****************************************************************************
//	File:   Scene2.cpp
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: Stub file for easy creation of new modules for this project
//*****************************************************************************

#include "Scene2.h"
#include "GfxMath.h"
#include "DckGfxEngine.h"

static glm::vec3 cyan(0.0f, 0.5f, 1.0f);

void Scene2Load()
{

}

void Scene2Init()
{
	DckESetBackColor(cyan);
}

void Scene2Update(float dt)
{
	if (DckEKeyIsTriggered(SDLK_F1))
		DckESetNextScene(SceneID::Scene1);

	else if (DckEKeyIsTriggered(SDLK_F2))
		DckESetNextScene(SceneID::SceneRestart);
}

void Scene2Shutdown()
{

}

void Scene2Unload()
{

}