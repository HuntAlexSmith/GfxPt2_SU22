//*****************************************************************************
//	File:   ShaderLib.cpp
//  Author: Hunter Smith
//  Date:   06/03/2022
//  Description: Shader library for handling any shaders created for the
//		Graphics System
//*****************************************************************************

#include "ShaderLib.h"

// Static Shader Library
static ShaderLib shaderLib;

// FUNCTIONS FOR ACCESSING SHADER MANAGER
void ShaderLibraryInit()
{
	shaderLib.Initialize();
}

void ShaderLibraryAdd(std::string name, Shader* shader)
{
	shaderLib.AddObject(name, shader);
}

Shader* ShaderLibraryGet(std::string name)
{
	return shaderLib.GetObject(name);
}

void ShaderLibraryShutdown()
{
	shaderLib.Shutdown();
}

// CLASS FUNCTIONS FOR SHADER LIBRARY
ShaderLib::ShaderLib() : shaders_()
{
}

void ShaderLib::Initialize()
{
	// Create and add the default shader to the shader manager
	Shader* defaultShader = new Shader("Data/3dShader.vert", "Data/3dShader.frag");
	AddObject("Default Shader", defaultShader);

	// Create and add the lighting shader to the shader manager
	Shader* phongShader = new Shader("Data/PhongShader.vert", "Data/PhongShader.frag");
	AddObject("Phong Shader", phongShader);
}

void ShaderLib::AddObject(std::string name, Shader* shader)
{
	Shader* search = GetObject(name);
	if (!search)
		shaders_.insert(std::pair<std::string, Shader*>(name, shader));
}

Shader* ShaderLib::GetObject(std::string name)
{
	auto result = shaders_.find(name);
	if (result != shaders_.end())
		return result->second;
	return nullptr;
}

void ShaderLib::Shutdown()
{
	for (auto& shader : shaders_)
	{
		if (shader.second)
			delete shader.second;
	}
	shaders_.clear();
}

ShaderLib::~ShaderLib()
{
}