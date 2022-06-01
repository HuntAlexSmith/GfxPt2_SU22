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

// Triangle vertices and buffers
Mesh* my3DMesh;
NormalMesh* myNormMesh;
bool renderFace = true;

// Rotation
static float rotation = 0.0f;

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

// Initial Cube Attributes
static glm::vec4 diagAxis = GfxMath::Vector(1, 1, 1);
static glm::vec4 center = GfxMath::Point(0, 0, 0);
static glm::mat4 cubeScale = GfxMath::Scale3D(1.f);

// Delta time stuff
Uint32 lastTick = 0;
Uint32 currentTick = 0;
float dt = 0;

int main(int argc, char* argv[]) {

	// Create and initialize the engine
	DckEInitialize();
	
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

	// Main loop
	while (DckEIsRunning())
	{
		lastTick = currentTick;
		currentTick = SDL_GetTicks();
		dt = (currentTick - lastTick) / 1000.0f;
		glm::clamp(dt, 0.0f, 0.125f);

		// Calculate a new rotation every frame
		rotation += 45.0f * dt;
		if (rotation > 360.0f)
			rotation = 0.0f;

		// Calculate a new model matrix for the mesh and render it
		glm::vec4 move = GfxMath::Point(3, 0, 0);
		glm::mat4 fullTrans = GfxMath::Translate(move) * GfxMath::Rotate3D(diagAxis, rotation) * cubeScale;
		DckERender(my3DMesh, RenderType::Triangles, fullTrans);

		// Calculate a new model matrix for the mesh and render it
		glm::vec4 otherMove = GfxMath::Point(-3, 0, 0);
		glm::mat4 otherTrans = GfxMath::Translate(otherMove) * GfxMath::Rotate3D(diagAxis, -rotation) * cubeScale;
		DckERender(myNormMesh, RenderType::Triangles, otherTrans);
		
		// Update the engine
		DckEUpdate(dt);
	}

	// Delete the program and vao stuff
	delete myNormMesh;
	delete my3DMesh;

	// Shut the engine down
	DckEShutdown();

	return 0;
}
