//*****************************************************************************
//	File:   RenderSystem.cpp
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: System for handling render data and drawing things to screen
//		every update
//*****************************************************************************

#include "RenderSystem.h"
#include "InputSystem.h"
#include "LightingSystem.h"
#include "GraphicsSystem.h"
#include "CameraSystem.h"
#include "Engine.h"

static glm::vec3 diff(1.0f, 1.0f, 0.2f);
static glm::vec3 specular(0.5f, 0.5f, 0.5f);
static float specExp = 0.5f;

RenderSystem::RenderSystem() : System(SysType::RenderSys),
	renderQueue_(),
	debugQueue_(),
	pointSize_(5.0f),
	lineWidth_(1.0f)
{
}

void RenderSystem::Initialize()
{
	// Nothing to really initialize here
	glLineWidth(lineWidth_);
	glPointSize(pointSize_);
}

void RenderSystem::Update(float dt)
{
	// Get necessary systems here (probably graphics and camera)
	GraphicsSystem* graphicSys = dynamic_cast<GraphicsSystem*>(GetParent()->GetSystem(SysType::GraphicsSys));
	if (!graphicSys)
	{
		while (!renderQueue_.empty())
			renderQueue_.pop();
		while (!debugQueue_.empty())
			debugQueue_.pop();
		return;
	}

	CameraSystem* camSys = dynamic_cast<CameraSystem*>(GetParent()->GetSystem(SysType::CameraSys));
	if (!camSys)
	{
		while (!renderQueue_.empty())
			renderQueue_.pop();
		while (!debugQueue_.empty())
			debugQueue_.pop();
		return;
	}

	// Get the active shader
	Shader* shader = graphicSys->GetActiveShader();
	shader->Use();

	// Get the Lighting System and see if we need to upload diffuse and specular data
	LightingSystem* lightSys = dynamic_cast<LightingSystem*>(GetParent()->GetSystem(LightingSys));
	if (lightSys && lightSys->IsActive())
	{
		GLint uDiffCoeff = shader->GetUniformLocation("diffuseCoeff");
		GLint uSpecCoeff = shader->GetUniformLocation("specularCoeff");
		GLint uSpecExp = shader->GetUniformLocation("specularExp");

		glUniform3fv(uDiffCoeff, 1, &diff[0]);
		glUniform3fv(uSpecCoeff, 1, &specular[0]);
		glUniform1f(uSpecExp, specExp);
	}

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
				glDrawElements(GL_POINTS, currentData.elementCount, GL_UNSIGNED_INT, 0);
				break;
			case RenderType::Lines:
				glDrawElements(GL_LINES, 2 * currentData.elementCount, GL_UNSIGNED_INT, 0);
				break;
			case RenderType::Triangles:
				glDrawElements(GL_TRIANGLES, 3 * currentData.elementCount, GL_UNSIGNED_INT, 0);
				break;
			default:
				break;
		}
		glBindVertexArray(0);
	}

	// Now to render debug stuff that can always be seen
	glClear(GL_DEPTH_BUFFER_BIT);
	while (!debugQueue_.empty())
	{
		// Get current render data
		RenderData currentData = debugQueue_.front();
		debugQueue_.pop();

		// Upload necessary uniforms
		glUniformMatrix4fv(uObjToWorld, 1, GL_FALSE, &currentData.objToWorld[0][0]);
		glUniformMatrix4fv(uNormMat, 1, GL_FALSE, &currentData.normalMat[0][0]);
		glUniform1i(uIgNorm, currentData.noNorm);

		// Render object using specified typing
		glBindVertexArray(currentData.vao);
		switch (currentData.type)
		{
			case RenderType::Points:
				glDrawElements(GL_POINTS, currentData.elementCount, GL_UNSIGNED_INT, 0);
				break;
			case RenderType::Lines:
				glDrawElements(GL_LINES, 2 * currentData.elementCount, GL_UNSIGNED_INT, 0);
				break;
			case RenderType::Triangles:
				glDrawElements(GL_TRIANGLES, 3 * currentData.elementCount, GL_UNSIGNED_INT, 0);
				break;
			default:
				break;
		}
		glBindVertexArray(0);
	}
}

void RenderSystem::Shutdown()
{

}

void RenderSystem::Render(DckMesh* mesh, RenderType type, glm::mat4 objToWorld)
{
	glm::mat4 normMat = GfxMath::NormalMatrix(objToWorld);
	switch (type)
	{
		case RenderType::Points:
			renderQueue_.push(RenderData(mesh->GetPointVAO(), mesh->GetPointCount(), type, 1, objToWorld, normMat));
			break;
		case RenderType::Lines:
			renderQueue_.push(RenderData(mesh->GetEdgeVAO(), mesh->GetEdgeCount(), type, 1, objToWorld, normMat));
			break;
		case RenderType::Triangles:
			if (mesh->HasNormals())
				renderQueue_.push(RenderData(mesh->GetFaceVAO(), mesh->GetFaceCount(), type, 0, objToWorld, normMat));
			else
				renderQueue_.push(RenderData(mesh->GetFaceVAO(), mesh->GetFaceCount(), type, 1, objToWorld, normMat));
			break;
	}
}

void RenderSystem::RenderDebug(DckMesh* mesh, RenderType type, glm::mat4 objToWorld)
{
	glm::mat4 normMat = GfxMath::NormalMatrix(objToWorld);
	switch (type)
	{
	case RenderType::Points:
		debugQueue_.push(RenderData(mesh->GetPointVAO(), mesh->GetPointCount(), type, 1, objToWorld, normMat));
		break;
	case RenderType::Lines:
		debugQueue_.push(RenderData(mesh->GetEdgeVAO(), mesh->GetEdgeCount(), type, 1, objToWorld, normMat));
		break;
	case RenderType::Triangles:
		if (mesh->HasNormals())
			debugQueue_.push(RenderData(mesh->GetFaceVAO(), mesh->GetFaceCount(), type, 0, objToWorld, normMat));
		else
			debugQueue_.push(RenderData(mesh->GetFaceVAO(), mesh->GetFaceCount(), type, 1, objToWorld, normMat));
		break;
	}
}

RenderSystem::~RenderSystem()
{

}