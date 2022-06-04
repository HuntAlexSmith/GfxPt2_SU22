#pragma once
//*****************************************************************************
//	File:   LightingSystem.h
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: LightingSystem that will manage light locations, colors, and
//		uploading that data to the GPU
//*****************************************************************************

#include "System.h"
#include "GfxMath.h"

static const int maxLights = 10;

class LightingSystem : public System {
public:

	struct Light {
		glm::vec4 lightPos;
		glm::vec3 lightColor;

		Light() : lightPos(0), lightColor(0) {}

		Light(glm::vec4 pos, glm::vec3 color) : lightPos(pos), lightColor(color) {}
	};

	LightingSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	~LightingSystem();

private:

	// Static array of lights
	Light lights_[maxLights];

	// How many lights are being used
	int lightCount_;

};