//*****************************************************************************
//	File:   Scene1.cpp
//  Author: Hunter Smith
//  Date:   06/02/2022
//  Description: Scene1 functions for the SceneSystem
//*****************************************************************************

#include "Scene1.h"
#include "DckGfxEngine.h"
#include "GfxMath.h"

// Rotation stuff
static float rotation = 0.0f;
glm::vec4 diagAxis = GfxMath::Vector(1, 1, 1);

// Color
static glm::vec3 gray(0.5f, 0.5f, 0.5f);

// Vertices of the cube
static const glm::vec4 cubeVertices[] = {
	GfxMath::Point(1.0f, 1.0f, 1.0f),
	GfxMath::Point(1.0f, 1.0f, -1.0f),
	GfxMath::Point(1.0f, -1.0f, 1.0f),
	GfxMath::Point(1.0f, -1.0f, -1.0f),
	GfxMath::Point(-1.0f, 1.0f, 1.0f),
	GfxMath::Point(-1.0f, 1.0f, -1.0f),
	GfxMath::Point(-1.0f, -1.0f, 1.0f),
	GfxMath::Point(-1.0f, -1.0f, -1.0f)
};

// Colors of vertices of cube
static const glm::vec3 cubeColors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

// Edges of the cube
static const Mesh::Edge cubeEdges[] = {
	Mesh::Edge(0, 1),
	Mesh::Edge(1, 3),
	Mesh::Edge(2, 3),
	Mesh::Edge(0, 2),
	Mesh::Edge(0, 4),
	Mesh::Edge(2, 6),
	Mesh::Edge(3, 7),
	Mesh::Edge(1, 5),
	Mesh::Edge(4, 5),
	Mesh::Edge(5, 7),
	Mesh::Edge(6, 7),
	Mesh::Edge(4, 6)
};

// Faces of the cube
static const Mesh::Face cubeFaces[] = {
	Mesh::Face(0, 3, 1),
	Mesh::Face(0, 2, 3),
	Mesh::Face(0, 5, 4),
	Mesh::Face(0, 1, 5),
	Mesh::Face(3, 7, 5),
	Mesh::Face(1, 3, 5),
	Mesh::Face(2, 6, 7),
	Mesh::Face(2, 7, 3),
	Mesh::Face(0, 6, 2),
	Mesh::Face(0, 4, 6),
	Mesh::Face(4, 7, 6),
	Mesh::Face(4, 5, 7)
};

static Mesh* my3DMesh;
static Mesh* myNormMesh;

void Scene1Load()
{
	// Create a new mesh
	my3DMesh = new Mesh();

	// Create the 3D Mesh
	for (int i = 0; i < 8; ++i)
	{
		my3DMesh->AddVertex(cubeVertices[i], cubeColors[i]);
	}
	for (int i = 0; i < 12; ++i)
	{
		my3DMesh->AddEdge(cubeEdges[i].v1, cubeEdges[i].v2);
		my3DMesh->AddFace(cubeFaces[i].v1, cubeFaces[i].v2, cubeFaces[i].v3);
	}

	// Create a normal mesh from the previous mesh
	myNormMesh = new NormalMesh(my3DMesh);
}

void Scene1Init()
{
	DckESetBackColor(gray);
	rotation = 0.0f;
}

void Scene1Update(float dt)
{
	// Check for input for going to different scene
	if (DckEKeyIsTriggered(SDLK_F1))
		DckESetNextScene(SceneID::SceneRestart);

	else if (DckEKeyIsTriggered(SDLK_F2))
		DckESetNextScene(SceneID::Scene2);

	// Calculate a new rotation every frame
	rotation += 45.0f * dt;
	if (rotation > 360.0f)
		rotation = 0.0f;

	// Calculate a new model matrix for the mesh and render it
	glm::vec4 move = GfxMath::Point(3, 0, 0);
	glm::mat4 fullTrans = GfxMath::Translate(move) * GfxMath::Rotate3D(diagAxis, rotation);
	DckERender(my3DMesh, RenderType::Triangles, fullTrans);

	// Calculate a new model matrix for the mesh and render it
	glm::vec4 otherMove = GfxMath::Point(-3, 0, 0);
	glm::mat4 otherTrans = GfxMath::Translate(otherMove) * GfxMath::Rotate3D(diagAxis, -rotation);
	DckERender(myNormMesh, RenderType::Triangles, otherTrans);
}

void Scene1Shutdown()
{

}

void Scene1Unload()
{
	delete myNormMesh;
	delete my3DMesh;
}
