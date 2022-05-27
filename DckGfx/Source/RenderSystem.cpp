//*****************************************************************************
//	File:   RenderSystem.cpp
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: System for handling render data and drawing things to screen
//		every update
//*****************************************************************************

#include "RenderSystem.h"
#include "GraphicsSystem.h"
#include "CameraSystem.h"
#include "Engine.h"

RenderSystem::RenderSystem() : System(SysType::RenderSys), renderQueue_()
{
}

void RenderSystem::Initialize()
{
	// Nothing to really initialize here
}

void RenderSystem::Update(float dt)
{
	// Get necessary systems here (probably graphics and camera)
	GraphicsSystem* graphicSys = dynamic_cast<GraphicsSystem*>(GetParent()->GetSystem(SysType::GraphicsSys));
	if (!graphicSys)
	{
		while (!renderQueue_.empty())
			renderQueue_.pop();
		return;
	}

	CameraSystem* camSys = dynamic_cast<CameraSystem*>(GetParent()->GetSystem(SysType::CameraSys));
	if (!camSys)
	{
		while (!renderQueue_.empty())
			renderQueue_.pop();
		return;
	}

	// Get the active shader
	Shader* shader = graphicSys->GetActiveShader();
	shader->Use();

	// Get the active camera and respective matrices needed
	glm::mat4 perspMat(0);
	glm::mat4 viewMat(0);
	Camera* activeCam = camSys->GetActiveCamera();
	if (activeCam)
	{
		viewMat = activeCam->GetViewMatrix();
		perspMat = activeCam->GetPerspMatrix();
	}

	// Get uniform locations here
	GLint uObjToWorld = shader->GetUniformLocation("objToWorld");
	GLint uWorldToCam = shader->GetUniformLocation("worldToCam");
	GLint uPerspMat = shader->GetUniformLocation("perspMat");
	GLint uNormMat = shader->GetUniformLocation("normMat");
	GLint uIgNorm = shader->GetUniformLocation("ignoreNorm");

	// Upload Perspective Matrix and view matrix here
	glUniformMatrix4fv(uPerspMat, 1, GL_FALSE, &perspMat[0][0]);
	glUniformMatrix4fv(uWorldToCam, 1, GL_FALSE, &viewMat[0][0]);

	// Go through the render queue and render everything
	while (!renderQueue_.empty())
	{
		// Get the current render data from the queue
		RenderData currentData = renderQueue_.front();
		renderQueue_.pop();

		// Upload the necessary uniforms
		glUniformMatrix4fv(uObjToWorld, 1, GL_FALSE, &currentData.objToWorld[0][0]);
		glUniformMatrix4fv(uNormMat, 1, GL_FALSE, &currentData.normalMat[0][0]);
		glUniform1i(uIgNorm, currentData.noNorm);

		// Render the object using the specified typing
		glBindVertexArray(currentData.vao);
		switch (currentData.type)
		{
			case RenderType::Points:
				break;
			case RenderType::Lines:
				glDrawElements(GL_LINES, 2 * currentData.elementCount, GL_UNSIGNED_INT, 0);
				break;
			case RenderType::Triangles:
				glDrawElements(GL_TRIANGLES, 3 * currentData.elementCount, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}
}

void RenderSystem::Shutdown()
{

}

void RenderSystem::Render(Mesh* mesh, RenderType type, glm::mat4 objToWorld)
{
	NormalMesh* normMesh = dynamic_cast<NormalMesh*>(mesh);
	glm::mat4 normMat = GfxMath::AffineInverse(objToWorld);
	switch (type)
	{
		case RenderType::Points:
			break;
		case RenderType::Lines:
			renderQueue_.push(RenderData(mesh->GetEdgeVAO(), mesh->GetEdgeCount(), type, 1, objToWorld, normMat));
			break;
		case RenderType::Triangles:
			if (normMesh)
				renderQueue_.push(RenderData(normMesh->GetNormalFaceVAO(), normMesh->GetFaceCount(), type, 0, objToWorld, normMat));
			else
				renderQueue_.push(RenderData(mesh->GetFaceVAO(), mesh->GetFaceCount(), type, 1, objToWorld, normMat));
			break;
	}
}

RenderSystem::~RenderSystem()
{

}