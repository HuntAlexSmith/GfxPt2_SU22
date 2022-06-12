//*****************************************************************************
//	File:   Scene1.cpp
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: Scene1 functions for the SceneSystem
//*****************************************************************************

#include "Scene1.h"
#include "DckGfxEngine.h"
#include "GfxMath.h"
#include "RenderObject.h"

// Rotation stuff
static float rotation = 0.0f;
glm::vec4 diagAxis = GfxMath::Vector(1, 1, 1);

// Color
static glm::vec3 gray(0.2f, 0.2f, 0.2f);

static RenderObject* lazyCube;
static RenderObject* phongCube;

void Scene1Load()
{
	lazyCube = new RenderObject();
	lazyCube->SetMesh(MeshLibraryGet("Cube"));
	lazyCube->SetRotation(GfxMath::Vector(1, 1, 1), 0.0f);
	lazyCube->SetPosition(GfxMath::Point(3, 0, 0));
	lazyCube->SetTint(glm::vec3(0.0f, 1.0f, 1.0f));

	phongCube = new RenderObject();
	phongCube->SetMesh(MeshLibraryGet("NormCube"));
	phongCube->SetRotation(GfxMath::Vector(1, 1, 1), 0.0f);
	phongCube->SetPosition(GfxMath::Point(-3, 0, 0));
	phongCube->SetDiffuse(glm::vec3(1.0f, 0.0f, 1.0f));
	phongCube->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

	DckEAddLight(GfxMath::Point(-3, 5, 10), glm::vec3(1.0f, 1.0f, 0.0f));
	DckEAddLight(GfxMath::Point(-3, 5, -10), glm::vec3(0.0f, 1.0f, 1.0f));
}

void Scene1Init()
{
	rotation = 0.0f;
}

void Scene1Update(float dt)
{
	// Check for input for going to different scene
	if (DckEKeyIsTriggered(SDLK_F1))
		DckESetNextScene(SceneID::SceneRestart);

	else if (DckEKeyIsTriggered(SDLK_F2))
		DckESetNextScene(SceneID::Scene2);

	// Get the current rotation data of the cubes and update their rotations accordingly
	float rotation;
	glm::vec4 rotVec;
	lazyCube->GetRotation(&rotVec, &rotation);
	rotation += 45.0f * dt;
	if (rotation > 360.0f)
		rotation = 0.0f;
	lazyCube->SetRotation(rotVec, rotation);

	phongCube->GetRotation(&rotVec, &rotation);
	rotation += 45.0f * dt;
	if (rotation > 360.0f)
		rotation = 0.0f;
	phongCube->SetRotation(rotVec, rotation);

	// Draw the objects
	lazyCube->Draw(RenderType::Lines);
	phongCube->Draw(RenderType::Triangles);

}

void Scene1Shutdown()
{

}

void Scene1Unload()
{
}
