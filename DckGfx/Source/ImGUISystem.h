#pragma once
//*****************************************************************************
//	File:   ImGUISystem.h
//  Author: Hunter Smith
//  Date:   06/13/2022
//  Description: ImGUI System, which will manage everything ImGUI is doing
//*****************************************************************************

#include "System.h"

class ImGUISystem : public System {
public:

	ImGUISystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	void ProcessEvents(const SDL_Event* sdlEvent);

	~ImGUISystem();

private:



};