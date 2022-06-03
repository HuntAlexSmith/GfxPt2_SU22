//*****************************************************************************
//	File:   Main.cpp
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: It's main, not much else to say
//*****************************************************************************
#include <iostream>
#include <string>
#include <stdexcept>
#include "SDL2/SDL.h"
#include "glad/glad.h"
#include "GfxMath.h"
#include "FileReader.h"
#include "Shader.h"
#include "Mesh.h"
#include "DckGfxEngine.h"

// Delta time stuff
Uint32 lastTick = 0;
Uint32 currentTick = 0;
float dt = 0;

int main(int argc, char* argv[]) {

	// Create and initialize the engine
	DckEInitialize();

	// Main loop
	while (DckEIsRunning())
	{
		// Calculate dt
		lastTick = currentTick;
		currentTick = SDL_GetTicks();
		dt = (currentTick - lastTick) / 1000.0f;
		glm::clamp(dt, 0.0f, 0.125f);
		
		// Update the engine
		DckEUpdate(dt);
	}

	// Shut the engine down
	DckEShutdown();

	return 0;
}
