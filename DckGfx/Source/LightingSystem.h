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
#include "glad/glad.h"
#include "RenderObject.h"

static const int maxLights = 10;

class LightingSystem : public System {
public:

	LightingSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	void AddLight(glm::vec4 pos, glm::vec3 color);

	void ClearLights();

	bool IsActive();

	~LightingSystem();

private:

	// Render object for Rendering a cube at the lights
	RenderObject* cubeLight_;
	
	// Shader for lighting
	Shader* phongShader_;

	// Static array of lights (pos and color
	glm::vec4 lightPos_[maxLights];
	glm::vec3 lightColor_[maxLights];

	// Ambient color
	glm::vec3 ambientColor_;

	// How many lights are being used
	int lightCount_;

};