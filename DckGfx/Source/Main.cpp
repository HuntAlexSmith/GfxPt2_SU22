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
bool renderFace = true;

static float rotation = 0.0f;

glm::vec4 triVertices[] = {
	glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
	glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
	glm::vec4(0.0f, 0.5f, 0.0f, 1.0f),
};

glm::vec4 squareVertices[] = {
	glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
	glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
	glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
	glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)
};

glm::vec3 colors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

// Initial Square attributes
glm::mat4 squareScale = GfxMath::Scale2D(3.0f);

// Camera attributes
glm::vec4 camRightVec = GfxMath::Vector(1, 0);
glm::vec4 camUpVec = GfxMath::Vector(0, 1);
glm::vec4 camBackVec = GfxMath::Vector(0, 0, 1);
glm::vec4 cameraPos = GfxMath::Point(0, 0);
float camWidth = 20.0f * (16.0f/9.0f);
float camHeight = 20.0f;

// Camera matrices
glm::mat4 cameraMat = GfxMath::Affine(camRightVec, camUpVec, camBackVec, cameraPos);
glm::mat4 viewMat = GfxMath::AffineInverse(cameraMat);

// To NDC matrix
glm::mat4 cameraToNDC = GfxMath::Scale(2.0f / camWidth, 2.0f / camHeight);

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

	// Set VSync
	SDL_GL_SetSwapInterval(1);

	// Make sure to set gl viewport and clear color
	int w, h;
	SDL_GetWindowSize(windowHandle, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Compile the shader and create a mesh
	// CompileShader();
	shader = new Shader("Data/2dShader.vert", "Data/2dShader.frag");
	myMesh = new Mesh(shader);

	// Create the mesh
	myMesh->AddVertex(squareVertices[0], colors[0]);
	myMesh->AddVertex(squareVertices[1], colors[1]);
	myMesh->AddVertex(squareVertices[2], colors[2]);
	myMesh->AddVertex(squareVertices[3], colors[3]);
	myMesh->AddFace(0, 1, 2);
	myMesh->AddFace(0, 2, 3);
	myMesh->AddEdge(0, 1);
	myMesh->AddEdge(1, 2);
	myMesh->AddEdge(2, 3);
	myMesh->AddEdge(0, 3);

	// Main loop
	SDL_Event windowEvent;
	bool quit = false;
	while (!quit)
	{
		rotation += 0.5f;
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the program, bind the vao, and draw it
		shader->Use();

		// Upload values to the uniforms
		GLint uObjToWorld = shader->GetUniformLocation("objToWorld");
		GLint uWorldToCam = shader->GetUniformLocation("worldToCam");
		GLint uCamToNDC = shader->GetUniformLocation("camToNDC");

		glm::mat4 fullTransform = GfxMath::Rotate2D(rotation) * squareScale;

		glUniformMatrix4fv(uObjToWorld, 1, GL_FALSE, &fullTransform[0][0]);
		glUniformMatrix4fv(uWorldToCam, 1, GL_FALSE, &viewMat[0][0]);
		glUniformMatrix4fv(uCamToNDC, 1, GL_FALSE, &cameraToNDC[0][0]);

		if (renderFace)
		{
			glBindVertexArray(myMesh->GetFaceVAO());
			glDrawElements(GL_TRIANGLES, 3 * myMesh->GetFaceCount(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		else
		{
			glBindVertexArray(myMesh->GetEdgeVAO());
			glDrawElements(GL_LINES, 2 * myMesh->GetEdgeCount(), GL_UNSIGNED_INT, 0);
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
	delete myMesh;
	delete shader;

	// Make sure to destroy the window and quit SDL
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(windowHandle);
	SDL_Quit();

	return 0;
}
