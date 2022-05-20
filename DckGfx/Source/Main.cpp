#include <iostream>
#include <string>
#include <stdexcept>
#include "SDL2/SDL.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

// Window Stuff
static const int width = 1280;
static const int height = 720;
static SDL_Window* windowHandle = nullptr;
static SDL_GLContext glContext;

// Quick and dirty OpenGL stuff for rendering a Triangle
const char* vertexShaderCode =
	"#version 450 core\n\
	 in vec3 position;\n\
	 in vec3 color;\n\
	 out vec3 myColor;\n\
	 void main()\n\
     {\n\
		gl_Position = vec4(position.x, position.y, position.z, 1.0);\n\
	    myColor = color;\n\
	 }\0";

const char* fragmentShaderCode = 
	"#version 450 core\n\
	 in vec3 myColor;\n\
	 out vec4 FragColor;\n\
	 void main()\n\
	 {\n\
		FragColor = vec4(myColor, 1.0f);\n\
	 }\0";

// Triangle vertices and buffers
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

void CompileShader();
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

	CompileShader();
	CreateMeshVAO();

	// Main loop
	SDL_Event windowEvent;
	bool quit = false;
	while (!quit)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		SDL_GL_SwapWindow(windowHandle);
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
	glUseProgram(0);
	glDeleteProgram(program);

	// Make sure to destroy the window and quit SDL
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(windowHandle);
	SDL_Quit();

	return 0;
}

void CompileShader()
{
	// Attempt to compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShader);

	GLint worked;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &worked);
	if (!worked)
	{
		std::string errMsg = "Vertex Shader failed to compile:\n";
		char errBuff[1024];
		glGetShaderInfoLog(vertexShader, 1024, 0, errBuff);
		errMsg += errBuff;
		throw std::runtime_error(errMsg);
	}

	// Attempt to compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &worked);
	if (!worked)
	{
		std::string errMsg = "Fragment Shader failed to compile:\n";
		char errBuff[1024];
		glGetShaderInfoLog(fragmentShader, 1024, 0, errBuff);
		errMsg += errBuff;
		throw std::runtime_error(errMsg);
	}

	// Both shaders compiled, now create program and try to link them
	program = glCreateProgram();
	glAttachShader(program, fragmentShader);
	glAttachShader(program, vertexShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &worked);
	if (!worked)
	{
		std::string errMsg = "Program failed to link:\n";
		char errBuff[1024];
		glGetProgramInfoLog(program, 1024, 0, errBuff);
		errMsg += errBuff;
		throw std::runtime_error(errMsg);
	}

	// We can delete the shaders now
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
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
	GLint posAttrib = glGetAttribLocation(program, "position");
	GLint colorAttrib = glGetAttribLocation(program, "color");


	glEnableVertexArrayAttrib(vao, posAttrib);
	glVertexArrayAttribBinding(vao, posAttrib, vbo-1);
	glVertexArrayAttribFormat(vao, posAttrib, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(vao, colorAttrib);
	glVertexArrayAttribBinding(vao, colorAttrib, cbo-1);
	glVertexArrayAttribFormat(vao, colorAttrib, 3, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayVertexBuffer(vao, posAttrib, vbo, 0, 3*sizeof(float));
	glVertexArrayVertexBuffer(vao, colorAttrib, cbo, 0, 3 * sizeof(float));

	glVertexArrayElementBuffer(vao, ebo);
}
