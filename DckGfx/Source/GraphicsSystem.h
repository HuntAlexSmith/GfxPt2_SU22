#pragma once
//*****************************************************************************
//	File:   GraphicsSystem.h
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: System that will manage the graphics API calls, such as init
//		OpenGL
//*****************************************************************************

#include "System.h"
#include "Shader.h"

class GraphicsSystem : public System {
public:

	GraphicsSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	Shader* GetActiveShader();

	~GraphicsSystem();

private:

	Shader* activeShader_;

};