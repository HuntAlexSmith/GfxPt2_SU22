#pragma once
//*****************************************************************************
//	File:   ShaderLib.h
//  Author: Hunter Smith
//  Date:   06/03/2022
//  Description: Shader library for handling any shaders created for the
//		Graphics System
//*****************************************************************************

#include "Library.h"
#include "Shader.h"

void ShaderLibraryInit();
void ShaderLibraryAdd(std::string name, Shader* shader);
Shader* ShaderLibraryGet(std::string name);
void ShaderLibraryShutdown();

class ShaderLib : public Library<Shader*> {
public:

	ShaderLib();

	void Initialize() override;
	void AddObject(std::string name, Shader* shader) override;
	Shader* GetObject(std::string name) override;
	void Shutdown() override;

	~ShaderLib();

private:

	std::map<std::string, Shader*> shaders_;

};