#pragma once
//*****************************************************************************
//	File:   RenderObject.h
//  Author: Hunter Smith
//  Date:   06/07/2022
//  Description: Object that will handle an objects mesh, color, diffuse,
//		specular, and position data. Makes for easier rendering
//*****************************************************************************

#include "MeshLib.h"

class RenderObject {
public:

	RenderObject();

	void SetMesh(DckMesh* mesh);
	void SetPosition(glm::vec4 pos);
	void SetScale(glm::vec3 scale);
	void SetRotation(glm::vec4 vec, float angle);
	void SetTint(glm::vec3 tint);
	void SetDiffuse(glm::vec3 coeff);
	void SetSpecular(glm::vec3 coeff, float exp);

	void GetRotation(glm::vec4* vec, float* angle);

	void Draw(RenderType type);

	~RenderObject();

private:

	// The mesh the object will render with
	DckMesh* mesh_;

	// The position and scale data of the object
	glm::vec4 pos_;
	glm::vec3 scale_;

	// The rotation data of the object (vector rotated around and angle)
	glm::vec4 rotVec_;
	float rotAngle_;

	// The tint the object will have
	glm::vec3 tint_;

	// Phong Lighting data
	glm::vec3 diffuse_;
	glm::vec3 specular_;
	float specularExp_;

	// The modeling matrix for the object
	bool isDirty_;
	glm::mat4 modelMat_;

};