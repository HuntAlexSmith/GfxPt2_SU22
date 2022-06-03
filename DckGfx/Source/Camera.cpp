//*****************************************************************************
//	File:   Camera.cpp
//  Author: Hunter Smith
//  Date:   05/27/2022
//  Description: Class to hold the data of a 3D camera
//*****************************************************************************

#include "Camera.h"

// Relative up will always be in the positive y direction
static const glm::vec4 relativeUp = GfxMath::Vector(0, 1, 0);

//*****************************************************************************
//  Description:
//		Constructor for the camera class, which takes a few parameters for
//		initialization
// 
//	Param eye:
//		The eye position of the camera
// 
//	Param lookAt:
//		The look at vector for the camera
// 
//	Param fov:
//		The field of view the camera will have
// 
//	Param aspect:
//		The aspect ratio the camera will have
// 
//	Param near:
//		The near distance of the camera
// 
//	Param far:
//		The far distance of the camera
//*****************************************************************************
Camera::Camera(glm::vec4 eye, glm::vec4 lookAt, float fov, float aspect, float near, float far) :
	camToWorldMat_(),
	viewMat_(),
	perspMat_(),
	camToWorldIsDirty_(true),
	viewIsDirty_(true),
	perspIsDirty_(true),
	eyePoint_(eye),
	fov_(fov),
	lookAtVec_(lookAt),
	rightVec_(),
	upVec_(),
	backVec_(),
	aspectRatio_(aspect),
	viewportWidth_(),
	viewportHeight_(),
	nearDist_(near),
	viewportDist_(),
	farDist_(far)
{
	// Calculate back vector
	backVec_ = -lookAt / glm::length(lookAt);

	// Calculate right vector
	glm::vec4 lookCrossRel = GfxMath::CrossProduct(lookAt, relativeUp);
	rightVec_ = lookCrossRel / glm::length(lookCrossRel);

	// Calculate up vector
	upVec_ = GfxMath::CrossProduct(backVec_, rightVec_);

	// Calculate the viewport distance (half-way between near and far)
	viewportDist_ = nearDist_ + (farDist_ - nearDist_) / 2.0f;

	// Calculate width and height of the camera
	viewportWidth_ = 2 * viewportDist_ * tanf(glm::radians(fov) / 2.0f);
	viewportHeight_ = viewportWidth_ / aspectRatio_;
}

//*****************************************************************************
//  Description:
//		Moves the eye position of the camera in the direction of the look at
//		vector (opposite the back vector) a specified amount
// 
//	Param amount:
//		The amount to move the camera forward by (can be negative to move back)
//*****************************************************************************
void Camera::ForwardMove(float amount)
{
	eyePoint_ += -amount * backVec_;
	SetDirtyFlags(true);
}

//*****************************************************************************
//  Description:
//		Moves the eye position of the camera in the direction of the right
//		vector a specified amount
// 
//	Param amount:
//		The amount to move the camera right by (can be negative to move left)
//*****************************************************************************
void Camera::SideMove(float amount)
{
	eyePoint_ += amount * rightVec_;
	SetDirtyFlags(true);
}

//*****************************************************************************
//  Description:
//		Rotates the camera around the up vector by a given amount of degrees
//		(Yaw)
// 
//	Param angle:
//		The angle in degrees to rotate the camera by
//*****************************************************************************
void Camera::Yaw(float angle)
{
	glm::mat4 rotation = GfxMath::Rotate3D(upVec_, angle);

	rightVec_ = rotation * rightVec_;
	upVec_ = rotation * upVec_;
	backVec_ = rotation * backVec_;

	SetDirtyFlags(true);
}

//*****************************************************************************
//  Description:
//		Rotates the camera around the right vector by a given amount of degrees
//		(Pitch)
// 
//	Param angle:
//		The angle in degrees to rotate the camera by
//*****************************************************************************
void Camera::Pitch(float angle)
{
	glm::mat4 rotation = GfxMath::Rotate3D(rightVec_, angle);

	rightVec_ = rotation * rightVec_;
	upVec_ = rotation * upVec_;
	backVec_ = rotation * backVec_;

	SetDirtyFlags(true);
}

//*****************************************************************************
//  Description:
//		Rotates the camera around the back vector by a given amount of degrees
//		(Roll)
// 
//	Param angle:
//		The angle in degrees to rotate the camera by
//*****************************************************************************
void Camera::Roll(float angle)
{
	glm::mat4 rotation = GfxMath::Rotate3D(backVec_, angle);

	rightVec_ = rotation * rightVec_;
	upVec_ = rotation * upVec_;
	backVec_ = rotation * backVec_;

	SetDirtyFlags(true);
}

//*****************************************************************************
//  Description:
//		Get the current fov of the camera
// 
//	Return:
//		The fov of the camera in degrees
//*****************************************************************************
float Camera::GetFOV()
{
	return fov_;
}

//*****************************************************************************
//  Description:
//		Zoom the camera by a given amount (increase or decrease the fov)
// 
//	Param zoom:
//		The amount to increase or decrease the fov by (in degrees)
//*****************************************************************************
void Camera::Zoom(float zoom)
{
	fov_ -= zoom;
	if (fov_ < 1.0f)
		fov_ = 1.0f;

	if (fov_ > 90.0f)
		fov_ = 90.0f;

	viewportWidth_ = 2 * viewportDist_ * tanf(glm::radians(fov_) / 2.0f);
	viewportHeight_ = viewportWidth_ / aspectRatio_;

	SetDirtyFlags(true);
}

//*****************************************************************************
//  Description:
//		Get the current look at vector of the camera (opposite the back vector)
// 
//	Return:
//		A vec4 representing the look at vector
//*****************************************************************************
glm::vec4 Camera::GetLookAt()
{
	return -backVec_;
}

//*****************************************************************************
//  Description:
//		Get the current eye point of the camera
// 
//	Return:
//		A vec4 representing the location of the eye point
//*****************************************************************************
glm::vec4 Camera::GetEyePoint()
{
	return eyePoint_;
}

//*****************************************************************************
//  Description:
//		Sets the dirty flags for the matrices (lazy calculations)
// 
//	Param val:
//		What to set the dirty flags to
//*****************************************************************************
void Camera::SetDirtyFlags(bool val)
{
	camToWorldIsDirty_ = val;
	viewIsDirty_ = val;
	perspIsDirty_ = val;
}

//*****************************************************************************
//  Description:
//		Calculates (if needed) and gets the camera to world matrix of the
//		camera
// 
//	Return:
//		A 4x4 matrix representing the CamToWorld matrix
//*****************************************************************************
glm::mat4 Camera::GetCamToWorldMatrix()
{
	if (camToWorldIsDirty_)
	{
		camToWorldMat_ = GfxMath::Affine(rightVec_, upVec_, backVec_, eyePoint_);
		camToWorldIsDirty_ = false;
	}
	return camToWorldMat_;
}

//*****************************************************************************
//  Description:
//		Calculates (if needed) and gets the viewing matrix of the camera
//		(world to cam)
// 
//	Return:
//		A 4x4 matrix representing the Viewing Matrix (world to cam)
//*****************************************************************************
glm::mat4 Camera::GetViewMatrix()
{
	if (viewIsDirty_)
	{
		if (camToWorldIsDirty_)
			GetCamToWorldMatrix();

		viewMat_ = GfxMath::AffineInverse(camToWorldMat_);
		viewIsDirty_ = false;
	}
	return viewMat_;
}

//*****************************************************************************
//  Description:
//		Calculates (if needed) and gets the perspective matrix of the camera
// 
//	Return
//		A 4x4 matrix representing the perspective matrix
//*****************************************************************************
glm::mat4 Camera::GetPerspMatrix()
{
	if (perspIsDirty_)
	{
		perspMat_ = glm::mat4(
			2 * viewportDist_ / viewportWidth_, 0, 0, 0,
			0, 2 * viewportDist_ / viewportHeight_, 0, 0,
			0, 0, (nearDist_ + farDist_) / (nearDist_ - farDist_), -1,
			0, 0, (2 * nearDist_ * farDist_) / (nearDist_ - farDist_), 0
		);
		perspIsDirty_ = false;
	}
	return perspMat_;
}

//*****************************************************************************
//  Description:
//		Destructor for the Camera class
//*****************************************************************************
Camera::~Camera()
{

}