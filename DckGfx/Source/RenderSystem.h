#pragma once
//*****************************************************************************
//	File:   RenderSystem.h
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: System for handling render data and drawing things to screen
//		every update
//*****************************************************************************

#include "System.h"
#include "Mesh.h"
#include <queue>

class RenderSystem : public System {
public:

	struct RenderData {
		GLuint vao;
		int elementCount;
		RenderType type;
		int noNorm;
		glm::mat4 objToWorld;
		glm::mat4 normalMat;

		RenderData(GLuint vao, int elementCount, RenderType type, int noNorm, glm::mat4 oTW, glm::mat4 nM) :
			vao(vao),
			elementCount(elementCount),
			type(type),
			noNorm(noNorm),
			objToWorld(oTW),
			normalMat(nM)
		{}
	};

	RenderSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	void Render(Mesh* mesh, RenderType type, glm::mat4 objToWorld);
	void RenderDebug(Mesh* mesh, RenderType type, glm::mat4 objToWorld);

	~RenderSystem();

private:

	std::queue<RenderData> renderQueue_;
	std::queue<RenderData> debugQueue_;

	float pointSize_;
	float lineWidth_;

};