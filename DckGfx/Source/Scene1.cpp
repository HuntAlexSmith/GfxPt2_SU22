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

void Scene1Load()
{
	// Local pointers of lazy cube and phong cube
	RenderObject* lazyCube;
	RenderObject* phongCube;

	// Create and initialize the lazy cube
	lazyCube = new RenderObject("LazyCube");
	lazyCube->SetMesh(MeshLibraryGet("Cube"));
	lazyCube->SetRenderMode(RenderType::Lines);
	lazyCube->SetRotation(GfxMath::Vector(1, 1, 1), 0.0f);
	lazyCube->SetPosition(GfxMath::Point(3, 0, 0));
	lazyCube->SetTint(glm::vec3(0.0f, 1.0f, 1.0f));

	// Create and initialize the phong cube
	phongCube = new RenderObject("PhongCube");
	phongCube->SetMesh(MeshLibraryGet("NormCube"));
	phongCube->SetRenderMode(RenderType::Triangles);
	phongCube->SetRotation(GfxMath::Vector(1, 1, 1), 0.0f);
	phongCube->SetPosition(GfxMath::Point(-3, 0, 0));
	phongCube->SetDiffuse(glm::vec3(1.0f, 0.0f, 1.0f));
	phongCube->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

	// Add the cubes to the object manager
	DckEObjectManagerAdd(lazyCube);
	DckEObjectManagerAdd(phongCube);

	DckEAddLight(GfxMath::Point(-3, 5, 10), glm::vec3(1.0f, 1.0f, 0.0f));
	DckEAddLight(GfxMath::Point(-3, 5, -10), glm::vec3(0.0f, 1.0f, 1.0f));
}

void Scene1Init()
{
	rotation = 0.0f;
}

void Scene1Update(float dt)
{
	RenderObject* lazyCube = DckEObjectManagerGet("LazyCube");

	// Check for input for going to different scene
	if (DckEKeyIsTriggered(SDLK_F1))
	{
		if (lazyCube)
			lazyCube->Destroy();
	}

	else if (DckEKeyIsTriggered(SDLK_F2))
		DckESetNextScene(SceneID::Scene2);


	// Get the current rotation data of the cubes and update their rotations accordingly
	float rotation;
	glm::vec4 rotVec;
	if (lazyCube)
	{
		lazyCube->GetRotation(&rotVec, &rotation);
		rotation += 45.0f * dt;
		if (rotation > 360.0f)
			rotation = 0.0f;
		lazyCube->SetRotation(rotVec, rotation);
	}

	RenderObject* phongCube = DckEObjectManagerGet("PhongCube");
	phongCube->GetRotation(&rotVec, &rotation);
	rotation += 45.0f * dt;
	if (rotation > 360.0f)
		rotation = 0.0f;
	phongCube->SetRotation(rotVec, rotation);
}

void Scene1Shutdown()
{

}

void Scene1Unload()
{
}
