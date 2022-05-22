#pragma once
//*****************************************************************************
//	File:   Shader.h
//  Author: Hunter Smith
//  Date:   05/20/2022
//  Description: Wrapper for an OpenGL Shader Program (class is named Shader)
//*****************************************************************************

#include <string>
#include <map>
#include "glad/glad.h"

class Shader {
public:

	Shader(const char* vertFile, const char* fragFile);

	void Use();

	GLint GetAttribLocation(std::string attribName);
	GLint GetUniformLocation(std::string uniformName);
	
	~Shader();

private:

	// OpenGL Program
	GLuint program_;

	// Attribute locations
	std::map<std::string, GLint> attribs_;

	// Uniform locations ( will do this when needed)
	std::map<std::string, GLint> uniforms_;
};
