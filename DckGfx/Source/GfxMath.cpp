//*****************************************************************************
//	File:   GfxMath.cpp
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Graphics Math functions for doing math in relations to
//		displaying graphics
//*****************************************************************************

#include "GfxMath.h"
#include <cmath>

//*****************************************************************************
//  Description:
//		Creates a 2D or 3D point given certain values
// 
//	Param x:
//		The x value of the point
// 
//	Param y:
//		The y value of the point
// 
//	Param z:
//		The z value of the point (this param can be ignored for a z of 0)
// 
//	Return:
//		A vec4 representing a 2D or 3D point
//*****************************************************************************
glm::vec4 GfxMath::Point(float x, float y, float z)
{
	return glm::vec4(x, y, z, 1);
}

//*****************************************************************************
//  Description:
//		Creates a 2D or 3D vector given certain values
// 
//	Param x:
//		The x value of the vector
// 
//	Param y:
//		The y value of the vector
// 
//	Param z:
//		The z value of the vector (this param can be ignored for a z of 0)
// 
//	Return:
//		A vec4 representing a 2D or 3D vector
//*****************************************************************************
glm::vec4 GfxMath::Vector(float x, float y, float z)
{
	return glm::vec4(x, y, z, 0);
}

//*****************************************************************************
//  Description:
//		Generates a 4x4 matrix that translates a point by a given amount
// 
//	Param v:
//		The vector to move the point by
// 
//	Return:
//		A 4x4 matrix that translates a point by a given amount
//*****************************************************************************
glm::mat4 GfxMath::Translate(const glm::vec4& v)
{
	glm::mat4 result(1);
	result[3][0] = v.x;
	result[3][1] = v.y;
	result[3][2] = v.z;

	return result;
}

//*****************************************************************************
//  Description:
//		Generates a 4x4 2D scale matrix by a given scale
// 
//	Param s:
//		The amount to scale the matrix by
// 
//	Return:
//		A 4x4 matrix that scales a 2D object by a given amount
//*****************************************************************************
glm::mat4 GfxMath::Scale2D(float s)
{
	glm::mat4 result(1);
	result[0][0] = s;
	result[1][1] = s;

	return result;
}

//*****************************************************************************
//  Description:
//		Generates a 4x4 3D scale matrix by a given scale
// 
//	Param s:
//		The amount to scale the matrix by
// 
//	Return:
//		A 4x4 matrix that scales a 3D object by a given amount
//*****************************************************************************
glm::mat4 GfxMath::Scale3D(float s)
{
	glm::mat4 result(s);
	result[3][3] = 1;

	return result;
}

//*****************************************************************************
//  Description:
//		Generates a 4x4 3D scale matrix by a given scale
// 
//	Param s:
//		The amount to scale the matrix by
// 
//	Return:
//		A 4x4 matrix that scales an object by a given amount
//*****************************************************************************
glm::mat4 GfxMath::Scale(float sx, float sy, float sz)
{
	glm::mat4 result(1);
	result[0][0] = sx;
	result[1][1] = sy;
	result[2][2] = sz;

	return result;
}

//*****************************************************************************
//  Description:
//		Generates a 4x4 2D Rotation matrix that rotates a 2D object by a given
//		amount
// 
//	Param angle:
//		The amount to rotate the object by in degrees
// 
//	Return:
//		A 4x4 matrix that rotates a 2D object by a given amount
//*****************************************************************************
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

//*****************************************************************************
//  Description:
//		Generates a 4x4 3D rotation matrix by a given amount
// 
//	Param rotVec:
//		The vector to rotate around in 3D
// 
//	Param angle:
//		The amount to rotate the object by in degrees
// 
//	Return:
//		A 4x4 matrix that rotates a 3D object around a given vector
//*****************************************************************************
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

//*****************************************************************************
//  Description:
//		Generates a 4x4 Affine matrix given 4 column vectors
// 
//	Param v1:
//		the first column vector
// 
//	Param v2:
//		The second column vector
// 
//	Param v3:
//		The third column vector
// 
//	Param v4:
//		The fourth column vector
// 
//	Return:
//		A 4x4 affine matrix made up of the given column vectors
//*****************************************************************************
glm::mat4 GfxMath::Affine(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec4& v4)
{
	return glm::mat4(v1, v2, v3, v4);
}

//*****************************************************************************
//  Description:
//		Inverses a given 4x4 affine matrix
// 
//	Param affine:
//		The affine matrix to inverse
// 
//	Return:
//		Returns the given affine matrix inversed
//*****************************************************************************
glm::mat4 GfxMath::AffineInverse(const glm::mat4& affine)
{
	// Inverse the linear part of the matrix
	glm::mat4 linearInvTrans = glm::mat4(
		glm::inverse(
			glm::mat3(affine)));
	
	glm::mat4 invTrans = glm::mat4(1);
	invTrans[3][0] = -affine[3][0];
	invTrans[3][1] = -affine[3][1];
	invTrans[3][2] = -affine[3][2];

	return linearInvTrans * invTrans;
}

//*****************************************************************************
//  Description:
//		Generates a normal matrix given a modeling matrix
// 
//	Param trans:
//		The modeling matrix to get a normal matrix from
// 
//	Return:
//		Returns a 4x4 matrix that will map object space normals to world norms
//*****************************************************************************
glm::mat4 GfxMath::NormalMatrix(const glm::mat4& trans)
{
	return glm::mat4(
		glm::transpose(
			glm::inverse(
				glm::mat3(trans)
			)
		)
	);
}

//*****************************************************************************
//  Description:
//		Calculates the cross product between two vectors
//	
//	Param u:
//		The first vector to cross product
// 
//	Param v:
//		The second vector to cross product
// 
//	Return:
//		Returns a vec4 that represents the result of the cross product
//*****************************************************************************
glm::vec4 GfxMath::CrossProduct(const glm::vec4& u, const glm::vec4& v)
{
	glm::vec3 u3(u);
	glm::vec3 v3(v);

	glm::vec3 crossResult = glm::cross(u3, v3);
	return glm::vec4(crossResult, u.w);
}
