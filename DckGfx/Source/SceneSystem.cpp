//*****************************************************************************
//	File:   SceneSystem.cpp
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: Scene System that will manage the different Scenes that the
//		renderer will render
//*****************************************************************************

#include "SceneSystem.h"
#include "LightingSystem.h"
#include "Engine.h"
#include "Scene1.h"
#include "Scene2.h"

SceneSystem::SceneSystem() : System(SysType::SceneSys),
	scenes_(),
	prevScene_(SceneID::SceneInvalid),
	currScene_(SceneID::SceneInvalid),
	nextScene_(SceneID::SceneInvalid)
{
}

void SceneSystem::Initialize()
{
	scenes_[SceneID::Scene1] = Scene(Scene1Load, Scene1Init, Scene1Update, Scene1Shutdown, Scene1Unload);
	scenes_[SceneID::Scene2] = Scene(Scene2Load, Scene2Init, Scene2Update, Scene2Shutdown, Scene2Unload);

	nextScene_ = SceneID::Scene1;
}

void SceneSystem::Update(float dt)
{
	// If we are switching scenes, perform the logic for doing so
	if (currScene_ != nextScene_)
	{
		// Get lighting system and clear the lights
		LightingSystem* lightSys = dynamic_cast<LightingSystem*>(GetParent()->GetSystem(LightingSys));
		if (lightSys)
			lightSys->ClearLights();

		// If valid, shutdown
		if (SceneIsValid(currScene_))
			scenes_[currScene_].shutdown();

		// If we are restarting, just set the next scene to the current one
		if (nextScene_ == SceneID::SceneRestart)
			nextScene_ = currScene_;

		// Otherwise, unload the current one and load the new one
		else
		{
			// If valid, unload current scene
			if (SceneIsValid(currScene_))
				scenes_[currScene_].unload();
			
			// Update scenes
			prevScene_ = currScene_;
			currScene_ = nextScene_;

			// If valid, Load the current scene
			if (SceneIsValid(currScene_))
				scenes_[currScene_].load();
		}

		// If valid, initialize the current scene
		if (SceneIsValid(currScene_))
			scenes_[currScene_].init();
	}

	// If valid, update the current scene
	if (SceneIsValid(currScene_))
		scenes_[currScene_].update(dt);
}

void SceneSystem::Shutdown()
{
	// On shutdown, we need to make sure to shutdown and unload the current scene
	if (SceneIsValid(currScene_))
	{
		scenes_[currScene_].shutdown();
		scenes_[currScene_].unload();
	}
}

void SceneSystem::SetNextScene(SceneID nextScene)
{
	if(SceneIsValid(nextScene))
		nextScene_ = nextScene;
}

SceneSystem::~SceneSystem()
{

}

bool SceneSystem::SceneIsValid(SceneID scene)
{
	if (scene > SceneID::SceneInvalid && scene < SceneID::SceneCount)
		return true;
	return false;
}
