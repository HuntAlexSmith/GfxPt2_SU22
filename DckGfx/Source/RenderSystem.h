#pragma once
//*****************************************************************************
//	File:   RenderSystem.h
//  Author: Hunter Smith
//  Date:   05/26/2022
//  Description: System for handling render data and drawing things to screen
//		every update
//*****************************************************************************

#include "System.h"
#include "MeshLib.h"
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
		glm::vec3 tint;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float specExp;

		RenderData(GLuint vao, int elementCount, RenderType type,
				   int noNorm, glm::mat4 oTW, glm::mat4 nM, glm::vec3 tint,
				   glm::vec3 diff = glm::vec3(0), glm::vec3 spec = glm::vec3(0), float sExp = 0.0f) :
			vao(vao),
			elementCount(elementCount),
			type(type),
			noNorm(noNorm),
			objToWorld(oTW),
			normalMat(nM),
			tint(tint),
			diffuse(diff),
			specular(spec),
			specExp(sExp)
		{}
	};

	RenderSystem();

	void Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	void Render(DckMesh* mesh, RenderType type, glm::mat4 objToWorld,
				glm::vec3 tint = glm::vec3(0), glm::vec3 diffuse = glm::vec3(0), glm::vec3 specular = glm::vec3(0), float sExp = 0.0f);

	void RenderDebug(DckMesh* mesh, RenderType type, glm::mat4 objToWorld,
					 glm::vec3 tint = glm::vec3(0), glm::vec3 diffuse = glm::vec3(0), glm::vec3 specular = glm::vec3(0), float sExp = 0.0f);

	~RenderSystem();

private:

	std::queue<RenderData> renderQueue_;
	std::queue<RenderData> debugQueue_;

	float pointSize_;
	float lineWidth_;

};