//*****************************************************************************
//	File:   RenderSystem.cpp
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: System for handling render data and drawing things to screen
//		every update
//*****************************************************************************

#include "RenderSystem.h"
#include "GraphicsSystem.h"
#include "Engine.h"

// 3D Camera Attributes
static glm::vec4 rightVec = GfxMath::Vector(1, 0, 0);
static glm::vec4 upVec = GfxMath::Vector(0, 1, 0);
static glm::vec4 backVec = GfxMath::Vector(0, 0, -1);
static glm::vec4 camPos = GfxMath::Point(0.0f, 0.0f, -10.0f);
static glm::mat4 camera3DMat = GfxMath::Affine(rightVec, upVec, backVec, camPos);
static glm::mat4 view3DMat = GfxMath::AffineInverse(camera3DMat);

// Calculate the perspective matrix
static float nearDist = 1.0f;
static float farDist = 100.0f;
static float camDist = nearDist + (farDist - nearDist) / 2.0f;
static float cam3DWidth = 2 * camDist;
static float cam3DHeight = cam3DWidth / (16.0f / 9.0f);

static glm::mat4 perspMat(
	2 * camDist / cam3DWidth, 0, 0, 0,
	0, 2 * camDist / cam3DHeight, 0, 0,
	0, 0, (nearDist + farDist) / (nearDist - farDist), -1,
	0, 0, (2 * nearDist * farDist) / (nearDist - farDist), 0
);

RenderSystem::RenderSystem() : System(SysType::RenderSys), renderQueue_()
{
}

void RenderSystem::Initialize()
{
	// Nothing to really initialize here
}

void RenderSystem::Update(float dt)
{
	// Get necessary systems here (probably graphics)
	GraphicsSystem* graphicSys = dynamic_cast<GraphicsSystem*>(GetParent()->GetSystem(SysType::GraphicsSys));
	if (!graphicSys)
	{
		while (!renderQueue_.empty())
			renderQueue_.pop();
		return;
	}

	// Get the active shader
	Shader* shader = graphicSys->GetActiveShader();
	shader->Use();

	// Get uniform locations here
	GLint uObjToWorld = shader->GetUniformLocation("objToWorld");
	GLint uWorldToCam = shader->GetUniformLocation("worldToCam");
	GLint uPerspMat = shader->GetUniformLocation("perspMat");
	GLint uNormMat = shader->GetUniformLocation("normMat");
	GLint uIgNorm = shader->GetUniformLocation("ignoreNorm");

	// Upload Perspective Matrix and view matrix here
	glUniformMatrix4fv(uPerspMat, 1, GL_FALSE, &perspMat[0][0]);
	glUniformMatrix4fv(uWorldToCam, 1, GL_FALSE, &view3DMat[0][0]);

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