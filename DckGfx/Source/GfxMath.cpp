//*****************************************************************************
//	File:   GfxMath.cpp
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Graphics Math functions for doing math in relations to
//		displaying graphics
//*****************************************************************************

#include "GfxMath.h"
#include <cmath>

glm::vec4 GfxMath::Point(float x, float y, float z)
{
	return glm::vec4(x, y, z, 1);
}

glm::vec4 GfxMath::Vector(float x, float y, float z)
{
	return glm::vec4(x, y, z, 0);
}

glm::mat4 GfxMath::Translate(const glm::vec4& v)
{
	glm::mat4 result(1);
	result[3][0] = v.x;
	result[3][1] = v.y;
	result[3][2] = v.z;

	return result;
}

glm::mat4 GfxMath::Scale2D(float s)
{
	glm::mat4 result(1);
	result[0][0] = s;
	result[1][1] = s;

	return result;
}

glm::mat4 GfxMath::Scale3D(float s)
{
	glm::mat4 result(s);
	result[3][3] = 1;

	return result;
}

glm::mat4 GfxMath::Scale(float sx, float sy, float sz)
{
	glm::mat4 result(1);
	result[0][0] = sx;
	result[1][1] = sy;
	result[2][2] = sz;

	return result;
}

glm::mat4 GfxMath::Rotate2D(float angle)
{
	glm::mat4 result(1);
	float cosResult = cosf(glm::radians(angle));
	float sinResult = sinf(glm::radians(angle));
	result[0][0] = cosResult;
	result[0][1] = sinResult;
	result[1][0] = -sinResult;
	result[1][1] = cosResult;

	return result;
}

glm::mat4 GfxMath::Rotate3D(const glm::vec4 &rotVec, float angle)
{
	// Convert the angle to radians
	float radians = glm::radians(angle);

	// Calculate the first matrix
	glm::mat3 firstMat(cosf(radians));

	// Calculate the second matrix
	glm::mat3 secondMat(
			rotVec.x * rotVec.x, rotVec.y * rotVec.x, rotVec.z * rotVec.x,
			rotVec.x * rotVec.y, rotVec.y * rotVec.y, rotVec.z * rotVec.y,
			rotVec.x * rotVec.z, rotVec.y * rotVec.z, rotVec.z * rotVec.z
		);
	float vecLength = glm::length(rotVec);
	float secCoeff = (1.0f - cosf(radians)) / (vecLength * vecLength);
	secondMat = secCoeff * secondMat;

	// Calculate the third matrix
	glm::mat3 thirdMat(
		0, rotVec.z, -rotVec.y,
		-rotVec.z, 0, rotVec.x,
		rotVec.y, -rotVec.x, 0
	);
	float thirdCoeff = sinf(radians) / vecLength;
	thirdMat = thirdCoeff * thirdMat;

	glm::mat4 result = glm::mat4(firstMat + secondMat + thirdMat);
	return result;
}

glm::mat4 GfxMath::Affine(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec4& v4)
{
	return glm::mat4(v1, v2, v3, v4);
}

glm::mat4 GfxMath::AffineInverse(const glm::mat4& affine)
{
	// Inverse the linear part of the matrix
	glm::mat4 linearInvTrans = glm::mat4(
		glm::transpose(
			glm::inverse(
				glm::mat3(affine))));
	
	glm::mat4 invTrans = glm::mat4(1);
	invTrans[3][0] = -affine[3][0];
	invTrans[3][1] = -affine[3][1];
	invTrans[3][2] = -affine[3][2];

	return linearInvTrans * invTrans;
}
