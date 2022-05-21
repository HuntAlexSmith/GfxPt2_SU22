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
#include "glm/glm.hpp"
#include "FileReader.h"
#include "Shader.h"

// Window Stuff
static const int width = 1280;
static const int height = 720;
static SDL_Window* windowHandle = nullptr;
static SDL_GLContext glContext;

// Triangle vertices and buffers
Shader* shader;
GLuint program;
GLuint vao;
GLuint vbo;
GLuint cbo;
GLuint ebo;

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

float colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 2,
};

// Functions for shader stuff
void CreateMeshVAO();

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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Compile the shader and create a mesh
	// CompileShader();
	shader = new Shader("Data/2dShader.vert", "Data/2dShader.frag");
	CreateMeshVAO();

	// Main loop
	SDL_Event windowEvent;
	bool quit = false;
	while (!quit)
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the program, bind the vao, and draw it
		shader->Use();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap buffers
		SDL_GL_SwapWindow(windowHandle);

		// Checking for the window closing event
		while (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT)
				quit = true;
		}
	}

	// Delete the program and vao stuff
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &cbo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	delete shader;

	// Make sure to destroy the window and quit SDL
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(windowHandle);
	SDL_Quit();

	return 0;
}

void CreateMeshVAO()
{
	// Generate the vertex buffer and the vertex array
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &ebo);

	// Upload vertex data to the vertex buffer
	glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Upload color data to the color buffer
	glNamedBufferData(cbo, sizeof(colors), colors, GL_STATIC_DRAW);

	// Upload index data to the element buffer
	glNamedBufferData(ebo, sizeof(indices), indices, GL_STATIC_DRAW);

	// Get the position and color attribute variables
	GLint posAttrib = shader->GetAttribLocation("position");
	GLint colorAttrib = shader->GetAttribLocation("color");

	// Enable the position attribute for the vao, make sure it links with the vbo, and format the attrib
	glEnableVertexArrayAttrib(vao, posAttrib);
	glVertexArrayAttribBinding(vao, posAttrib, vbo-1);
	glVertexArrayAttribFormat(vao, posAttrib, 3, GL_FLOAT, GL_FALSE, 0);

	// Enable the color attribute for the vao, make sure it links with the vbo, and format the attrib
	glEnableVertexArrayAttrib(vao, colorAttrib);
	glVertexArrayAttribBinding(vao, colorAttrib, cbo-1);
	glVertexArrayAttribFormat(vao, colorAttrib, 3, GL_FLOAT, GL_FALSE, 0);

	// Bind the buffers to the respective attributes and bind them with the vao
	glVertexArrayVertexBuffer(vao, posAttrib, vbo, 0, 3*sizeof(float));
	glVertexArrayVertexBuffer(vao, colorAttrib, cbo, 0, 3 * sizeof(float));

	// Bind the ebo with the vao
	glVertexArrayElementBuffer(vao, ebo);
}
