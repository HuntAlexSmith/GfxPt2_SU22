//*****************************************************************************
//	File:   Main.cpp
//  Author: Hunter Smith
//  Date:   05/20/2022
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

// Window Stuff
static const int width = 1280;
static const int height = 720;
static SDL_Window* windowHandle = nullptr;
static SDL_GLContext glContext;

// Triangle vertices and buffers
Shader* shader;
Mesh* myMesh;
Mesh* my3DMesh;
bool renderFace = true;

static float rotation = 0.0f;

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

// Continue this later
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

static glm::vec4 squareVertices[] = {
	glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
	glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
	glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
	glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)
};

static glm::vec3 colors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

// Initial Cube Attributes
static glm::vec4 xAxis = GfxMath::Vector(1, 1, 1);
static glm::vec4 yAxis = GfxMath::Vector(0, 1, 0);
static glm::vec4 zAxis = GfxMath::Vector(0, 0, 1);
static glm::vec4 center = GfxMath::Point(0, 0, 0);
static glm::mat4 cubeScale = GfxMath::Scale3D(1.f);

// 3D Camera Attributes
static glm::vec4 rightVec = GfxMath::Vector(1, 0, 0);
static glm::vec4 upVec = GfxMath::Vector(0, 1, 0);
static glm::vec4 backVec = GfxMath::Vector(0, 0, -1);
static glm::vec4 camPos = GfxMath::Point(0.0f, 0.0f, -10.0f);
static glm::mat4 camera3DMat = GfxMath::Affine(rightVec, upVec, backVec, camPos);
static glm::mat4 view3DMat = GfxMath::AffineInverse(camera3DMat);

// Calculate the perspective matrix
static float nearDist = 1.0f;
static float farDist = 100.0f;
static float camDist = nearDist + (farDist - nearDist) / 2.0f;
static float cam3DWidth = 2 * camDist;
static float cam3DHeight = cam3DWidth / (16.0f / 9.0f);

static glm::mat4 perspMat(
	2*camDist/cam3DWidth, 0, 0, 0,
	0, 2*camDist/cam3DHeight, 0, 0,
	0, 0, (nearDist+farDist)/(nearDist-farDist), -1,
	0, 0, (2*nearDist*farDist)/(nearDist-farDist), 0
);


int main(int argc, char* argv[]) {
	// Attempt to initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL Failed to Initialize" << std::endl;
		return 1;
	}
	SDL_GL_LoadLibrary(nullptr);

	// Set Window Attributes:
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	// Depth Buffer:
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Now create the window
	windowHandle = SDL_CreateWindow("DckGfx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if (!windowHandle)
	{
		std::cout << "Window failed to be created" << std::endl;
		SDL_Quit();
		return 1;
	}
	glContext = SDL_GL_CreateContext(windowHandle);

	// Now Load OpenGL and see properties
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		std::cout << "Glad failed to Load" << std::endl;
		SDL_Quit();
		return 1;
	}
	glEnable(GL_DEPTH_TEST);

	// Set VSync
	SDL_GL_SetSwapInterval(1);

	// Make sure to set gl viewport and clear color
	int w, h;
	SDL_GetWindowSize(windowHandle, &w, &h);
	glViewport(0, 0, w, h);

	// Compile the shader and create a mesh
	// CompileShader();
	shader = new Shader("Data/3dShader.vert", "Data/3dShader.frag");
	my3DMesh = new Mesh(shader);

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

	// Main loop
	SDL_Event windowEvent;
	bool quit = false;
	while (!quit)
	{
		rotation += 0.5f;
		// Clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// Use the program, bind the vao, and draw it
		shader->Use();

		// Upload values to the uniforms
		GLint uObjToWorld = shader->GetUniformLocation("objToWorld");
		GLint uWorldToCam = shader->GetUniformLocation("worldToCam");
		GLint uPersp = shader->GetUniformLocation("perspMat");

		glm::mat4 fullTrans = GfxMath::Rotate3D(xAxis, rotation) * cubeScale;

		glUniformMatrix4fv(uObjToWorld, 1, GL_FALSE, &fullTrans[0][0]);
		glUniformMatrix4fv(uWorldToCam, 1, GL_FALSE, &view3DMat[0][0]);
		glUniformMatrix4fv(uPersp, 1, GL_FALSE, &perspMat[0][0]);

		
		if (renderFace)
		{
			glBindVertexArray(my3DMesh->GetFaceVAO());
			glDrawElements(GL_TRIANGLES, 3 * my3DMesh->GetFaceCount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		else
		{
			glBindVertexArray(my3DMesh->GetEdgeVAO());
			glDrawElements(GL_LINES, 2 * my3DMesh->GetEdgeCount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		// Swap buffers
		SDL_GL_SwapWindow(windowHandle);

		// Checking for the window closing event
		while (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.key.keysym.sym == SDLK_e)
			{
				if (renderFace)
					renderFace = false;
				else
					renderFace = true;
			}
			if (windowEvent.type == SDL_QUIT)
				quit = true;
		}
	}

	// Delete the program and vao stuff
	delete my3DMesh;
	delete shader;

	// Make sure to destroy the window and quit SDL
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(windowHandle);
	SDL_Quit();

	return 0;
}
