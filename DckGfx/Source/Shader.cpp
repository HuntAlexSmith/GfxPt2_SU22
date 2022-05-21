//*****************************************************************************
//	File:   Shader.cpp
//  Author: Hunter Smith
//  Date:   05/21/2022
//  Description: Wrapper for an OpenGL Shader Program (class is named Shader)
//*****************************************************************************

#include <iostream>
#include "Shader.h"
#include "FileReader.h"

//*****************************************************************************
//  Description
//		Constructor for a shader object, which takes in a vertex and fragment
//		shader filepath for construction
//	
//	Param vertFile
//		The filepath of the vertex shader
// 
//	Param fragFile
//		The filepath of the fragment shader
//*****************************************************************************
Shader::Shader(const char* vertFile, const char* fragFile) : program_(0), attribs_() {
	// Read the vertex shader file
	std::string vertCodeStr = ReadShaderFile(vertFile);
	if (vertCodeStr.empty())
	{
		std::cout << "Shader failed to be created, could not read Vertex Shader File" << std::endl;
		return;
	}
	const char* vertexShaderCode = vertCodeStr.c_str();

	// Create the vertex shader, give it code, and compile it
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShader);

	// Error check that compilation worked
	GLint worked;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &worked);
	if (!worked)
	{
		std::string errMsg = "Vertex Shader failed to compile:\n";
		char errBuff[1024];
		glGetShaderInfoLog(vertexShader, 1024, 0, errBuff);
		errMsg += errBuff;
		std::cout << errMsg << std::endl;
		return;
	}

	// Read the fragment shader file
	std::string fragCodeStr = ReadShaderFile(fragFile);
	if (fragCodeStr.empty())
	{
		std::cout << "Shader failed to be created, could not read Fragment Shader File" << std::endl;
		glDeleteShader(vertexShader);
		return;
	}
	const char* fragmentShaderCode = fragCodeStr.c_str();

	// Create the fragment shader, give it the code, and compile it
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShader);

	// Error check that compilation worked
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &worked);
	if (!worked)
	{
		std::string errMsg = "Fragment Shader failed to compile:\n";
		char errBuff[1024];
		glGetShaderInfoLog(fragmentShader, 1024, 0, errBuff);
		errMsg += errBuff;
		std::cout << errMsg << std::endl;
		glDeleteShader(vertexShader);
		return;
	}

	// Both shaders compiles, time to make the actual program
	program_ = glCreateProgram();
	glAttachShader(program_, fragmentShader);
	glAttachShader(program_, vertexShader);
	glLinkProgram(program_);

	// Check that linking worked
	glGetProgramiv(program_, GL_LINK_STATUS, &worked);
	if (!worked)
	{
		std::string errMsg = "Program failed to link:\n";
		char errBuff[1024];
		glGetProgramInfoLog(program_, 1024, 0, errBuff);
		errMsg += errBuff;
		glDeleteProgram(program_);
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		std::cout << errMsg << std::endl;
		return;
	}

	// Delete the shaders, they are linked to the program now
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	// Now get all the attributes in the program
	GLint attribCount;
	glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &attribCount);

	GLint size;
	GLenum type;
	const GLsizei bufSize = 16;
	GLchar varName[bufSize];
	GLsizei length;

	for (GLint i = 0; i < attribCount; ++i)
	{
		glGetActiveAttrib(program_, i, bufSize, &length, &size, &type, varName);
		std::pair<std::string, GLint> attribToAdd(std::string(varName), i);
		attribs_.insert(attribToAdd);
	}
}

//*****************************************************************************
//  Description
//		Tells OpenGL that we want to use this Shader's Program for rendering
//*****************************************************************************
void Shader::Use() {
	glUseProgram(program_);
}

//*****************************************************************************
//  Description
//		Get the location of a specified attribute from the shader
//	
//	Param attribName
//		The name of the attribute we want the location of
// 
//	Return
//		Returns the location of the attribute, -1 if it wasn't found
//*****************************************************************************
GLint Shader::GetAttribLocation(std::string attribName) {
	auto result = attribs_.find(attribName);
	if (result == attribs_.end())
		return -1;
	return result->second;
}

//*****************************************************************************
//  Description
//		Shader destructor, which handles clearing the attribute map and 
//		deleting the OpenGL Shader Program
//*****************************************************************************
Shader::~Shader() {
	attribs_.clear();
	glUseProgram(0);
	glDeleteProgram(program_);
}
