#pragma once
//*****************************************************************************
//	File:   GfxMath.h
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Graphics Math functions for doing math in relations to
//		displaying graphics
//*****************************************************************************

#include "glm/glm.hpp"

namespace GfxMath {
	glm::vec4 Point(float x, float y, float z=0);
	glm::vec4 Vector(float x, float y, float z=0);

	glm::mat4 Translate(const glm::vec4 &v);

	glm::mat4 Scale2D(float s);
	glm::mat4 Scale3D(float s);
	glm::mat4 Scale(float sx, float sy, float sz = 1);

	glm::mat4 Rotate2D(float angle);
	glm::mat4 Rotate3D(const glm::vec4 &rotVec, float angle);

	glm::mat4 Affine(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec4& v4);
	glm::mat4 AffineInverse(const glm::mat4& affine);
	glm::mat4 NormalMatrix(const glm::mat4& trans);

	glm::vec4 CrossProduct(const glm::vec4& u, const glm::vec4& v);
}