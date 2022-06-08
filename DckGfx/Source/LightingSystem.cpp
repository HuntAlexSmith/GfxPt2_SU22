//*****************************************************************************
//	File:   LightingSystem.cpp
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: LightingSystem that will manage light locations, colors, and
//		uploading that data to the GPU
//*****************************************************************************

#include "Engine.h"
#include "CameraSystem.h"
#include "LightingSystem.h"
#include "GraphicsSystem.h"
#include "RenderSystem.h"
#include "ShaderLib.h"

LightingSystem::LightingSystem() : System(SysType::LightingSys), cubeLight_(nullptr), phongShader_(nullptr), lightPos_(), lightColor_(), ambientColor_(0.25f), lightCount_(0)
{
}

void LightingSystem::Initialize()
{
	lightCount_++;
	lightPos_[0] = GfxMath::Point(-3, 5, -10);
	lightColor_[0] = glm::vec3(0, 1, 1);

	lightCount_++;
	lightPos_[1] = GfxMath::Point(-3, 5, 10);
	lightColor_[1] = glm::vec3(1, 1, 0);
}

void LightingSystem::Update(float dt)
{
	// Make sure to get what shader lighting will be using
	if (!phongShader_)
		phongShader_ = ShaderLibraryGet("Phong Shader");

	// If there isn't a cube for the lights, create it
	if (!cubeLight_)
	{
		cubeLight_ = new RenderObject();
		cubeLight_->SetMesh(MeshLibraryGet("Cube"));
		cubeLight_->SetScale(glm::vec3(0.25f));
	}

	glm::vec4 camEyePoint(0);

	// Get the camera system so we can know what the eye point is
	CameraSystem* camSys = dynamic_cast<CameraSystem*>(GetParent()->GetSystem(SysType::CameraSys));
	if (camSys)
	{
		Camera* activeCam = camSys->GetActiveCamera();
		if (activeCam)
			camEyePoint = activeCam->GetEyePoint();
	}

	// Get the graphics system to check for the current active program
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(GetParent()->GetSystem(SysType::GraphicsSys));
	if (graphics)
	{
		// If the current shader is the Phong shader, time to upload data
		if (phongShader_ == graphics->GetActiveShader())
		{
			// Get all the uniform locations we need
			GLint uEyePos = phongShader_->GetUniformLocation("eyePos");
			GLint uAmbientColor = phongShader_->GetUniformLocation("ambientColor");
			GLint uLightCount = phongShader_->GetUniformLocation("lightCount");
			GLint uLightPos = phongShader_->GetUniformLocation("lightPos[0]");
			GLint uLightColor = phongShader_->GetUniformLocation("lightColor[0]");

			// Upload all the data needed
			glUniform4fv(uEyePos, 1, &(camEyePoint[0]));
			glUniform3fv(uAmbientColor, 1, &(ambientColor_[0]));
			glUniform1i(uLightCount, lightCount_);
			glUniform4fv(uLightPos, lightCount_, &(lightPos_[0][0]));
			glUniform3fv(uLightColor, lightCount_, &(lightColor_[0][0]));
		}
	}

	RenderSystem* renderSys = dynamic_cast<RenderSystem*>(GetParent()->GetSystem(SysType::RenderSys));
	if (renderSys)
	{
		for (int i = 0; i < lightCount_; ++i)
		{
			cubeLight_->SetPosition(lightPos_[i]);
			cubeLight_->SetTint(lightColor_[i]);
			cubeLight_->Draw(RenderType::Triangles);
		}
	}
}

void LightingSystem::Shutdown()
{
	if (cubeLight_)
		delete cubeLight_;
}

bool LightingSystem::IsActive()
{
	GraphicsSystem* graphics = dynamic_cast<GraphicsSystem*>(GetParent()->GetSystem(SysType::GraphicsSys));
	if (graphics)
		return phongShader_ == graphics->GetActiveShader();
	return false;
}

LightingSystem::~LightingSystem()
{

}
